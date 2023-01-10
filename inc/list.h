#ifndef LIST_H
#define LIST_H
#include<stdio.h>
#include<stdlib.h>


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
//void regret();

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
        return data;    
    }
}


#endif