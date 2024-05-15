#ifndef __INGEST_H_
#define __INGEST_H_

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "chain.h"

typedef struct WordChild_t WordChild_t;
typedef struct WordParent_t WordParent_t;

struct WordChild_t {
    char word[STR_SIZE];
    uint32_t count;
    WordChild_t* nextChildWord;
};

struct WordParent_t {
    char word[STR_SIZE];
    WordChild_t* rootChildWord;
    uint32_t childCount;
    uint32_t probCount;
    WordParent_t* nextWord;
};

char* fileToString(char* filename, int* filesize);
char* cleanString(char* str);

int getWordCount(char* str);

char** createWordsArray(char* str, int wordCount);
WordParent_t* createWordList(char** wordsArray, int wordCount, int* wordLengthCount);
void createWordChildren(WordParent_t* wordList, char** text, int textWordCount);

void printWordList(WordParent_t* rootNode);

Chain_t* createChainFromText(WordParent_t* wordList, int wordCount, char* machineName);

#endif