#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tinyCPU.h"
#include "tinyCPUcore.h"
#include "tinyDebugger.h"

int main(int argc, char *argv[])
{
    // アセンブリのプログラム
    CODE srcCode[CODE_MAX_LINE];
    // アセンブリコードの有効範囲
    int maxLine;
    // アセンブリプログラムファイルのパス
    char filePath[FILEPATH_MAX_LENGTH];
    // デバッグモードのフラグ
    int isDebug;

    // 引数の確認。不正であれば終了
    if (checkArgs(argc, argv, &isDebug, filePath) != 0)
    {
        return -1;
    }

    // プログラムファイルを読み込み
    if (readCode(filePath, srcCode, &maxLine) != 0)
    {
        return -1;
    }

    /* == 以下処理系 == */

    // Program Counter
    int pc = 0;
    // 演算結果を格納するレジスタ (Arithmetic Register)
    double ariReg = 0;
    // PRINTから出力された文字列
    char PRINTOutStr[CODE_MAX_LENGTH];

    // ブレークポイントが設定されたアドレス
    int bp = -1;
    // ステップ実行を行うかどうか
    int isStepRun = 1;

    while (1)
    {
        if (isDebug)
        {
            // デバッグモードの場合はデバッガーを1ステップ実行
            if (stepDebug(filePath, srcCode, maxLine, pc, ariReg,
                          PRINTOutStr, &bp, &isStepRun) != 0)
            {
                return -1;
            }
        }
        else
        {
            // 通常モードの場合はPRINTがあった時のみ出力
            if (strcmp(PRINTOutStr, "") != 0)
            {
                printf("[PRINT] %s\n", PRINTOutStr);
            }
        }

        // PCの示す番地のプログラムを実行
        int res = executeLine(&pc, &ariReg, PRINTOutStr, srcCode);

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

int checkArgs(int argc, char *argv[], int *isDebug, char *filePath)
{
    if (argc == 2)
    {
        *isDebug = 0;
    }
    else if (argc == 3)
    {
        if (strcmp(argv[1], "-d") == 0)
        {
            *isDebug = -1;
        }
        else
        {
            printf("\e[31mInvalid command option.\e[0m");
            printf(" : %s\n", argv[1]);
            return -1;
        }
    }
    // 引数が不正であれば終了
    else
    {
        if (argc != 1)
        {
            printf("\e[31mIncorrect command format.\e[0m\n\n");
        }

        printf("\e[33mHow to use tinyCPU : \e[0m");
        printf("%s [-d] [filePath]\n", argv[0]);
        printf("  -d : Enable Debug Mode\n");
        return -1;
    }

    // ファイルパスを読み取り
    if (strlen(argv[argc - 1]) < FILEPATH_MAX_LENGTH)
    {
        strcpy(filePath, argv[argc - 1]);
    }
    else
    {
        printf("\e[31mToo long filePath.\e[0m\n");
        return -1;
    }

    return 0;
}

int readCode(char *filePath, CODE *src, int *maxLine)
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
    while (fgets(src[cursor].str, CODE_MAX_LENGTH, fp) != NULL)
    {
        // Remove LF
        char *p = strchr(src[cursor].str, '\n');
        if (p != NULL)
        {
            *p = '\0';
        }

        // オペコードを含まないメモリ領域だった場合はフラグを立てる
        if (isNumericMemory(src[cursor].str, CODE_MAX_LENGTH))
        {
            src[cursor].isNum = -1; //True
        }
        else
        {
            src[cursor].isNum = 0; //False
        }

        // Clear Label
        strcpy(src[cursor].label, "");

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

    // プログラムファイルの行数（有効範囲）を記録
    *maxLine = cursor - 1;

    return 0;
}

int isNumericMemory(char *str, int max_length)
{
    // NULL文字が最大文字数で
    for (int i = 0; i < max_length; i++)
    {
        if (str[i] == '\0')
        {
            // 先頭文字がNULLだった場合は空行なので偽
            if (i == 0)
            {
                break;
            }
            // 全て数値に関する文字だった場合は真
            else
            {
                return 1;
            }
        }

        // + - . 数字 以外の文字が出現した場合は偽
        if (str[i] < '+' || str[i] > '9' || str[i] == ',')
        {
            break;
        }
    }

    // 偽
    return 0;
}

int stepDebug(char *filePath, CODE *src, int maxLine,
              int pc, int ariReg, char *PRINTOutStr,
              int *bp, int *isStepRun)
{
    // ブレークポイントチェック
    if (pc == *bp)
    {
        *isStepRun = 1;
    }

    // 有効コマンドが入力されるまで試行
    while (1)
    {
        if (drawDebugger(filePath, src, maxLine,
                         pc, *bp, ariReg, PRINTOutStr) != 0)
        {
            printf("[Error] Could NOT get window size.\n");
            return -1;
        }

        // ステップ実行時はコマンド入力を処理
        if (*isStepRun)
        {
            // プロンプト入力のバッファ
            char prompt_buf[64] = "";

            // ユーザ入力を安全に読み取り
            int res_scanf = scanf("%63[^\n]%*[^\n]", prompt_buf);
            scanf("%*c");

            // 読み取りに失敗した場合
            if (res_scanf == EOF)
            {
                return -1;
            }
            // 改行のみの入力だった場合は再描画して入力待ち
            else if (res_scanf == 0)
            {
                continue;
            }

            // 入力されたコマンドを区切る
            char *ptr = strtok(prompt_buf, " ");

            // nextならステップ実行
            if (strcmp(ptr, "n") == 0 ||
                strcmp(ptr, "next") == 0)
            {
                *isStepRun = 1;
                break;
            }
            // runならステップ実行解除
            else if (strcmp(ptr, "r") == 0 ||
                     strcmp(ptr, "run") == 0)
            {
                *isStepRun = 0;
                break;
            }
            // breakならブレークポイントを設定
            else if (strcmp(ptr, "b") == 0 ||
                     strcmp(ptr, "break") == 0)
            {
                ptr = strtok(NULL, " ");
                
                // breakの末尾に何もなければbreakpoint無効化
                if(ptr == NULL) {
                    *bp = -1;
                }
                // 変換失敗時はbreakpoint無効化
                else if (sscanf(ptr, "%d", bp) == EOF)
                {
                    *bp = -1;
                }
                continue;
            }
            // quitやexitでエミュ終了
            else if (strcmp(ptr, "q") == 0 ||
                     strcmp(ptr, "quit") == 0 ||
                     strcmp(ptr, "exit") == 0)
            {
                return -1;
            }
        }
        // ステップ実行でなければスルー
        else
        {
            break;
        }
    }

    return 0;
}