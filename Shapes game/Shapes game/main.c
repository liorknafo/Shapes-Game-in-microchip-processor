#include "Level.h"
#include "LCDG_Display.h"
#include "DrawLCD.h"
#include "outputAnalog.h"
#include <stdio.h>

#ifndef _WIN32
#include <p32xxxx.h>
#pragma config FPLLMUL = MUL_20, FPLLIDIV = DIV_2, FPLLODIV = DIV_1, FWDTEN = OFF
#pragma config POSCMOD = HS, FNOSC = PRIPLL, FPBDIV = DIV_8
#else
#pragma warning(disable:4996)
#endif

#define MAX_NUM_LEVELS 4


void initLevel2(level_t* level)
{
	addCircleCollider(level, initVec2(32, 32), 20);
}

void initLevel1(level_t* level)
{
	addCircleCollider(level, initVec2(32, 32), 20);
	addSquare(level, initVec2(74, 22), initVec2(74, 42), initVec2(94, 42), initVec2(94, 22));
}

void initLevel0(level_t* level)
{
	addRect(level,initVec2(22,11),30,14,45);
	addRect(level,initVec2(22,46),30,14,-45);
	addRect(level,initVec2(102,11),30,14,-45);
	addRect(level,initVec2(102,46),30,14,45);
}
void initLevel3(level_t* level)
{
	addCircleCollider(level,initVec2(64,10),10);
	addCircleCollider(level,initVec2(10,54),10);
	addCircleCollider(level,initVec2(118,54),10);
}
void initLevelNum(level_t* level , int i)
{
	*level = initLevel();
	switch (i)
	{
	case 0:
	default:
		initLevel0(level);
		break;
	case 1:
		initLevel1(level);
		break;
	case 2:
		initLevel2(level);
		break;
	case 3:
		initLevel3(level);
		break;
	}
}

int main()
{
	int i;
	level_t level;
	buffer_screen_t screen;
	char data_buffer[20] = { 0 };
	
	//start the game
	initLCDG();
	for (i = 0; i < MAX_NUM_LEVELS; i++)
	{
		sprintf(data_buffer , "Level %d start", i + 1);
		displayLCD(data_buffer, "");
		initLevelNum(&level, i);
		runLevel(&level, &screen);
		freeColliders(&level);
		sprintf(data_buffer ,"Level %d complete", i + 1);
		displayLCD(data_buffer, "");
	}
	displayLCD("The Game Finished", "");
	while (1)
	return 1;
}