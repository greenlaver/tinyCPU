// 1行あたりの最大文字数-1
#define MAX_LENGTH  128
// プログラムの最大行数
#define MAX_LINE    128
// 区切り文字
#define SEP_CHAR    " "

typedef enum {
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
int executeLine(int *pc);
int readCode(char *path);
OPCODE convertStrToOpcode(char *code);

void add(int address);
void subtract(int address);
void multiply(int address);
void divide(int address);
void load(int address);
void store(int address);
void jump(int *pc, int address);
void jumpzero(int *pc, int address);
void print();