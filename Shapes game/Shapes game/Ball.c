#include "Ball.h"

ball_t initBall(vec3 startPos , float radius)
{
	ball_t out;
	out.pos = startPos;
	out.radius = radius;
	return out;
}

void drawBall(ball_t* ball , buffer_screen_t* screen)
{
	int i , j;
	for(i = (int)(ball->pos.X_ - ball->radius) ; i <= ball->pos.X_ + ball->radius ; i++)
	{
		for(j = (int)(ball->pos.Y_ - ball->radius) ; j <= ball->pos.Y_ + ball->radius ; j++)
		{	
			if(vec3length(vec3Minus(initVec2((float)i,(float)j), ball->pos)) < ball->radius)
				lcdg_writeXYtoBuffer(screen , i , j);		
		}
	}
}