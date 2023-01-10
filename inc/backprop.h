#ifndef BACKPROP_H
#define BACKPROP_H
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#define ROW 9
#define COLUMN 9
#define player1 "\033[1;34mPlayer1\033[m"
#define player2 "\033[1;31mPlayer2\033[m"

//list
int i, j;
int top = -1;//-1:空的
int tie = 0;//1:和局

typedef struct piece {
    int id;
    int color;
}Piece;

typedef struct item{
    Piece p0;//移動的棋子
    Piece p1;//目的地的棋子
    int position0[2];//position0:原來的位置，position0[0]:y, position0[1]:x
    int position1[2];//position1:後來的位置，position1[0]:y, position1[1]:x
    struct item *next;
    struct item *prev;
}Item;

Item *stack = NULL;

int empty();
void push(void *voidptr0, void *voidptr1, int xa, int ya, int xb, int yb);
void pushold(Item *data);
Item pop();

//move
short IsStandard = 1;//是否符合規則，初始值1，符合
short restart = 0;//是否重新輸入，1:是
short WrongPlayer = 0;//1: 下對方的棋
char regret[2];//是否悔棋
char xi;//輸入的x座標
int xj, yi, yj;//輸入的座標
int x0, x1, Y0, Y1;
short turn;//1:player1, -1:player2
int Write = 0;
int amount = 1;

Item *tmp;
Item pdata;
Item *savefile = NULL;//記錄存檔位置

Item *newnext;

FILE *fp;

void BlueMove();
void RedMove();
void RulesOfAllKindsOfPiece();

enum name {
    osho = 1, hisha, kakugyou, kinshou, ginshou, keima, kyousha, fuhyo
};
enum player {
    blue = 1, red
};

Piece board[ROW][COLUMN];//棋盤

