#include <ncurses.h>
#include <menu.h>
#include <stdlib.h>

#define SIZE 8
/*char a[SIZE][SIZE] = {
			"000*00*0",
			"*000000*",
			"00000**0",
			"000000*0",
			"00*00000",
			"0*00*000",
			"0000000*",
			"**000000",
		};*/
/*void createmine(int y, char *a[y][y]) {
	int x, i, j;
	while(x < y) { 
		i = rand() % y;
		j = rand() % y;
		while(a[i][j] != a) {
			a[i][j] = a;
			x++;
		}
	}
	for(i = 0; i < y; i++) {
		for(j = 0; j < y; j++) {
			while(a[i][j] != a)
				a[i][j] = b;
		}
	}
}*/
		
int main() {
	int i, j, k, y, x, p, q, i1 = 0, i2,j2;
	int c, a[SIZE][SIZE];
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);

	start_color();
	init_pair(1, COLOR_WHITE, COLOR_RED);
	init_pair(2, COLOR_BLACK, COLOR_RED);
	attrset(COLOR_PAIR(2));

	box(stdscr, 0, 0);
	mvprintw(0, 0, "Use Arrow Keys To Navigate");
	mvprintw(1, 0, "Press A To Select Tile");
	mvprintw(2, 0, "Press Q To Exit");
	mvprintw(3, 0, "Press F To Flag");
	attrset(COLOR_PAIR(1));

	srand(time(NULL));
	while(i1 < SIZE * 2) {
		i2 = rand() % SIZE;
		j2 = rand() % SIZE;
		while(a[i2][j2] != '*') {
			a[i2][j2] = '*';
			i1++;
		}
	}
	for(i = 0; i < SIZE; i++) {
		for(j = 0; j < SIZE; j++) {	
			if(a[i][j] != '*')
				a[i][j] = ' ';
		}
	}
				
	x = (COLS - ((SIZE - 1) * 4 + 1)) / 2;
	p = x;
	y = (LINES - (SIZE - 1) * 2) / 2;
	q = y;
	move(y, x);

	move(y, x);	
	for(i = y - 1; i < y + (SIZE - 1) * 2 + 2; i++) {
		for(j = x - 3; j < x + (SIZE - 1) * 4 + 4; j++) {
			mvprintw(i, j, " ");
		}
	}
	x = p;
	y = q;
	move(y, x);
	for(i = 0; i < SIZE; i++) {
		for(j = 0; j < SIZE; j++) {
			printw(".");
			x += 4;
			move(y, x);
		}
		x = p;
		y += 2;
		move(y, x);
	}
	move(q, p);
	x = p;
	y = q;
//	attron(A_REVERSE);
//	mvprintw( 15, 61, ".");
//	attroff(A_REVERSE);
		while((c = getch()) != 'q') {
		switch(c) {
			case KEY_UP:
				if(y == q)
					y = q + (SIZE - 1) * 2;
				else
					y -= 2;
				break ;
			case KEY_DOWN:
				if(y == q + (SIZE - 1) * 2) 
					y = q;
				else
					y += 2;
				break ;
			case KEY_RIGHT:
				if(x == p + (SIZE - 1) * 4)
					x = p;
				else
					x += 4;
				break;
			case KEY_LEFT:
				if(x == p)
					x = p + (SIZE - 1) * 4;
				else
					x -= 4;
				break;
			case 'a':
				printw("%c", a[(y-q)/2][(x - p) / 4]);
				break;
			case 'f':
				printw("#");
				break ;
		}
	move(y, x);
}
	refresh();
	endwin();
}
