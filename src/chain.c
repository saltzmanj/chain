#include "chain.h"

Chain_t* createChain(char chainLabel[STR_SIZE], int chainSize) {
    // Allocate
    Chain_t* chain = malloc(sizeof(Chain_t));
    Node_t** nodes = malloc(chainSize*sizeof(Node_t*));
    // Set base variables
    strcpy(chain->chainLabel, chainLabel);
    chain->size = 0;
    chain->nodes = nodes;


    return chain;
};

void printChain(Chain_t* chain) {
    printf("%s (size %i)\n", 
        chain->chainLabel,
        chain->size
    );

    printf("Nodes:\n");
    if(chain->size == 0) {
        printf("\t(None)\n");
    } else {
        for(int i = 0; i < chain->size; i++) {
            
            Node_t* curNode = chain->nodes[i];
            printf("\t%s (size %d): \n", curNode->nodeLabel, curNode->size);
            if(curNode->size == 0){
                printf("\t\tNo exit vectors\n");
            } else {
                for(int j = 0; j < curNode->size; j++) {
                    printf("\t\t -> %s (P = %u)\n",
                        curNode->vectors[j]->toNode->nodeLabel,
                        curNode->vectors[j]->probability
                    );
                }
            }
        }
    }

    printf("\n");
    
};

Node_t* createNode(Chain_t* chain, char nodeLabel[STR_SIZE]) {
    Node_t* node = malloc(sizeof(Node_t));
    Vector_t** vectors = malloc(VECTORS_NUM*sizeof(Vector_t*));

    // Set base variables
    strcpy(node->nodeLabel, nodeLabel);
    node->vectors = vectors;
    node->size = 0;

    // Append to chain
    chain->nodes[chain->size] = node;
    chain->size = chain->size + 1;

    return node;

};

Vector_t* createVector(Chain_t* chain, Node_t* fromNode, Node_t* toNode, uint16_t probability) {

    Vector_t* vector = malloc(sizeof(Vector_t));
    vector->fromNode = fromNode;
    vector->toNode = toNode;
    vector->probability = probability;
    fromNode->vectors[fromNode->size]=vector;
    fromNode->size = fromNode -> size + 1;
    return vector;

};

int validateChain(Chain_t* chain) {
    for(int i = 0; i < chain->size; i++) {
        Node_t* curNode = chain->nodes[i];
        uint16_t cumProb = 0;
        
        for(int j = 0; j<curNode->size; j++) {
            cumProb += curNode->vectors[j]->probability;
        }

        if(cumProb != 0xffff){
            printf("Error on %s\n", curNode->nodeLabel);
            return 0;
        }
    }
    return 1;
}

Node_t* getNodeByLabel(Chain_t* chain, char* nodeLabel) {
    for(int i = 0; i<chain->size; i++) {
        if(strcmp(chain->nodes[i]->nodeLabel, nodeLabel)==0){
            return chain->nodes[i];
        }
    }
    return NULL;
}

void exportChainToFile(char* filename, Chain_t* chain) {
    FILE *f = fopen(filename, "w");
    if (f == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }
    fprintf(f, "%s\n", chain->chainLabel);
    fprintf(f, "%d\n", chain->size);
    fprintf(f, "%s\n", "NIDX,VIDX,PARENT,CHILD,PROB");
    for(int i = 0; i<chain->size; i++) {
        Node_t* fromNode = chain->nodes[i];
        for(int k = 0; k<fromNode->size; k++) {
            Vector_t* vector = fromNode->vectors[k];

            fprintf(f, "%d,%d,%s,%s,%d\n",
                i,
                k,
                fromNode->nodeLabel,
                vector->toNode->nodeLabel,
                vector->probability
            );
        }
    }
    fclose(f);
}

Chain_t* importChainFromFile(char* filename) {
    FILE* f = fopen(filename, "r");
    if(f == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }
    char chainName[256];
    int chainSize = 0;
    Chain_t* chain;

    char charbuf[256];
    int lineNum = 0;

    while(fgets(charbuf, 256, f)){
        charbuf[strcspn(charbuf, "\n")] = 0;
        if(lineNum==0) {
            strcpy(chainName, charbuf);
        } else if (lineNum == 1){
            chainSize = atoi(charbuf);
            chain = createChain(chainName, chainSize);
        } else if (lineNum == 2) {
            //do nothing
        } else {
            char* token = strtok(charbuf, ",");
            int tokIdx = 0;
            char fromNodeName[STR_SIZE];
            char toNodeName[STR_SIZE];
            uint16_t probability;

            while(token != NULL) {
                if(tokIdx==2) {
                    strcpy(fromNodeName, token);
                } else if (tokIdx == 3){
                    strcpy(toNodeName, token);
                } else if (tokIdx == 4){
                    probability = atoi(token);
                }
                token = strtok(NULL, ",");
                tokIdx += 1;
            }

            Node_t* fromNode = getNodeByLabel(chain, fromNodeName);
            Node_t* toNode = getNodeByLabel(chain, toNodeName);

            if(fromNode == NULL) {
                fromNode = createNode(chain, fromNodeName);
            }
            if(toNode == NULL) {
                toNode = createNode(chain, toNodeName);
            }

            createVector(chain, fromNode, toNode, probability);
        }
        lineNum+=1;
    }
    return chain;
}