void BlueMove(){
    if(restart){
        if(WrongPlayer == 1){
            printf("\033[1;33m不可以下別人的棋\033[m\n");
            WrongPlayer = 0;
            IsStandard = 1;
            restart = 0;    
        }else{
            printf("\033[1;33m違反規則，請從新輸入\033[m\n");
            restart = 0;
        }
        
    }
    printf("%s請輸入你要移動的棋子:(若%s想悔棋請輸入0，存檔則輸入s或S)\n", player1, player2);
    scanf(" %c", &xi);
    if(xi - '0' == 0){
        //悔棋
        printf("是否悔棋?[y/n]:");//是否悔棋
        scanf("%s", regret);
        if(regret[0] == 'y'){
            if(empty()){
                printf("\033[1;33m已為初始狀態，無法悔棋!\033[m\n");
                turn *= -1;
            }else{
                // if(stack == savefile){
                //     savefile = savefile->next;
                //     amount--;
                //     fseek(fp, -17, SEEK_CUR);
                // }
                pdata = pop();
                board[pdata.position0[0]][pdata.position0[1]] = pdata.p0;
                board[pdata.position1[0]][pdata.position1[1]] = pdata.p1;
                
            }
        }else{
            printf("%s請輸入你要移動的棋子:\n", player1);
            scanf(" %c %d", &xi, &yi);
            if(((xi - '0') > COLUMN) || (yi > ROW) || ((xi - '0') < 1) || (yi < 1)){
                restart = 1;
            }
            Y0 = yi - 1;
            x0 = 9 - (xi - '0');
            if(board[Y0][x0].color != blue){
                WrongPlayer = 1;
            }
            printf("%s請輸入你要放的位置\n", player1);
            scanf("%d %d", &xj, &yj);
            if((xj > COLUMN) || (yj > ROW) || (xj < 1) || (yj < 1)){
                restart = 1;
            }
            Y1 = yj - 1;
            x1 = 9 - xj;
            RulesOfAllKindsOfPiece();
        }
    }else{
        //存檔
        if(xi == 's' || xi == 'S'){
            tmp = stack;
            while(tmp->next != NULL){
                tmp = tmp->next;
            }
            fseek(fp, 0, SEEK_SET);
            while(tmp->prev != NULL){
                //amount++;
                fprintf(fp, "%d %d %d %d %d %d %d %d %d\n", amount, tmp->p0.id, tmp->p0.color, tmp->p1.id, tmp->p1.color, tmp->position0[0], tmp->position0[1], tmp->position1[0], tmp->position1[1]);
                tmp = tmp->prev;
                // printf("Write = %d\n", Write);
            }
            //amount++;
            fprintf(fp, "%d %d %d %d %d %d %d %d %d\n", amount, tmp->p0.id, tmp->p0.color, tmp->p1.id, tmp->p1.color, tmp->position0[0], tmp->position0[1], tmp->position1[0], tmp->position1[1]);
            //savefile = stack;

            printf("\033[1;33m儲存成功!\033[m!\n");
            printf("%s請輸入你要移動的棋子:\n", player1);
            scanf(" %c %d",&xi, &yi);
            if(((xi - '0') > COLUMN) || (yi > ROW) || ((xi - '0') < 1) || (yi < 1)){
                restart = 1;
            }
            Y0 = yi - 1;
            x0 = 9 - (xi - '0');
            if(board[Y0][x0].color != blue){
                WrongPlayer = 1;
            }
            printf("%s請輸入你要放的位置\n", player1);
            scanf("%d %d", &xj, &yj);
            if((xj > COLUMN) || (yj > ROW) || (xj < 1) || (yj < 1)){
                restart = 1;
            }
            Y1 = yj - 1;
            x1 = 9 - xj;
            RulesOfAllKindsOfPiece();
        }else{
            scanf("%d", &yi);
            if(((xi - '0') > COLUMN) || (yi > ROW) || ((xi - '0') < 1) || (yi < 1)){
                restart = 1;
            }
            Y0 = yi - 1;
            x0 = 9 - (xi - '0');
            if(board[Y0][x0].color != blue){
                WrongPlayer = 1;
            }
            printf("%s請輸入你要放的位置\n", player1);
            scanf("%d %d", &xj, &yj);
            if((xj > COLUMN) || (yj > ROW) || (xj < 1) || (yj < 1)){
                restart = 1;
            }
            Y1 = yj - 1;
            x1 = 9 - xj;
            RulesOfAllKindsOfPiece();
        }        
    }
}

