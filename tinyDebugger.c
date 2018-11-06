#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include "tinyCPUcore.h"
#include "tinyDebugger.h"

// 端末の画面サイズ
struct winsize ws;

int drawDebugger(char *fileName, CODE *src, int maxLine,
                 int pc, int bp, float ariReg, char *PRINTOutStr,
                 char lastCmd)
{
    // get terminal size
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1)
    {
        return -1;
    }

    // Clear Screen
    clearScreenESC();

    // Head Bar
    drawHeadBar(fileName);
    // Body Left part
    drawArithReg(ariReg);
    drawMemoryList(src, maxLine);
    drawCodeList(pc, src, maxLine);
    // Body Right part
    drawPC(pc);
    drawBreakpoint(bp, maxLine);

    // PRINT output message
    drawPRINTOut(PRINTOutStr);
    // Footer
    drawFooter();
    // Prompt
    drawPrompt(lastCmd);

    return 0;
}

void drawHeadBar(char *fileName)
{
    setCursorESC(1, 1);
    resetESC();
    setRevESC();
    printf(" tinyCPU : %s", fileName);
    for (int i = 0; i < ws.ws_col - 11 - strlen(fileName); i++)
    {
        printf(" ");
    }
}

void drawPRINTOut(char *PRINTOutStr)
{
    setCursorESC(1, ws.ws_row - 3);
    resetESC();
    setRevESC();
    printf(" [ PRINT ]");
    for (int i = 0; i < ws.ws_col - 10; i++)
    {
        printf(" ");
    }

    setCursorESC(1, ws.ws_row - 2);
    resetESC();
    printf("  %s", PRINTOutStr);
}

void drawFooter()
{
    setCursorESC(1, ws.ws_row - 1);
    resetESC();
    setBoldESC();
    setRevESC();
    printf(" COMMAND : ");
    resetESC();
    setRevESC();

    if (ws.ws_col > 70)
    {
        printf("next | break [address] | run | label [address] [name] | quit");
        for (int i = 0; i < ws.ws_col - 71; i++)
        {
            printf(" ");
        }
    }
    else {
        printf("n,b A,r,l A N,q");
        for (int i = 0; i < ws.ws_col - 26; i++)
        {
            printf(" ");
        }
    }
}

void drawPrompt(char lastCmd)
{
    setCursorESC(1, ws.ws_row);
    resetESC();

    if (lastCmd == '\0')
    {
        printf("   ");
    }
    else
    {
        printf("(%c)", lastCmd);
    }

    printf(" > ");
}

void drawArithReg(float ariReg)
{
    setCursorESC(2, 3);
    resetESC();
    setUnderlineESC();
    setBoldESC();
    printf("Arithmetic Register");

    resetESC();
    setCursorESC(4, 4);
    printf("%f", ariReg);
}

void drawMemoryList(CODE *src, int maxLine)
{
    int listed_count = 0;

    // Print area title
    setCursorESC(2, 6);
    setUnderlineESC();
    setBoldESC();
    printf("Memory Area");

    for (int i = 0; i <= maxLine; i++)
    {
        // 数値のみのメモリ領域を出力
        if (src[i].isNum)
        {
            resetESC();
            setBoldESC();
            setCursorESC(4, 7 + listed_count);
            printf("%2d : ", i);
            resetESC();
            printf("%f", atof(src[i].str));

            // Labelが設定されていれば出力
            if (strcmp(src[i].label, "") != 0)
            {
                printf(" (%s)", src[i].label);
            }

            listed_count++;
        }
    }
}

void drawPC(int pc)
{
    setCursorESC(ws.ws_col / 2, 3);
    resetESC();
    setBoldESC();
    setUnderlineESC();
    printf("Program Counter");

    resetESC();
    setCursorESC(ws.ws_col / 2 + 2, 4);
    printf("%2d", pc);
}

void drawBreakpoint(int bp, int maxLine)
{
    if (bp != -1)
    {
        setCursorESC(ws.ws_col / 2 + 17, 3);
        resetESC();
        setBoldESC();
        setUnderlineESC();
        printf("Breakpoint");

        resetESC();
        setCursorESC(ws.ws_col / 2 + +19, 4);
        printf("%2d", bp);

        // breakpointが有効範囲外だった場合は警告
        if (bp > maxLine)
        {
            printf(" ");
            setRevESC();
            printf("\e[31m>OUTofCODE<\e[0m");
        }
    }
}

void drawCodeList(int pc, CODE *src, int maxLine)
{
    int listed_count = 0;

    setCursorESC(ws.ws_col / 2, 6);
    resetESC();
    setBoldESC();
    setUnderlineESC();
    printf("Assembly Code");

    resetESC();
    for (int i = 0; i <= maxLine; i++)
    {
        resetESC();
        setCursorESC(ws.ws_col / 2 + 2, 7 + listed_count);

        // オペコードの存在するプログラム行を出力
        if (!src[i].isNum)
        {
            // PCが指している行をハイライト
            if (i == pc)
            {
                setRevESC();
            }

            setBoldESC();
            printf("%2d : ", i);
            resetESC();

            if (i == pc)
            {
                setRevESC();
            }

            printf("%s", src[i].str);

            listed_count++;
        }
    }
}

void setCursorESC(int x, int y)
{
    printf("\033[%d;%dH", y, x);
}

void setRevESC()
{
    printf("\033[7m");
}

void setBoldESC()
{
    printf("\033[1m");
}

void setUnderlineESC()
{
    printf("\033[4m");
}

void resetESC()
{
    printf("\033[0m");
}

void clearScreenESC()
{
    printf("\033[2J");
}
