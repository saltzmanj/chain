#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "chain.h"
#include "statemachine.h"
#include "ingest.h"

int main(int argc, char* argv[]) {
        if(argc != 3) {
        printf("Supply input file as argument 1, output file as argument 2");
    } else {

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
        exportChainToFile(argv[2], chain);
    }
    return 0;
}

