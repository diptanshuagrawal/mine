/*****************************************************************************
 * Copyright (C) Diptanshu Agrawal agrawaldiptanshu@gmail.com
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

#include <stdio.h>
#include <ncurses.h>
#include <menu.h>
#include "mine.h"
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#define CTRLD	4
/* globally declared variables */
int ro = 8, col = 8;/*.................................default for row and columns is 8 */
int dif = 8;/*.............................default for difficulty is 8 i.e no. of mines */
int flag1 = 0;/*........flag1 variable decides whether the game is to be resumed or not */

void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string, chtype color){
	int length, x, y;
	float temp;

	if(win == NULL)
		win = stdscr;
	getyx(win, y, x);
	if(startx != 0)
		x = startx;
	if(starty != 0)
		y = starty;
	if(width == 0)
		width = 80;

	length = strlen(string);
	temp = (width - length)/ 2;
	x = startx + (int)temp;
	wattron(win, color);
	mvwprintw(win, y, x, "%s", string);
	wattroff(win, color);
	refresh();
}
/*making a menu using a window*/
void menu() {
	endwin();
	clear();
	refresh();
	#define CTRLD	4

	/* menu options defined here */	
	char *choices[] = {
				"            Play Game!!!         ",
				"            Resume Game          ",
				"               About             ",
				"              Options            ",
				"               Exit              ",
			};
	ITEM **my_items;
	int c;
	WINDOW *my_menu_win;
	MENU *my_menu;
	int n_choices = 5, i;
	ITEM *cur_item;

	initscr();
	noecho();
	cbreak();
	keypad(stdscr, TRUE);
	/* initialising colours */
	start_color();
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(4, COLOR_WHITE, COLOR_BLACK);
	attrset(COLOR_PAIR(1));
	
	my_items = (ITEM **)calloc(n_choices + 1, sizeof(ITEM *));
	
	for(i = 0; i < n_choices; ++i)
		my_items[i] = new_item(choices[i], NULL);
	my_items[n_choices] = (ITEM *)NULL;

	/* assigning functions to the menu options using user pointers */
	set_item_userptr(my_items[0], game);
	set_item_userptr(my_items[1], resume);
	set_item_userptr(my_items[2], about);
	set_item_userptr(my_items[3], options);
	set_item_userptr(my_items[4], exitmenu);
	my_menu = new_menu((ITEM **)my_items);

	/* printing the window in the middle of the screen */
	my_menu_win = newwin(10, 40, (LINES - 10) / 2, (COLS - 40) / 2);
	keypad(my_menu_win, TRUE);

	/* setting main and sub window */
	set_menu_win(my_menu, my_menu_win);
	set_menu_sub(my_menu, derwin(my_menu_win, 6, 38, 3, 1));

	set_menu_mark(my_menu, "->");
	box(my_menu_win, 0, 0);
	print_in_middle(my_menu_win, 1, 0, 40, "KA-BOOM", COLOR_PAIR(1));

	mvwaddch(my_menu_win, 2, 0, ACS_LTEE);
	mvwhline(my_menu_win, 2, 1, ACS_HLINE, 38);
	mvwaddch(my_menu_win, 2, 39, ACS_RTEE);
	attron(COLOR_PAIR(4));
	mvprintw(LINES - 2, 2, "Press F1 to EXIT");
	attroff(COLOR_PAIR(4));
	refresh();

	/* posting menu on the window */
	post_menu(my_menu);
	wrefresh(my_menu_win);

	/* setting what happens till the f1 key is pressed */
	while((c = wgetch(my_menu_win)) != KEY_F(1)) {
		switch(c) {
			case KEY_DOWN:
				menu_driver(my_menu, REQ_DOWN_ITEM);
				break ;
			case KEY_UP:
				menu_driver(my_menu, REQ_UP_ITEM);
				break ;
			case 10:
			{	/* unposted the menu so that the new function works properly */
				unpost_menu(my_menu);
				endwin();
				ITEM *cur;

				void (*p) (char *);
				cur = current_item(my_menu);
				p = item_userptr(cur);
				p((char *)item_name(cur));
				pos_menu_cursor(my_menu);
				/* cleared the screen after the function is executed */
				clear();
				endwin();
				refresh();
				init_pair(1, COLOR_RED, COLOR_BLACK);
				break;
			}
			break;
		}
		wrefresh(my_menu_win);
		endwin();
		initscr();
		init_pair(3, COLOR_RED, COLOR_BLACK);
		bkgd(COLOR_PAIR(3));
		attrset(COLOR_PAIR(3));
		set_menu_win(my_menu, my_menu_win);
		set_menu_sub(my_menu, derwin(my_menu_win, 6, 38, 3, 1));
		mvprintw(LINES - 2, 2, "Press F1 to EXIT");
		post_menu(my_menu);
		box(my_menu_win, 0, 0);
		wrefresh(my_menu_win);
	}
	
	unpost_menu(my_menu);
	for(i = 0; i < n_choices; i++)
		free_item(my_items[i]);
	free_menu(my_menu);
	endwin();
}
/* the options function is similar to the menu function */
void options(){
	clear();
	refresh();
	#define CTRLD	4

	char *choices[] = {
				"            Difficulty          ",
				"          Rows & Columns        ",
				"              Back              ",
			};
	ITEM **my_items;
	int c;
	WINDOW *my_menu_win;
	MENU *my_menu;
	int n_choices = 3, i;
	ITEM *cur_item;

	initscr();
	noecho();
	cbreak();
	keypad(stdscr, TRUE);
	start_color();
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(4, COLOR_WHITE, COLOR_BLACK);
	attrset(COLOR_PAIR(1));
	
	my_items = (ITEM **)calloc(n_choices + 1, sizeof(ITEM *));
	
	for(i = 0; i < n_choices; ++i)
		my_items[i] = new_item(choices[i], NULL);
	my_items[n_choices] = (ITEM *)NULL;

	set_item_userptr(my_items[0], diff);
	set_item_userptr(my_items[1], row);
	set_item_userptr(my_items[2], back);
	my_menu = new_menu((ITEM **)my_items);

	my_menu_win = newwin(10, 40, (LINES - 10) / 2, (COLS - 40) / 2);
	keypad(my_menu_win, TRUE);

	set_menu_win(my_menu, my_menu_win);
	set_menu_sub(my_menu, derwin(my_menu_win, 6, 38, 3, 1));

	set_menu_mark(my_menu, "->");
	box(my_menu_win, 0, 0);
	print_in_middle(my_menu_win, 1, 0, 40, "Options", COLOR_PAIR(1));

	mvwaddch(my_menu_win, 2, 0, ACS_LTEE);
	mvwhline(my_menu_win, 2, 1, ACS_HLINE, 38);
	mvwaddch(my_menu_win, 2, 39, ACS_RTEE);
	attron(COLOR_PAIR(4));
	mvprintw(LINES - 2, 2, "Press Q To EXIT");
	attroff(COLOR_PAIR(4));
	refresh();

	post_menu(my_menu);
	wrefresh(my_menu_win);

	while((c = wgetch(my_menu_win)) != 'q') {
		switch(c) {
			case KEY_DOWN:
				menu_driver(my_menu, REQ_DOWN_ITEM);
				break ;
			case KEY_UP:
				menu_driver(my_menu, REQ_UP_ITEM);
				break ;
			case 10:
			{	unpost_menu(my_menu);
				endwin();
				ITEM *cur;

				void (*p) (char *);
				cur = current_item(my_menu);
				p = item_userptr(cur);
				p((char *)item_name(cur));
				pos_menu_cursor(my_menu);
				if(cur == my_items[2])
					return;
				clear();
				endwin();
				refresh();
				break;
			}
			break;
		}
		endwin();
		init_pair(1, COLOR_RED, COLOR_BLACK);
		attrset(COLOR_PAIR(1));
		post_menu(my_menu);
		init_pair(4, COLOR_WHITE, COLOR_BLACK);
		attron(COLOR_PAIR(4));
		mvprintw(LINES - 2, 2, "Press Q To EXIT");
		attroff(COLOR_PAIR(4));
		box(my_menu_win, 0, 0);
		wrefresh(my_menu_win);
	}
	
	unpost_menu(my_menu);
	for(i = 0; i < n_choices; i++)
		free_item(my_items[i]);
	free_menu(my_menu);
	endwin();
}
void row() {
	endwin();
	clear();
	refresh();
	initscr();
	int c;
	cbreak();
	keypad(stdscr, TRUE);
	start_color();
	init_pair(1, COLOR_WHITE, COLOR_BLACK);
	attrset(COLOR_PAIR(1));

	mvprintw( LINES - 2, 1, "Press Q To Go Back");
	mvprintw((LINES - 4) / 2, (COLS - 26) / 2,  "Enter the number of rows: ");
	refresh();
	/* taking input from the user for the no. of rows */
	scanf("%d", &ro);
	printw("%d", ro);
	refresh();
	mvprintw(LINES / 2, (COLS - 28) / 2,  "Enter the number of columns: ");
	refresh();
	/* taking input from the user for the no. of columns */
	scanf("%d", &col);
	printw("%d", col);
	refresh();
	mvprintw( LINES / 2 + 2,(COLS - 28) / 2 ,"You're choice has been saved");
	while((c = getch()) != 'q');	
	refresh();
	endwin();
	clear();
	refresh();
}
void diff() {
	endwin();
	clear();
	refresh();
	initscr();
	int c, y, x;
S:
	noecho();
	cbreak();
	keypad(stdscr, TRUE);
	start_color();
	init_pair(1,COLOR_WHITE, COLOR_BLACK);
	attrset(COLOR_PAIR(1));

	mvprintw(LINES - 2, 0, "Press Q To Go Back");
	/* displaying the difficulty levels */
	mvprintw((LINES/ 2 - 2), (COLS - 8) / 2,  "Beginner");
	mvprintw(LINES / 2, (COLS - 12) / 2,  "Intermediate");
	mvprintw((LINES / 2 + 2), (COLS - 6) / 2,  "Expert");
	y = LINES / 2 ;
	x = COLS / 2;
	move(y, x);
	while((c = getch()) != 'q') {
		switch(c) {
			case KEY_UP:
				if(y == LINES / 2 - 2)
					y = LINES / 2 + 2;
				else
					y -= 2;
				break ;
			case KEY_DOWN:
				if(y == LINES / 2 + 2) 
					y = LINES / 2 - 2;
				else
					y += 2;
				break ;
			case 10:
				endwin();
				clear();
				initscr();
				/* if the user has selected intermediate */
				if(y == LINES / 2){
					dif = (ro + col) / 2;
					mvprintw(4, (COLS - 28) / 2, "You've selected Intermediate");
				}
				/* if the user has selected beginner */
				if(y == LINES / 2 - 2) {
					dif = (ro + col) / 4;
					mvprintw(4, (COLS - 24) / 2, "You've selected Beginner");
				}
				/* if the user has selected expert */
				if(y == LINES / 2 + 2) {
					dif = ro + col;
					mvprintw(4, (COLS - 22 ) / 2, "You've selected Expert");
				}
				goto S;
				break ;
			}
	move(y ,x);
	}
	refresh();
	endwin();
	clear();
	refresh();
}
void back() {	
}
void exitmenu() {
	exit(0);
}
void about() {
	int c, i, j;
	clear();
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	
	start_color();
	init_pair(1, COLOR_WHITE, COLOR_CYAN);
	bkgd(COLOR_PAIR(1));
	attrset(COLOR_PAIR(1));
	box(stdscr, 0, 0);

	/* printing the lines */
	mvprintw( LINES - 2, 1, "Press Q To Go Back");
	mvprintw(LINES / 2 - 10, COLS / 2 - 3, "ABOUT");
	mvprintw(LINES / 2 - 8, COLS / 2 - 45, "The purpose of the game is to open all the cells of the board which do not contain a bomb.");
	mvprintw(LINES/ 2 - 6, COLS / 2 - 19, "You lose if you set off a bomb cell.");
	mvprintw(LINES / 2 - 4, COLS / 2 - 50, "Every non-bomb cell you open will tell you the total number of bombs in the eight neighboring cells.");
	mvprintw(LINES / 2 - 2, COLS / 2 - 24, " Once you are sure that a cell contains a bomb");
	mvprintw(LINES / 2 - 1, COLS / 2 - 20, " you can put a flag on it as a reminder.");
	mvprintw(LINES / 2 + 1, COLS / 2 - 29, "Once you have flagged all the bombs around an open cell");
	mvprintw(LINES / 2 + 2, COLS / 2 - 20, " you can quickly open the remaining non-bomb cells.");
	mvprintw(LINES / 2 + 4, COLS / 2 - 10, "Happy mine hunting!");
	
	while((c = getch()) != 'q');
	clear();
	refresh();
}
	
