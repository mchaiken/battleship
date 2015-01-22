#include "battleship.h"
#include <unistd.h>

int main(){
    // new_game();
    //printf("Start a New Game! To begin, set up your board.\n\n");
    //set_board();
    
    printf( "Do you want to join a game, or start a game?\n" );
    printf( "\t1. Start a game\n\t2. Join a game\n" );
    
    char buff[100];
    int i;
    fgets( buff, sizeof( buff ), stdin );
    buff[1] = 0;
    
    printf( "Buff: <%s>\n", buff );
    game_over=0;
    
    if(! strcmp( buff, "1" ) ){
        printf( "Initiating Game\n" );
        initiate_game();
        my_turn = 1;
        int i = read( socket_client, buff, sizeof(buff) );
        buff[ i/sizeof( char ) ] = 0;
        printf( "Recieved <%s>", buff );
        printf( "Send your Info:" );
        fgets( buff, sizeof(buff), stdin );
        buff[99] = 0;
        write( socket_client, buff, sizeof(buff) );
    }
    
    else if(! strcmp(buff, "2") ){
        printf( "socket_server: %d\n", socket_id );
        printf( "enter the IP address you wish to connect to:\n" );
        char ipaddress[100];
        fgets( ipaddress, sizeof(ipaddress), stdin );
        join_game( ipaddress );
        my_turn = 0;
        printf( "Send your Info:" );
        fgets( buff, sizeof(buff), stdin );
        buff[99] = 0;
        write( socket_id, buff, sizeof(buff) );
        i = read( socket_id, buff, sizeof(buff) );
        buff[ i/sizeof( char ) ] = 0;
        printf( "Recieved <%s>", buff );
    }
    new_game();
    set_board();
    while( !game_over ){
        if( my_turn ){
            printf( "Other player's board\n" );
            print_board( opponent_board );
            printf( "Where do you want to hit?\n" );
        }
    }
}


void initiate_game(){
    printf( "Intiating Connection\n" );
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
    printf( "Connected\n" );
    other_player = socket_client;
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
    
    inet_aton( &args[1], &(sock.sin_addr) );
    
    bind( socket_id, (struct sockaddr *)&sock, sizeof(sock) );
    i = connect( socket_id,(struct sockaddr *)&sock, sizeof(sock) );
    printf( "Connected\n" );
    other_player = socket_id;
}


//set board to zeros
void reset_board( char board[] ){
    int x = 0;
    int y = 0;
    while( x < 10 ){
        while( y < 10 ){
            board[ (x * 10) + y ] = '-';
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
        if( your_board[i] != '-' ){
            return 0;
        }
        i = i + increment;
        len = len - 1;
    }
    return 1;
}

//get first space
int get_i (){
    char start[100];
    fgets( start, sizeof( start ), stdin );
    
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
    
    print_board( your_board );
}

void place_ship( int len ){
    char orientation[100];
    
    printf( "Placing ship of length %d.\n", len );
    printf( "Enter 'H' to place it horizontally, or 'V' to place it vertically: " );
    
    fgets( orientation, sizeof(orientation), stdin );
    char * s1 = orientation;
    
    s1 = strsep( &s1, "\n" );
    if (! (!strcmp(s1,"H") || !strcmp(s1,"V") ) ){
        printf( "Invalid input. Please try again.\n\n" );
        return place_ship( len );
    }
    
    int increment;
    int i;
    
    //horizontal
    if (! strcmp(s1, "H") ){
        increment = 1;
        printf( "Enter the space where you want the left of this ship to be (ie 'D3'): " );
        i = get_i();
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
    else if(! strcmp(s1, "V") ){
        increment = 10;
        printf( "Enter the space where you want the top of this ship to be (ie 'D3'): " );
        i = get_i();
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
    
    place_ship(6);
    place_ship(4);
    place_ship(4);
    place_ship(3);
    place_ship(3);
    place_ship(3);
    place_ship(2);
    place_ship(2);
    place_ship(2);
    place_ship(2);
}

