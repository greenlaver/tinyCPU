#include "tinyCPUcore.h"

#define FILEPATH_MAX_LENGTH 512

// Function Prototype
int checkArgs(int argc, char *argv[], int *isDebug, char *filePath);
int readCode(char *filePath, CODE *src, int *maxLine);
int isNumericMemory(char *str, int max_length);
int stepDebug(char *filePath, CODE *src, int maxLine,
           int pc, int ariReg, char *PRINTOutStr,
           int *bp, int *isStepRun);