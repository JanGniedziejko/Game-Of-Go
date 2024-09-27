#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include"conio2.h"

/* Comment: in the final program declare appropriate constants, e.g.,
   to eliminate from your program numerical values by replacing them.
   with well defined identifiers */
int bounded (char **src, const int n, int x, int y,char direction) // checking if the stone is bounded (the function is always checking only 3 sides)
{
    int k=0, max=0;
    if (direction != 'r'){ if(x>0) {max+=1; if (src[y][x-1]!=' ') k+=1;}}
    if (direction != 'l') {if (x<n-1) {max+=1; if (src[y][x+1]!=' ') k+=1;}}
    if (direction != 'd') {if (y>0) {max+=1; if (src[y-1][x]!=' ') k+=1;}}
    if (direction != 'u') {if (y<n-1) {max+=1; if (src[y+1][x]!=' ') k+=1;}}
    if (k==max) return true;
    return false;
}
int tokill (char **src, const int n, int x, int y,int turn,char direction) // checking if the stone/stones are bounded by another player's stones
{
    int max=0,sorrounded=0;
    char stone;
    if (turn==1) stone='o';
    else stone='x';
    if (bounded(src,n,x,y,direction)){
        src[y][x]='|';
        if (direction != 'u') {
            if (y<n-1) {
                max += 1;
                if (src[y + 1][x] == stone) {
                    if (tokill(src, n, x, y + 1, turn, 'd'))
                        sorrounded += 1;
                }
                else sorrounded+=1;
            }
        }
        if (direction != 'd') {
            if (y>0) {
                max += 1;
                if (src[y - 1][x] == stone) {
                    if (tokill(src, n, x, y - 1, turn, 'u'))
                        sorrounded += 1;
                }
                else sorrounded+=1;
            }
        }
        if (direction != 'l')  {
            if (x<n-1) {
                max += 1;
                if (src[y][x+1] == stone) {
                    if (tokill(src, n, x+1, y, turn, 'r'))
                        sorrounded += 1;
                }
                else sorrounded+=1;
            }
        }
        if (direction != 'r') {
            if (x>0) {
                max += 1;
                if (src[y][x-1] == stone) {
                    if (tokill(src, n, x-1, y, turn, 'l'))
                        sorrounded += 1;
                }
                else sorrounded+=1;
            }
        }
        src[y][x]=stone;
        if (max==sorrounded) return true;
        else return false;
    }
    return false;
}
void killingprocess(char **src,const int n, int x, int y, char direction) // function is changing a stone/stones into empty spaces
{
    char temp=src[y][x];
    src[y][x]=' ';
    if (direction != 'u') {
        if (y<n-1) {if (src[y + 1][x] == temp) killingprocess(src,n,x,y+1,'d'); }
    }

    if (direction != 'd') {
        if (y>0) { if (src[y - 1][x] == temp) killingprocess(src,n,x,y-1,'u');}
    }
    if (direction != 'l'){
        if (x<n-1) {if (src[y][x+1] == temp) killingprocess(src,n,x+1,y,'r');}
    }

    if (direction != 'r') {
        if (x>0) {if (src[y][x-1] == temp) killingprocess(src,n,x-1,y,'l');}
    }
}
int suicide(char **src, const int n, int x, int y, int turn) //checking if the potentially placed stone won't be surrounded by another player's stone
{
    int neighbours=0,max=0;
    char enemy;
    if (turn==1) enemy='x';
    else enemy='o';
    if (x>0) {max+=1; if (src[y][x-1]==enemy) neighbours+=1;}
    if (x<n-1) {max+=1; if (src[y][x+1]==enemy) neighbours+=1;}
    if (y>0) {max+=1; if (src[y-1][x]==enemy) neighbours+=1;}
    if (y<n-1) {max+=1; if (src[y+1][x]==enemy) neighbours+=1;}
    if (neighbours==max) return 0;
    else return 1;
}
int count(char **src, const int n, int turn) // this function counts how many stones of each player are placed on the board
{
    int amount=0;
    char stone;
    if (turn==1) stone='o';
    else if (turn==2) stone='x';
    else if (turn==3) stone=','; //black
    else if (turn==4) stone='.'; //white
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            if (src[j][i]==stone) amount+=1;
        }

    }
    return amount;
}
void copyboard(char **src, char **copy, const int n) // copying board
{
    for (int i=0;i<n;i++){
        for (int j=0;j<n;j++){
            copy[i][j]=src[i][j];
        }
    }
}
int kocheck(char **src, char **copy, const int n, int x, int y, int turn) // checking if player wants to make a move that will return the game state to the previous position
{
    char stone;
    int max, correspondence=0;
    if (turn == 1) stone='o';
    else stone='x';
    max = count (copy,n,turn);
    src[y][x]=stone;
    for (int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            if(src[i][j]==stone && copy[i][j]==stone) correspondence+=1;
        }
    }
    src[y][x]=' ';
    if (max == correspondence) return false;
    else {
        return true;
    }
}
int fields(char **src,const int n, int x, int y, int turn, char direction) // checking if an area made of empty squares is surrounded by one player's stones
{
    int max=0,surrounded=0;
    char stone, field;
    if (turn==1) {stone='o'; field=',';}
    else {stone='x'; field='.';}
    src[y][x]=field;
    if (direction != 'u') {
        if (y<n-1) {
            max += 1;
            if (src[y + 1][x] == ' ') {
                if (fields(src,n,x,y+1,turn,'d')){
                    surrounded+=1;
                }
            }
            else if(src[y+1][x] == field || src[y+1][x] == stone) surrounded+=1;
        }
    }
    if (direction != 'l')  {
        if (x<n-1) {
            max += 1;
            if (src[y][x + 1] == ' ') {
                if (fields(src, n, x + 1, y, turn, 'r')) {
                    surrounded += 1;
                }
            }
            else if(src[y][x+1] == field || src[y][x+1] == stone) surrounded+=1;
        }
    }
    if (direction != 'd') {
        if (y>0) {
            max += 1;
            if (src[y - 1][x] == ' ') {
                if (fields(src, n, x, y - 1, turn, 'u')) {
                    surrounded += 1;
                }
            }
            else if(src[y-1][x] == field || src[y-1][x] == stone) surrounded+=1;
        }
    }
    if (direction != 'r') {
        if (x>0) {
            max += 1;
            if (src[y][x-1] == ' ') {
                if (fields(src, n, x-1, y, turn, 'l')) {
                    surrounded += 1;
                }
            }
            else if(src[y][x-1] == field || src[y][x-1] == stone) surrounded+=1;
        }
    }
    src[y][x]=' ';
    if (max==surrounded) return true;
    else return false;
}
void changing (char **src, const int n, int x, int y, char character, char direction) // changing a chain of empty spaces into particular character
{
    src[y][x]=character;
    if (direction != 'u') {
        if (y<n-1) {if (src[y + 1][x] == ' ') changing(src,n,x,y+1,character,'d'); }
    }

    if (direction != 'd') {
        if (y>0) { if (src[y - 1][x] ==' ') changing(src,n,x,y-1,character,'u');}
    }
    if (direction != 'l'){
        if (x<n-1) {if (src[y][x+1] == ' ') changing(src,n,x+1,y,character,'r');}
    }

    if (direction != 'r') {
        if (x>0) {if (src[y][x-1] == ' ') changing(src,n,x-1,y,character,'l');}
    }
}
int main() {
	int zn = 0, x = 60, y = 3, attr = 7, back = 0, zero = 0, player = 1,choose,size=9;
    int before=0,after=0; // im using those variables in order to count score of each player
    const int boardX  = x, boardY = y,instructionsX=2,instructionsY=1; // constant values of legend and also position of the start of the board
    int start=true,end=false,loaded=false; // variables that are showing the state of the game
    int handicap=false, ko=false, during=false; // essential variables to conditional statements
    int tempX, tempY; // values of those variables are equal to boardX - x, for ease of use
    float score1=0,score2=0,additional=6.5; // score of each player
	char txt[32];
    char **board, **copy;

    //choosing the size of the board
    gotoxy(1, 1);
    cputs("Game of Go");
    gotoxy(1, 3);
    cputs("Choose the board size: ");
    gotoxy(1, 4);
    cputs("A) 9x9 [default]");
    gotoxy(1, 5);
    cputs("B) 13x13");
    gotoxy(1, 6);
    cputs("C) 19x19");
    gotoxy(1, 7);
    cputs("D) Different size:");
    gotoxy(1,9);
    cputs("l = load  the game state from the text file");
    zn = getch();
    int temp;

    switch (zn) {
        case 'l': {
            char nameoffileloaded[50];
            start = false;
            loaded=true;
            clrscr();
            gotoxy(1, 1);
            cputs("Type the name of the file:");
            gotoxy(28, 1);
            for (int i = 0; i < 50; i++) {
                temp = getche();
                if (temp == 0x0d) break;
                else nameoffileloaded[i] = (char) temp;
            }

            FILE *file;
            file = fopen(nameoffileloaded, "r");
            if (file == NULL) {
                printf("Error opening file.\n");
                return 1;
            }
            int row = 0;
            while (!feof(file)) {
                if (ferror(file)) {
                    printf("Error reading file.\n");
                    return 1;
                }
                if (row == 0) {
                    fscanf(file, "%d %d %f %f %f", &size, &player, &score1, &score2, &additional);
                    board = new char *[size];
                    copy = new char *[size];
                    for (int i = 0; i < size; i++) {
                        board[i] = new char[size];
                        copy[i] = new char[size];
                    }
                } else {

                    for (int i = 0; i < size; i++) {
                        if (row == 1 && i == 0) fscanf(file, " %c", &board[row - 1][i]);
                        else fscanf(file, "%c", &board[row - 1][i]);
                    }
                }
                row++;
                if (row == size + 1) break;
                fclose(file);
            }
            break;
        }
        case 'a':
            size = 9;
            break;
        case 'b':
            size = 13;
            break;
        case 'c':
            size = 19;
            break;
        case 'd':
            int num = 0, poweroften = 1, rem = 0, reversed_num = 0;
            gotoxy(20, 7);
            do {
                temp = getche();
                switch (temp) {
                    case 0x31:
                        num += 1 * poweroften;
                        break;
                    case 0x32:
                        num += 2 * poweroften;
                        break;
                    case 0x33:
                        num += 3 * poweroften;
                        break;
                    case 0x34:
                        num += 4 * poweroften;
                        break;
                    case 0x35:
                        num += 5 * poweroften;
                        break;
                    case 0x36:
                        num += 6 * poweroften;
                        break;
                    case 0x37:
                        num += 7 * poweroften;
                        break;
                    case 0x38:
                        num += 8 * poweroften;
                        break;
                    case 0x39:
                        num += 9 * poweroften;
                        break;
                    case 0x30:
                        if (poweroften > 1) {
                            num += 1 * poweroften;
                        }
                        break;
                }
                poweroften *= 10;
            } while (temp != 0x0d);
            while (num != 0) {
                rem = num % 10;
                reversed_num = reversed_num * 10 + rem;
                num /= 10;
            }
            size = reversed_num;
            break;
    }
    clrscr();
	// if the program is compiled in pure C, then we need to initialize
	// the library ourselves; __cplusplus is defined only if a C++ compiler
	// is used
#ifndef __cplusplus
	Conio2_Init();
#endif
	// settitle sets the window title
	settitle("Jan Gniedziejko, 193633");

	// hide the blinking cursor
	_setcursortype(_NOCURSOR);
    if(!loaded) { // if the game wasn't loaded, then we are initialize an empty board
        board = new char *[size];
        copy = new char *[size];
        for (int i = 0; i < size; i++) {
            board[i] = new char[size];
            copy[i] = new char[size];
        }
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                board[i][j] = ' ';
                copy[i][j] = ' ';
            }
        }
    }
	do { // start of the game --> asking if a player 1 wants to play with handicap
        if (start){
            int decision;
            gotoxy(1,1);
            cputs("Do you want to play with handicap? y/n");
            decision=getch();
            switch(decision){
                case 0x79: {
                    handicap=true;
                    start=false;
                }
                    break;
                case 0x6e: {
                    start=false;
                    continue;
                }
                    break;
            }
            clrscr();
        }
        else {
            // we set black to be the background color
            // check conio2.h for available colors
            textbackground(BLACK);
            // clear the screen: we fill it out with spaces with
            // the specified background color
            clrscr();
            // we set the text color (7 == LIGHTGRAY)
            textcolor(7);
            // we move the coursor to column 48 and row 1
            // rows and column are numbered starting with 1
            gotoxy(instructionsX, instructionsY);
            cputs("Jan Gniedziejko, 193633");
            gotoxy(instructionsX, instructionsY+2);
            // we print out a text at a given cursor position
            // the cursor will move by the length of the text,
            cputs("q       = exit");
            gotoxy(instructionsX, instructionsY+3);
            cputs("cursors = moving");
            gotoxy(instructionsX, instructionsY+4);
            cputs("space   = change color");
            gotoxy(instructionsX, instructionsY+5);
            cputs("enter   = confirm choice and end player's turn");
            gotoxy(instructionsX, instructionsY+6);
            cputs("i       = place a stone on the board");
            gotoxy(instructionsX, instructionsY+7);
            cputs("esc     = cancel current action");
            gotoxy(instructionsX, instructionsY+8);
            cputs("s       = save the game state");
            gotoxy(instructionsX, instructionsY+9);
            cputs("n       = start a new game");
            gotoxy(instructionsX, instructionsY+10);
            cputs("l       = load the game state");
            gotoxy(instructionsX, instructionsY+11);
            cputs("f       = finish the game");
            gotoxy(instructionsX, instructionsY+13);
            sprintf(txt,"Player %d's turn ", player);
            cputs(txt);
            gotoxy(instructionsX, instructionsY+14);
            sprintf(txt,"Position: %d, %d ", (x - boardX + 1), (y - boardY + 1));
            cputs(txt);
            gotoxy(instructionsX, instructionsY+15);
            sprintf(txt,"Player 1(Black stones): %.0f", score1);
            cputs(txt);
            gotoxy(instructionsX, instructionsY+16);
            sprintf(txt,"Player 2(White stones): %.0f",score2);
            cputs(txt);
            gotoxy(instructionsX, instructionsY+17);
            // print out the code of the last key pressed
            if (zero) sprintf(txt, "key code: 0x%02x", zn);
            else sprintf(txt, "key code: 0x%02x", zn);
            gotoxy(instructionsX, instructionsY+18);
            cputs(txt);

            // displaying the board and the frame of the board
            for (int i = -1; i < (size + 1); i++) {
                for (int j = -1; j < (size + 1); j++) {
                    if (i == -1 && j == -1) {
                        gotoxy(boardX + j, boardY + i);
                        cputs("+");
                    }
                    else if (i == -1 && j == size) {
                        gotoxy(boardX + j, boardY + i);
                        cputs("+");
                    }
                    else if (i == size && j == size) {
                        gotoxy(boardX + j, boardY + i);
                        cputs("+");
                    }
                    else if (i == size && j == -1) {
                        gotoxy(boardX + j, boardY + i);
                        cputs("+");
                    }
                    else if (i == -1 || i == size) {
                        gotoxy(boardX + j, boardY + i);
                        cputs("-");
                    }
                    else if (j == -1 || j == size) {
                        gotoxy(boardX + j, boardY + i);
                        cputs("|");
                    }
                    else {
                        sprintf(txt,"%c", board[i][j]);
                        gotoxy(boardX + j, boardY + i);
                        cputs(txt);
                    }
                }
            }

            if (during == true) { // only printing stone on the board, the stone will be assigned to array 'board' if user will press 'enter' key, or the stone will disappear if user will press 'esc' key
                gotoxy(tempX + boardX, tempY + boardY);
                if (player == 1) putch('o');
                else putch('x');
            }
            // we draw a star
            gotoxy(x, y);
            textcolor(attr);
            textbackground(back);
            // putch prints one character and moves the cursor to the right
            putch('*');

            // we wait for key press and get its code
            // most key codes correspond to the characters, like
            // a is 'a', 2 is '2', + is '+', but some special keys
            // like cursors provide two characters, where the first
            // one is zero, e.g., "up arrow" is zero and 'H'
            zero = 0;
            zn = getch();
            // we do not want the key 'H' to play role of "up arrow"
            // so we check if the first code is zero
            if (zn == 0) {
                zero = 1;        // if this is the case then we read
                zn = getch();        // the next code knowing that this
                if (zn == 0x48) { if (y != boardY) y--; else continue; }    // will be a special key
                else if (zn == 0x50) { if (y != boardY + size - 1) y++; else continue; }
                else if (zn == 0x4b) { if (x != boardX) x--; else continue; }
                else if (zn == 0x4d) { if (x != boardX + size - 1) x++; else continue; }
            } else if (zn == ' ') attr = (attr + 1) % 16;
            else if (zn == 0x69) { // 'i' key --> place stone
                if (board[y - boardY][x - boardX] == ' ' && during==false) {
                    tempX = x - boardX;
                    tempY = y - boardY;
                    if (handicap) {
                        board[tempY][tempX] = 'o';
                        additional=0.5;
                    }
                    else if (ko){ //if the previous player killed a stone in that way that other player can return game to previous state, then we're checking if that player won't place a stone in that way
                        if (player==1){
                            if (kocheck(board,copy,size,tempX,tempY,1)) {during=true; ko=false;}
                        }
                        else {
                            if (kocheck(board,copy,size,tempX,tempY,2)) {during=true; ko=false;}
                        }
                    }
                    else if (player == 1) { // checking if there's not an obvious suicide situation
                        if (suicide(board, size, tempX, tempY, 1) == 1) {
                            during = true;
                        }
                        else { // a situation where a player can place a stone among the other player's stones, because that way the player can capture the other player's stone
                            if (tempY < size - 1) {
                                if (board[tempY + 1][tempX] == 'x') {
                                    if (tokill(board, size, tempX, tempY + 1, 2, 'd')) {
                                        during = true;
                                    }
                                }
                            }
                            if (tempY > 0) {
                                if (board[tempY - 1][tempX] == 'x') {
                                    if (tokill(board, size, tempX, tempY - 1, 2, 'u')) {
                                        during = true;
                                    }
                                }
                            }
                            if (tempX < size - 1) {
                                if (board[tempY][tempX + 1] == 'x') {
                                    if (tokill(board, size, tempX + 1, tempY, 2, 'r')) {
                                        during = true;
                                    }
                                }
                            }
                            if (tempX > 0) {
                                if (board[tempY][tempX - 1] == 'x') {
                                    if (tokill(board, size, tempX - 1, tempY, 2, 'l')) {
                                        during = true;
                                    }
                                }
                            }
                            if (during) {ko=true; copyboard(board,copy,size);} //if the situation described above took place, then we're copying the board to prevent returning game to previous state
                        }
                    }
                    else if (player == 2) { // the same thing as with the first player
                        if (suicide(board, size, x - boardX, y - boardY, 2) == 1) {
                            during = true;
                        } else {
                            if (tempY < size - 1) {
                                if (board[tempY + 1][tempX] == 'o') {
                                    if (tokill(board, size, tempX, tempY + 1, 1, 'd')) {
                                        during = true;
                                    }
                                }
                            }
                            if (tempY > 0) {
                                if (board[tempY - 1][tempX] == 'o') {
                                    if (tokill(board, size, tempX, tempY - 1, 1, 'u')) {
                                        during = true;
                                    }
                                }
                            }
                            if (tempX < size - 1) {
                                if (board[tempY][tempX + 1] == 'o') {
                                    if (tokill(board, size, tempX + 1, tempY, 1, 'r')) {
                                        during = true;
                                    }
                                }
                            }
                            if (tempX > 0) {
                                if (board[tempY][tempX - 1] == 'o') {
                                    if (tokill(board, size, tempX - 1, tempY, 1, 'l')) {
                                        during = true;
                                    }
                                }
                            }
                            if (during) {ko=true; copyboard(board,copy,size);}
                        }
                    }
                }
            } else if (zn == 0x1b) {during = false;} // 'esc' key --> cancel current action.
            else if (zn == 0x0d) { // 'enter' key --> confirms a player's round
                if (handicap){
                    player=2;
                    handicap=false;
                }
                else if (during) { //placing stone and checking if the player captures the other player's stones, and if so, adds point to the score
                    if (player == 1) {
                        board[tempY][tempX] = 'o';
                        before= count(board,size,2);
                        if (tempY < size - 1) {
                            if (board[tempY + 1][tempX] == 'x') {
                                if (tokill(board, size, tempX, tempY + 1, 2, 'd'))
                                    killingprocess(board, size, tempX, tempY + 1, 'd');
                            }
                        }
                        if (tempY > 0) {
                            if (board[tempY - 1][tempX] == 'x') {
                                if (tokill(board, size, tempX, tempY - 1, 2, 'u'))
                                    killingprocess(board, size, tempX, tempY - 1, 'u');
                            }
                        }
                        if (tempX < size - 1) {
                            if (board[tempY][tempX + 1] == 'x') {
                                if (tokill(board, size, tempX + 1, tempY, 2, 'r'))
                                    killingprocess(board, size, tempX + 1, tempY, 'r');
                            }
                        }
                        if (tempX > 0) {
                            if (board[tempY][tempX - 1] == 'x') {
                                if (tokill(board, size, tempX - 1, tempY, 2, 'l'))
                                    killingprocess(board, size, tempX - 1, tempY, 'l');
                            }
                        }
                        after = count(board,size,2);
                        score1 += (float)(before-after);
                        player = 2;
                    } else {
                        board[tempY][tempX] = 'x';
                        before = count(board,size,1);
                        if (tempY < size - 1) {
                            if (board[tempY + 1][tempX] == 'o') {
                                if (tokill(board, size, tempX, tempY + 1, 1, 'd'))
                                    killingprocess(board, size, tempX, tempY + 1, 'd');
                            }
                        }
                        if (tempY > 0) {
                            if (board[tempY - 1][tempX] == 'o') {
                                if (tokill(board, size, tempX, tempY - 1, 1, 'u'))
                                    killingprocess(board, size, tempX, tempY - 1, 'u');
                            }
                        }
                        if (tempX < size - 1) {
                            if (board[tempY][tempX + 1] == 'o') {
                                if (tokill(board, size, tempX + 1, tempY, 1, 'r'))
                                    killingprocess(board, size, tempX + 1, tempY, 'r');
                            }
                        }
                        if (tempX > 0) {
                            if (board[tempY][tempX - 1] == 'o') {
                                if (tokill(board, size, tempX - 1, tempY, 1, 'l'))
                                    killingprocess(board, size, tempX - 1, tempY, 'l');
                            }
                        }
                        after = count(board,size,1);
                        score2 += (float)(before-after);
                        player = 1;
                    }
                    during = false;
                }
            } else if (zn == 0x6e) { // 'n' key --> starts a new game
                for (int i = 0; i < size; i++) {
                    for (int j = 0; j < size; j++) {
                        board[i][j] = ' ';
                    }
                }
                player = 1;
                score2=0;
                score1=0;
                start=true;
                additional=6.5;
                x = boardX;
                y = boardY;
                clrscr();
            }
            else if (zn == 0x73) { // 's' key --> save the game state
                char nameoffilesaved[50];
                gotoxy(instructionsX,instructionsY+20);
                cputs("Name of the file: ");
                for (int i = 0; i < 50; i++) {
                    temp = getche();
                    if (temp == 0x0d) break;
                    else nameoffilesaved[i] = (char) temp;
                }

                FILE *file;
                file = fopen(nameoffilesaved,"w");
                fprintf(file,"%d %d %f %f %f ",size,player,score1,score2,additional);
                for (int i=0;i<size;i++){
                    for (int j=0;j<size;j++){
                        fprintf(file,"%c",board[i][j]);
                    }
                }
                fclose(file);
            }
            else if (zn == 0x66) { // 'f' key --> finish the game and display the final score
                for(int i=0;i<size;i++){
                    for(int j=0;j<size;j++){
                        if (board[i][j] == ' '){
                            if (fields(board,size,j,i,1,'-')) changing(board,size,j,i,',','-');
                            else if (fields(board,size,j,i,2,'-')) changing(board,size,j,i,'.','-');
                            else changing(board,size,j,i,'|','-');
                        }
                    }
                }
                score1 += (float)count(board,size,3);
                score2 += (float)count(board,size,4);
                score2 += additional;
                end=true;
                break;
            }
        }
	} while (zn != 'q');
    if (end) {
        clrscr();
        int winner;
        if (score2>score1) winner=2;
        else winner=1;
        gotoxy(1,1);
        sprintf(txt,"Player %d won!!!",winner);
        cputs(txt);
        gotoxy(1,2);
        sprintf(txt,"Player 1: %.1f\tPlayer 2: %.1f",score1,score2);
        cputs(txt);
        getch();
    }
	_setcursortype(_NORMALCURSOR);	// show the cursor so it will be
					// visible after the program ends
	return 0;
	}
