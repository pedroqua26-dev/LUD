#include "lud.h"
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

Token* lex(const char* code, int* token_count){
    static Token tokens[MAX_TOKENS];
    int count = 0;
    int i = 0;

    while(code[i] != '\0' && count < MAX_TOKENS){
        if(isspace(code[i])) { i++; continue; }

        if(code[i] == '#'){
            while(code[i] != '\n' && code[i] != '\0') i++;
            continue;
        }

        if(isalpha(code[i]) || code[i]=='_'){
            int j = 0;
            while(isalnum(code[i]) || code[i]=='_'){
                tokens[count].value[j++] = code[i++];
            }
            tokens[count].value[j] = '\0';
            tokens[count].type = TOKEN_IDENTIFIER;
            count++;
            continue;
        }

        if(isdigit(code[i])){
            int j = 0;
            while(isdigit(code[i])){
                tokens[count].value[j++] = code[i++];
            }
            tokens[count].value[j] = '\0';
            tokens[count].type = TOKEN_NUMBER;
            count++;
            continue;
        }

        if(code[i] == '"'){
            i++; 
            int j = 0;
            while(code[i] != '"' && code[i] != '\0'){
                tokens[count].value[j++] = code[i++];
            }
            tokens[count].value[j] = '\0';
            tokens[count].type = TOKEN_STRING;
            if(code[i] == '"') i++; 
            count++;
            continue;
        }

        char op[3] = {0};
        op[0] = code[i];
        op[1] = code[i+1];

        if((op[0]=='=' && op[1]=='=') ||
           (op[0]=='!' && op[1]=='=') ||
           (op[0]=='<' && op[1]=='=') ||
           (op[0]=='>' && op[1]=='=')){
            tokens[count].value[0] = op[0];
            tokens[count].value[1] = op[1];
            tokens[count].value[2] = '\0';
            tokens[count].type = TOKEN_OPERATOR;
            count++;
            i += 2;
            continue;
        }

        tokens[count].value[0] = code[i];
        tokens[count].value[1] = '\0';
        tokens[count].type = TOKEN_OPERATOR;
        count++;
        i++;
    }

    *token_count = count;
    return tokens;
}
