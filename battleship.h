#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

char your_board[100];
char opponent_board[100];
char ship_marker = 'a';
int socket_id;
int socket_client;
int my_turn;
int game_over;
int my_boats[6]; //int my_boats[6] = [ 6, 4, 4, 3, 3, 3, 2, 2, 2, 2 ];
int other_player;
int boats_left; //int boats_left = 10;

char * clean_input ( char * input );

void initiate_game();

void join_game( char * args );

void reset_board( char board[] );

void new_game();

void print_board( char board[] );

int check_collisions( int len, int increment, int i );

int get_i( char start[] );

void alter_array( int len, int increment, int i );

void place_ship( int len );

void set_board();