void resume() {
	/* marking flag1 as 1 if resume is selected */
	flag1 = 1;
	game();
}

void banner() {
	int i, j, n = 1;
	/* initialising screen */
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);

	/* initialising colors */
	start_color();
	init_pair(8, COLOR_WHITE, COLOR_RED);
	init_pair(9, COLOR_YELLOW, COLOR_RED);
	attrset(COLOR_PAIR(8));

	for(i = 0; i < 9; i++) {
		for(j = 0; j < 31; j++)
			mvprintw(LINES / 2 + i - 4, COLS / 2 + j - 15, " ");
		printw("\n");
	}
	mvprintw( LINES / 2, COLS / 2 - 3, "KA-BOOM");
	box(stdscr, 0, 0);

	/* loop for changing the color */
	for(i = 0; i < 10; i++) {
		refresh();
		sleep(1);
		/* if color is red then change it to yellow */
		if(n == 1) {
			attrset(COLOR_PAIR(9));
			mvprintw( LINES / 2, COLS / 2 - 3, "KA-BOOM");
			n = 2;
		}
		/* if color is yellow then change it to red */
		else if (n == 2) {
			n = 1;
			attrset(COLOR_PAIR(8));
			mvprintw( LINES / 2, COLS / 2 - 3, "KA-BOOM");
		}
		i++;
	}
	refresh();
	refresh();
	clear();
	refresh();
}
void game() {
S:
	/* clearing everything before starting the game */
	clear();
	int i, j, y, x, p, q, i1 = 0, i2,j2, mines = 0;
	int c;
	char b;
	char str[4];
	
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);

	start_color();
	init_pair(1, COLOR_RED, COLOR_WHITE);
	init_pair(2, COLOR_WHITE, COLOR_RED);
	attrset(COLOR_PAIR(2));

	box(stdscr, 0, 0);
	/* printing the instructions on the top */
	mvprintw(0, 0, "Use Arrow Keys To Navigate");
	mvprintw(1, 0, "Press Enter To Select Tile");
	mvprintw(2, 0, "Press Q To Exit");
	mvprintw(3, 0, "Press F To Flag");
	mvprintw(4, 0, "Press N For New Game");

	/* declaring variable of datatype mine */
	mine m;

	/* if resume is called */
	if(flag1 == 1) {
		FILE *gp;
		gp = fopen("resume1.txt", "r");

		if(gp == NULL){
			printf("operation failed\n");
			perror("a");
		}

		/* reading data from the saved file */
		fscanf(gp, "%d", &(m.k));
		fscanf(gp, "%d", &ro);
		fscanf(gp, "%d", &col);
		fscanf(gp, "%d", &dif);

		fclose(gp);
	}

	/* allocating memory to every element of the datatype mine */
	m.v = (int **)malloc(sizeof(int *) * ro);
	for(i = 0; i < ro; i++)
		m.v[i] = (int *)malloc(sizeof(int) * col);

	m.a = (char **)malloc(sizeof(char *) * ro);
	for(i = 0; i < ro; i++)
		m.a[i] = (char *)malloc(sizeof(char *) * col);

	m.d = (char **)malloc(sizeof(char *) * ro);
	for(i = 0; i < ro; i++)
		m.d[i] = (char *)malloc(sizeof(char *) * col);

	m.e = (char **)malloc(sizeof(char *) * ro);
	for(i = 0; i < ro; i++)
		m.e[i] = (char *)malloc(sizeof(char *) * col);

	/* if resume is called */
	if(flag1 == 1) {
		/* reading data from the saved file */
		FILE *fp;
		int i, j;
		char z;
		fp = fopen("resume.txt", "r");

		if(fp == NULL){
			printf("operation failed\n");
			perror("a");
		}

		for(i = 0; i < ro; i++) {
			for(j = 0; j < col; j++)
				 fscanf(fp, "%c", &(m.a[i][j]));
		}
		for(i = 0; i < ro; i++) {
			for(j = 0; j< col; j++)
				 fscanf(fp, "%c", &(m.d[i][j]));
		}
		for(i = 0; i < ro; i++) {
			for(j = 0; j < col; j++)
				fscanf(fp, "%c", &(m.e[i][j]));
		}
		for(i = 0; i < ro; i++) {
			for(j = 0; j < col; j++) {
				fscanf(fp, "%c", &z);
				i2 = atoi(&z);
				m.v[i][j] = i2;
			}
		}

		fclose(fp);
}

	/* if flag is not called */
	if(flag1 == 0) {

		/* initialising each element of the mine */
		m.k = 0;
		for(i = 0; i < ro; i++) {
			for(j = 0; j < col; j++)
				m.v[i][j] = 0;
		}

		for(i = 0; i < ro; i++) {
			for(j = 0; j < col; j++) 
				m.a[i][j] = 'q';
		}

		for(i = 0; i < ro; i++) {
			for(j = 0; j < col; j++) 
				m.d[i][j] = '3';
		}

		/* making the minefield using the function rand */
		srand(time(NULL));
		while(i1 < dif) {
			i2 = rand() % ro;
			j2 = rand() % col;
			while(m.a[i2][j2] != '*') {
				m.a[i2][j2] = '*';
				i1++;
			}
		}
		for(i = 0; i < ro; i++) {
			for(j = 0; j < col; j++) 
				m.e[i][j] = '.';
		}

	/*counting the no. of mines around each box*/
		for(i2 = 0; i2 < ro; i2++) {
			for(j2 = 0; j2 < col; j2++) {
				mines = 0;
				for(i = -1; i <= 1; i++) {
					for(j = -1; j <= 1; j++) {
							if(((i2 + i) < 0) || ((i2 + i) > (ro - 1)) || ((j2 + j) < 0) || ((j2 + j) > (col - 1)))
								mines = mines + 0;
							else if(m.a[i2 + i][j2 + j] != '*')
								mines = mines + 0;
							else if(m.a[i2 + i][j2 + j] == '*')
								mines = mines + 1;
					}
				}
				/* 48 is ascii of 0 */
				b = mines + 48;
				if(b == 48)
					m.a[i2][j2] = ' ';
				if(m.a[i2][j2] != '*' && m.a[i2][j2] != ' ')
					m.a[i2][j2] = b;
			}
		}
	}

	mvprintw((LINES - ro - 20) / 2, (COLS - 14) / 2, "Mines left: %d", (dif - m.k));

	/*printing the minefield in the middle*/	
	x = (COLS - ((col - 1) * 4 + 1)) / 2;
	p = x;
	y = (LINES - (ro - 1) * 2) / 2;
	q = y;
	move(y, x);

	printscr(x, y, p, q, m);
	move(q, p);
	x = p;
	y = q;

	/*assigning functions to keys till the key q is pressed */
	while((c = getch()) != 'q') {
		int flag = 0;
		switch(c) {
			case KEY_UP:
				if(y == q)
					y = q + (ro - 1) * 2;
				else
					y -= 2;
				break ;
			case KEY_DOWN:
				if(y == q + (ro - 1) * 2) 
					y = q;
				else
					y += 2;
				break ;
			case KEY_RIGHT:
				if(x == p + (col - 1) * 4)
					x = p;
				else
					x += 4;
				break;
			case KEY_LEFT:
				if(x == p)
					x = p + (col - 1) * 4;
				else
					x -= 4;
				break;
			/* for displaying the complete minefield when the cheat code "cheat" is pressed */
			case 'c':
				getstr(str);
				if(strcmp(str, "heat") == 0){
					/* displays the mine in the center right corner */
					for( i = 0; i < ro; i++) {
						for(j = 0; j < col; j++)
							mvprintw((LINES - ro) / 2 + i, COLS - col + j,"%c",m.a[i][j]);
							mvprintw((LINES - ro) / 2 + i, COLS - col + j, "\n");
					}
				}
				break;
			/* for unveiling the mine when enter is pressed */
			case 10:
				/* does not unveil the tile if there is a mine or it is flagged already */
				if(m.a[(y - q) / 2][(x - p) / 4] == '*' && m.d[(y - q) / 2][(x - p) / 4] != '#') {
					revealmines(m, p, q);
					attron(COLOR_PAIR(1));
					refresh();
					sleep(1);
					/* diplaying the end of game */
					mvprintw((LINES) / 2, (COLS - 16) / 2, "BOOM!!! YOU LOST");
					attroff(COLOR_PAIR(1));
					refresh();
					sleep(2);
					return;
				}
				/* do nothing if it is a flagged tile */
				else if( m.d[(y - q) / 2][(x - p) / 4] == '#');
				/* unveil the tile if the above conditions are false */
				else if( m.a[(y - q) / 2][(x - p) / 4] != '*') { 
					printw("%c", m.a[(y - q) / 2][(x - p) / 4]);
					m.e[(y - q) / 2][(x - p) / 4] =  m.a[(y - q) / 2][(x - p) / 4];
					if(m.a[(y - q) / 2][(x - p) / 4] == ' '){
						move(y, x);
						openmine(y, x, p, q, m);
					}
				}
				break;
			/* for starting a new game when n is pressed */
			case 'n':
				mvprintw( LINES / 2, (COLS - 25) / 2, "Do Want To Start A New Game");
				mvprintw( LINES / 2 + 1, COLS / 2 - 3 ,"(y/n)?");
				/* if the user presses y */
				if((c = getch()) == 'y') {
					clear();
					refresh();
					endwin();
					goto S;
				}
				/* if the user presses n */
				else if((c == 'n')) {
					clear();
					refresh();
					flag1 = 1;
					save(m);
					goto S;
					}
				break;
			/* for marking the mine as flagged */
			case 'f':
				if(m.e[(y - q) / 2][(x - p) / 4] == '.') {
					printw("#");
					m.e[(y - q) / 2][(x - p) / 4] = '#';
					/* k is the counter for the no. of tiles that are flagged */
					m.k++;
					m.d[(y - q) / 2][(x - p) / 4] = '#';
				}
				/* unflag the flagged tile */
				else if(m.e[(y - q) / 2][(x - p) / 4] == '#') {
					m.e[(y - q) / 2][(x - p) / 4] = '.';
					m.d[(y - q) / 2][(x - p) / 4] = '3';
					printw(".");
					/* decrease the count */
					m.k--;
				}
				/* printing the no. of mines left */
				mvprintw((LINES - ro - 20) / 2, (COLS - 14) / 2, "Mines left: %d", (dif - m.k));

				if((dif - m.k) < 10 && (dif - m.k) >= 0)
					mvprintw((LINES - ro - 20) / 2, (COLS - 14) / 2 + 13," "); 
				break ;
		}
	/* checking if the tiles flagged are actually mines */
	if(m.k == dif) {
		for(i = 0; i < ro; i++) {
			for(j = 0; j < col; j++) {
				if((m.a[i][j] != '*' && m.d[i][j] == '3') || ( m.a[i][j] == '*' && m.d[i][j] == '#'))
					flag++;
			}
		}
	}
		refresh();
	if(flag == (ro * col)) {
		attron(COLOR_PAIR(1));
		refresh();
		sleep(1);
		mvprintw((LINES) / 2, (COLS - 20) / 2, "CONGRATS!!! YOU WIN");
		attroff(COLOR_PAIR(1));
	
		refresh();
		sleep(3);
		return;
	}

	move(y, x);
	}
	flag1 = 0;
	/* before quitting the game pop up appears */
	mvprintw( LINES / 2, (COLS - 28) / 2, "Do You Want To Save The Game");
	mvprintw( LINES / 2 + 1, COLS / 2 - 3 ,"(y/n)?");
	while(1) {
		/* if y is pressed */
		if((c = getch()) == 'y') {
			save(m);
			break;
			}
		/* if n is pressed */
		else if(c == 'n')
			break;
	}
	/* free all the elements */
	for(i = 0; i < ro; i++)
		free(m.v[i]);
	free(m.v);
	for(i = 0; i < ro; i++)
		free(m.a[i]);
	free(m.a);
	for(i = 0; i < ro; i++)
		free(m.d[i]);
	free(m.d);
	for(i = 0; i < ro; i++)
		free(m.e[i]);
	free(m.e);

	refresh();
	clear();
	refresh();
	/* resetting everything to default */
	dif = 8;
	ro = 8;
	col = 8;
}

