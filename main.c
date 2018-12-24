#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "time.h"
#include "sha.h"



void SHA1(unsigned char*, char**);

typedef struct S_StuProf {
    int sap;
    int marks;
    char name[100];
}StuProf;

typedef struct S_Block{
    
    char timestamp[64]; //since sizeof ...
    char hash[100];
    struct S_Block *next;
    struct S_Block *previous;
    struct S_StuProf stu_data;
    
}Block;

void PrintList(Block *start) {
    Block *currentBlock = start;
    int count = 0;
    
    Block *ahead = NULL;
    Block *behind = NULL;
    
    while(currentBlock != NULL) {
        count++;
        
        ahead = currentBlock->next;
        behind = currentBlock->previous;
        
        printf("Stu:%d \t Sap:%d \n Name:%s \t Marks:%d \n Next:%s \t Previous:%s \n Time:%s",
               count,
               currentBlock->stu_data.sap,
               currentBlock->stu_data.name,
               currentBlock->stu_data.marks,
               (ahead == NULL) ? "None" : ahead->stu_data.name,
               (behind == NULL) ? "None" : behind->stu_data.name,
               currentBlock->timestamp
               );
        
        printf(" PrevHash:%s \n",(behind == NULL) ?"None":behind->hash);
        printf("\n");
        printf("CurrHash:%s \n",currentBlock->hash);
        printf("\n");
        currentBlock = currentBlock->next;
        ahead = NULL;
        behind = NULL;
    }
    printf("Total Blocks:%d \n",count);
}
Block *MakeNewBlock() {
    
    printf("Enter SAP ID, Name And marks: ");
    char input[36];
    
    fgets( input, 35, stdin);
    
    
    Block *newBlock = malloc(sizeof(Block));
    sscanf(input, "%d %s %d",
           &newBlock->stu_data.sap,
           newBlock->stu_data.name,
           &newBlock->stu_data.marks);
    
    char * z;
    SHA1((unsigned char *)&input, &(newBlock->hash));
    

    
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    char timeString[64];
    strftime(timeString, sizeof(timeString), "%c", tm);
    strcpy(newBlock->timestamp , timeString);
    
    newBlock->next = NULL;
    newBlock->previous = NULL;
    z = malloc(10*sizeof(char));
    printf("%s\n",z);
    
    
    
    
    printf("Added:\n SAP:%u Name:%s Time:%s\n\n",
           newBlock->stu_data.sap,
           newBlock->stu_data.name,
           newBlock->timestamp
           );
    printf("Hash : %s",newBlock->hash);
    return newBlock;
}
Block *AddToStart(Block *startPtr) {
    Block *newBlock = MakeNewBlock();
    if(startPtr != NULL) {
        startPtr->previous = newBlock;
        newBlock->next = startPtr;
    }
    
    return newBlock;
}
Block *AddToEnd(Block *startPtr) {
    Block *returnPtr = startPtr;
    Block *newBlock = NULL;
    
    if(startPtr == NULL) {
        newBlock = AddToStart(startPtr);
        returnPtr = newBlock;
    } else {
        Block *indexBlock = startPtr;
        while (indexBlock->next != NULL) {
            indexBlock = indexBlock->next;
        }
        newBlock = MakeNewBlock();
        indexBlock->next = newBlock;
        newBlock->next = NULL;
        newBlock->previous = indexBlock;
    }
    return returnPtr;
}
void CleanUp(Block *start) {
    
    Block *freeMe = start;
    Block *holdMe = NULL;
    while(freeMe != NULL) {
        holdMe = freeMe->next;
        printf("Free Sap:%u Name:%s marks:%d Hash: %s\n TimeStamp : %s \n",
               freeMe->stu_data.sap,
               freeMe->stu_data.name,
               freeMe->stu_data.marks,
               freeMe->hash,
               freeMe->timestamp);
        free(freeMe);
        freeMe = holdMe;
    }
}
void WriteListToFile(Block *start) {
    FILE *pFile;
    pFile = fopen("myFile.txt", "w");
    
    if(pFile != NULL) {
        Block *currentBlock = start;
        
        Block *holdNext = NULL;
        Block *holdPrevious = NULL;
        
        fprintf(pFile, "\n");
        while(currentBlock != NULL) {
            holdNext = currentBlock->next;
            holdPrevious = currentBlock->previous;
            
            currentBlock->next = NULL;
            currentBlock->previous = NULL;
            
            fseek(pFile, 0, SEEK_END);
            //fwrite(currentBlock, sizeof(Block), 1, pFile);
            fprintf(pFile, "Name: %s\n", currentBlock->stu_data.name);
            fprintf(pFile, "SAP: %d\n", currentBlock->stu_data.sap);
            fprintf(pFile, "Marks: %d\n", currentBlock->stu_data.marks);
            fprintf(pFile, "Time: %s\n", currentBlock->timestamp);
            if(holdPrevious != NULL){
                fprintf(pFile, "%s\n", holdPrevious->hash);
            }
            else{
                fprintf(pFile, "0000\n");
            }
            fprintf(pFile, "%s\n\n", currentBlock->hash);
            printf("Writing:%s to file\n",currentBlock->stu_data.name);
            
            currentBlock->next = holdNext;
            currentBlock->previous = holdPrevious;
            
            holdNext = NULL;
            holdPrevious = NULL;
            
            currentBlock = currentBlock->next;
        }
        fclose(pFile);
        pFile = NULL;
    } else {
        printf("FILE OPEN ERROR\n");
    }
    
}
Block *ReadListIn(Block *start) {
    
    FILE *pFile;
    char c;
    pFile = fopen("myFile.txt", "r+");
    
    
    if(pFile != NULL) {
        CleanUp(start);
        start = NULL;
        c = fgetc(pFile);
        while (c != EOF)
        {
            printf ("%c", c);
            c = fgetc(pFile);
        }
        
        
        
    }  else {
        printf("FILE OPEN ERROR FOR READ\n");
    }
    
    return start;
    
}
Block *VerifyChain(Block *start) {
    
    FILE *pFile;
    int count = 0, i;
    char line[100];
    char prevH[100] = { 0 };
    
    pFile = fopen("myFile.txt", "r+");
    
    
    if(pFile != NULL) {
        CleanUp(start);
        start = NULL;
        
        fseek(pFile, 0, SEEK_END);
    
        rewind(pFile);
        
        

        while(fgets(line, 100, pFile)){
            count++;
            if(count % 6 == 0){
                if(strcmp(prevH, line) == 0)
                    printf("Matching\n");
                else
                    printf("Not matching... since it's the first one :) \n");
            }
            if(count % 7 == 0){
                for(i = 0; i < 100; i++){
                    prevH[i] = line[i];
                }
                count = 0;
            }
        }

    }  else {
        printf("FILE OPEN ERROR FOR READ\n");
    }
    
    return start;
    
}


int main(){
    char command[16];
    char input[16];
    
    Block *start = NULL;
    
    printf("Please enter one of the following commands to operate\n");
    printf("add \n");
    printf("print\n");
    printf("write \n");
    printf("read \n");
    printf("verify \n");
    printf("quit \n\n");
    
    while( fgets( input, 15, stdin) ) {
        
        sscanf(input,"%s",command);
        
        if ( strncmp(command, "quit", 4) == 0) {
            printf("\n\nBreaking...\n");
            break;
        } else if ( strncmp(command, "print", 5) == 0) {
            PrintList(start);
        } else if ( strncmp(command, "write", 5) == 0) {
            WriteListToFile(start);
        } else if ( strncmp(command, "read", 4) == 0) {
            start = ReadListIn(start);
        }else if ( strncmp(command, "verify", 6) == 0) {
            start = VerifyChain(start);
        }
        else if( strncmp(command, "add", 3) == 0){
            start = AddToEnd(start);
            PrintList(start);
        }
        else {
            printf("Wrong command!");
        }
    }
    CleanUp(start);
    
    return 0;
}
