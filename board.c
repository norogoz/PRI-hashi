#include"board.h"

void clearTab ( int tab[9][9] )
{
	for ( int i = 0; i < 9; i++ )
		for ( int j = 0; j < 9; j++ ) 
			tab[i][j] = 0;
}

int surround ( int tab[9][9], int y, int x ) // check if adjacent points are 1..8
{
	switch ( tab[y-1][x] )
	{
		case 1:	case 2:	case 3:	case 4:	case 5:	case 6: case 7: case 8: case 9: return 0;
	}
	
	switch ( tab[y+1][x] )
	{
		case 1:	case 2:	case 3:	case 4:	case 5:	case 6: case 7: case 8: case 9: return 0;
	}
	
	switch ( tab[y][x-1] )
	{
		case 1:	case 2:	case 3:	case 4:	case 5:	case 6: case 7: case 8: case 9: return 0;
	}
	
	switch ( tab[y][x+1] )
	{
		case 1:	case 2:	case 3:	case 4:	case 5:	case 6: case 7: case 8: case 9: return 0;
	}
	
	return 1;
}//surround

int analyze ( int tab[9][9], int *y, int *x, int maxLength[4], int currBrid[4], int availBrid[4], int *availSum, int *currSum )
{	
	*availSum = 0;
	*currSum = 0;
	for ( int i = 0; i < 4; i++ )
	{
		currBrid[i] = availBrid[i] = 0;
		maxLength[i] = -1;
	}
	// UP (0)
	for ( int i = *y - 1; i >= 0; i--)
	{
		if ( tab[i][*x] == 0 && surround ( tab, i, *x ) ) maxLength[0]++; // calculate max lenght of a bridge in given direction
		else break;
	}
	
	if ( tab[*y-1][*x] == -11 ) currBrid[0]++; // include currently connected bridges in given direction
	else if ( tab[*y-1][*x] == -21 ) currBrid[0] += 2;
	
	// DOWN (1)
	for ( int i = *y + 1; i < 9; i++)
	{
		if ( tab[i][*x] == 0 && surround ( tab, i, *x ) ) maxLength[1]++;
		else break;
	}
	
	if ( tab[*y+1][*x] == -11 ) currBrid[1]++; 
	else if ( tab[*y+1][*x] == -21 ) currBrid[1] += 2;
	
	// LEFT (2)
	for ( int i = *x - 1; i >= 0; i--)
	{
		if ( tab[*y][i] == 0 && surround ( tab, *y, i ) ) maxLength[2]++;
		else break;
	}
	
	if ( tab[*y][*x-1] == -12 ) currBrid[2]++; 
	else if ( tab[*y][*x-1] == -22 ) currBrid[2] += 2;

			
	// RIGHT (3)
	for ( int i = *x + 1; i < 9; i++)
	{
		if ( tab[*y][i] == 0 && surround ( tab, *y, i ) ) maxLength[3]++;
		else break;
	}
	
	if ( tab[*y][*x+1] == -12 ) currBrid[3]++; 
	else if ( tab[*y][*x+1] == -22 ) currBrid[3] += 2;
		
	for ( int i=0; i < 4; i++ )
	{
		if ( maxLength[i] > 0 ) availBrid[i] += 2; // sum up all the availabe and currently existing brides
		*availSum += availBrid[i];
		*currSum += currBrid[i];
	}	

	if ( *availSum ) { tab[*y][*x] = rand() % *availSum + *currSum + 1; return 1; }
	else return 0; // 0 if can't build any more bridges
}//analyze
	
void build ( int tab[9][9], int *y, int *x, int maxLength[4], int availBrid[4], int *currSum) // mark bridges in the array
{
	int tmp1;
	int length; 

	for ( int i = 0; i < (tab[*y][*x] - *currSum); i++ )
	{
		tmp1 = rand() % 4; //choose direction for a bridge
		
		if ( availBrid[0] == 0 && availBrid[1] == 0 && availBrid[2] == 0 && availBrid[3] == 0 ) break;
		
		if ( availBrid[tmp1] == 2 )
		{
			length = rand() % maxLength[tmp1] + 1; //choose length for the bridge
			availBrid[tmp1]--;
			switch ( tmp1 ) // mark points as single bridges in the array
			{
				case 0: for ( int j = 1; j <= length; j++ ) tab[*y-j][*x] = -11; tab[*y-length-1][*x] = 9; break;
				case 1: for ( int j = 1; j <= length; j++ ) tab[*y+j][*x] = -11; tab[*y+length+1][*x] = 9; break;
				case 2: for ( int j = 1; j <= length; j++ ) tab[*y][*x-j] = -12; tab[*y][*x-length-1] = 9; break;
				case 3: for ( int j = 1; j <= length; j++ ) tab[*y][*x+j] = -12; tab[*y][*x+length+1] = 9; break;
			}
		}
		else if ( availBrid[tmp1] == 1 ) 
		{
			switch ( tmp1 ) // mark points as double bridges in the array
			{
				case 0: for ( int j = 1; tab[*y-j][*x] == -11; j++ ) tab[*y-j][*x] = -21; break;
				case 1: for ( int j = 1; tab[*y+j][*x] == -11; j++ ) tab[*y+j][*x] = -21; break;
				case 2: for ( int j = 1; tab[*y][*x-j] == -12; j++ ) tab[*y][*x-j] = -22; break;
				case 3: for ( int j = 1; tab[*y][*x+j] == -12; j++ ) tab[*y][*x+j] = -22; break;
			}
			availBrid[tmp1]--;
		}
		else i--;
	}	// all until all available bridges are used up		
}//build

