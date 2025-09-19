#include "lud.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

ASTNode* parse(Token* tokens, int token_count, int* node_count){
    static ASTNode nodes[MAX_AST_NODES];
    *node_count = 0;

    int i = 0;
    while(i < token_count && *node_count < MAX_AST_NODES){
        Token t = tokens[i];
        ASTNode* node = &nodes[*node_count];

        if(t.type == TOKEN_IDENTIFIER){
            if(strncmp(t.value,"DRAW_",5) == 0){
                node->type = NODE_DRAW;
                strcpy(node->value,t.value);
                node->left = NULL;
                node->right = NULL;
                node->body = NULL;
                node->body_len = 0;
                (*node_count)++;
                i++;
                continue;
            }

            if(strncmp(t.value,"SET_",4) == 0){
                node->type = NODE_ASSIGN;
                char var[32]; int val;
                sscanf(t.value,"SET_%[^_]_%d",var,&val);
                strcpy(node->value,var);
                node->left = malloc(sizeof(ASTNode));
                node->left->type = NODE_NUMBER;
                sprintf(node->left->value,"%d",val);
                node->right = NULL;
                node->body = NULL;
                node->body_len = 0;
                (*node_count)++;
                i++;
                continue;
            }

            if(strncmp(t.value,"GET_",4)==0){
                node->type = NODE_VAR;
                char var[32];
                sscanf(t.value,"GET_%s",var);
                strcpy(node->value,var);
                node->left = NULL;
                node->right = NULL;
                node->body = NULL;
                node->body_len = 0;
                (*node_count)++;
                i++;
                continue;
            }

            if(strncmp(t.value,"IF_",3)==0){
                node->type = NODE_IF;
                char var[32], cmd[64]; int cmp;
                sscanf(t.value,"IF_%[^_]_EQ_%d_%s",var,&cmp,cmd);
                strcpy(node->value,var);
                node->left = malloc(sizeof(ASTNode));
                node->left->type = NODE_NUMBER;
                sprintf(node->left->value,"%d",cmp);
                node->right = malloc(sizeof(ASTNode));
                node->right->type = NODE_DRAW;
                strcpy(node->right->value,cmd);
                node->body = NULL;
                node->body_len = 0;
                (*node_count)++;
                i++;
                continue;
            }

            if(strncmp(t.value,"LOOP_",5)==0){
                node->type = NODE_LOOP;
                int n; char cmd[64];
                sscanf(t.value,"LOOP_%d_%s",&n,cmd);
                node->left = malloc(sizeof(ASTNode));
                node->left->type = NODE_NUMBER;
                sprintf(node->left->value,"%d",n);
                node->right = malloc(sizeof(ASTNode));
                node->right->type = NODE_DRAW;
                strcpy(node->right->value,cmd);
                node->body = NULL;
                node->body_len = 0;
                (*node_count)++;
                i++;
                continue;
            }

            if(strncmp(t.value,"CREATE_OBJ_",11)==0 || strncmp(t.value,"MOVE_OBJ_",9)==0){
                node->type = NODE_UNKNOWN;
                strcpy(node->value,t.value);
                node->left = NULL; node->right=NULL;
                node->body=NULL; node->body_len=0;
                (*node_count)++;
                i++;
                continue;
            }

            node->type = NODE_UNKNOWN;
            strcpy(node->value,t.value);
            node->left=NULL; node->right=NULL; node->body=NULL; node->body_len=0;
            (*node_count)++;
            i++;
            continue;
        }

        if(t.type==TOKEN_NUMBER){
            node->type = NODE_NUMBER;
            strcpy(node->value,t.value);
            node->left=NULL; node->right=NULL; node->body=NULL; node->body_len=0;
            (*node_count)++;
            i++;
            continue;
        }

        if(t.type==TOKEN_STRING){
            node->type = NODE_STRING;
            strcpy(node->value,t.value);
            node->left=NULL; node->right=NULL; node->body=NULL; node->body_len=0;
            (*node_count)++;
            i++;
            continue;
        }

        if(t.type==TOKEN_OPERATOR){
            node->type = NODE_OPERATION;
            strcpy(node->value,t.value);
            node->left=NULL; node->right=NULL; node->body=NULL; node->body_len=0;
            (*node_count)++;
            i++;
            continue;
        }

        i++;
    }

    return nodes;
}
