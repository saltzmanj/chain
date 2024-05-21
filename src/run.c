#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "chain.h"
#include "statemachine.h"
#include "ingest.h"

int printNewBook(Chain_t* chain, char* firstWord, int nchars) {
    Node_t* currentNode = getNodeByLabel(chain, firstWord);
    if(currentNode == NULL) {
        printf("Word not found :(\n");
        return 1;
    }
    if(nchars > 10000) {
        printf("Too many characters >:(\n");
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
    if(argc != 4) {
        printf("Enter a model file, character count, and starting word!");
        exit(0);
    }

    Chain_t* chain = importChainFromFile(argv[1]);

    int chainStatus = validateChain(chain);
    if(chainStatus != 1) {
        printf("Text fails validation\n");
        return 1;
    } 
    char firstWord[64];
    strcpy(firstWord, argv[3]);

    int exitcode = printNewBook(chain, firstWord, atoi(argv[2]));
    printf("\n");
    return exitcode;
}