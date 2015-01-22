#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

char your_board [100];
char opponent_board[100];
char ship_marker = 'a';
int socket_id;
int socket_client;
int other_player;
int my_turn;
int game_over;

void initiate_game();

void join_game();

void reset_board( char board[] );

void new_game();

void print_board( char board[] );

int check_collisions( int len, int increment, int i );

int get_i(char start[]);

void alter_array( int len, int increment, int i );

void place_ship( int len );

void set_board();