#ifndef __STATEMACHINE_H_
#define __STATEMACHINE_H_

#include <stdlib.h>
#include "chain.h"

#define PROB_MASK 0xffff
#define PROB_MAX 0xffff

typedef struct StateMachine_t StateMachine_t;
struct StateMachine_t {
    Chain_t* chain;
    Node_t* currentNode;
};

StateMachine_t* createStateMachine(Chain_t* chain, Node_t* startNode);
Node_t* getNextNode(StateMachine_t* stateMachine, int echoFlag);
Node_t* advanceStateMachine(StateMachine_t* stateMachine, int readStdinFlag, int echoFlag);

#endif