#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char your_board [100];
char opponent_board[100];

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
    printf("  0 1 2 3 4 5 6 7 8 9 \n");
    while(x<10){
        printf("%c ",x+65);
        while(y<10){
            printf("%c ",board[(x*10)+y]);
            y++;
        }
        printf("\n");
        y=0;
        x++;
    }
}

void newGame(){
    reset_board(your_board);
    reset_board(opponent_board);
}

int check_collisions(char board[], char * start, int increment,int length){
    int i = ((start[0]-65)*10) +(start[1]-48);
    while (length<=9 && length>=0 ){
        if (board[i] == 'O'){
            printf("Oh no! This would collide with another boat! Try again.\n");
            return 1;
        }
        i+=increment;
        length--;
    }
    return 0;
    
    
}

int placeShip(char board[], char * start, char * orientation, int length){
    
    int i=((start[0]-65)*10) +(start[1]-48);
    int increment;
    if (!strcmp(orientation,"up")){
        increment=-10;
    }
    else if (!strcmp(orientation,"down")){
        increment=10;
    }
    else if (!strcmp(orientation,"left")){
        increment= 1;
    }
    else{
        increment= -1;
    }
    if ((i + (length* increment)) > 99 ||(i + (length* increment)) < 0|| length > 10){
        printf("That goes off the board! %d\n",i + (length* increment));
        return 0;
        
    }
    else if (check_collisions(board,start,increment,length)){
        return 0;
    }
    while(length){
        board[i]='O';
        i+=increment;
        length--;
        
    }
    print_board(your_board);
    return 1;
}

int main(){
    char start[100];
    char orientation[100];
    reset_board(your_board);
    while(1){
       /* printf("Start a New Game! To begin, set up your board.\n");
        printf("To place your ships, simply type the starting location of the ship (ie A0, B7), followed by if the rest of the ship should be moving up down left or right.\n");
        print_board(your_board);
        printf("First ship. Length 6\n");
        printf("Where would you like to place it(ie A0, B2): ");
        fgets( start, sizeof( start ), stdin );
        printf("In what direction should the tail go (ie if your start is A5 and you want it to go towards A4, up");
          fgets( orientation, sizeof( orientation ), stdin );
        placeShip(your_board,&start[0],&orientation[0], 6);
    }*/
    
    return 0;
}