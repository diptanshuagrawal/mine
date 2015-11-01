#include <stdio.h>
#include <ncurses.h>
#include <menu.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

//#define SIZE	8
#define CTRLD	4

int ro = 8, col = 8;
int dif = 8;
int flag1 = 0, k = 0;

void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string, chtype color);
void back();
void menu();
void game();
void resume();
void exitmenu();
void options();
void diff();
void row();
int opn(int i, int j, int i1, int j1, int p, int q, char a[ro][col],char e[ro][col]);
void about();
void revealmines(char a[ro][col], int p, int q);
int openmine(int y, int x, int p, int q, char a[ro][col], int v[ro][col], char e[ro][col]);
void save(char a[ro][col], char d[ro][col], char e[ro][col], int v[ro][col], int r, int co, int dif, int k);
void printscr(int x, int y, int p, int q, char e[ro][col]);
int main() {
	menu();
}
/*function to print the title in the center*/
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
	#define CTRLD	4

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
	start_color();
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(4, COLOR_WHITE, COLOR_BLACK);
	attrset(COLOR_PAIR(1));
	
	my_items = (ITEM **)calloc(n_choices + 1, sizeof(ITEM *));
	
	for(i = 0; i < n_choices; ++i)
		my_items[i] = new_item(choices[i], NULL);
	my_items[n_choices] = (ITEM *)NULL;

	set_item_userptr(my_items[0], game);
	set_item_userptr(my_items[1], resume);
	set_item_userptr(my_items[2], about);
	set_item_userptr(my_items[3], options);
	set_item_userptr(my_items[4], exitmenu);
	my_menu = new_menu((ITEM **)my_items);

	my_menu_win = newwin(10, 40, (LINES - 10) / 2, (COLS - 40) / 2);
	keypad(my_menu_win, TRUE);

	set_menu_win(my_menu, my_menu_win);
	set_menu_sub(my_menu, derwin(my_menu_win, 6, 38, 3, 1));

	set_menu_mark(my_menu, "->");
	box(my_menu_win, 0, 0);
	print_in_middle(my_menu_win, 1, 0, 40, "Minesweeper", COLOR_PAIR(1));

	mvwaddch(my_menu_win, 2, 0, ACS_LTEE);
	mvwhline(my_menu_win, 2, 1, ACS_HLINE, 38);
	mvwaddch(my_menu_win, 2, 39, ACS_RTEE);
	attron(COLOR_PAIR(4));
	mvprintw(LINES - 2, 2, "Press F1 to EXIT");
	attroff(COLOR_PAIR(4));
	refresh();

	post_menu(my_menu);
	wrefresh(my_menu_win);

	while((c = wgetch(my_menu_win)) != KEY_F(1)) {
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
//				clear();
				ITEM *cur;

				void (*p) (char *);
				cur = current_item(my_menu);
				p = item_userptr(cur);
				p((char *)item_name(cur));
				pos_menu_cursor(my_menu);
				clear();
				endwin();
				refresh();
				init_pair(1, COLOR_RED, COLOR_BLACK);
				break;
			}
			break;
		}
		endwin();
		initscr();
		init_pair(3, COLOR_RED, COLOR_BLACK);
		bkgd(COLOR_PAIR(3));
		attrset(COLOR_PAIR(3));
//		wrefresh(my_menu_win);
		post_menu(my_menu);
		box(my_menu_win, 0, 0);
