#include "tinyCPUcore.h"

int drawDebugger(char *fileName, CODE *src, int maxLine,
                  int pc, int bp, float ariReg, char *PRINTOutStr);
void drawHeadBar(char *fileName);
void drawPRINTOut(char *PRINTOutStr);
void drawFooter();
void drawPrompt();
void drawArithReg(float ariReg);
void drawMemoryList(CODE *src, int maxLine);
void drawCodeList(int pc, CODE *src, int maxLine);
void drawPC(int pc);
void drawBreakpoint(int bp, int maxLine);

// ESC character drawer
void setCursorESC(int x, int y);
void setRevESC();
void setBoldESC();
void setUnderlineESC();
void resetESC();
void clearScreenESC();