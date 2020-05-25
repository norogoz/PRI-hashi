#include"board.h"

	/*
	0 - empty
	1..8 - spots
	9 - unset spot
	-11, -21 - single/double vertical
	-12, -22 - single/double horizontal
	*/
	
int main()	
{
	srand(time(NULL));
	int tab[9][9], game[9][9];
	int c = 0;
	int out;
	static int round = 1;
	
	initscr();
	cbreak();
	keypad(stdscr, TRUE);
	noecho();
	curs_set(2);
	
	#ifdef WIN32
	system("mode con: cols=70 lines=16");
	#else
	system("stty cols 70 rows 16");
	#endif
	
	do{
	clearTab ( tab );
	buildBoard ( tab );

	out = 0;
	
	printInfo (&round);
	
	for(int i=0; i<9; i++) // print the generated board and initialize playing board
	{
		move ( i, 0 );
			for(int j=0; j<9; j++) 
				switch ( tab[i][j] )    
				{
					case 1: case 2: case 3: case 4: case 5: case 6: case 7: case 8: 
					game[i][j] = tab[i][j];
					printw("(%d)", game[i][j]);				     
					break;
					
					default: 
					game [i][j] = 0;
					printw("   ");
				}
	}
	
	playGame ( game, &c, & out, &round );	
	}while( c != 27 );
	
	endwin();
	return 0;
}//main
