#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char your_board [100];
char opponent_board[100];
char ship_marker = 'a';

void reset_board( char board[] );

void new_game();

void print_board( char board[] );

int check_collisions( int len, int increment, int i );

int get_i();

void place_ship_2( int len, int increment, int i );

void place_ship( int len );

void set_board();