void RedMove(){
    if(restart){
        if(WrongPlayer == 1){
            printf("\033[1;33m不可以下別人的棋\033[m\n");
            WrongPlayer = 0;
            restart = 0;    
        }else{
            printf("\033[1;33m違反規則，請從新輸入\033[m\n");
            restart = 0;
        }
    }
    printf("%s請輸入你要移動的棋子:(若%s想悔棋請輸入0，存檔則輸入s或S)\n", player2, player1);
    scanf(" %c", &xi);
    if((xi - '0') == 0){
        //悔棋
        printf("是否悔棋?[y/n]:");//是否悔棋
        scanf("%s", regret);
        if(regret[0] == 'y'){
            if(empty()){
                printf("\033[1;33m已為初始狀態，無法悔棋!\033[m\n");
                turn *= -1;
            }else{
                // if(stack == savefile){
                //     savefile = savefile->next;
                //     amount--;
                //     fseek(fp, -17, SEEK_CUR);
                // }
                pdata = pop();
                board[pdata.position0[0]][pdata.position0[1]] = pdata.p0;
                board[pdata.position1[0]][pdata.position1[1]] = pdata.p1;
                
            }
        }else{
            printf("%s請輸入你要移動的棋子:\n", player2);
            scanf(" %c %d",&xi, &yi);
            if(((xi - '0') > COLUMN) || (yi > ROW) || ((xi - '0') < 1) || (yi < 1)){
                restart = 1;
            }
            Y0 = yi - 1;
            x0 = 9 - (xi - '0');
            if(board[Y0][x0].color != red){
                WrongPlayer = 1;
            }
            printf("%s請輸入你要放的位置\n", player2);
            scanf("%d %d", &xj, &yj);
            if((xj > COLUMN) || (yj > ROW) || (xj < 1) || (yj < 1)){
                restart = 1;
            }
            Y1 = yj - 1;
            x1 = 9 - xj;
            RulesOfAllKindsOfPiece();
        }
    }else{
        if(xi == 's' || xi == 'S'){
            //存檔
            tmp = stack;
            while(tmp->next != NULL){
                tmp = tmp->next;
            }
            fseek(fp, 0, SEEK_SET);
            while(tmp->prev != NULL){
                fprintf(fp, "%d %d %d %d %d %d %d %d %d\n", amount, tmp->p0.id, tmp->p0.color, tmp->p1.id, tmp->p1.color, tmp->position0[0], tmp->position0[1], tmp->position1[0], tmp->position1[1]);
                tmp = tmp->prev;
            }
            fprintf(fp, "%d %d %d %d %d %d %d %d %d\n", amount, tmp->p0.id, tmp->p0.color, tmp->p1.id, tmp->p1.color, tmp->position0[0], tmp->position0[1], tmp->position1[0], tmp->position1[1]);
        
            printf("\033[1;33m儲存成功!\033[m!\n");

            printf("%s請輸入你要移動的棋子:\n", player2);
            scanf(" %c %d",&xi, &yi);
            if(((xi - '0') > COLUMN) || (yi > ROW) || ((xi - '0') < 1) || (yi < 1)){
                restart = 1;
            }
            Y0 = yi - 1;
            x0 = 9 - (xi - '0');
            if(board[Y0][x0].color != red){
                WrongPlayer = 1;
            }
            printf("%s請輸入你要放的位置\n", player2);
            scanf("%d %d", &xj, &yj);
            if((xj > COLUMN) || (yj > ROW) || (xj < 1) || (yj < 1)){
                restart = 1;
            }
            Y1 = yj - 1;
            x1 = 9 - xj;
            RulesOfAllKindsOfPiece();
        }else{
            scanf("%d", &yi);
            if(((xi - '0') > COLUMN) || (yi > ROW) || ((xi - '0') < 1) || (yi < 1)){
                restart = 1;
            }
            Y0 = yi - 1;
            x0 = 9 - (xi - '0');
            if(board[Y0][x0].color != red){
                WrongPlayer = 1;
            }
            printf("%s請輸入你要放的位置\n", player2);
            scanf("%d %d", &xj, &yj);
            if((xj > COLUMN) || (yj > ROW) || (xj < 1) || (yj < 1)){
                restart = 1;
            }
            Y1 = yj - 1;
            x1 = 9 - xj;
            RulesOfAllKindsOfPiece();
        }
    }
}

