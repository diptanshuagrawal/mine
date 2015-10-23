#include <ncurses.h>
#include <menu.h>
#include <stdlib.h>
#include <string.h>

#define SIZE	8
#define CTRLD	4

void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string, chtype color);
void menu();
void game();
void resume();
void exitmenu();
void instruction();
void revealmines(char a[SIZE][SIZE], int p, int q);
int openmine(int y, int x, int p, int q, char a[SIZE][SIZE]);
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

void menu() {
	#define CTRLD	4

	char *choices[] = {
				"            Play Game!!!         ",
				"            Resume Game          ",
				"              Options            ",
				"           Instructions          ",
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
	attrset(COLOR_PAIR(1));
	
	my_items = (ITEM **)calloc(n_choices + 1, sizeof(ITEM *));
	
	for(i = 0; i < n_choices; ++i)
		my_items[i] = new_item(choices[i], NULL);
	my_items[n_choices] = (ITEM *)NULL;

	set_item_userptr(my_items[0], game);
	set_item_userptr(my_items[1], resume);
//	set_item_userptr(my_items[2], options);
	set_item_userptr(my_items[3], instruction);
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
	mvprintw(LINES - 2, 0, "F1 to EXIT");
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
				attrset(COLOR_PAIR(1));
				break;
			}
			break;
		}
		initscr();
		init_pair(1, COLOR_RED, COLOR_BLACK);
		bkgd(COLOR_PAIR(1));
		attrset(COLOR_PAIR(1));
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
void exitmenu() {
	exit(0);
}
void instruction() {
	int c, i, j;
	clear();
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	
	start_color();
	bkgd(COLOR_PAIR(1));
	init_pair(1, COLOR_WHITE, COLOR_CYAN);
	attrset(COLOR_PAIR(1));
//	for(i = 0; i < LINES; i++){
//		for(j = 0; j < COLS; j++){
//			mvprintw(i, j, " ");
//			sleep(1);
//		}
//	}
	box(stdscr, 0, 0);
	mvprintw(LINES / 2 - 10, COLS / 2 - 6, "INSTRUCTIONS");
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
	int c;
	initscr();
	cbreak;
	noecho();
	keypad(stdscr, TRUE);
	//while((c = getch()) != KEY_F(1))
	//	;
	printw("hi");
	start_color();
	init_pair(1, COLOR_WHITE, COLOR_BLACK);
	attrset(COLOR_PAIR(1));
	while(c = getch() != 'q');
	clear();
//	endwin();
	//exit();
}
	
