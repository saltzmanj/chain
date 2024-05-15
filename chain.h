#ifndef __CHAIN_H_
#define __CHAIN_H_

#include "stdint.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "math.h"

#define STR_SIZE 64

#define VECTORS_NUM 10000

typedef struct Vector_t Vector_t;
typedef struct Node_t Node_t;
typedef struct Chain_t Chain_t;

struct Node_t {
    char nodeLabel[STR_SIZE];
    Vector_t** vectors; 
    int32_t size;
};

struct Vector_t {
    Node_t* fromNode;
    Node_t* toNode;

    uint16_t probability;
};

struct Chain_t {
    char chainLabel[STR_SIZE];
    Node_t** nodes;
    int32_t size;
};

// Debug functions
void printChain(Chain_t* chain);


// Create a chain; returns a pointer to the chain
Chain_t* createChain(
    char in_chainLabel[STR_SIZE],
    int32_t chainSize
);

// Create a node and add it to a chain
Node_t* createNode(
    Chain_t* chain,
    char nodeLabel[STR_SIZE]
);

// Create a vector between two nodes
Vector_t* createVector(
    Chain_t* chain, 
    Node_t* fromNode, 
    Node_t* toNode,
    uint16_t probability
);

Node_t* getNodeByLabel(Chain_t* chain, char* nodeLabel);

// Ensure the chain is structurally sound. Returns 1 if chain passes all checks
int validateChain(Chain_t* chain);

#endif