#include <stdio.h>
#include <stdlib.h>

char board [100];
void reset_board(char board []){
    int x=0;
    int y=0;
    while(x<10){
        while(y<10){
            board[(x*10)+y]= '-';
            y++;
            
        }
        y=0;
        x++;
    }
}
void print_board(char board[]){
    int x=0;
    int y=0;
    printf("X 0 1 2 3 4 5 6 7 8 9 \n");
    while(x<10){
        printf("%d ",x);
        while(y<10){
            printf("%c ",board[(x*10)+y]);
            y++;
        }
        printf("\n");
        y=0;
        x++;
    }
}

int main(){
    reset_board(board);
    print_board(board);
    return 0;
}