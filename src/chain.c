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