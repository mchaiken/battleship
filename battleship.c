#include "battleship.h"

int main(){
    system( "clear" );
    printf( "Welcome to Battleship!\n\n" );
    printf( "Do you want to join a game, or start a game?\n" );
    printf( "\t1. Start a game\n\t2. Join a game\n" );
    
    char buff[100];
    int i;
    fgets( buff, sizeof(buff), stdin );
    clean_input( buff );
    //printf( "Buff: <%s>\n", buff );
    
    if(! strcmp( buff, "1" ) ){
        printf( "Initiating game.\n" );
        initiate_game();
        
        my_turn = 1;
    }
    else if(! strcmp(buff, "2") ){
        //printf( "socket_server: %d\n",socket_id );
        printf( "Enter the IP address you wish to connect to:\n" );
        char ipaddress[100];
        fgets( ipaddress, sizeof(ipaddress), stdin );
        join_game( ipaddress );
        my_turn = 0;
    }
    
    new_game();
    set_board();
    
    while( !game_over ){
        if( my_turn ){
            printf( "Your turn!\n\n" );
            printf( "Other player's board:\n");
            print_board( opponent_board );
            printf( "\nWhere do you want to hit? " );
            
            fgets( buff, sizeof(buff), stdin );
            
            int i;
            i = get_i( buff );
            if( i == -1 ){
                system("clear");
                printf( "Invalid input. Please try again. " );
            }
            else{
                if( opponent_board[i] == '~' ){ //not tried yet
                    write( other_player, buff, sizeof(buff) );
                    system( "clear" );
                    
                    char response[100];
                    int i = read( other_player, response, sizeof(response) );
                    response[ i/sizeof(char) ] = 0;
                    int hit = get_i( buff );
                    if(! strcmp(response, "hit") ){
                        opponent_board[hit] = 'X';
                        printf( "You got a hit! " );
                        my_turn = !my_turn;
                    }
                    else if(! strcmp(response, "fatal") ){
                        opponent_board[hit] = 'X';
                        printf( "You sunk a ship! " );
                        my_turn = !my_turn;
                    }
                    else if(! strcmp(response, "gameover") ){
                        opponent_board[hit] = 'X';
                        printf( "You won!\n\n\n" );
                        game_over = 1;
                    }
                    else{
                        opponent_board[hit] = 'O';
                        printf( "All you hit was a seagull... " );
                        my_turn = !my_turn;
                    }
                }
                else{ //already tried
                    printf( "You've already fired on this spot.\n\n" );
                }
            }
        }
        else{
            printf( "Other player's turn.\n\n" );
            printf( "Your board:\n" );
            print_board( your_board );
            printf( "\nWaiting for other player's move...\n" );
            
            char received[100];
            int i = read( other_player, received, sizeof(received) );
            system( "clear" );
            received[ i/sizeof(char) - 1 ] = 0;
            
            printf( "They fired on %s, and ", clean_input(received) );
            int hit = get_i( received );
            if( your_board[hit] == '~' ){
                write( other_player, "miss", sizeof("miss") );
                printf( "they missed! " );
                my_turn = !my_turn;
                your_board[hit] = "O";
            }
            else{
                my_boats[ your_board[hit] - 97 ] = my_boats[ your_board[hit] - 97 ] - 1;
                //printf( "Left to kill: %d\n", my_boats[ your_board[hit] - 97 ] );
                
                if ( my_boats[your_board[hit]-97] != 0 ){
                    
                    write( other_player, "hit", sizeof("hit") );
                    printf( "hit your ship! " );
                    my_turn = !my_turn;
                }
                else{
                    printf( "they sunk your ship! " );
                    
                    boats_left -= 1;
                    if( !boats_left ){
                        printf( "You lost!\n\n\n" );
                        write( other_player, "gameover", sizeof("gameover") );
                        game_over = 1;
                    }
                    else{
                        write( other_player, "fatal", sizeof("fatal") );
                        my_turn = !my_turn;
                    }
                    
                }
                your_board[hit] = 'X';
            }
        }
    }
}


char * clean_input( char * input ) {
    char * s1 = input;
    s1 = strsep( &s1, "\n" );
    return s1;
}

void initiate_game(){
    //printf( "Initiating Connection\n" );
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
    
    printf( "Waiting for connection...\n" );
    bind( socket_id, (struct sockaddr *)&listener, sizeof(listener) );
    listen( socket_id, 1 );
    socket_client = accept( socket_id, NULL, NULL );
    //system("clear");
    printf( "Connected\n" );
    other_player = socket_client;
    //printf( "other player:%d\n", other_player );
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
    
    inet_aton( args, &(sock.sin_addr) );
    
    bind( socket_id, (struct sockaddr *)&sock, sizeof(sock) );
    
    i = connect( socket_id, (struct sockaddr *)&sock, sizeof(sock) );
    //printf( "Connected\n" );
    other_player = socket_id;
    //printf( "other player:%d\n", other_player );
    //system( "clear" );
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
int get_i( char start[] ){
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
    
    //system( "clear" );
    //print_board( your_board );
}

void place_ship( int len ){
    char orientation[100];
    
    printf( "\nPlacing ship of length %d.\n", len );
    printf( "Enter 'H' to place it horizontally, or 'V' to place it vertically: " );
    
    fgets( orientation, sizeof(orientation), stdin );
    clean_input( orientation );
    if (! ( !strcmp(orientation, "H") || !strcmp(orientation, "V") ) ){
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
        i = get_i( start );
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
        return place_ship( len );
    }
}

void set_board () {
    system("clear");
    printf( "Set your board!\n\n" );
    printf( "Your board:\n" );
    print_board( your_board );
    
    my_boats[0] = 6;
    my_boats[1] = 4;
    my_boats[2] = 4;
    my_boats[3] = 3;
    my_boats[4] = 2;
    
    place_ship(6);
    system( "clear" );
    printf( "Ship placed.\n\n" );
    printf( "Your board:\n" );
    print_board( your_board );
    
    place_ship(4);
    system( "clear" );
    printf( "Ship placed.\n\n" );
    printf( "Your board:\n" );
    print_board( your_board );
    
    place_ship(4);
    system( "clear" );
    printf( "Ship placed.\n\n" );
    printf( "Your board:\n" );
    print_board( your_board );
    
    place_ship(3);
    system( "clear" );
    printf( "Ship placed.\n\n" );
    printf( "Your board:\n" );
    print_board( your_board );
    
    place_ship(2);
    system( "clear" );
    printf( "Ship placed.\n\n" );
    printf( "Your board:\n" );
    print_board( your_board );
    
    //printf( "board set\n" );
    
    int i;
    char buff[100];
    
    write( other_player, "set", sizeof(buff) );
    buff[99] = 0;
    printf( "\nWaiting for other player...\n" );
    i = read( other_player, buff, sizeof(buff) );
    buff[ i/sizeof(char) ] = 0;
    if (! strcmp(buff, "set") ){
        //system( "clear" );
        printf( "Other board set\n" );
        //return 1;
    }
    else{
        printf( "Other board not set\n" );
        //return 0;
    }
    system( "clear" );
}