void buildBoard (int tab[9][9]) // build the board in the array
{
	/*
	0 - empty
	1..8 - spots
	9 - unset spot
	-11, -21 - single/double vertical
	-12, -22 - single/double horizontal
	*/

	int x,y;
	int currBrid[4];
	int availBrid[4];
	int maxLength[4];
	int availSum = 0;
	int currSum = 0;
	int verify;

	x = rand() % 9; // choose first point randomly
	y = rand() % 9;
		
	do
	{				
		if ( analyze ( tab, &y, &x, maxLength, currBrid, availBrid, &availSum, &currSum ) )
			build ( tab, &y, &x, maxLength, availBrid, &currSum);
		else tab[y][x] = currSum;
		
		for(int i = 0; i < 9; i++) 
		{
			for(int j = 0; j < 9; j++) 
			{ 
				if ( tab[i][j] == 9 ) // choose next point marked as "unset"
				{ 
					tab[i][j] = 0; 
					y = i; 
					x = j; 
					verify = 1; 
					break;
				} 
				else verify = 0; 
			}
			if ( verify ) break; // if all points are complete, finish
		}		
	} while ( verify );
}//buildBoard

int isEqual ( int tab[9][9] ) // is number and sum of bridges equal
{
	int currSum = 0;
	
	for ( int i = 0; i < 9; i++ )
		for ( int j = 0; j < 9; j++)
			if ( tab[i][j] > 0 )
			{
				// UP (0)
				if ( tab[i-1][j] == -11 ) currSum++; 
				else if ( tab[i-1][j] == -21 ) currSum += 2;
	
				// DOWN (1)
				if ( tab[i+1][j] == -11 ) currSum++; 
				else if ( tab[i+1][j] == -21 ) currSum += 2;
	
				// LEFT (2)
				if ( tab[i][j-1] == -12 ) currSum++; 
				else if ( tab[i][j-1] == -22 ) currSum += 2;

				// RIGHT (3)
				if ( tab[i][j+1] == -12 ) currSum++; 
				else if ( tab[i][j+1] == -22 ) currSum += 2;
				
				currSum -= tab[i][j];
				if ( currSum ) return 0;
				currSum = 0;
			}	
	return 1;
}//isEqual

void mark ( int tab[9][9], int y, int x, int up, int down, int left, int right ) // mark points connected to tab[y][x]
{	
	tab[y][x] += 10;
	int i;
	
	if ( up )// UP (0)
	{
	for ( i = 1; tab[y-i][x] == -11 || tab[y-i][x] == -21; i++ ); 
	if ( tab[y-i][x] > 0 && tab[y-i][x] < 9) mark ( tab, y - i, x, 1, 0, 1, 1 );
	}
	
	if ( down )	// DOWN (1)
	{
	for ( i = 1; tab[y+i][x] == -11 || tab[y+i][x] == -21; i++ ); 
	if ( tab[y+i][x] > 0 && tab[y+i][x] < 9) mark ( tab, y + i, x, 0, 1, 1, 1 );
	}
	
	if ( left )	// LEFT (2)
	{
	for ( i = 1; tab[y][x-i] == -12 || tab[y][x-i] == -22; i++ ); 
	if ( tab[y][x-i] > 0 && tab[y][x-i] < 9) mark ( tab, y, x - i, 1, 1, 1, 0 );
	}
	
	if ( right ) // RIGHT (3)
	{
	for ( i = 1; tab[y][x+i] == -12 || tab[y][x+i] == -22; i++ ); 
	if ( tab[y][x+i] > 0 && tab[y][x+i] < 9) mark ( tab, y, x + i, 1, 1, 0, 1 );
	}
}

void unmark ( int tab[9][9] ) // reverse mark
{
	for ( int i = 0; i < 9; i++ )
		for ( int j = 0; j < 9; j++)
			if ( tab[i][j] > 10 ) tab[i][j] -= 10;
}

int isSolved ( int tab[9][9] ) // returns 1 if solution is correct, 0 if it's not
{
	int out = 0;
	if ( isEqual ( tab ) )
	{
		for ( int i = 0; i < 9; i++ )
		{
			for ( int j = 0; j < 9; j++)
			{
				if ( tab[i][j] > 0 ) 
				{ mark ( tab, i, j, 1, 1, 1, 1); out = 1; break; }
			}
			if ( out ) break;
		}
		for ( int i = 0; i < 9; i++ )
			for ( int j = 0; j < 9; j++)
				if ( tab[i][j] > 0 && tab[i][j] < 9 ) 
				{ unmark ( tab ); return 0; }
	unmark ( tab );		
	return 1;	
	}
	else return 2;
	return 0;
}//isSolved

