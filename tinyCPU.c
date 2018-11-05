#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tinyCPU.h"

#define RES_HALT    1
#define RES_INVALID -1

// 演算結果を格納するレジスタ (Arithmetic Register)
double ariReg;

// アセンブリのプログラム
typedef struct code {
    char str[MAX_LENGTH];
} CODE;
CODE src[MAX_LINE];

int main(int argc, char *argv[]) {
    // Program Counter
    int pc = 0;

    // 引数1つでなければ終了
    if(argc != 2) {
        printf("\e[31mGIVE ME FILE PATH.\e[0m\n");
        printf("\e[33mExample : \e[0m");
        printf("%s [filePath]\n", argv[0]);
        return -1;
    }

    // 第1引数がファイル名
    if(readCode(argv[1]) != 0) {
        return -1;
    }

    /* == 以下処理系 == */
    int res;

    while(1) {
        res = executeLine(&pc);

        if(res == RES_HALT) {
            printf("[HALT]\n");
            break;
        }
        else if(res == RES_INVALID) {
            printf("[Error] Invalid OPCODE occured at %d\n", pc);
            return -1;
        }
    }

    return 0;
}

int executeLine(int *pc) {
    // strtok用のバッファ
    char buffer[MAX_LENGTH];

    strcpy(buffer, src[*pc].str);
    char *ptr = strtok(buffer, SEP_CHAR);

    // Skip Blank Line
    if(ptr == NULL) {
        *pc += 1;
        return 0;
    }

    // Get OPCODE from code string
    OPCODE opcode_buf = convertStrToOpcode(ptr);
    ptr = strtok(NULL, SEP_CHAR);

    switch(opcode_buf) {
        case ADD:
            add(atof(ptr));
            break;
        case SUBTRACT:
            subtract(atof(ptr));
            break;
        case MULTIPLY:
            multiply(atof(ptr));
            break;
        case DIVIDE:
            divide(atof(ptr));
            break;
        case LOAD:
            load(atof(ptr));
            break;
        case STORE:
            store(atof(ptr));
            break;
        case JUMP:
            jump(pc, atof(ptr));
            return 0;
        case JUMPZERO:
            jumpzero(pc, atof(ptr));
            return 0;
        case PRINT:
            print();
            break;
        case HALT:
            return RES_HALT;
        default:
            return RES_INVALID;
    }

    // Increment Program Counter
    *pc += 1;

    return 0;
}

int readCode(char *filePath) {
    FILE *fp;
    int cursor = 0;

    fp = fopen(filePath, "r");
    // ファイルオープンに失敗するとNULLが入る
	if(fp == NULL) {
		printf("[Error] Could NOT open %s file!\n", filePath);
		return -1;
	}

    // 1行ずつ末端まで
    while(fgets(src[cursor].str, MAX_LENGTH, fp) != NULL) {
        // Remove LF
        char *p = strchr(src[cursor].str, '\n');
        if(p != NULL) {
            *p = '\0';
        }

        // 次の行へ移動
        cursor++;
        // MAX_LINEをオーバーしていたら取りやめ終了
        if(cursor >= MAX_LINE) {
            printf("[Error] Too Long Code!! Couldn't process.\n");
            fclose(fp);
            return -1;
        }
	}

    // 全部読み終わったので閉じる
    fclose(fp);

    return 0;
}

OPCODE convertStrToOpcode(char *code) {
    if(strcmp(code, "ADD") == 0) {
        return ADD;
    }
    else if(strcmp(code, "SUBTRACT") == 0) {
        return SUBTRACT;
    }
    else if(strcmp(code, "MULTIPLY") == 0) {
        return MULTIPLY;
    }
    else if(strcmp(code, "DIVIDE") == 0) {
        return DIVIDE;
    }
    else if(strcmp(code, "LOAD") == 0) {
        return LOAD;
    }
    else if(strcmp(code, "STORE") == 0) {
        return STORE;
    }
    else if(strcmp(code, "JUMP") == 0) {
        return JUMP;
    }
    else if(strcmp(code, "JUMPZERO") == 0) {
        return JUMPZERO;
    }
    else if(strcmp(code, "PRINT") == 0) {
        return PRINT;
    }
    else if(strcmp(code, "HALT") == 0) {
        return HALT;
    }
    else {
        printf("[Error] Illegal opcode : %s\n", code);
        return ILLEGAL_OPCODE;
    }
}

void add(int address) {
    // addressに入っている数値を参照
    double num = atof(src[address].str);
    // 演算レジスタのデータに加算する
    ariReg += num;

    // printf("[%d] ADD, %d (%f)\n", pc, address, ariReg);
}

void subtract(int address) { 
    // addressに入っている数値を参照
    double num = atof(src[address].str);
    // 演算レジスタのデータに減算する
    ariReg = num - ariReg;

    // printf("[%d] SUB, %d (%f)\n", pc, address, ariReg);
}

void multiply(int address) {
    // addressに入っている数値を参照
    double num = atof(src[address].str);
    // 演算レジスタのデータに乗算する
    ariReg *= num;

    // printf("[%d] MUL, %d (%f)\n", pc, address, ariReg);
}

void divide(int address) {
    // addressに入っている数値を参照
    double num = atof(src[address].str);
    // 演算レジスタのデータに除算する
    ariReg /= num;

    // printf("[%d] DIV,%d (%f)\n", pc, address, ariReg);
}

void load(int address) {
    // addressに入っている数値を参照
    double num = atof(src[address].str);
    // 演算レジスタへデータを格納する
    ariReg = num;

    // printf("[%d] LOAD, %d (%f)\n", pc, address, ariReg);
}

void store(int address) {
    // 演算レジスタに入っている数値をaddressに書き込む
    snprintf(src[address].str,
     MAX_LENGTH, "%f", ariReg);

    // printf("[%d] STORE, %d (%f)\n", pc, address, ariReg);
}

void jump(int *pc, int address) {
    // printf("[%d] JUMP, %d\n", pc, address);

    // Jump to address
    *pc = address;
}

void jumpzero(int *pc, int address) {
    // printf("[%d] JZ, %d (%f)\n", pc, address, ariReg);

    if(ariReg == 0) {
        // Jump to address
        *pc = address;
    }
    else {
        *pc += 1;
    }
}

void print() {
    printf("[PRINT] : %f\n", ariReg);
}
