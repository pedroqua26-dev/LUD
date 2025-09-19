#include "lud.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_OUTPUT 65536
#define MAX_VARS 100
#define MAX_OBJS 50

typedef struct {
    char name[32];
    int value;
} Variable;

typedef struct {
    char name[32];
    int x,y;
} Object;

Variable vars[MAX_VARS];
int varCount = 0;

Object objs[MAX_OBJS];
int objCount = 0;

void setVar(const char* name, int value){
    for(int i=0;i<varCount;i++){
        if(strcmp(vars[i].name,name)==0){vars[i].value=value; return;}
    }
    strcpy(vars[varCount].name,name);
    vars[varCount].value=value;
    varCount++;
}

int getVar(const char* name){
    for(int i=0;i<varCount;i++){ if(strcmp(vars[i].name,name)==0) return vars[i].value;}
    return 0;
}

void createObject(const char* name){
    if(objCount<MAX_OBJS){
        strcpy(objs[objCount].name,name);
        objs[objCount].x = 0; objs[objCount].y = 0;
        objCount++;
    }
}

void moveObject(const char* name, int dx, int dy){
    for(int i=0;i<objCount;i++){
        if(strcmp(objs[i].name,name)==0){
            objs[i].x += dx;
            objs[i].y += dy;
        }
    }
}

char* run_from_memory(const char* code){
    static char output[MAX_OUTPUT];
    output[0]='\0';
    char temp[65536];
    strcpy(temp,code);
    char* token=strtok(temp,";");

    while(token){
        if(strcmp(token,"PRINT_HELLO")==0) strcat(output,"Olá!\n");
        else if(strcmp(token,"PRINT_BYE")==0) strcat(output,"Tchau!\n");
        else if(strcmp(token,"PRINT_YAY")==0) strcat(output,"Yay!\n");
        else if(strcmp(token,"PRINT_OHNO")==0) strcat(output,"Oh No!\n");

        else if(strcmp(token,"ADD_1_2")==0) strcat(output,"1 + 2 = 3\n");
        else if(strcmp(token,"SUB_5_3")==0) strcat(output,"5 - 3 = 2\n");
        else if(strcmp(token,"MUL_2_3")==0) strcat(output,"2 * 3 = 6\n");
        else if(strcmp(token,"DIV_6_2")==0) strcat(output,"6 / 2 = 3\n");
        else if(strcmp(token,"SQUARE_5")==0) strcat(output,"5^2 = 25\n");
        else if(strcmp(token,"SQRT_16")==0) strcat(output,"√16 = 4\n");
        else if(strcmp(token,"PERCENT_50_OF_200")==0) strcat(output,"50% de 200 = 100\n");
        else if(strcmp(token,"INC_X")==0){ setVar("X", getVar("X")+1); strcat(output,"Incrementou X\n");}
        else if(strcmp(token,"DEC_X")==0){ setVar("X", getVar("X")-1); strcat(output,"Decrementou X\n");}

        else if(strncmp(token,"SET_",4)==0){
            char name[32]; int value;
            sscanf(token,"SET_%[^_]_%d",name,&value);
            setVar(name,value);
            strcat(output,"Variável definida\n");
        }
        else if(strncmp(token,"GET_",4)==0){
            char name[32];
            sscanf(token,"GET_%s",name);
            char buf[64];
            sprintf(buf,"%d\n",getVar(name));
            strcat(output,buf);
        }

        else if(strncmp(token,"IF_",3)==0){
            char var[32],cmd[64]; int cmp;
            sscanf(token,"IF_%[^_]_EQ_%d_%s",var,&cmp,cmd);
            if(getVar(var)==cmp){
                strcat(output,"Condição verdadeira: ");
                strcat(output,cmd); strcat(output,"\n");
            }
        }

        else if(strncmp(token,"LOOP_",5)==0){
            int n; char cmd[64];
            sscanf(token,"LOOP_%d_%s",&n,cmd);
            for(int i=0;i<n;i++){
                strcat(output,"Loop executou: "); strcat(output,cmd); strcat(output,"\n");
            }
        }

        else if(strcmp(token,"DRAW_CIRCLE")==0) strcat(output,"Desenhou um círculo\n");
        else if(strcmp(token,"DRAW_SQUARE")==0) strcat(output,"Desenhou um quadrado\n");
        else if(strcmp(token,"DRAW_TRIANGLE")==0) strcat(output,"Desenhou um triângulo\n");
        else if(strcmp(token,"DRAW_RECT")==0) strcat(output,"Desenhou um retângulo\n");
        else if(strcmp(token,"DRAW_LINE")==0) strcat(output,"Desenhou uma linha\n");

        else if(strcmp(token,"BOOL_TRUE")==0) strcat(output,"Verdadeiro\n");
        else if(strcmp(token,"BOOL_FALSE")==0) strcat(output,"Falso\n");

        else if(strcmp(token,"TEXT_HELLO")==0) strcat(output,"Texto: Hello\n");
        else if(strcmp(token,"TEXT_WORLD")==0) strcat(output,"Texto: World\n");

        else if(strcmp(token,"RAND_0_10")==0){
            char buf[32];
            sprintf(buf,"%d\n",rand()%11);
            strcat(output,buf);
        }

        else if(strncmp(token,"CREATE_OBJ_",11)==0){
            char name[32];
            sscanf(token,"CREATE_OBJ_%s",name);
            createObject(name);
            strcat(output,"Objeto criado\n");
        }
        else if(strncmp(token,"MOVE_OBJ_",9)==0){
            char name[32]; int dx, dy;
            sscanf(token,"MOVE_OBJ_%[^_]_%d_%d",name,&dx,&dy);
            moveObject(name,dx,dy);
            strcat(output,"Objeto movido\n");
        }

        else strcat(output,"Comando desconhecido\n");

        token=strtok(NULL,";");
    }
    return output;
}
