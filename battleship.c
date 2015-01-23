#include "battleship.h"
#include <unistd.h>


int my_boats[6];
int boats_left;
int other_player;


void clear_window() {
    int f = fork();
    if (!f) {
        execlp("clear", "clear", NULL);
        exit(0);
    }
    else {
        wait(&f);
        f = 0;
    }
}


int main(){
    // new_game();
    //printf("Start a New Game! To begin, set up your board.\n\n");
    //set_board();
    
    printf( "Do you want to join a game, or start a game?\n" );
    printf( "\t1. Start a game\n\t2. Join a game\n" );
    
    char buff[100];
    int i;
    fgets( buff, sizeof( buff ), stdin );
    clean_input(buff);
    //printf( "Buff: <%s>\n", buff );
    game_over=0;
    
    
    if(! strcmp(buff,"1") ){
        printf( "Initiating Game\n" );
        initiate_game();
        
        my_turn=1;
        
    }
    else if(! strcmp(buff, "2") ){
        
        //printf("socket_server: %d\n",socket_id);
        printf("enter the IP address you wish to connect to:\n");
        char ipaddress [100];
        fgets (ipaddress, sizeof(ipaddress), stdin);
        join_game(ipaddress);
        my_turn=0;
    }
    new_game();
    set_board();
    while(!game_over){
        if (my_turn){
            printf("\n\nYour Turn!");
            printf("Other player's board\n");
            
            printf("Where do you want to hit?\n");
            fgets(buff,sizeof(buff),stdin);
            //buff=&clean(buff);
            write(other_player,buff,sizeof(buff));
            char response[100];
            int i= read(other_player,response,sizeof(response));
            response[i/sizeof(char)]=0;
            int hit = get_i(buff);
            if(!strcmp(response,"hit")){
                opponent_board[hit]= 'X';
                printf("You got a hit!\n");
            }
            else if(!strcmp(response,"fatal")){
                opponent_board[hit]= 'X';
                printf("You sunk a ship!\n");
            }
            else if(!strcmp(response,"gameover")){
                opponent_board[hit]= 'X';
                printf("You Won!\n");
                game_over=1;
            }
            else{
                opponent_board[hit]='O';
                printf("All you hit was a seagull :(\n");
                my_turn=!my_turn;
            }
            
            print_board(opponent_board);
        }
        else{
            print_board(your_board);
            printf("\n\n\nOther player is playing...\n");


            char recieved[100];
            int i = read(other_player,recieved,sizeof(recieved));
            recieved[i/sizeof(char)-1]=0;
            printf("They hit: %s\n",recieved);
            int hit= get_i(recieved);
            if (your_board[hit] == '~'){
                write(other_player,"miss",sizeof("miss"));
                printf("They missed!\n");
                my_turn = !my_turn;
            }
            else{
 
                my_boats[your_board[hit]-97]=my_boats[your_board[hit]-97]-1;
                
                //printf("My boats [%d]=%d\n",your_board[hit]-97,my_boats[your_board[hit]-65]);
                if(my_boats[your_board[hit]-97]){
                    printf("Other_Player:%d\n",other_player);
                    your_board[hit]='o';
                    write(other_player,"hit",sizeof("hit"));
                    printf("They got a hit!\n");
                    
                }
                else{
                    
                    printf("They sunk your boat!\n");
                    your_board[hit]='o';
                    boats_left-=1;
                    if (!boats_left){
                        printf("You Lost!\n");
                        write(other_player,"gameover",sizeof("gameover"));
                        game_over=1;
                    }
                    else{
                        write(other_player,"fatal",sizeof("fatal"));
                    }
                }
                
            }
            
            print_board(your_board);
        }
    }
}

char * clean_input (char * input) {
    char * s1 = input;
    s1 = strsep( &s1, "\n" );
    return s1;
    
}



void initiate_game(){
    
    //printf("Initiating Connection\n");
    int socket_id;
    char buffer[256];
    int i, b;
    
    //create the socket
    socket_id = socket( AF_INET, SOCK_STREAM, 0 );
    
    
    //bind to port/address
    struct sockaddr_in listener;
    listener.sin_family = AF_INET;
    listener.sin_port = htons(24601);
    listener.sin_addr.s_addr = INADDR_ANY;
    
    printf("Waiting for Connection\n");
    bind( socket_id, (struct sockaddr *)&listener, sizeof(listener) );
    listen( socket_id, 1 );
    socket_client = accept( socket_id, NULL, NULL );
    clear_window();
    printf( "Connected\n" );
    other_player = socket_client;
    //printf("other player:%d\n",other_player);
}


