
#include "stdafx.h"
#include <iostream>
#include <windows.h>

using namespace std;

/*
Data Structures Pre-Visa Project - Maze Solver
Alperen Coþkun
*/

void SetColor(int ForgC) {
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

const int mazeHeight = 15;
const int mazeWidth = 15;

const bool maze[mazeHeight][mazeWidth] =
{
	1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,1,0,1,1,1,1,1,1,1,0,1,0,0,
	0,0,1,1,1,0,1,0,0,1,1,1,1,0,0,
	0,0,0,1,0,0,1,1,0,1,0,0,0,0,0,
	0,1,1,1,0,1,1,0,0,1,0,1,1,1,0,          //Random maze. It should work with any maze
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

bool burda[mazeHeight][mazeWidth];

bool canMoveRight = false;
bool canMoveLeft = false;
bool canMoveUp = false;
bool canMoveDown = false;

short facing = 1;// 1=right 2=left 3=up 4=down

short wherex() {
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if (!GetConsoleScreenBufferInfo(
		GetStdHandle(STD_OUTPUT_HANDLE),
		&csbi
		))
		return -1;
	return csbi.dwCursorPosition.X;
}

short wherey() {
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if (!GetConsoleScreenBufferInfo(
		GetStdHandle(STD_OUTPUT_HANDLE),
		&csbi
		))
		return -1;
	return csbi.dwCursorPosition.Y;
}

void moveIn(int x, int y) {

	Sleep(45);

	if (y == 0 && x == 0)
	{
		canMoveDown = false;
		canMoveLeft = false;
		canMoveRight = false;
		canMoveUp = false;

		//Checking edges

		if (maze[wherey() - 1][((wherex() - 1) / 2) + 1]) { canMoveRight = true; }
		if (maze[wherey() - 1][((wherex() - 1) / 2) - 1]) { canMoveLeft = true; }
		if (maze[(wherey() - 1) - 1][(wherex() - 1) / 2]) { canMoveUp = true; }
		if (maze[(wherey() - 1) + 1][(wherex() - 1) / 2]) { canMoveDown = true; }
	}
	else {
		SetColor(34);
		printf("1");
		burda[wherey() - 1][(wherex() - 1) / 2] = false;
	}
	if (y == 1 || y == -1) {
		gotoXY(wherex() - 1, wherey() + y);
		SetColor(44);
		printf("1");
		gotoXY(wherex() - 1, wherey());
		burda[(wherey() - 1)][(wherex() - 1) / 2] = true;
	}
	if (x == 1) {
		gotoXY(wherex() + x, wherey() + y);
		SetColor(44);
		printf("1");
		gotoXY(wherex() - 1, wherey());
		burda[wherey() - 1][((wherex() - 1) / 2)] = true;
	}
	if (x == -1) {
		gotoXY(wherex() - 3, wherey() + y);
		SetColor(44);
		printf("1");
		gotoXY(wherex() - 1, wherey());
		burda[wherey() - 1][((wherex() - 1) / 2)] = true;
	}
}

void runForestRun(int destinationXY) { //Solving maze with right-hand rule
	short tempx, tempy;

	gotoXY(1, 1);

	while (wherey() != mazeWidth)
	{
		if ((wherex() - 1) / 2 == mazeHeight - 1)
			return;
		moveIn(0, 0);

		//sorry for too much if-else

		if (facing == 1 && canMoveDown) { facing = 4; moveIn(0, 1); moveIn(0, 0); }
		else if (facing == 1) {
			if (!canMoveRight)facing = 2;
			else moveIn(1, 0);
		}

		if (facing == 2 && canMoveUp) { facing = 3; moveIn(0, -1); moveIn(0, 0); }
		else if (facing == 2) {
			if (!canMoveLeft)facing = 1;
			else moveIn(-1, 0);
		}

		if (facing == 3 && canMoveRight) { facing = 1; moveIn(1, 0); moveIn(0, 0); }
		else if (facing == 3) {
			if (!canMoveUp)facing = 4;
			else moveIn(0, -1);
		}

		if (facing == 4 && canMoveLeft) { facing = 2; moveIn(-1, 0); moveIn(0, 0); }
		else if (facing == 4) {
			if (!canMoveDown)facing = 3;
			else moveIn(0, 1);
		}

		tempx = wherex();
		tempy = wherey();
		gotoXY(20, 20);
		printf("%s", "      ");
		moveIn(0, 0);

		for (int q = 0; q<mazeHeight; q++)
			for (int w = 0; w<mazeWidth; w++)
				if (burda[q][w]) {
					gotoXY(20, 20);
					printf("%s", "(");
					printf("%d", w);
					printf("%s", ",");
					printf("%d", q);
					printf("%s", ")");
					Sleep(30);
				}
		gotoXY(tempx, tempy);
	}
}

void launchMaze() { //Print maze
	for (int i = 0; i<mazeHeight; i++) {
		printf("\n");
		for (int j = 0; j<mazeWidth; j++)
		{
			//maze[i][j]=1;
			if (maze[i][j]) SetColor(34);
			else SetColor(55);
			printf("%2d", maze[i][j]);
		}
	}
	printf("\n");
}
int main() {
	launchMaze();
	runForestRun(27);

	gotoXY(28, 20);
	system("pause");
}