#include "ingest.h"

char* fileToString(char* filename, int* filesize) {
    FILE* fileptr = fopen(filename, "r");
    if(fileptr == NULL) {
        exit(0);
    }

    fseek(fileptr, 0L, SEEK_END);
    int fsize = ftell(fileptr);
    char* filestring = malloc(fsize);
    fseek(fileptr, 0, SEEK_SET);

    fread(filestring, fsize, 1, fileptr);
    fclose(fileptr);

    *filesize = fsize;
    return filestring;
}

char* cleanString(char* str) {

    unsigned long i = 0;
    unsigned long j = 0;
    char c;

    while ((c = str[i++]) != '\0')
    {
        if (isalnum(c) == 1 || c=='\n' || c==' ' || c=='.')
        {
            if(c == '\n') {
                str[j++] = ' ';
            } else {
                str[j++] = c;
            }
            
        }
    }
    str[j] = '\0';
    return str;
}

int getWordCount(char* str) {
    int count = 0;
    
    char* tempStr = malloc(strlen(str)+1);
    strcpy(tempStr, str);

    char* token = strtok(tempStr, " ");
    while(token != NULL) {
        count+=1;
        token = strtok(NULL, " ");
    }
    free(tempStr);
    return count;
}


char** createWordsArray(char* str, int wordCount) {
    char* tempStr = malloc(strlen(str)+1);
    strcpy(tempStr, str);
    char** charbuffer = malloc(wordCount*sizeof(char*));
    
    for(int i = 0; i < wordCount; i++) {
        char* ptr;
        if (i==0){
            ptr = tempStr;
        } else {
            ptr = NULL;
        }
        char* token = strtok(ptr, " ");
        if(token == NULL) break;

        int wordlen = strlen(token);
        char* newWord = malloc(wordlen+1);

        int k = 0;
        for(k = 0; k < wordlen; k++) {
            newWord[k] = token[k];
        };
        newWord[k] = '\0';
        charbuffer[i] = newWord;
    }
    free(tempStr);
    return charbuffer;
}

WordParent_t* createWordList(char** wordsArray, int wordCount, int* wordLengthCount) {
    WordParent_t* rootNode;
    WordParent_t* wordTree;
    int uniqueWordCount = 0;

    for(int i = 0; i<wordCount; i++) {
        char* currentWord = wordsArray[i];
        int doesWordExist = 0;

        WordParent_t* treeItr = rootNode;
        for(int k = 0; k<uniqueWordCount; k++) {
            if (strcmp(currentWord, treeItr->word)==0) {
                doesWordExist = 1;
                break;
            }
            treeItr = treeItr->nextWord;
        }

        if (doesWordExist == 1) {
            // Todo
        } else {
            WordParent_t* newWord = malloc(sizeof(WordParent_t));
            strcpy(newWord->word, currentWord);
            newWord->childCount = 0;
            newWord->probCount = 0;
            newWord->nextWord = NULL;

            if(uniqueWordCount == 0) {
                rootNode = newWord;
            } else {
                wordTree->nextWord = newWord;
            }

            wordTree = newWord;
            uniqueWordCount+=1;
        }
    }
    *wordLengthCount = uniqueWordCount;
    return rootNode;
};

void createWordChildren(WordParent_t* wordList, char** text, int textWordCount) {
    WordParent_t* currentWord = wordList;

    while(currentWord->nextWord != NULL) {
        char* parentWord = currentWord->word;
        int uniqueChildCount = 0;
        // printf("Searching for %s ...", parentWord);
        for(int i = 0; i<textWordCount-2; i++) {
            char* textWord = text[i];

            if(strcmp(textWord, parentWord) == 0) {
                // We've detected a match in the parent word, so we need to check the next word to decide what to do with
                char* newChildTextWord = text[i+1];

                // Determine if the next word is in the list of children for this parent by iterating over EXISTING list
                int doesChildExist = 0;
                WordChild_t* childTree;
                WordChild_t* currentChildItr = currentWord->rootChildWord;

                for(int j = 0; j<uniqueChildCount; j++) {
                    if(strcmp(newChildTextWord, currentChildItr->word)==0) {
                        doesChildExist = 1;
                        currentChildItr->count += 1;
                        currentWord->probCount += 1;
                        break;
                    }
                    currentChildItr = currentChildItr->nextChildWord;
                } 

                if(doesChildExist == 0) {
                    WordChild_t* newChild = malloc(sizeof(WordChild_t));
                    strcpy(newChild->word, newChildTextWord);
                    newChild->count = 1;
                    newChild->nextChildWord = NULL;

                    if(uniqueChildCount == 0) {
                        currentWord->rootChildWord = newChild;
                    } else {
                        childTree->nextChildWord = newChild;
                    }
                    childTree = newChild;
                    currentWord->childCount += 1;
                    currentWord->probCount += 1;
                    uniqueChildCount += 1;
                } else {
                    
                }



            } else {
                // If the word in the text isn't the parent, ignore it
            }
        }
        // Advance the iteration
        currentWord = currentWord->nextWord;
    }
}

void printWordList(WordParent_t* rootNode) {
    WordParent_t* curWord = rootNode;
    while(curWord->nextWord != NULL) {
        printf("%s (%i children, %d occurrences)\n", curWord->word, curWord->childCount, curWord->probCount);

        WordChild_t* curChild = curWord->rootChildWord;
        while(curChild != NULL) {
            printf("\t%s (%i occurrences)\n", curChild->word, curChild->count);
            curChild = curChild->nextChildWord;
        }
        curWord = curWord->nextWord;
    }
}

Chain_t* createChainFromText(WordParent_t* wordList, int wordCount, char machineName[STR_SIZE]) {
    
    Chain_t* chain = createChain(machineName, wordCount);

    WordParent_t* curWord = wordList;
    while(curWord->nextWord != NULL) {
        createNode(chain, curWord->word);
        curWord = curWord->nextWord;
    }
    curWord = wordList;

    while(curWord->nextWord != NULL) {
        Node_t* fromNode = getNodeByLabel(chain, curWord->word);
        if(fromNode == NULL) return chain;
        WordChild_t* curChild = curWord->rootChildWord;

        uint16_t probSum = 0;

        while(curChild != NULL) {
            Node_t* toNode = getNodeByLabel(chain, curChild->word);
            uint16_t prob = ((double) curChild->count / (double) curWord->probCount) * UINT16_MAX;
            Vector_t* newVector = createVector(chain, fromNode, toNode, prob);
            probSum += prob;
            curChild = curChild->nextChildWord;
            if(curChild == NULL && probSum < UINT16_MAX) {
                newVector->probability += UINT16_MAX - probSum;
                probSum = UINT16_MAX;
            }
        }
        curWord = curWord->nextWord;
    }

    return chain;
}