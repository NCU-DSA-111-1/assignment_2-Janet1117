#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<ev.h>
#include<time.h>
#include"../inc/move.h"
#define B(Piece) "\033[1;34m"#Piece"\033[m"
#define R(Piece) "\033[1;31m"#Piece"\033[m"

/*棋盤長這樣:
先段(行)再筋(列)
 ９ ８ ７ ６ ５ ４ ３ ２ １
|香 桂 銀 金 玉 金 銀 桂 香|1
|　 飛 　 　 　 　 　 角 　|2
|步 步 步 步 步 步 步 步 步|3
|　 　 　 　 　 　 　 　 　|4
|　 　 　 　 　 　 　 　 　|5
|　 　 　 　 　 　 　 　 　|6
|步 步 步 步 步 步 步 步 步|7
|　 角 　 　 　 　 　 飛 　|8
|香 桂 銀 金 玉 金 銀 桂 香|9
*/

void initialize();
void printboard();
void IsGameOver();
void FOrB();

short WhoWin = 0;//0: 遊戲繼續 1:player1 win, -1:player2 win 
int l_top = -1;
int counter = 0;//計txt裡有多少筆資料
char fb[2];
int end = 0;//1:離開棋譜
char save[2];//是否存檔

int NewOrOld = -1;//1:開新檔，0:開舊檔
char *filename;//檔名



Item *tmpnext = NULL;

int main(int argc, char **argv) {
    
    char n[] = "-n";//-n
    char s[] = "-s";//-s
    char l[] = "-l";//-l
    if(argc == 4){//除程式本身外還有三個參數
        if(strncmp(argv[1], n, 2) == 0){
            if(strncmp(argv[2], s, 2) == 0){
                filename = argv[3];
                if((fp = fopen(filename, "w")) == NULL){
                    printf("\033[1;31m無法創建檔案!\033[m");
                }else{

                    initialize();
                    // InitializeStack(stack);
                    while(WhoWin == 0 && tie == 0){
                        IsStandard = 1;
                        printboard();
                        switch(turn){
                            //輪流下棋
                            case 1://藍方
                                BlueMove();
                                turn = (restart) ? turn : (turn * (-1));
                                break;
                            case -1://紅方
                                RedMove();
                                turn = (restart) ? turn : (turn * (-1));
                                break;
                        }

                        IsGameOver();
                    }
                    printf("遊戲結束，是否存檔?[y/n]:");
                    scanf("%s", save);
                    if(save[0] == 'y'){
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
                    }
                    printf("遊戲結束，");
                    if(WhoWin == 1){
                        printf("\033[1;34mPlayer1\033[m 獲勝!\n");
                    }else if(WhoWin == -1){
                        printf("\033[1;31mPlayer2\033[m 獲勝!\n");
                    }else if(tie == 1){
                        printf("和局\n");
                    }
                }
                fprintf(fp, "%d", 0);
                fclose(fp);
            }
            else{
                printf("Error!\n");
            }
        }
        else{
            printf("Error!\n");
        }
    
    }
    else if(argc == 3){//除程式本身外還有兩個參數
        if(strncmp(argv[1], l, 2) == 0){
            filename = argv[2];
            if((fp = fopen(filename, "r")) == NULL){
                printf("\033[1;31m找不到檔案!\033[m");
            }
            else{
                Item *oldtmp;
                oldtmp = (Item *) malloc (sizeof(Item));
                counter = 0;
                end = 0;
                fscanf(fp, "%d", &amount);
                while(amount != 0){
                    fscanf(fp, "%d%d%d%d%d%d%d%d", &oldtmp->p0.id, &oldtmp->p0.color, &oldtmp->p1.id, &oldtmp->p1.color, &oldtmp->position0[0], &oldtmp->position0[1], &oldtmp->position1[0], &oldtmp->position1[1]);
                    pushold(oldtmp);
                    fscanf(fp, "%d", &amount);
                    counter++;
                }
                initialize();
                
                printf("counter = %d\n", counter);
                printboard();

                tmp = stack;
                while (tmp->next != NULL){
                    tmp = tmp->next;
                }
                while(1){
                    FOrB();
                    if(end == 1){
                        break;
                    }
                    printboard();
                }
            }
        }
        else{
            printf("Error\n");
        }
        fclose(fp);
    }
    else{
        printf("Error!\n");
    }
}

void FOrB(){
    printf("輸入f移動下一手，輸入b退回上一手[f/b]:");
    scanf("%s", fb);
    if(fb[0] == 'f'){
        if((tmp == NULL) && (tmpnext != stack)){
            tmp = tmpnext;
        }
        if(tmp == NULL){
            printf("已為最後一手，是否結束?[y/n]:");
            scanf("%s", fb);
            if(fb[0] == 'y'){
                end = 1;
            }
        }else{
            board[tmp->position0[0]][tmp->position0[1]].color = 0;
            board[tmp->position0[0]][tmp->position0[1]].id = 0;
            board[tmp->position1[0]][tmp->position1[1]].color = tmp->p0.color;
            board[tmp->position1[0]][tmp->position1[1]].id = tmp->p0.id;
            if(tmp->prev == NULL){
                tmpnext = tmp;
            }
            tmp = tmp->prev;
        }
    }else if(fb[0] == 'b'){
        printf("hhhhh\n");
        if((tmp == NULL) && (tmpnext == stack)){
            tmp = stack;
        }
        if(tmp == NULL){
            printf("已為第一手，是否結束?[y/n]:\n");
            scanf("%s", fb);
            if(fb[0] == 'y'){
                end = 1;
            }
        }else{
            board[tmp->position0[0]][tmp->position0[1]].color = tmp->p0.color;
            board[tmp->position0[0]][tmp->position0[1]].id = tmp->p0.id;
            board[tmp->position1[0]][tmp->position1[1]].color = tmp->p1.color;
            board[tmp->position1[0]][tmp->position1[1]].id = tmp->p1.id;
            if(tmp->next == NULL){
                tmpnext = tmp;
            }
            tmp = tmp->next;
            
            
        }
    }else{
        printf("輸入錯誤，請從新輸入!\n");
    }
}

