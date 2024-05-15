#include "statemachine.h"


StateMachine_t* createStateMachine(Chain_t* chain, Node_t* startNode) {
    
    StateMachine_t* stateMachine = malloc(sizeof(StateMachine_t));
    
    stateMachine->chain = chain;
    stateMachine->currentNode = startNode;

    return stateMachine;

};

Node_t* getNextNode(StateMachine_t* stateMachine, int echoFlag) {
    uint16_t urand16 = rand() & PROB_MASK;
    int32_t nodeSize = stateMachine->currentNode->size;
    uint16_t cumProb = 0;
    if(echoFlag == 1) {printf("Rolled %u", urand16);};

    for(int32_t i = 0; i<nodeSize; i++) {
        Vector_t* curVector = stateMachine->currentNode->vectors[i];
        cumProb = cumProb + curVector->probability - 1;
        // printf("\n%i", cumProb);
        if(urand16 < cumProb) {
            // printf("\n%d is less than %d, going to %s\n", urand16, cumProb, curVector->toNode->nodeLabel);
            return curVector->toNode;
        }
        
    }
    return stateMachine->currentNode;
};

Node_t* advanceStateMachine(StateMachine_t* stateMachine, int readStdinFlag, int echoFlag) {
    
    char buf[2];
    if(readStdinFlag == 1) {

        printf("Current state is %s...", stateMachine->currentNode->nodeLabel);
        fgets(buf, 2, stdin);
    }
    Node_t* nextState = getNextNode(stateMachine, echoFlag);
    
    if(nextState != NULL){
        stateMachine->currentNode = nextState;
    } else {
        printf("Panic! Null state encountered.");
        exit(0);
    }

    if(echoFlag == 1) {
        printf("\nNew state is: %s\n", stateMachine->currentNode->nodeLabel);
    }
    return nextState;
}