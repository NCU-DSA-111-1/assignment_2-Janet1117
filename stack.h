#ifndef STACK_H
#define STACK_H
#include<stdio.h>
#include<stdlib.h>
#define MAXNUMBER 500

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
}Item;

Item stack[MAXNUMBER];

int IsEmpty();//1:空的，0:有東西
int IsFull();//1:塞滿，0:還有空間
void InitializeStack(Item *s);//初始化stack
void *pop();
void push(void *voidptr0, void *voidptr1, int xa, int ya, int xb, int yb);

void InitializeStack(Item *s){
    for(i = 0; i < MAXNUMBER; i++){
        s[i].p0.color = 0;
        s[i].p0.id = 0;
        s[i].p1.color = 0;
        s[i].p1.id = 0;
        s[i].position0[0] = 0;
        s[i].position0[1] = 0;
        s[i].position1[0] = 0;
        s[i].position1[1] = 0;
    }
}

int IsEmpty(){
    if(top == -1){
        return 1;
    }else{
        return 0;
    }
}

int IsFull(){
    if(top == MAXNUMBER - 1){
        return 1;
    }else{
        return 0;
    }
}

void *pop(){
    void *data;
    if(IsEmpty() != 1){
        data = &(stack[top]);
        top--;
        return data;
    }
}

void push(void *voidptr0, void *voidptr1, int xa, int ya, int xb, int yb){
    if(IsFull()){
        tie = 1;
    }else{
        top++;
        stack[top].p0.id = ((Piece*)voidptr0)->id;
        stack[top].p0.color = ((Piece*)voidptr0)->color;
        stack[top].p1.id = ((Piece*)voidptr1)->id;
        stack[top].p1.color = ((Piece*)voidptr1)->color;
        stack[top].position0[0] = ya;
        stack[top].position0[1] = xa;
        stack[top].position1[0] = yb;
        stack[top].position1[1] = xb;
    }
}

#endif