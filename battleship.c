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

void newGame(){
    reset_board(your_board);
    reset_board(opponent_board);
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

int check_collisions (int len, int increment, int i) {
    while (len) {
        if (your_board[i] == 'O') {
            return 0;
        }
        i = i + increment;
        len = len - 1;
    }
    return 1;
}

int get_i () {
    char start[100];
    fgets( start, sizeof( start ), stdin );
    int i=((start[0]-65)*10) + (start[1]-48);
    if (start[0]<65 || start[0]>74 || start[1]<48 || start[1] > 57) {
        i = -1;
    }
    return i;
}

void placeShip2 (int len, int increment, int i) {
    while (len) {
        your_board[i] = 'O';
        i = i + increment;
        len = len - 1;
    }
    print_board(your_board);
    
}

void placeShip (int len) {
    char orientation[100];
    printf("Placing ship of length %d.\n", len);
    
    printf("Enter 'H' to place it horizontally, or 'V' to place it vertically: ");
    fgets(orientation, sizeof(orientation), stdin);
    char * s1 = orientation;
    s1 = strsep(&s1,"\n");
    if (! (!strcmp(s1,"H") || !strcmp(s1,"V") )) {
        printf("Invalid input. Please try again.\n\n");
        return placeShip(len);
    }
    
    int increment;
    int i;
    
    //horizontal
    if (!strcmp(s1, "H")) {
        increment = 1;
        printf("Enter the space where you want the left of this ship to be (ie 'D3'): ");
        i = get_i();
        if (i == -1) {
            printf("Invalid input. Please Try again.\n\n");
            return placeShip(len);
        }
        if (i/10 != ((len*increment+i-1)/10)){
            printf("That goes off the board! Try again.\n\n");
            return placeShip(len);
        }
    }
    
    //vertical
    else if (!strcmp(s1, "V")) {
        increment = 10;
        printf("Enter the space where you want the top of this ship to be (ie 'D3'): ");
        i = get_i();
        if (i == -1) {
            printf("Invalid input. Please Try again.\n\n");
            return placeShip(len);
        }
        if ((len*increment+i)/10 > 10){
            printf("That goes off the board! Try again.\n\n");
            return placeShip(len);
        }
        
    }
    if (check_collisions(len, increment,i)) {
        placeShip2(len, increment,i);
    }
    else {
        printf("That would collide with another ship! Try again.\n\n");
        return placeShip(len);
    }
}

void setBoard () {
    print_board(your_board);
    //couldn't find definitively online how many of each piece length there's supposed to be?
    placeShip(6);
    placeShip(4);
    placeShip(4);
    placeShip(3);
    placeShip(3);
    placeShip(2);
    placeShip(2);
    placeShip(2);
    placeShip(2);
}

int main(){
    newGame();
    printf("Start a New Game! To begin, set up your board.\n\n");
    setBoard();
}