void RulesOfAllKindsOfPiece(){
    if(WrongPlayer){
        restart = 1;
    }
    //王將
    else if(board[Y0][x0].id == osho){
        if (((abs(x0 - x1) == 1) && (abs(Y0 - Y1) == 1)) || ((abs(x0 - x1) == 1) && (Y0 == Y1)) || ((x0 == x1) && (abs(Y0 - Y1) == 1))) {
            if((board[Y0][x0].color == blue) && (board[Y1][x1].color != blue)){
                push(&(board[Y0][x0]), &(board[Y1][x1]), x0, Y0, x1, Y1);
                
                board[Y1][x1] = board[Y0][x0];
                board[Y0][x0].id = 0;
                board[Y0][x0].color = 0;
            }else if((board[Y0][x0].color == red) && (board[Y1][x1].color != red)){
                push(&(board[Y0][x0]), &(board[Y1][x1]), x0, Y0, x1, Y1);

                board[Y1][x1] = board[Y0][x0];
                board[Y0][x0].id = 0;
                board[Y0][x0].color = 0;
            }else{
                restart = 1;
            }
        }
        else {
            restart = 1;
        }
    }
    //飛車
    else if(board[Y0][x0].id == hisha){
        if(x0 == x1){//直著走
            printf("2\n");
            for(i = Y0 + 1; i < Y1; i++){
                if(board[i][x0].id != 0){
                    IsStandard = 0;
                }
            }
            for(i = Y0 - 1; i > Y1; i--){
                if(board[i][x0].id != 0){
                    IsStandard = 0;
                }
            }
        }else if(Y0 == Y1){//橫著走
            for(i = x0 + 1; i < x1; i++){
                if(board[Y0][i].id != 0){
                    IsStandard = 0;
                }
            }
            for(i = x0 - 1; i > x1; i--){
                if(board[Y0][i].id != 0){
                    IsStandard = 0;
                }
            }
        }else{
            IsStandard = 0;
        }

        if((board[Y0][x0].color == blue) && IsStandard && (board[Y1][x1].color != blue)){
            push(&(board[Y0][x0]), &(board[Y1][x1]), x0, Y0, x1, Y1);

            board[Y1][x1] = board[Y0][x0];
            board[Y0][x0].id = 0;
            board[Y0][x0].color = 0;
        }else if((board[Y0][x0].color == red) && IsStandard && (board[Y1][x1].color != red)){
            push(&(board[Y0][x0]), &(board[Y1][x1]), x0, Y0, x1, Y1);
            
            board[Y1][x1] = board[Y0][x0];
            board[Y0][x0].id = 0;
            board[Y0][x0].color = 0;
        }else{
            restart = 1;
        }
    }
    //角形
    else if(board[Y0][x0].id == kakugyou){
        if(x1 == x0){
            restart = 1;
        }else{
            if(((float)Y1 - (float)Y0) / ((float)x1 - (float)x0) == 1){
                for(i = x0 + 1, j = Y0 + 1; i < x1; i++, j++){
                    if(board[j][i].id != 0){
                        IsStandard = 0;
                    }
                }

                for(i = x0 - 1, j = Y0 - 1; i > x1; i--, j--){
                    if(board[j][i].id != 0){
                        IsStandard = 0;
                    }
                }
            }else if(((float)Y1 - (float)Y0) / ((float)x1 - (float)x0) == -1){
                for(i = x0 + 1, j = Y0 - 1; i < x1; i++, j--){
                    if(board[j][i].id != 0){
                        IsStandard = 0;
                    }
                }

                for(i = x0 - 1, j = Y0 + 1; i > x1; i--, j++){
                    if(board[j][i].id != 0){
                        IsStandard = 0;
                    }
                }
            }else{
                IsStandard = 0;
            }

            if((board[Y0][x0].color == blue) && IsStandard && (board[Y1][x1].color != blue)){
                push(&(board[Y0][x0]), &(board[Y1][x1]), x0, Y0, x1, Y1);
                
                board[Y1][x1] = board[Y0][x0];
                board[Y0][x0].id = 0;
                board[Y0][x0].color = 0;
            }else if((board[Y0][x0].color == red) && IsStandard && (board[Y1][x1].color != red)){
                push(&(board[Y0][x0]), &(board[Y1][x1]), x0, Y0, x1, Y1);
                
                board[Y1][x1] = board[Y0][x0];
                board[Y0][x0].id = 0;
                board[Y0][x0].color = 0;
            }else{
                restart = 1;
            }
        }
    }
    //金將
    else if(board[Y0][x0].id == kinshou){
        if(board[Y0][x0].color == blue){
            if((((abs(x1 - x0) == 1) && (Y1 == Y0)) || ((abs(x1 - x0) == 1) && (Y0 - Y1 == 1)) || ((abs(Y1 - Y0) == 1) && (x1 == x0))) && (board[Y1][x1].color != blue)){
                push(&(board[Y0][x0]), &(board[Y1][x1]), x0, Y0, x1, Y1);
                
                board[Y1][x1] = board[Y0][x0];
                board[Y0][x0].id = 0;
                board[Y0][x0].color = 0;
            }else{
                restart = 1;
            }
        }else if(board[Y0][x0].color == red){
            if((((abs(x1 - x0) == 1) && (Y1 == Y0)) || ((abs(x1 - x0) == 1) && (Y0 - Y1 == -1)) || ((abs(Y1 - Y0) == 1) && (x1 == x0))) && (board[Y1][x1].color != red)){
                push(&(board[Y0][x0]), &(board[Y1][x1]), x0, Y0, x1, Y1);
                
                board[Y1][x1] = board[Y0][x0];
                board[Y0][x0].id = 0;
                board[Y0][x0].color = 0;
            }else{
                restart = 1;
            }
        }
    }
    //銀將
    else if(board[Y0][x0].id == ginshou){
        if (((abs(x0 - x1) == 1 && (abs(Y0 - Y1) == 1)) || ((x0 = x1) && (Y0 - Y1 == -1))) && (board[Y0][x0].color == red)){
            if(board[Y1][x1].color != red){
                push(&(board[Y0][x0]), &(board[Y1][x1]), x0, Y0, x1, Y1);
                
                board[Y1][x1] = board[Y0][x0];
                board[Y0][x0].id = 0;
                board[Y0][x0].color = 0;
            }else{
                restart = 1;
            }
        }else if(((abs(x0 - x1) == 1 && (abs(Y0 - Y1) == 1)) || ((x0 = x1) && (Y0 - Y1 == 1))) && (board[Y0][x0].color == blue)){
            if(board[Y1][x1].color != blue){
                push(&(board[Y0][x0]), &(board[Y1][x1]), x0, Y0, x1, Y1);
                
                board[Y1][x1] = board[Y0][x0];
                board[Y0][x0].id = 0;
                board[Y0][x0].color = 0;
            }else{
                restart = 1;
            }
        }else{
            restart = 1;
        }
    }
    //桂馬
    else if(board[Y0][x0].id == keima){
        if ((abs(x0 - x1) == 1) && (Y1 - Y0 == 2) && (board[Y0][x0].color == red)){
            if(board[Y1][x1].color != red){
                push(&(board[Y0][x0]), &(board[Y1][x1]), x0, Y0, x1, Y1);
                
                board[Y1][x1] = board[Y0][x0];
                board[Y0][x0].id = 0;
                board[Y0][x0].color = 0;
            }else{
                restart = 1;
            }
        }else if((abs(x0 - x1) == 1) && (Y1 - Y0 == -2) && (board[Y0][x0].color == blue)){
            if(board[Y1][x1].color != blue){
                push(&(board[Y0][x0]), &(board[Y1][x1]), x0, Y0, x1, Y1);
                
                board[Y1][x1] = board[Y0][x0];
                board[Y0][x0].id = 0;
                board[Y0][x0].color = 0;
            }else{
                restart = 1;
            }
        }else{
            restart = 1;
        }
    }
    //香車
    else if(board[Y0][x0].id == kyousha){
        if((x0 == x1)){
            if((board[Y0][x0].color == blue) && (Y0 > Y1)){
                for(i = Y0 - 1; i > Y1; i--){
                    if(board[i][x0].id != 0){
                        IsStandard = 0;
                    }
                }
                
                if((board[Y1][x1].color != blue) && IsStandard){
                    push(&(board[Y0][x0]), &(board[Y1][x1]), x0, Y0, x1, Y1);
                    
                    board[Y1][x1] = board[Y0][x0];
                    board[Y0][x0].id = 0;
                    board[Y0][x0].color = 0;
                }else{
                    restart = 1;
                }
            }else if((board[Y0][x0].color == red) && (Y0 < Y1)){
                for(i = Y0 + 1; i < Y1; i++){
                    if(board[i][x0].id != 0){
                        IsStandard = 0;
                    }
                }

                if((board[Y1][x1].color != red) && IsStandard){
                    push(&(board[Y0][x0]), &(board[Y1][x1]), x0, Y0, x1, Y1);
                    
                    board[Y1][x1] = board[Y0][x0];
                    board[Y0][x0].id = 0;
                    board[Y0][x0].color = 0;
                }else{
                    restart = 1;
                }
            }else{
                restart = 1;
            }
        }else{
            restart = 1;
        }
    }
    //步兵
    else if(board[Y0][x0].id == fuhyo){
        if(x1 == x0){
            if((board[Y0][x0].color == blue) && (Y0 - Y1 == 1)){
                if(board[Y1][x1].color != blue){
                    push(&(board[Y0][x0]), &(board[Y1][x1]), x0, Y0, x1, Y1);
                    
                    board[Y1][x1] = board[Y0][x0];
                    board[Y0][x0].id = 0;
                    board[Y0][x0].color = 0;
                }else{
                    restart = 1;
                }
            }else if((board[Y0][x0].color == red) && (Y1 - Y0 == 1)){
                if(board[Y1][x1].color != red){
                    push(&(board[Y0][x0]), &(board[Y1][x1]), x0, Y0, x1, Y1);
                    
                    board[Y1][x1] = board[Y0][x0];
                    board[Y0][x0].id = 0;
                    board[Y0][x0].color = 0;
                }else{
                    restart = 1;
                }
            }else{
                restart = 1;
            }
        }else{
            restart = 1;
        }
    }
    //空白
    else{
        restart = 1;
    }
}

