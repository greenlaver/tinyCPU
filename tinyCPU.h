// 1行あたりの最大文字数-1
#define MAX_LENGTH  128
// プログラムの最大行数
#define MAX_LINE    128
// 区切り文字
#define SEP_CHAR    " "

// Function Prototype
int readCode(char *path);
void add(int address);
void subtract(int address);
void multiply(int address);
void divide(int address);
void load(int address);
void store(int address);
void jump(int address);
void jumpzero(int address);
void print();