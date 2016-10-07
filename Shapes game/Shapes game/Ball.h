#ifndef __BALL__H
#define __BALL__H
#include "Mat3d.h"
#include "LCDG_Display.h"
typedef struct 
{
	vec3 pos;
	float radius;
}ball_t;

ball_t initBall(vec3 startPos , float radius);
void drawBall(ball_t* ball , buffer_screen_t* screen);
#endif