//list


int empty(){
    if(stack == NULL){
        return 1;
    }else{
        return 0;
    }
}

void push(void *voidptr0, void *voidptr1, int xa, int ya, int xb, int yb){
    if(empty()){
        stack = (Item *) malloc (sizeof(Item));
        stack->p0.id = ((Piece*)voidptr0)->id;
        stack->p0.color = ((Piece*)voidptr0)->color;
        stack->p1.id = ((Piece*)voidptr1)->id;
        stack->p1.color = ((Piece*)voidptr1)->color;
        stack->position0[0] = ya;
        stack->position0[1] = xa;
        stack->position1[0] = yb;
        stack->position1[1] = xb;
        stack->next = stack->prev = NULL; 
    }else{
        Item *tmp;
        tmp = (Item *) malloc (sizeof(Item));
        tmp->p0.id = ((Piece*)voidptr0)->id;
        tmp->p0.color = ((Piece*)voidptr0)->color;
        tmp->p1.id = ((Piece*)voidptr1)->id;
        tmp->p1.color = ((Piece*)voidptr1)->color;
        tmp->position0[0] = ya;
        tmp->position0[1] = xa;
        tmp->position1[0] = yb;
        tmp->position1[1] = xb;
        stack->prev = tmp;
        tmp->next = stack;
        tmp->prev = NULL;
        stack = tmp;
    }
}

