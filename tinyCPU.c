#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tinyCPU.h"

// プログラムカウンタ
int pc;
// 演算結果を格納するレジスタ (Arithmetic Register)
double ariReg;

// アセンブリのプログラム
typedef struct code {
    char str[MAX_LENGTH];
} CODE;
CODE src[MAX_LINE];

int main(int argc, char *argv[]) {
    // strtok用のバッファ
    char buffer[MAX_LENGTH];

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

    // Reset Program Counter
    pc = 0;
    
    while(1) {
        strcpy(buffer, src[pc].str);
        char *ptr = strtok(buffer, SEP_CHAR);

        // Skip Blank Line
        if(ptr == NULL) {
            pc++;
            continue;
        }

        if(strcmp(ptr, "ADD") == 0) {
            ptr = strtok(NULL, SEP_CHAR);
            add(atof(ptr));
        }
        else if(strcmp(ptr, "SUBTRACT") == 0) {
            ptr = strtok(NULL, SEP_CHAR);
            subtract(atof(ptr));
        }
        else if(strcmp(ptr, "MULTIPLY") == 0) {
            ptr = strtok(NULL, SEP_CHAR);
            multiply(atof(ptr));
        }
        else if(strcmp(ptr, "DIVIDE") == 0) {
            ptr = strtok(NULL, SEP_CHAR);
            divide(atof(ptr));
        }
        else if(strcmp(ptr, "LOAD") == 0) {
            ptr = strtok(NULL, SEP_CHAR);
            load(atof(ptr));
        }
        else if(strcmp(ptr, "STORE") == 0) {
            ptr = strtok(NULL, SEP_CHAR);
            store(atof(ptr));
        }
        else if(strcmp(ptr, "JUMP") == 0) {
            ptr = strtok(NULL, SEP_CHAR);
            jump(atof(ptr));
        }
        else if(strcmp(ptr, "JUMPZERO") == 0) {
            ptr = strtok(NULL, SEP_CHAR);
            jumpzero(atof(ptr));
        }
        else if(strcmp(ptr, "PRINT") == 0) {
            ptr = strtok(NULL, SEP_CHAR);
            print();
        }
        else if(strcmp(ptr, "HALT") == 0) {
            printf("[HALT]\n");
            break;
        }
        else {
            printf("Illegal opcode : %s\n", ptr);
            break;
        }

        // Increment Program Counter
        pc++;
    }

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

void jump(int address) {
    // printf("[%d] JUMP, %d\n", pc, address);
    
    // pc will be increment at the end of loop
    pc = address - 1;
}

void jumpzero(int address) {
    // printf("[%d] JZ, %d (%f)\n", pc, address, ariReg);

    if(ariReg == 0) {
        // pc will be increment at the end of loop
        pc = address - 1;
    }
}

void print() {
    printf("[AriReg] : %f\n", ariReg);
}
