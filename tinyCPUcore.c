#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tinyCPUcore.h"

int executeLine(int *pc, double *ariReg, char *PRINTOutStr, CODE *src)
{
    // strtok用のバッファ
    char buffer[CODE_MAX_LENGTH];

    // PRINT出力をクリア
    strcpy(PRINTOutStr, "");

    strcpy(buffer, src[*pc].str);
    char *ptr = strtok(buffer, SEP_CHAR);

    // Skip Blank Line
    if (ptr == NULL)
    {
        *pc += 1;
        return 0;
    }

    // Get OPCODE from code string
    OPCODE opcode_buf = convertStrToOpcode(ptr);
    ptr = strtok(NULL, SEP_CHAR);

    switch (opcode_buf)
    {
    case ADD:
        add(atof(ptr), ariReg, src);
        break;
    case SUBTRACT:
        subtract(atof(ptr), ariReg, src);
        break;
    case MULTIPLY:
        multiply(atof(ptr), ariReg, src);
        break;
    case DIVIDE:
        divide(atof(ptr), ariReg, src);
        break;
    case LOAD:
        load(atof(ptr), ariReg, src);
        break;
    case STORE:
        store(atof(ptr), *ariReg, src);
        break;
    case JUMP:
        jump(atof(ptr), pc);
        return 0;
    case JUMPZERO:
        jumpzero(atof(ptr), *ariReg, pc);
        return 0;
    case PRINT:
        print(PRINTOutStr, *ariReg);
        break;
    case HALT:
        return 1;
    default:
        return -1;
    }

    // Increment Program Counter
    *pc += 1;

    return 0;
}

OPCODE convertStrToOpcode(char *code)
{
    if (strcmp(code, "ADD") == 0)
    {
        return ADD;
    }
    else if (strcmp(code, "SUBTRACT") == 0)
    {
        return SUBTRACT;
    }
    else if (strcmp(code, "MULTIPLY") == 0)
    {
        return MULTIPLY;
    }
    else if (strcmp(code, "DIVIDE") == 0)
    {
        return DIVIDE;
    }
    else if (strcmp(code, "LOAD") == 0)
    {
        return LOAD;
    }
    else if (strcmp(code, "STORE") == 0)
    {
        return STORE;
    }
    else if (strcmp(code, "JUMP") == 0)
    {
        return JUMP;
    }
    else if (strcmp(code, "JUMPZERO") == 0)
    {
        return JUMPZERO;
    }
    else if (strcmp(code, "PRINT") == 0)
    {
        return PRINT;
    }
    else if (strcmp(code, "HALT") == 0)
    {
        return HALT;
    }
    else
    {
        printf("[Error] Illegal opcode : %s\n", code);
        return ILLEGAL_OPCODE;
    }
}

void add(int address, double *ariReg, CODE *src)
{
    // addressに入っている数値を参照
    double num = atof(src[address].str);
    // 演算レジスタのデータに加算する
    *ariReg += num;

    // printf("[%d] ADD, %d (%f)\n", pc, address, ariReg);
}

void subtract(int address, double *ariReg, CODE *src)
{
    // addressに入っている数値を参照
    double num = atof(src[address].str);
    // 演算レジスタのデータに減算する
    *ariReg = num - *ariReg;

    // printf("[%d] SUB, %d (%f)\n", pc, address, ariReg);
}

void multiply(int address, double *ariReg, CODE *src)
{
    // addressに入っている数値を参照
    double num = atof(src[address].str);
    // 演算レジスタのデータに乗算する
    *ariReg *= num;

    // printf("[%d] MUL, %d (%f)\n", pc, address, ariReg);
}

void divide(int address, double *ariReg, CODE *src)
{
    // addressに入っている数値を参照
    double num = atof(src[address].str);
    // 演算レジスタのデータに除算する
    *ariReg /= num;

    // printf("[%d] DIV,%d (%f)\n", pc, address, ariReg);
}

void load(int address, double *ariReg, CODE *src)
{
    // addressに入っている数値を参照
    double num = atof(src[address].str);
    // 演算レジスタへデータを格納する
    *ariReg = num;

    // printf("[%d] LOAD, %d (%f)\n", pc, address, ariReg);
}

void store(int address, double ariReg, CODE *src)
{
    // 演算レジスタに入っている数値をaddressに書き込む
    snprintf(src[address].str,
             CODE_MAX_LENGTH, "%f", ariReg);

    // printf("[%d] STORE, %d (%f)\n", pc, address, ariReg);
}

void jump(int address, int *pc)
{
    // printf("[%d] JUMP, %d\n", pc, address);

    // Jump to address
    *pc = address;
}

void jumpzero(int address, double ariReg, int *pc)
{
    // printf("[%d] JZ, %d (%f)\n", pc, address, ariReg);

    if (ariReg == 0)
    {
        // Jump to address
        *pc = address;
    }
    else
    {
        *pc += 1;
    }
}

void print(char *PRINTOutStr, double ariReg)
{
    snprintf(PRINTOutStr,
             CODE_MAX_LENGTH, "%f", ariReg);

    // printf("[PRINT] : %f\n", ariReg);
}