void game() {
//	unpost_menu(my_menu);
//	free_item(my_items[0]);
//	free_item(my_items[1]);
//	free_item(my_items[2]);
//	free_item(my_items[3]);
//	free_menu(my_menu);
//	endwin();
	clear();
	int i, j, k = 0, y, x, p, q, i1 = 0, i2,j2, mines = 0, flag = 0;
	int c;
	char a[SIZE][SIZE], d[SIZE][SIZE], e[SIZE][SIZE], b;
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);

	start_color();
	init_pair(3, COLOR_WHITE, COLOR_RED);
	init_pair(2, COLOR_BLACK, COLOR_YELLOW);
	init_pair(1, COLOR_RED, COLOR_BLACK);
	attrset(COLOR_PAIR(3));

	box(stdscr, 0, 0);
	mvprintw(0, 0, "Use Arrow Keys To Navigate");
	mvprintw(1, 0, "Press Enter To Select Tile");
	mvprintw(2, 0, "Press Q To Exit");
	mvprintw(3, 0, "Press F To Flag");
	attrset(COLOR_PAIR(3));

	srand(time(NULL));
	while(i1 < SIZE) {
		i2 = rand() % SIZE;
		j2 = rand() % SIZE;
		while(a[i2][j2] != '*') {
			a[i2][j2] = '*';
			i1++;
		}
	}
	for(i = 0; i < SIZE; i++) {
		for(j = 0; j < SIZE; j++) 
			e[i][j] = a[i][j];
	}
	/*counting the no. of mines around each box*/
	for(i2 = 0; i2 < SIZE; i2++) {
		for(j2 = 0; j2 < SIZE; j2++) {
			mines = 0;
			for(i = -1; i <= 1; i++) {
				for(j = -1; j <= 1; j++) {
					if( ((a[i2 + i][j2 + j] != '*') || (i2 + i) < 0) || ((i2 + i) > (SIZE - 1)) || ((j2 + j) < 0) || ((j2 + j) > SIZE - 1)) 
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
	for( i = 0; i < SIZE; i++) {
		for(j = 0; j < SIZE; j++)
			mvprintw(30 + i,30 + j,"%c",a[i][j]);
		mvprintw(30 + i, 30 + j, "\n");
	}
	mvprintw((LINES + (SIZE - 1) + 20) / 2, (COLS - 14) / 2, "Mines left: %d", SIZE);
	/*printing in the miidle*/	
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

	/*assigning functions to keys*/
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
				if(a[(y - q) / 2][(x - p) / 4] == '*') {
					revealmines(a, p, q);
					attron(COLOR_PAIR(1));
					refresh();
					sleep(1);
					mvprintw((LINES) / 2, (COLS - 16) / 2, "BOOM!!! YOU LOST");
					attroff(COLOR_PAIR(1));
				}
				else if( d[(y - q) / 2][(x - p) / 4] == '#');
				else { 
				printw("%c", a[(y - q) / 2][(x - p) / 4]);
				//openmine(y, x, p, q, a);
				}
				break;
/*			case 'n':
				clear();
				refresh();
				endwin();
				game();
				break;*/
			case 'f':
				if(d[(y - q) / 2][(x - p) / 4] != '#') {
					printw("#");
					k++;
					d[(y - q) / 2][(x - p) / 4] = '#';
				}
				else {
					d[(y - q) / 2][(x - p) / 4] = '.';
					printw(".");
					k--;
				}
				mvprintw((LINES + (SIZE - 1) + 20) / 2, (COLS - 14) / 2, "Mines left: %d", (SIZE) - k);
/*				if(k = SIZE) {
					for(i = 0; i < SIZE; i++) {
						for(j = 0; j < SIZE; j++) {
							if((e[i][j] != '*' && d[i][j] != '#') || ( e[i][j] == '*' && d[i][j] == '#'))
								flag = 1;*/
				break ;
		}
	move(y, x);
	}
	refresh();
	clear();
//	endwin();
//	menu();
	refresh();
}
void revealmines(char a[SIZE][SIZE], int p, int q) {
	int i, j;
	for(i = 0; i < SIZE; i++) {
		for(j = 0; j < SIZE; j++) {
			if(a[j][i] == '*')
				mvprintw((2 * j) + q, (4 * i) + p, "*");
		}
	}
}
/*int openmine(int y, int x, int p, int q, char a[SIZE][SIZE]) {
	int i = 0, j = 0;
	j = (y - q) / 2;
	i = (x - p ) / 4;
	for(i1 = -1; i1 <= 1; i1++) {
		for(j1 = -1; j1 <= 1; j1++) {
			if(a[i + i1][j + j1] != ' ') {
				mvprintw( ((j + j1) * 2) + q, ((i1 + i) * 4) + p, "%c", a[i + i1][j + j1]);
				return 0;
			}
			else if(a[i + i1][j + j1] == ' ') {
				mvprintw( ((j + j1) * 2) + q, ((i1 + i) * 4) + p, " ");
				openmine( ((j + j1) * 2) + q, ((i1 + i) * 4) + p, p, q, a);
			}
		}
	}
	if(a[j][i] == ' ') {
	if( ((a[j + 1][i + 1] != ' ') || (i + 1) < 0) || ((i + 1) > (SIZE - 1)) || ((j + 1) < 0) || ((j + 1) > SIZE - 1))
		return 0;
	else if(a[j + 1][i + 1] == ' '){
		mvprintw( ((j + 1) * 2) + q, ((1 + i) * 4) + p," ");
		openmine( ((j + 1) * 2) + q, ((1 + i) * 4) + p, p, q, a);
	}
	
	if( ((a[j + 1][i + 0] != ' ') || (i + 0) < 0) || ((i + 0) > (SIZE - 1)) || ((j + 1) < 0) || ((j + 1) > SIZE - 1))
		return 0;
	else if(a[j + 1][1 + 0] == ' '){
		mvprintw( ((j + 1) * 2) + q, ((0 + i) * 4) + p, " ");
		openmine( ((j + 1) * 2) + q, ((0 + i) * 4) + p, p, q, a);
	}
	if( ((a[j + -1][i + -1] != ' ') || (i + -1) < 0) || ((i + -1) > (SIZE - 1)) || ((j + -1) < 0) || ((j + -1) > SIZE - 1))
		return 0;
	else if(a[j + -1][i + -1] == ' '){
		mvprintw( ((j + -1) * 2) + q, ((-1 + i) * 4) + p, " ");
		openmine( ((j + -1) * 2) + q, ((-1 + i) * 4) + p, p, q, a);
	}
	if( ((a[j + 0][i + -1] != ' ') || (i + -1) < 0) || ((i + -1) > (SIZE - 1)) || ((j + 0) < 0) || ((j + 0) > SIZE - 1))
		return 0;
	else if(a[j + 0][i + -1] == ' '){
		mvprintw( ((j + 0) * 2) + q, ((-1 + i) * 4) + p," ");
		openmine( ((j + 0) * 2) + q, ((-1 + i) * 4) + p, p, q, a);
	}
	if( ((a[j + 1][i + -1] != ' ') || (i + -1) < 0) || ((i + -1) > (SIZE - 1)) || ((j + 1) < 0) || ((j + 1) > SIZE - 1))
		return 0;
	else if(a[j + 1][i + -1] == ' '){
		mvprintw( ((j + 1) * 2) + q, ((-1 + i) * 4) + p, " ");
		openmine( ((j + 1) * 2) + q, ((-1 + i) * 4) + p, p, q, a);
	}
	if( ((a[j + -1][i + 0] != ' ') || (i + 0) < 0) || ((i + 0) > (SIZE - 1)) || ((j + -1) < 0) || ((j + -1) > SIZE - 1))
		return 0;
	else if(a[j + -1][i + 0] == ' '){
		mvprintw( ((j + -1) * 2) + q, ((0 + i) * 4) + p, " ");
		openmine( ((j + 0) * 2) + q, ((1 + i) * 4) + p, p, q, a);
	}
	if( ((a[j + -1][i + 1] != ' ') || (i + 1) < 0) || ((i + 1) > (SIZE - 1)) || ((j + -1) < 0) || ((j + -1) > SIZE - 1))
		return 0;
	else if(a[j + -1][i + 1] == ' '){
		mvprintw( ((j + -1) * 2) + q, ((1 + i) * 4) + p, " ");
		openmine( ((j + -1) * 2) + q, ((1 + i) * 4) + p, p, q, a);
	}
	if( ((a[j + 0][i + 1] != ' ') || (i + 1) < 0) || ((i + 1) > (SIZE - 1)) || ((j + 0) < 0) || ((j + 0) > SIZE - 1))
		return 0;
	else if(a[j + 0][i + 1] == ' '){
		mvprintw( ((j + 0) * 2) + q, ((1 + i) * 4) + p, " ");
		openmine( ((j + 0) * 2) + q, ((1 + i) * 4) + p, p, q, a);
	}
}
}*/ 