void join_game( char * args ){
    
    char buffer[256];
    int i, b;
    
    //create the socket
    socket_id = socket( AF_INET, SOCK_STREAM, 0 );
    
    
    //bind to port/address
    struct sockaddr_in sock;
    sock.sin_family = AF_INET;
    sock.sin_port = htons(24601);
    
    inet_aton(args, &(sock.sin_addr) );
    
    bind( socket_id, (struct sockaddr *)&sock, sizeof(sock) );
    i = connect( socket_id,(struct sockaddr *)&sock, sizeof(sock) );
    //printf( "Connected\n" );
    other_player = socket_id;
    //printf("other player:%d\n",other_player);
    clear_window();
}


//set board to zeros
void reset_board( char board[] ){
    int x = 0;
    int y = 0;
    while( x < 10 ){
        while( y < 10 ){
            board[ (x * 10) + y ] = '~';
            y++;
            
        }
        y = 0;
        x++;
    }
}

//reset the game
void new_game(){
    reset_board( your_board );
    reset_board( opponent_board );
}

//print board
void print_board( char board[] ){
    //system("clear");
    int x = 0;
    int y = 0;
    printf( "  0 1 2 3 4 5 6 7 8 9 \n" );
    while( x < 10 ){
        printf( "%c ", x + 65 );
        while( y < 10 ){
            printf( "%c ", board[ (x * 10) + y ] );
            y++;
        }
        printf( "\n" );
        y = 0;
        x++;
    }
}

//place ship in array
int check_collisions( int len, int increment, int i ){
    while( len ){
        if( your_board[i] != '~' ){
            return 0;
        }
        i = i + increment;
        len = len - 1;
    }
    return 1;
}

//get first space
int get_i (char start[]){
    
    
    int i = ( ( start[0] - 65 ) * 10 ) + ( start[1] - 48 );
    if( start[0] < 65 || start[0] > 74 || start[1] < 48 || start[1] > 57){
        i = -1;
    }
    
    return i;
}

void alter_array( int len, int increment, int i ){
    while( len ){
        your_board[i] = ship_marker;
        i = i + increment;
        len = len - 1;
    }
    ship_marker++;
    
    clear_window();
    print_board( your_board );
}

void place_ship( int len ){
    char orientation[100];
    
    printf( "Placing ship of length %d.\n", len );
    printf( "Enter 'H' to place it horizontally, or 'V' to place it vertically: " );
    
    fgets( orientation, sizeof(orientation), stdin );
    clean_input(orientation);
    if (! (!strcmp(orientation,"H") || !strcmp(orientation,"V") ) ){
        printf( "Invalid input. Please try again.\n\n" );
        return place_ship( len );
    }
    char start[100];
    
    int increment;
    int i;
    
    //horizontal
    if (! strcmp(orientation, "H") ){
        increment = 1;
        printf( "Enter the space where you want the left of this ship to be (ie 'D3'): " );
        fgets( start, sizeof( start ), stdin );
        i = get_i(start);
        if( i == -1 ){
            printf( "Invalid input. Please try again.\n\n" );
            return place_ship( len );
        }
        if( i/10 != ( (len * increment + i - 1)/10 ) ){
            printf( "That goes off the board! Try again.\n\n" );
            return place_ship( len );
        }
    }
    
    //vertical
    else if(! strcmp(orientation, "V") ){
        increment = 10;
        printf( "Enter the space where you want the top of this ship to be (ie 'D3'): " );
        fgets( start, sizeof( start ), stdin );
        i = get_i(start);
        if( i == -1 ){
            printf( "Invalid input. Please Try again.\n\n" );
            return place_ship( len );
        }
        if( (len * increment + i)/10 > 10){
            printf( "That goes off the board! Try again.\n\n" );
            return place_ship( len );
        }
    }
    
    if( check_collisions( len, increment, i ) ){
        alter_array( len, increment, i );
    }
    else {
        printf( "That would collide with another ship! Try again.\n\n" );
        return place_ship(len);
    }
}

void set_board () {
    print_board( your_board );
    my_boats[0]=6;
    my_boats[1]=4;
    place_ship(6);
    //place_ship(4);
    place_ship(4);
    boats_left=2;
    //place_ship(3);
    //place_ship(3);
    //place_ship(3);
    //place_ship(2);
    /*place_ship(2);
     place_ship(2);
     place_ship(2);*/
    printf("board set\n");
    int i;
    char buff[100];
    write(other_player,"set", sizeof(buff));
    buff[99]=0;
    printf("Waiting...\n");
    i=read(other_player, buff, sizeof(buff));
    buff[i/sizeof(char)]=0;
    if (!strcmp(buff,"set")){
        clear_window();
        printf("Other baord set\n");
        //return 1;
    }
    else{
        printf("Other board not set");
        //return 0;
    }
    
    
    
}

