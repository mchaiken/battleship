#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
char your_board [100];
char opponent_board[100];
int socket_id;
int socket_client;

void initiate_game(){
    printf("Initializing Game\n");
   
    
    //make socket
    socket_id = socket (AF_INET, SOCK_STREAM,0);
    
    //bind to an ip address
    struct sockaddr_in sock;
    sock.sin_family = AF_INET;
    sock.sin_port = htons(24601);
    sock.sin_addr.s_addr = INADDR_ANY;
    
    bind(socket_id, (struct sockaddr *)&sock, sizeof(sock));
    
    printf("Waiting for conenction\n");
    listen (socket_id,1); //wait until it get's a connection
    
    socklen_t s;
    while(1){
        s = sizeof(sock);
        
        socket_client= accept( socket_id, NULL,NULL);
    }
    
    /*while(1){
        b= read(socket_client, buffer, sizeof(buffer));
        printf("Received: %s\n");
    }*/
    printf("Conntected\n");
}

void join_game(){
    int socket_id;
    char buffer[256];
    int i, b;
    
    //create the socket
    printf("creating socket\n");
    socket_id = socket(AF_INET, SOCK_STREAM, 0);
    
    printf("binding to port\n");
    //bind to port/address
    struct sockaddr_in sock;
    sock.sin_family = AF_INET;
    sock.sin_port = htons(24601);
    
    inet_aton( "207.244.82.145", &(sock.sin_addr));
    printf("About to bind\n");
    bind( socket_id, (struct sockaddr *)&sock, sizeof(sock));
    printf("Conntected\n");
    /*  while(1){
        printf("Enter message: ");
        fgets( buffer, sizeof(buffer),stdin);
        *(strchr(buffer));}*/
}

//set board to zeros
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
//reset the game
void newGame(){
    reset_board(your_board);
    reset_board(opponent_board);
}
//print board
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
//place ship in array
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
//get first space
int get_i () {
    char start[100];
    fgets( start, sizeof( start ), stdin );
    int i=((start[0]-65)*10) + (start[1]-48);
    if (start[0]<65 || start[0]>74 || start[1]<48 || start[1] > 57) {
        i = -1;
    }
    return i;
}


void alterArray (int len, int increment, int i) {
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
        alterArray(len, increment,i);
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
   // newGame();
    //printf("Start a New Game! To begin, set up your board.\n\n");
    //setBoard();
    
    printf("Do you want to join a game, or start game?\n");
    printf("\t1. Start a game\n\t2. Join a game\n");
    char buff[100];
    int i;
    fgets( buff, sizeof( buff ), stdin );
    buff[1]=0;
    printf("Buff: <%s>\n",buff);
    if (!strcmp(buff,"1")){
        printf("Initiating Game\n");
        initiate_game();
    }
    else if (!strcmp(buff, "2")) {
        join_game();
    }
}