#ifndef LUD_H
#define LUD_H

#define MAX_TOKENS 2048
#define MAX_TOKEN_LEN 64
#define MAX_VARS 200
#define MAX_OBJS 100
#define MAX_FUNCS 50
#define MAX_AST_NODES 1024
#define MAX_OUTPUT 65536

typedef enum {
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_STRING,
    TOKEN_KEYWORD,
    TOKEN_OPERATOR,
    TOKEN_UNKNOWN
} TokenType;

typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LEN];
} Token;

typedef enum {
    NODE_ASSIGN,
    NODE_NUMBER,
    NODE_STRING,
    NODE_VAR,
    NODE_OPERATION,
    NODE_IF,
    NODE_LOOP,
    NODE_FUNC_CALL,
    NODE_DRAW,
    NODE_UNKNOWN
} NodeType;

typedef struct ASTNode {
    NodeType type;
    char value[MAX_TOKEN_LEN];
    struct ASTNode* left;
    struct ASTNode* right;
    struct ASTNode* body; // Para loops e ifs
    int body_len;
} ASTNode;

typedef struct {
    char name[32];
    int value;
} Variable;

typedef struct {
    char name[32];
    int x,y;
} Object;

typedef struct {
    char name[32];
    ASTNode* body;
    int body_len;
} Function;

extern Variable vars[MAX_VARS];
extern int varCount;

extern Object objs[MAX_OBJS];
extern int objCount;

extern Function funcs[MAX_FUNCS];
extern int funcCount;

Token* lex(const char* code, int* token_count);
ASTNode* parse(Token* tokens, int token_count, int* node_count);
char* execute(ASTNode* nodes, int node_count);

void setVar(const char* name, int value);
int getVar(const char* name);
void createObject(const char* name);
void moveObject(const char* name, int dx, int dy);

#endif