void save(mine m) {
	/* writing the necessary data to files */
	FILE *fp, *gp;
	int i, j;
	char c = ' ';

	fp = fopen("resume.txt", "w");
	gp = fopen("resume1.txt", "w");

	if(fp == NULL) {
		printf("operation failed\n");
		perror("a");
	}
	if(gp == NULL) {
		printf("operation failed\n");
		perror("a");
	}

	for(i = 0; i < ro; i++) {
		for(j = 0; j < col; j++)
			fprintf(fp, "%c", m.a[i][j]);
	}
	for(i = 0; i < ro; i++) {
		for(j = 0; j < col; j++)
			fprintf(fp, "%c", m.d[i][j]);
	}
	for(i = 0; i < ro; i++) {
		for(j = 0; j < col; j++)
			fprintf(fp, "%c", m.e[i][j]);
	}
	fprintf(fp, "%c", c);
	for(i = 0; i < ro; i++) {
		for(j = 0; j < col; j++) {
			fprintf(fp, "%d", m.v[i][j]);
		}
	}

	fprintf(gp, "%d", m.k);
	fprintf(gp, "%c", c);
	fprintf(gp, "%d", ro);
	fprintf(gp, "%c", c);
	fprintf(gp, "%d", col);
	fprintf(gp, "%c", c);
	fprintf(gp, "%d", dif);
	fclose(fp);
	fclose(gp);
}
void printscr(int x, int y, int p, int q, mine m) {
	int i, j;
	move(y, x);
	/* for printing the red box using spaces */
	for(i = y - 1; i < y + (ro - 1) * 2 + 2; i++) {
		for(j = x - 3; j < x + (col - 1) * 4 + 4; j++)
			mvprintw(i, j, " ");
	}

	x = p;
	y = q;
	move(y, x);
	/* for printing the dots */
	for(i = 0; i < ro; i++) {
		for(j = 0; j < col; j++) {
			printw("%c", m.e[i][j]);
			x += 4;
			move(y, x);
		}
		x = p;
		y += 2;
		move(y, x);
	}
}

