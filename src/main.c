#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "chain.h"
#include "statemachine.h"
#include "ingest.h"

#define NODES_NUM 256

int printNewBook(Chain_t* chain, char* firstWord, int nchars) {
    Node_t* currentNode = getNodeByLabel(chain, firstWord);
    if(currentNode == NULL) {
        printf("Word not found :(\n");
        return 1;
    }

    StateMachine_t* stateMachine = createStateMachine(chain, currentNode);
    for(int k = 0; k < nchars; k++) {
        printf("%s", currentNode->nodeLabel);
        if(strstr(currentNode->nodeLabel, ".") != NULL){
            printf("\n\n");
        } else {
            printf(" ");
        }
        currentNode = advanceStateMachine(stateMachine, 0, 0);

    }
    return 0;
}

int main(int argc, char* argv[]) {
    srand(time(0));
    int filesize = 0;
    char* str = cleanString(fileToString(argv[1], &filesize));

    int wordCount = getWordCount(str);
    

    char** strArray = createWordsArray(str, wordCount);

    int wordLengthCount = 0;
    WordParent_t* wordList = createWordList(strArray, wordCount, &wordLengthCount);
    createWordChildren(wordList, strArray, wordCount);
    Chain_t* chain = createChainFromText(wordList, wordLengthCount, argv[1]);

    int chainStatus = validateChain(chain);
    if(chainStatus != 1) {
        printf("Text fails validation\n");
        return 1;
    } 
    char firstWord[64];
    strcpy(firstWord, argv[3]);

    int exitcode = 0;

    if(argc>4 && strcmp(argv[4], "-c")==0){
        printChain(chain);
    } else {
        exitcode = printNewBook(chain, firstWord, atoi(argv[2]));
    };

    return exitcode;
}



int testChain(int argc, char* argv[]) {
    srand(time(0));

    printf("\nChain Testing\n");

    char chainLabel[STR_SIZE] = "My Chain";
    Chain_t* markovChain = createChain(chainLabel, NODES_NUM);

    char node1Label[STR_SIZE] = "Apple";
    char node2Label[STR_SIZE] = "Banana";
    char node3Label[STR_SIZE] = "Carrot";
    
    Node_t* node1 = createNode(markovChain, node1Label);
    Node_t* node2 = createNode(markovChain, node2Label);
    Node_t* node3 = createNode(markovChain, node3Label);

    createVector(markovChain, node1, node2, PROB_MAX/2);
    createVector(markovChain, node1, node1, PROB_MAX/2+1);
    createVector(markovChain, node2, node1, PROB_MAX/3);
    createVector(markovChain, node2, node2, PROB_MAX/3);
    createVector(markovChain, node2, node3, PROB_MAX/3);
    createVector(markovChain, node3, node2, PROB_MAX/2);
    createVector(markovChain, node3, node3, PROB_MAX/2+1);

    printChain(markovChain);

    int chainStatus = validateChain(markovChain);
    if(chainStatus == 1) {
        printf("Chain passes validation!\n");
    }

    StateMachine_t* stateMachine = createStateMachine(markovChain, node1);


    while(1) {
        advanceStateMachine(stateMachine, 1,1);
        // printf()
    }
    return 0;
}