void pushold(Item *data){
    if(empty()){
        stack = (Item *) malloc (sizeof(Item));
        stack->p0.id = data->p0.id;
        stack->p0.color = data->p0.color;
        stack->p1.id = data->p1.id;
        stack->p1.color = data->p1.color;
        stack->position0[0] = data->position0[0];
        stack->position0[1] = data->position0[1];
        stack->position1[0] = data->position1[0];
        stack->position1[1] = data->position1[1];
        stack->next = stack->prev = NULL; 
    }else{
        Item *tmp;
        tmp = (Item *) malloc (sizeof(Item));
        tmp->p0.id = data->p0.id;
        tmp->p0.color = data->p0.color;
        tmp->p1.id = data->p1.id;
        tmp->p1.color = data->p1.color;
        tmp->position0[0] = data->position0[0];
        tmp->position0[1] = data->position0[1];
        tmp->position1[0] = data->position1[0];
        tmp->position1[1] = data->position1[1];
        stack->prev = tmp;
        tmp->next = stack;
        tmp->prev = NULL;
        stack = tmp;
    }
}

Item pop(){
    Item *tmp;
    Item data;
    if(empty() != 1){
        tmp = stack;
        if(stack->next == NULL){
            stack = NULL;
        }else{
            stack = stack->next;
            stack->prev = NULL;
        }
        data.p0 = tmp->p0;
        data.p1 = tmp->p1;
        data.position0[0] = tmp->position0[0];
        data.position0[1] = tmp->position0[1];
        data.position1[0] = tmp->position1[0];
        data.position1[1] = tmp->position1[1];
        free(tmp);    
            
    }
    return data;
}



#endif