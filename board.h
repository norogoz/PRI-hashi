#ifndef BOARD_H
#define BOARD_H

#include<stdio.h>
#include<time.h>
#include<stdlib.h>

#ifdef WIN32
#include<curses.h>
#else
#include<ncurses.h>
#endif

void buildBoard();
int isSolved( int tab[9][9] );
void clearTab ( int tab[9][9] );
void printInfo ( int *round );
void playGame( int game[9][9], int *c, int *out, int *round );

#endif