void initialize() {
    turn = 1;
    IsStandard = 1;
    WrongPlayer = 0;
    restart = 0;
    amount = 0;
    for (i = 0; i < ROW; i++) {
        for (j = 0; j < COLUMN; j++) {
            board[i][j].id = 0;
            board[i][j].color = 0;
        }
    }

    //香車
    for (i = 0, j = 0; j < COLUMN; j += 8) {
        board[i][j].id = kyousha;
        board[i][j].color = red;

        board[i + 8][j].id = kyousha;
        board[i + 8][j].color = blue;
    }

    //桂馬
    for (i = 0, j = 1; j < COLUMN; j += 6) {
        board[i][j].id = keima;
        board[i][j].color = red;

        board[i + 8][j].id = keima;
        board[i + 8][j].color = blue;
    }

    //銀將
    for (i = 0, j = 2; j < COLUMN; j += 4) {
        board[i][j].id = ginshou;
        board[i][j].color = red;

        board[i + 8][j].id = ginshou;
        board[i + 8][j].color = blue;
    }

    //金將
    board[0][3].id = kinshou;
    board[0][3].color = red;
    board[8][3].id = kinshou;
    board[8][3].color = blue;
    board[0][5].id = kinshou;
    board[0][5].color = red;
    board[8][5].id = kinshou;
    board[8][5].color = blue;

    //玉將
    board[0][4].id = osho;
    board[0][4].color = red;
    board[8][4].id = osho;
    board[8][4].color = blue;

    //飛車
    board[1][1].id = hisha;
    board[1][1].color = red;
    board[7][7].id = hisha;
    board[7][7].color = blue;

    //角形
    board[1][7].id = kakugyou;
    board[1][7].color = red;
    board[7][1].id = kakugyou;
    board[7][1].color = blue;

    //步兵
    for (j = 0, i = 2; j < COLUMN; j++) {
        board[i][j].id = fuhyo;
        board[i][j].color = red;

        board[i + 4][j].id = fuhyo;
        board[i + 4][j].color = blue;
    }
}

void printboard() {
    system("clear");
    printf("\n");
    printf("先輸入段(行)再輸入筋(列)\n");
    printf(" ９ ８ ７ ６ ５ ４ ３ ２ １\n");
    for(i = 0; i < COLUMN; i++){
        printf("|");
        for(j = 0; j < ROW; j++){
            if(board[i][j].id == osho){
                if(board[i][j].color == red){
                    printf("%s", R(玉));
                }else if(board[i][j].color == blue){
                    printf("%s", B(玉));
                }                
            }else if(board[i][j].id == hisha){
                if(board[i][j].color == red){
                    printf("%s", R(飛));
                }else if(board[i][j].color == blue){
                    printf("%s", B(飛));
                }
            }else if(board[i][j].id == kakugyou){
                if(board[i][j].color == red){
                    printf("%s", R(角));
                }else if(board[i][j].color == blue){
                    printf("%s", B(角));
                }
            }else if(board[i][j].id == kinshou){
                if(board[i][j].color == red){
                    printf("%s", R(金));
                }else if(board[i][j].color == blue){
                    printf("%s", B(金));
                }
            }else if(board[i][j].id == ginshou){
                if(board[i][j].color == red){
                    printf("%s", R(銀));
                }else if(board[i][j].color == blue){
                    printf("%s", B(銀));
                }
            }else if(board[i][j].id == keima){
                if(board[i][j].color == red){
                    printf("%s", R(桂));
                }else if(board[i][j].color == blue){
                    printf("%s", B(桂));
                }
            }else if(board[i][j].id == kyousha){
                if(board[i][j].color == red){
                    printf("%s", R(香));
                }else if(board[i][j].color == blue){
                    printf("%s", B(香));
                }
            }else if(board[i][j].id == fuhyo){
                if(board[i][j].color == red){
                    printf("%s", R(步));
                }else if(board[i][j].color == blue){
                    printf("%s", B(步));
                }
            }else{
                printf("　");
            }

            printf("%s",(j == COLUMN - 1)? "" : " ");
        }
        printf("|%d\n", i + 1);
    }
}

void IsGameOver(){
    short sign_r = 0;
    short sign_b = 0;
    for(i = 0; i < ROW; i++){
        for(j = 0; j < COLUMN; j++){
            if(board[i][j].id == osho){
                if(board[i][j].color == blue){
                    sign_b = 1;
                }else if(board[i][j].color == red){
                    sign_r = 1;
                }
            }
        }
    }
    if((sign_b == 0) && (sign_r == 1)){
        WhoWin = -1;
    }else if((sign_r == 0) && (sign_b == 1)){
        WhoWin = 1;
    }
    if(WhoWin == 1 || WhoWin == -1){
        
    }
}


