#include <client_registry.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "csapp.h"
#include "debug.h"
#include "maze.h"

typedef struct mastruct{
    char **contents;
    int rows;
    int columns;
    sem_t sem;
    pthread_mutex_t mutex;
} MAZE;

MAZE *mazy;
MAZE *maz;
void maze_init(char **template){
    mazy = Malloc(sizeof(MAZE));
    maz = mazy;
    mazy->rows = 0;
    mazy->columns = 0;
    int size = strlen(template[0]);
    mazy->contents = template;
    mazy->columns = size;
    int i = 0;
    while(template[i] != NULL) {
        if(strlen(template[i]) != size) {
            exit(-1);
        }
        else {
            for(int j = 0; j < size; j++) {
                if(!IS_WALL(template[i][j]) && !IS_EMPTY(template[i][j]) && !IS_AVATAR(template[i][j])) {
                    exit(-1);
                }
            }
        }
        i++;
    }
    mazy->rows = i;
    Sem_init(&(mazy->sem), 0, 0);
    pthread_mutex_init(&(mazy->mutex), NULL);
}

void maze_fini(){
    int i = 0;
    while(mazy->contents[i] != NULL) {
        Free(mazy->contents[i]);
        i++;
    }
    Free(mazy->contents);
}
int maze_get_rows(){
    return mazy->rows;
}
int maze_get_cols(){
    return mazy->columns;
}
int maze_set_player(OBJECT avatar, int row, int col){
    pthread_mutex_lock(&(maz->mutex));
    if(row > mazy->rows || row < 0 || col > mazy->columns || col < 0) {
        pthread_mutex_unlock(&(maz->mutex));
        return -1;
    }
    if((mazy->contents)[row][col] != ' ') {
        pthread_mutex_unlock(&(maz->mutex));
        return -1;
    }
    else {
        mazy->contents[row][col] = avatar;
    }
    pthread_mutex_unlock(&(maz->mutex));
    return 0;
}

int maze_set_player_random(OBJECT avatar, int *rowp, int *colp){
    pthread_mutex_lock(&(maz->mutex));

    int row = rand()%(mazy->rows);
    int col = rand()%(mazy->columns);
    int i = 0;
    while(!IS_EMPTY(mazy->contents[row][col])) {
        row = rand()%(mazy->rows);
        col = rand()%(mazy->columns);
        i++;
        if(i == 999999) {
            pthread_mutex_unlock(&(mazy->mutex));
            return -1;
        }
    }
    mazy->contents[row][col] = (char) avatar;
    *rowp = row;
    *colp = col;
    // char *str = Malloc(strlen(mazy->contents[row]));
    // strcpy(str, mazy->contents[row]);
    // //Free(mazy->contents[row]);
    // str[col] = avatar;
    // mazy->contents[row] = str;
    pthread_mutex_unlock(&(maz->mutex));
    return 0;
}
void maze_remove_player(OBJECT avatar, int row, int col){
    pthread_mutex_lock(&(maz->mutex));
    if(row < mazy->rows && col < mazy->columns) {
        if(mazy->contents[row][col] == avatar) {
            mazy->contents[row][col] = ' ';
        }
    }
    pthread_mutex_unlock(&(maz->mutex));
}
int maze_move(int row, int col, int dir){
    pthread_mutex_lock(&(maz->mutex));
    if(dir == NORTH) {
        if(IS_EMPTY(mazy->contents[row-1][col])) {
            mazy->contents[row-1][col] = mazy->contents[row][col];
            mazy->contents[row][col] = ' ';
        }
        else {
            pthread_mutex_unlock(&(maz->mutex));
            return -1;
        }
    }
    else if(dir == SOUTH) {
        if(IS_EMPTY(mazy->contents[row+1][col])) {
            mazy->contents[row+1][col] = mazy->contents[row][col];
            mazy->contents[row][col] = ' ';
        }
        else {
            pthread_mutex_unlock(&(maz->mutex));
            return -1;
        }
    }
    else if(dir == EAST) {
        if(IS_EMPTY(mazy->contents[row][col-1])) {
            mazy->contents[row][col-1] = mazy->contents[row][col];
            mazy->contents[row][col] = ' ';
        }
        else {
            pthread_mutex_unlock(&(maz->mutex));
            return -1;
        }
    }
    else
        if(IS_EMPTY(mazy->contents[row][col+1])) {
            mazy->contents[row][col+1] = mazy->contents[row][col];
            mazy->contents[row][col] = ' ';
        }
        else {
            pthread_mutex_unlock(&(maz->mutex));
            return -1;
        }
    pthread_mutex_unlock(&(maz->mutex));
    return 0;
}

OBJECT maze_find_target(int row, int col, DIRECTION dir){
    return 0;
}
int maze_get_view(VIEW *view, int row, int col, DIRECTION gaze, int depth){
    return 0;
}
void show_view(VIEW *view, int depth){}
void show_maze(){
    int k = 0;
    while(mazy->contents[k] != NULL) {
        fprintf(stderr, "%s\n", mazy->contents[k]);
        k++;
    }
}
