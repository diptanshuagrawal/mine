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

/* making a datatype of name mine where all the important information of the minefield is stored */
typedef struct mine{
	char **a;
	char **d;
	char **e;
	int **v;
	int k;
}mine;

/* function to display minesweeper in the beginning */
void banner();
/* function to print the title in the center */
void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string, chtype color);
/* function to go back */
void back();
/* function to display menu */
void menu();
/* function to start the game */
void game();
/* function to resume game */
void resume();
/* function to exit from the menu */
void exitmenu();
/* function to display the options menu */
void options();
/* function to set the difficulty level */
void diff();
/* function to set the no. of rows and columns as per user */
void row();
/* function to unveil the tile */
int opn(int i, int j, int i1, int j1, int p, int q, mine m);
int openmine(int y, int x, int p, int q, mine m);
/* function to dispaly the about screen of the game */
void about();
/* function to reveal all the mines on the minefield when the user clicks on any mine */
void revealmines(mine m, int p, int q);
/* function to save all the data to files */
void save(mine m);
/* function to print the screen with the red box and the grid */
void printscr(int x, int y, int p, int q, mine m);