void printInfo( int *round )
{
	move (1, 40);
	clrtoeol();
	mvprintw(1, 40, "HASHIWOKAKERO ROUND #%d", *round);
	mvprintw(10, 0, "RULES");
	mvprintw(11, 0, "-The number in each island tells how many bridges are connected to it");
	mvprintw(12, 0, "-There are no more than two bridges in the same direction.");
	mvprintw(13, 0, "-Bridges are not allowed to cross islands or other bridges.");
	mvprintw(14, 0, "-When completed, all bridges are interconnected");
	mvprintw(3, 30, "ARROW KEYS = move/choose direction");
	mvprintw(4, 30, "ENTER = choose the point");
	mvprintw(5, 30, "C = check / move to next level");
	mvprintw(6, 30, "S = skip level");
	mvprintw(7, 30, "ESC = end the game");
}

void playGame( int game[9][9], int *c, int *out, int *round  )
{
	int i, j, x, y; 
	i = j = x = y = 0;
	int cursorY, cursorX;
	int dir;
	move ( 0, 1 );
	
	for ( i = 1; *c != 27;  )
	{
		dir = 0;
		*c = getch();
		getyx ( stdscr, cursorY, cursorX );
		switch ( *c )
		{       
			case KEY_UP: move ( --cursorY, cursorX ); break;
			case KEY_DOWN: if ( cursorY < 8 ) move ( ++cursorY, cursorX ); break;
			case KEY_LEFT: move ( cursorY, cursorX-=3 ); break;
			case KEY_RIGHT: if ( cursorX < 24 ) move ( cursorY, cursorX+=3 ); break;
			case 'c': if ( isSolved ( game ) == 1 ) { mvprintw ( 9, 30, "CORRECT            " ); { (*round)++; mvprintw(20, 0, "HAHAHAHAH, %d", *round);} *out = 1; break; }
						else { mvprintw ( 9, 30, "WRONG           " ); move (cursorY, cursorX); break; }
			case 's': { mvprintw ( 9, 30, "LEVEL SKIPPED" ); *out = 1; break; }

			case '\n': 
					x = cursorX / 3;
					y = cursorY;
					if ( game[y][x] > 0 ) dir = 1;
		}
		
		move ( cursorY, cursorX );
				
		if ( dir ) 
			{	
				int d = getch();
				
				switch ( d )
				{       
					case KEY_UP: 
						for ( j = 1; game[y-j][x] < 1; j++ );

						if ( game[y-j][x] > 0 && y-j >= 0 && y-j <= 8 )
							for ( int k = 1; k < j; k++ ) { if ( game[y-k][x] == -21 ) game[y-k][x] = 0; else if ( game[y-k][x] == -11 ) game[y-k][x] = -21; else if ( game[y-k][x] == 0 ) game[y-k][x] = -11; }
						break;
						
					case KEY_DOWN: 
						for ( j = 1; game[y+j][x] < 1; j++ );			
						if ( game[y+j][x] > 0 && y+j >= 0 && y+j <= 8) 
							for ( int k = 1; k < j; k++ ) { if ( game[y+k][x] == -21 ) game[y+k][x] = 0; else if ( game[y+k][x] == -11 ) game[y+k][x] = -21; else if ( game[y+k][x] == 0 ) game[y+k][x] = -11; }
						break;
						
					case KEY_LEFT: 
						for ( j = 1; game[y][x-j] < 1; j++ ); 
						if ( game[y][x-j] > 0 && x-j >= 0 && x-j <= 8) 
							for ( int k = 1; k < j; k++ ) { if ( game[y][x-k] == -22 ) game[y][x-k] = 0; else if ( game[y][x-k] == -12 ) game[y][x-k] = -22; else if ( game[y][x-k] == 0 ) game[y][x-k] = -12; }
						break;
						
					case KEY_RIGHT: 
						for ( j = 1; game[y][x+j] < 1; j++ ); 
						if ( game[y][x+j] > 0 && x+j >= 0 && x+j <= 8) 
							for ( int k = 1; k < j; k++ ) { if ( game[y][x+k] == -22 ) game[y][x+k] = 0; else if ( game[y][x+k] == -12 ) game[y][x+k] = -22; else if ( game[y][x+k] == 0 ) game[y][x+k] = -12; }
						break;
				}	
				move(0,0);
				for(int k=0; k<9; k++)
				{
					move(k,0);
					for(int l=0; l<9; l++)  
					switch ( game[k][l] )    
					{
						case 1: case 2: case 3: case 4: case 5: case 6: case 7: case 8: 
						printw("(%d)", game[k][l]);	break;
						case -11: printw(" | "); break;
						case -12: printw(" - "); break;
						case -21: printw(" H "); break;
						case -22: printw(" = "); break;
						default: printw("   ");
					}
				}
		
		}
			
			move(cursorY, cursorX);
			refresh();
			if ( *out ) break;
	}
		move ( 0, 1 );
		refresh();
}