//		print_in_middle(my_menu_win, 1, 0, 40, "Minesweeper", COLOR_PAIR(1));
		wrefresh(my_menu_win);
	}
	
	unpost_menu(my_menu);
	for(i = 0; i < n_choices; i++)
		free_item(my_items[i]);
	free_menu(my_menu);
	endwin();
}
void options(){
	clear();
	refresh();
	#define CTRLD	4

	char *choices[] = {
				"            Difficulty          ",
				"          Rows & Columns        ",
//				"             Colour             "
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
//	set_item_userptr(my_items[2], options);
	set_item_userptr(my_items[2], back);
//	set_item_userptr(my_items[4], exitmenu);
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
//				clear();
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
//		wrefresh(my_menu_win);
		post_menu(my_menu);
		init_pair(4, COLOR_WHITE, COLOR_BLACK);
		attron(COLOR_PAIR(4));
		mvprintw(LINES - 2, 2, "Press Q To EXIT");
		attroff(COLOR_PAIR(4));
		box(my_menu_win, 0, 0);
//		print_in_middle(my_menu_win, 1, 0, 40, "Minesweeper", COLOR_PAIR(1));
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
	noecho();
	cbreak();
	keypad(stdscr, TRUE);
	start_color();
	init_pair(1, COLOR_WHITE, COLOR_BLACK);
	attrset(COLOR_PAIR(1));

	mvprintw( LINES - 2, 1, "Press Q To Go Back");
	mvprintw((LINES - 4) / 2, (COLS - 26) / 2,  "Enter the number of rows: ");
	refresh();
	scanf("%d", &ro);
	printw("%d", ro);
	refresh();
	mvprintw(LINES / 2, (COLS - 28) / 2,  "Enter the number of columns: ");
	refresh();
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
				if(y == LINES / 2){
					dif = (ro + col) / 2;
					mvprintw(4, (COLS - 28) / 2, "You've selected Intermediate");
				}
				if(y == LINES / 2 - 2) {
					dif = (ro + col) / 4;
					mvprintw(4, (COLS - 24) / 2, "You've selected Beginner");
				}
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
//	for(i = 0; i < LINES; i++){
//		for(j = 0; j < COLS; j++){
//			mvprintw(i, j, " ");
//			sleep(1);
//		}
//	}
	box(stdscr, 0, 0);
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
	flag1 = 1;
	game();
}
	
void game() {
S:
	clear();
	int i, j, y, x, p, q, i1 = 0, i2,j2, mines = 0;
	int c, r, co;
	int v[ro][col];
	char a[ro][col], d[ro][col], e[ro][col], b;
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
	mvprintw(0, 0, "Use Arrow Keys To Navigate");
	mvprintw(1, 0, "Press Enter To Select Tile");
	mvprintw(2, 0, "Press Q To Exit");
	mvprintw(3, 0, "Press F To Flag");
	mvprintw(4, 0, "Press N For New Game");

	if(flag1 == 1) {
		FILE *fp;
		int i, j;
		fp = fopen("resume.txt", "r");
		if(fp == NULL){
			printf("operation failed\n");
			perror("a");
		}
		for(i = 0; i < ro; i++) {
			for(j = 0; j < col; j++)
				 fscanf(fp, "%c", &a[i][j]);
		}
		for(i = 0; i < ro; i++) {
			for(j = 0; j< col; j++)
				 fscanf(fp, "%c", &d[i][j]);
		}
		for(i = 0; i < ro; i++) {
			for(j = 0; j < col; j++)
				fscanf(fp, "%c", &e[i][j]);
		}
		for(i = 0; i < ro; i++) {
			for(j = 0; j < col; j++) {
				fscanf(fp, "%d", &v[i][j]);
				v[i][j] = i2;
	//			mvprintw(20 + i, 20 + j, "%d", v[i][j]);
			}
		}

		fscanf(fp, "%d", &k);
		fscanf(fp, "%d", &ro);
//		mvprintw(LINES - 2, 3, "%d", ro);
		fscanf(fp, "%d", &col);
		fscanf(fp, "%d", &dif);
		mvprintw(10, 10 , "%d", k);
		fclose(fp);
		for(i = 0; i < ro; i++) {
			for(j = 0; j < col; j++)
			v[i][j] = 0;
		}

}

	if(flag1 == 0) {
	for(i = 0; i < ro; i++) {
		for(j = 0; j < col; j++)
			v[i][j] = 0;
	}
/*	for(i = 0; i < ro; i++) {
		for(j = 0; j < col; j++)
			mvprintw(30 + i,5 + j,"%d", v[i][j]);
	}*/

	for(i = 0; i < ro; i++) {
		for(j = 0; j < col; j++) 
			a[i][j] = 'q';
	}

	for(i = 0; i < ro; i++) {
		for(j = 0; j < col; j++) 
			d[i][j] = '3';
	}


	srand(time(NULL));
	while(i1 < dif) {
		i2 = rand() % ro;
		j2 = rand() % col;
		while(a[i2][j2] != '*') {
			a[i2][j2] = '*';
			i1++;
		}
	}
	for(i = 0; i < ro; i++) {
		for(j = 0; j < col; j++) 
			e[i][j] = '.';
	}
/*	for(i = 0; i < ro; i++) {
		for(j = 0; j < col; j++)
			mvprintw(10 + i, 10 + j,"%c", e[i][j]);
	}*/


	/*counting the no. of mines around each box*/
	for(i2 = 0; i2 < ro; i2++) {
		for(j2 = 0; j2 < col; j2++) {
			mines = 0;
			for(i = -1; i <= 1; i++) {
				for(j = -1; j <= 1; j++) {
					if(((i2 + i) < 0) || ((i2 + i) > (ro - 1)) || ((j2 + j) < 0) || ((j2 + j) > (col - 1)))
						mines = mines + 0;
					else if(a[i2 + i][j2 + j] != '*')
						mines = mines + 0;
					else if(a[i2 + i][j2 + j] == '*')
						mines = mines + 1;
				}
			}
			b = mines + 48;
			if(b == 48)
				a[i2][j2] = ' ';
			if(a[i2][j2] != '*' && a[i2][j2] != ' ')
				a[i2][j2] = b;
		}
	}

	}

	mvprintw((LINES - ro - 20) / 2, (COLS - 14) / 2, "Mines left: %d", (dif - k));
	/*printing in the middle*/	
	x = (COLS - ((col - 1) * 4 + 1)) / 2;
	p = x;
	y = (LINES - (ro - 1) * 2) / 2;
	q = y;
	move(y, x);

/*	move(y, x);	
	for(i = y - 1; i < y + (ro - 1) * 2 + 2; i++) {
		for(j = x - 3; j < x + (col - 1) * 4 + 4; j++)
			mvprintw(i, j, " ");
	}

	x = p;
	y = q;
	move(y, x);
	for(i = 0; i < ro; i++) {
		for(j = 0; j < col; j++) {
			printw(".");
			x += 4;
			move(y, x);
		}
		x = p;
		y += 2;
		move(y, x);
	}*/
	printscr(x, y, p, q, e);
	move(q, p);
	x = p;
	y = q;


//	attron(A_REVERSE);
//	mvprintw( 15, 61, ".");
//	attroff(A_REVERSE);
/*	for(i = 0; i < ro; i++) {
		for(j = 0; j < col; j++) 
			v[i][j] = 0;
	}*/

	/*assigning functions to keys*/
	while((c = getch()) != 'q') {
		int flag = 0;
	//	init_pair(2, COLOR_WHITE, COLOR_RED);
	//	attrset(COLOR_PAIR(2));
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
			case 'c':
				getstr(str);
				if(strcmp(str, "heat") == 0){
					for( i = 0; i < ro; i++) {
						for(j = 0; j < col; j++)
							mvprintw((LINES - ro) / 2 + i, COLS - col + j,"%c",a[i][j]);
							mvprintw((LINES - ro) / 2 + i, COLS - col + j, "\n");
					}
				}
				break;
			case 10:
				if(a[(y - q) / 2][(x - p) / 4] == '*' && d[(y - q) / 2][(x - p) / 4] != '#') {
				//	init_pair(1, COLOR_RED, COLOR_BLACK);
					revealmines(a, p, q);
					attron(COLOR_PAIR(1));
					refresh();
					sleep(1);
					mvprintw((LINES) / 2, (COLS - 16) / 2, "BOOM!!! YOU LOST");
					attroff(COLOR_PAIR(1));
					refresh();
					sleep(2);
					return;
				}
				else if( d[(y - q) / 2][(x - p) / 4] == '#');
				else if( a[(y - q) / 2][(x - p) / 4] != '*') { 
					printw("%c", a[(y - q) / 2][(x - p) / 4]);
					e[(y - q) / 2][(x - p) / 4] =  a[(y - q) / 2][(x - p) / 4];
					if(a[(y - q) / 2][(x - p) / 4] == ' '){
//						printw("%c", a[(y - q) / 2][(x - p) / 4]);
						move(y, x);
						openmine(y, x, p, q, a, v, e);
					}
				}
				break;
			case 'n':
			//	init_pair(2, COLOR_WHITE, COLOR_RED);
			//	init_pair(1, COLOR_RED, COLOR_BLACK);
			//	attrset(COLOR_PAIR(2));
			//	attron(COLOR_PAIR(1));
				mvprintw( LINES / 2, (COLS - 25) / 2, "Do Want To Start A New Game");
				mvprintw( LINES / 2 + 1, COLS / 2 - 3 ,"(y/n)?");
			//	attroff(COLOR_PAIR(1));
			//	init_pair(1, COLOR_WHITE, COLOR_RED);
				if((c = getch()) == 'y') {
					clear();
					refresh();
					endwin();
					goto S;
				}
				else if((c == 'n')) {
			//		init_pair(1, COLOR_WHITE, COLOR_RED);
			//		attrset(COLOR_PAIR(1));
					move(q, p);
					x = p;
					y = q;
					printscr(x, y, p, q, e);
					move(q, p);
					x = p;
					y = q;
					}
				break;
			case 'f':
				if(e[(y - q) / 2][(x - p) / 4] == '.') {
					printw("#");
					e[(y - q) / 2][(x - p) / 4] = '#';
					k++;
					d[(y - q) / 2][(x - p) / 4] = '#';
				}
				else if(e[(y - q) / 2][(x - p) / 4] == '#') {
					e[(y - q) / 2][(x - p) / 4] = '.';
					d[(y - q) / 2][(x - p) / 4] = '3';
					printw(".");
					k--;
				}
				mvprintw((LINES - ro - 20) / 2, (COLS - 14) / 2, "Mines left: %d", (dif - k));

				if((dif - k) < 10 && (dif - k) >= 0)
					mvprintw((LINES - ro - 20) / 2, (COLS - 14) / 2 + 13," "); 
				break ;
		}
	
	if(k == dif) {
		for(i = 0; i < ro; i++) {
			for(j = 0; j < col; j++) {
				if((a[i][j] != '*' && d[i][j] == '3') || ( a[i][j] == '*' && d[i][j] == '#'))
					flag++;
			}
		}
	}
/*	for(i = 0; i < ro; i++) {
		for(j = 0; j < col; j++)
			mvprintw(10 + i,10 + j,"%c", e[i][j]);
	}*/
/*
		mvprintw(2, 50, "%d", (ro * col));
		mvprintw(3, 50, "%d", flag);*/
		refresh();
	if(flag == (ro * col)) {
	//	init_pair(1, COLOR_RED, COLOR_BLACK);
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
	r = ro;
	co = col;
	mvprintw( LINES / 2, (COLS - 24) / 2, "Do Want To Save The Game");
	mvprintw( LINES / 2 + 1, COLS / 2 - 3 ,"(y/n)?");
	while(1) {
		if((c = getch()) == 'y') {
			save(a, d, e, v, r, c, dif, k);
			break;
			}
		else if(c == 'n')
			break;
	}
/*	for(i = 0; i < col; i++)
		free(v[i]);
	free(v);*/
	refresh();
	clear();
//	endwin();
//	menu();
	refresh();
}

void save(char a[ro][col], char d[ro][col], char e[ro][col], int v[ro][col], int r, int co, int dif, int k) {
	FILE *fp;
	int i, j;
	fp = fopen("resume.txt", "w");
	if(fp == NULL)
		printw("operation failed\n");
	for(i = 0; i < ro; i++) {
		for(j = 0; j < col; j++)
			fprintf(fp, "%c", a[i][j]);
	}
	for(i = 0; i < ro; i++) {
		for(j = 0; j < col; j++)
			fprintf(fp, "%c", d[i][j]);
	}
	for(i = 0; i < ro; i++) {
		for(j = 0; j < col; j++)
			fprintf(fp, "%c", e[i][j]);
	}
	for(i = 0; i < ro; i++) {
		for(j = 0; j < col; j++)
			fprintf(fp, "%d", v[i][j]);
	}

	fprintf(fp, "%d", k);
	fprintf(fp, "%d", ro);
	fprintf(fp, "%d", col);
	fprintf(fp, "%d", dif);
	fclose(fp);
}
void printscr(int x, int y, int p, int q, char e[ro][col]) {
	int i, j;
	move(y, x);	
	for(i = y - 1; i < y + (ro - 1) * 2 + 2; i++) {
		for(j = x - 3; j < x + (col - 1) * 4 + 4; j++)
			mvprintw(i, j, " ");
	}

	x = p;
	y = q;
	move(y, x);
	for(i = 0; i < ro; i++) {
		for(j = 0; j < col; j++) {
			printw("%c", e[i][j]);
			x += 4;
			move(y, x);
		}
		x = p;
		y += 2;
		move(y, x);
	}
/*	move(q, p);
	x = p;
	y = q;*/
}

void revealmines(char a[ro][col], int p, int q) {
	int i, j;
	for(i = 0; i < ro; i++) {
		for(j = 0; j < col; j++) {
			if(a[i][j] == '*')
				mvprintw((2 * i) + q, (4 * j) + p, "*");
		}
	}
}

int opn(int i, int j, int i1, int j1, int p, int q, char a[ro][col], char e[ro][col]) {
	if((i + i1) < 0 || ((i + i1) > (col - 1)) || ((j + j1) < 0) || ((j + j1) > (ro - 1)))
		return 0;
//	else if(a[j + j1][i + i1] != ' ')
		
//		return 0;
	else{
		e[j + j1][i1 + i] =  a[j + j1][i1 + i];	
		mvprintw( ((j + j1) * 2) + q, ((i1 + i) * 4) + p,"%c", a[j + j1][i1 + i]);
		return 0;
	}
}

int openmine(int y, int x, int p, int q, char a[ro][col], int v[ro][col], char e[ro][col]) {
	int i = 0, j = 0;
	j = (y - q) / 2;
	i = (x - p ) / 4;
	refresh();
//	sleep(1);
	if(a[j][i] == ' ') {
		v[j][i] = 1;
		opn(i, j, 1, -1, p, q, a, e);
		opn(i, j, 1, 0, p, q, a, e);
		opn(i, j, 1, 1, p, q, a, e);
		opn(i, j, 0, 1, p, q, a, e);
		opn(i, j, -1, 1, p, q, a, e);
		opn(i, j, -1, 0, p, q, a, e);
		opn(i, j, -1, -1, p, q, a, e);
		opn(i, j, 0, -1, p, q, a, e);
		if(!((i + 1) < 0) && !((j + -1) < 0) && !((col - 1) < (i + 1)) && !((ro - 1) < (j + -1)) && !v[j + -1][i + 1]){
//			if(a[j + -1][i + 1] == ' ')
				openmine(2 * (j + -1) + q, 4 * (i + 1) + p, p, q, a, v, e);
		}
		refresh();
	//	sleep(1);
		if(!((i + 1) < 0) && !((j + 0) < 0) && !((col - 1) < (i + 1)) && !((ro - 1) < (j + 0)) && !v[j + 0][i + 1]){
//			if(a[j + 0][i + 1] == ' ')
				openmine(2 * (j + 0) + q, 4 * (i + 1) + p, p, q, a, v, e);
		}
		refresh();

		if(((i + 1) >= 0) && ((j + 1) >= 0) && ((col - 1) >= (i + 1)) && ((ro - 1) >= (j + 1)) && !v[j + 1][i + 1]){
//			if(a[j + 1][i + 1] == ' ')
				openmine(2 * (j + 1) + q, 4 * (i + 1) + p, p, q, a, v, e);
		}
		refresh();
//		sleep(1);
		if(!((i + 0) < 0) && !((j + 1) < 0) && !((col - 1) < (i + 0)) && !((ro - 1) < (j + 1)) && !v[j + 1][i + 0]){
//			if(a[j + 1][i + 0] == ' ')
				openmine(2 * (j + 1) + q, 4 * (i + 0) + p, p, q, a, v, e);
		}
		refresh();
		if(!((i + -1) < 0) && !((j + 1) < 0) && !((col - 1) < (i + -1)) && !((ro - 1) < (j + 1)) && !v[j + 1][i + -1]){
//			if(a[j + 1][i + -1] == ' ')
				openmine(2 * (j + 1) + q, 4 * (i + -1) + p, p, q, a, v, e);
		}	
		refresh();
		if(!((i + -1) < 0) && !((j + 0) < 0) && !((col - 1) < (i + -1)) && !((ro - 1) < (j + 0)) && !v[j + 0][i + -1]){
//			if(a[j + 0][i + -1] == ' ')
				openmine(2 * (j + 0) + q, 4 * (i + -1) + p, p, q, a, v, e);
		}
			refresh();
		if(!((i + -1) < 0) && !((j + -1) < 0) && !((col - 1) < (i + -1)) && !((ro - 1) < (j + -1)) && !v[j + -1][i + -1]){
//			if(a[j + -1][i + -1] == ' ')
				openmine(2 * (j + -1) + q, 4 * (i + -1) + p, p, q, a, v, e);
		}
				refresh();
		if(!((i + 0) < 0) && !((j + -1) < 0) && !((col - 1) < (i + 0)) && !((ro - 1) < (j + -1)) && !v[j + -1][i + 0]){
//			if(a[j + -1][i + 0] == ' ')
				openmine(2 * (j + -1) + q, 4 * (i + 0) + p, p, q, a, v, e);
		}
			refresh();

}
return 0;
}
