#include "stdafx.h"
#include <iostream>
#include <windows.h>

void setColor(int ForgC) {
	WORD wColor;

	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	if (GetConsoleScreenBufferInfo(hStdOut, &csbi)) {
		wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
		SetConsoleTextAttribute(hStdOut, wColor);
	}
	return;
}

void gotoXY(int x, int y) {
	COORD p = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), p);
}

const int HEIGHT = 15;
const int WIDTH = 15;

//0's are walls , 1's are paths

const bool MAZE[HEIGHT][WIDTH] =
{
	1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,1,0,1,1,1,1,1,1,1,0,1,0,0,
	0,0,1,1,1,0,1,0,0,1,1,1,1,0,0,
	0,0,0,1,0,0,1,1,0,1,0,0,0,0,0,
	0,1,1,1,0,1,1,0,0,1,0,1,1,1,0,          
	0,1,0,0,1,0,1,1,0,1,0,1,0,1,0,
	0,1,0,0,1,1,0,1,0,1,0,1,1,1,0,
	0,1,0,1,1,0,0,1,0,1,1,1,0,0,0,
	0,1,0,1,0,1,1,1,0,1,0,0,0,1,0,
	0,1,0,1,0,0,0,0,0,1,1,1,1,1,0,
	0,1,0,1,0,1,1,1,0,0,0,0,0,1,0,
	0,1,0,1,0,1,0,1,0,1,1,1,0,1,0,
	0,1,0,1,0,1,0,1,0,0,1,0,0,1,0,
	0,1,1,1,1,1,0,1,1,1,1,1,0,1,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,
};

bool isHere[HEIGHT][WIDTH];

bool canMoveRight = false;
bool canMoveLeft = false;
bool canMoveUp = false;
bool canMoveDown = false;

short facing = 1; // 1=right 2=left 3=up 4=down

int whereX() {
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if (!GetConsoleScreenBufferInfo(
		GetStdHandle(STD_OUTPUT_HANDLE),
		&csbi
		))
		return -1;
	return csbi.dwCursorPosition.X;
}

int whereY() {
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if (!GetConsoleScreenBufferInfo(
		GetStdHandle(STD_OUTPUT_HANDLE),
		&csbi
		))
		return -1;
	return csbi.dwCursorPosition.Y;
}

void moveIn(int x, int y) {

	Sleep(40);

	if (y == 0 && x == 0)
	{
		canMoveDown = false;
		canMoveLeft = false;
		canMoveRight = false;
		canMoveUp = false;

		//Checking edges

		if (MAZE[whereY() - 1][((whereX() - 1) / 2) + 1]) {
			canMoveRight = true;
		}
		if (MAZE[whereY() - 1][((whereX() - 1) / 2) - 1]) {
			canMoveLeft = true; 
		}
		if (MAZE[(whereY() - 1) - 1][(whereX() - 1) / 2]) {
			canMoveUp = true;
		}
		if (MAZE[(whereY() - 1) + 1][(whereX() - 1) / 2]) {
			canMoveDown = true; 
		}
	}
	else {
		setColor(34);
		printf("1");
		isHere[whereY() - 1][(whereX() - 1) / 2] = false;
	}
	if (y == 1 || y == -1) {
		gotoXY(whereX() - 1, whereY() + y);
		setColor(44);
		printf("1");
		gotoXY(whereX() - 1, whereY());
		isHere[(whereY() - 1)][(whereX() - 1) / 2] = true;
	}
	if (x == 1) {
		gotoXY(whereX() + x, whereY() + y);
		setColor(44);
		printf("1");
		gotoXY(whereX() - 1, whereY());
		isHere[whereY() - 1][((whereX() - 1) / 2)] = true;
	}
	if (x == -1) {
		gotoXY(whereX() - 3, whereY() + y);
		setColor(44);
		printf("1");
		gotoXY(whereX() - 1, whereY());
		isHere[whereY() - 1][((whereX() - 1) / 2)] = true;
	}
}

void runForestRun(int destinationXY) { //Solving MAZE with right-hand rule
	int tempX, tempY;

	gotoXY(1, 1);

	while (whereY() != WIDTH)
	{
		if ((whereX() - 1) / 2 == HEIGHT - 1)
			return;
		moveIn(0, 0);

		//sorry for too much if-else

		if (facing == 1 && canMoveDown) { 
			facing = 4;
			moveIn(0, 1);
			moveIn(0, 0);
		}
		else if (facing == 1) {
			if (!canMoveRight)facing = 2;
			else moveIn(1, 0);
		}

		if (facing == 2 && canMoveUp) {
			facing = 3;
			moveIn(0, -1); 
			moveIn(0, 0);
		}
		else if (facing == 2) {
			if (!canMoveLeft)facing = 1;
			else moveIn(-1, 0);
		}

		if (facing == 3 && canMoveRight) {
			facing = 1;
			moveIn(1, 0);
			moveIn(0, 0);
		}
		else if (facing == 3) {
			if (!canMoveUp)facing = 4;
			else moveIn(0, -1);
		}

		if (facing == 4 && canMoveLeft) {
			facing = 2;
			moveIn(-1, 0);
			moveIn(0, 0);
		}
		else if (facing == 4) {
			if (!canMoveDown)facing = 3;
			else moveIn(0, 1);
		}

		tempX = whereX();
		tempY = whereY();
		gotoXY(20, 20);
		printf("%s", "      ");
		moveIn(0, 0);

		for (int q = 0; q<HEIGHT; q++)
			for (int w = 0; w<WIDTH; w++)
				if (isHere[q][w]) {
					gotoXY(20, 20);
					printf("%s", "(");
					printf("%d", w);
					printf("%s", ",");
					printf("%d", q);
					printf("%s", ")");
					Sleep(30);
				}
		gotoXY(tempX, tempY);
	}
}

void launchMaze() { //Print MAZE
	for (int i = 0; i<HEIGHT; i++) {
		printf("\n");
		for (int j = 0; j<WIDTH; j++)
		{
			//MAZE[i][j]=1;
			if (MAZE[i][j]) setColor(34);
			else setColor(55);
			printf("%2d", MAZE[i][j]);
		}
	}
	printf("\n");
}

int main() {
	launchMaze();
	runForestRun(27);

	gotoXY(28, 20);
	
	system("pause"); 
	return 0;
}