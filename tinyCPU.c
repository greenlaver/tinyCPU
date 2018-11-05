#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tinyCPU.h"
#include "tinyCPUcore.h"

// アセンブリのプログラム
CODE srcCode[CODE_MAX_LINE];

int main(int argc, char *argv[])
{
    // 引数1つでなければ終了
    if (argc != 2)
    {
        printf("\e[31mGIVE ME FILE PATH.\e[0m\n");
        printf("\e[33mExample : \e[0m");
        printf("%s [filePath]\n", argv[0]);
        return -1;
    }

    // 第1引数がファイル名
    if (readCode(argv[1]) != 0)
    {
        return -1;
    }

    /* == 以下処理系 == */

    // Program Counter
    int pc = 0;
    // 演算結果を格納するレジスタ (Arithmetic Register)
    double ariReg;
    // executeLineの戻り値
    int res;

    while (1)
    {
        // PCの示す番地のプログラムを実行
        res = executeLine(&pc, &ariReg, srcCode);

        // HALTが実行された場合は正常終了
        // 未知のオペコードが存在した場合は以上終了
        if (res == 1)
        {
            printf("[HALT]\n");
            break;
        }
        else if (res == -1)
        {
            printf("[Error] Invalid OPCODE occured at %d\n", pc);
            return -1;
        }
    }

    return 0;
}

int readCode(char *filePath)
{
    FILE *fp;
    int cursor = 0;

    fp = fopen(filePath, "r");
    // ファイルオープンに失敗するとNULLが入る
    if (fp == NULL)
    {
        printf("[Error] Could NOT open %s file!\n", filePath);
        return -1;
    }

    // 1行ずつ末端まで
    while (fgets(srcCode[cursor].str, CODE_MAX_LENGTH, fp) != NULL)
    {
        // Remove LF
        char *p = strchr(srcCode[cursor].str, '\n');
        if (p != NULL)
        {
            *p = '\0';
        }

        // 次の行へ移動
        cursor++;
        // MAX_LINEをオーバーしていたら取りやめ終了
        if (cursor >= CODE_MAX_LINE)
        {
            printf("[Error] Too Long Code!! Couldn't process.\n");
            fclose(fp);
            return -1;
        }
    }

    // 全部読み終わったので閉じる
    fclose(fp);

    return 0;
}
