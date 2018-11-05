// 1行あたりの最大文字数-1
#define CODE_MAX_LENGTH 128
// プログラムの最大行数
#define CODE_MAX_LINE 128
// 区切り文字
#define SEP_CHAR " "

typedef struct code
{
    char str[CODE_MAX_LENGTH];
} CODE;

typedef enum
{
    ADD,
    SUBTRACT,
    MULTIPLY,
    DIVIDE,
    LOAD,
    STORE,
    JUMP,
    JUMPZERO,
    PRINT,
    HALT,
    ILLEGAL_OPCODE
} OPCODE;

// Function Prototype
int executeLine(int *pc, double *ariReg, CODE *src);
OPCODE convertStrToOpcode(char *code);

void add(int address, double *ariReg, CODE *src);
void subtract(int address, double *ariReg, CODE *src);
void multiply(int address, double *ariReg, CODE *src);
void divide(int address, double *ariReg, CODE *src);
void load(int address, double *ariReg, CODE *src);
void store(int address, double ariReg, CODE *src);
void jump(int address, int *pc);
void jumpzero(int address, double ariReg, int *pc);
void print(double ariReg);