void revealmines(mine m, int p, int q) {
	int i, j;
	for(i = 0; i < ro; i++) {
		for(j = 0; j < col; j++) {
			if(m.a[i][j] == '*')
				mvprintw((2 * i) + q, (4 * j) + p, "*");
		}
	}
}

int opn(int i, int j, int i1, int j1, int p, int q, mine m) {
	/* if the neighbour does not exist then return */
	if((i + i1) < 0 || ((i + i1) > (col - 1)) || ((j + j1) < 0) || ((j + j1) > (ro - 1)))
		return 0;
	/* if neighbour exists and is equal to # then print # */
	else if(m.d[j + j1][i + i1] == '#')
		mvprintw( ((j + j1) * 2) + q, ((i1 + i) * 4) + p,"%c", m.d[j + j1][i1 + i]);			 
	/* if the tile exists and is not a flagged one then unviel the tile */
	else{
		m.e[j + j1][i1 + i] =  m.a[j + j1][i1 + i];	
		mvprintw( ((j + j1) * 2) + q, ((i1 + i) * 4) + p,"%c", m.e[j + j1][i1 + i]);
		return 0;
	}
}
/* the function openmine uses recursion */
int openmine(int y, int x, int p, int q, mine m) {
	int i = 0, j = 0;
	/* calculating the position in the array using the positoin of the cusror on the screen */
	j = (y - q) / 2;
	i = (x - p ) / 4;
	refresh();
	/* if the tile contains nothing */
	if(m.a[j][i] == ' ') {
		m.v[j][i] = 1;
		/* unveil all the neighbouring tiles */
		opn(i, j, 1, -1, p, q, m);
		opn(i, j, 1, 0, p, q, m);
		opn(i, j, 1, 1, p, q, m);
		opn(i, j, 0, 1, p, q, m);
		opn(i, j, -1, 1, p, q, m);
		opn(i, j, -1, 0, p, q, m);
		opn(i, j, -1, -1, p, q, m);
		opn(i, j, 0, -1, p, q, m);
		/* in every if condition below i am checking whether the neighbouring tile exists and if it is visited or not */
		if(!((i + 1) < 0) && !((j + -1) < 0) && !((col - 1) < (i + 1)) && !((ro - 1) < (j + -1)) && !m.v[j + -1][i + 1]){
				openmine(2 * (j + -1) + q, 4 * (i + 1) + p, p, q, m);
		}
		refresh();
		if(!((i + 1) < 0) && !((j + 0) < 0) && !((col - 1) < (i + 1)) && !((ro - 1) < (j + 0)) && !m.v[j + 0][i + 1]){
				openmine(2 * (j + 0) + q, 4 * (i + 1) + p, p, q, m);
		}
		refresh();
		if(((i + 1) >= 0) && ((j + 1) >= 0) && ((col - 1) >= (i + 1)) && ((ro - 1) >= (j + 1)) && !m.v[j + 1][i + 1]){
				openmine(2 * (j + 1) + q, 4 * (i + 1) + p, p, q, m);
		}
		refresh();
		if(!((i + 0) < 0) && !((j + 1) < 0) && !((col - 1) < (i + 0)) && !((ro - 1) < (j + 1)) && !m.v[j + 1][i + 0]){
				openmine(2 * (j + 1) + q, 4 * (i + 0) + p, p, q, m);
		}
		refresh();
		if(!((i + -1) < 0) && !((j + 1) < 0) && !((col - 1) < (i + -1)) && !((ro - 1) < (j + 1)) && !m.v[j + 1][i + -1]){
				openmine(2 * (j + 1) + q, 4 * (i + -1) + p, p, q, m);
		}	
		refresh();
		if(!((i + -1) < 0) && !((j + 0) < 0) && !((col - 1) < (i + -1)) && !((ro - 1) < (j + 0)) && !m.v[j + 0][i + -1]){
				openmine(2 * (j + 0) + q, 4 * (i + -1) + p, p, q, m);
		}
			refresh();
		if(!((i + -1) < 0) && !((j + -1) < 0) && !((col - 1) < (i + -1)) && !((ro - 1) < (j + -1)) && !m.v[j + -1][i + -1]){
				openmine(2 * (j + -1) + q, 4 * (i + -1) + p, p, q, m);
		}
				refresh();
		if(!((i + 0) < 0) && !((j + -1) < 0) && !((col - 1) < (i + 0)) && !((ro - 1) < (j + -1)) && !m.v[j + -1][i + 0]){
				openmine(2 * (j + -1) + q, 4 * (i + 0) + p, p, q, m);
		}
			refresh();

}
return 0;
}
