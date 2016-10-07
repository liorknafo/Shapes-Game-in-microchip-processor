#include "Level.h"
#include "KeyPedInput.h"
#include "Ball.h"
#include "LAllocator.h"

//#include <stdio.h>

#define CURSOR_SIZE 5

#ifndef BOOL
#define BOOL
#define TRUE 1
#define FALSE 0
#endif

#ifndef NULL
#define NULL 0
#endif

void drawColliders(level_t* level , buffer_screen_t* screen , char isColliderHit[])
{
	int i = 0;
	collider_t* collider;
	iterator_t itr = initIterator(&level->list);
	while(collider = (collider_t*)nextElement(&itr))
	{
		if (!isColliderHit[i])
		{
			collider->drawCollider(collider, screen);
		}
		i++;
	}
}

void freeColliders(level_t* level)
{
	freeList(&(level->list));
}

level_t initLevel()
{
	level_t out;
	out.list = initList(freeCollider);
	return out;
}
void addCollider(level_t* level , collider_t* collider)
{
	addElement(&(level->list), collider);
}

void drawCursor(vec3 cursor, buffer_screen_t* screen)
{
	int i;
	for (i = (int)(cursor.X_ - CURSOR_SIZE);i <= (int)(cursor.X_ + CURSOR_SIZE); i++)
	{
		lcdg_writeXYtoBuffer(screen, i, (int)cursor.Y_);
	}
	for (i = (int)(cursor.Y_ - CURSOR_SIZE);i <= (int)(cursor.Y_ + CURSOR_SIZE); i++)
	{
		lcdg_writeXYtoBuffer(screen, (int)cursor.X_ , i);
	}
}

vec3 getSelectedCursor(vec3 cursor, ball_t* ball , buffer_screen_t* screen , level_t* level , char isColliderHit[])
{
	int x, y;
	do
	{
		getKeyPedIInput(&x, &y);
		if (x == -1)
			continue;
		x -= 1;
		x *= -1;
		y -= 1;
		y *= -1;
		cursor = vec3Minus(cursor, initVec2((float)x, (float)y));
		lcdg_clear_buffer(screen, 0);
		drawColliders(level, screen,isColliderHit);
		drawCursor(cursor, screen);
		if (ball)
		{
			drawBall(ball, screen);
			if (vec3Equals(cursor, ball->pos))
			{
				x = -1;
			}
		}
		lcdg_writeFromBuffer(screen);
	} while (x != 0 || y != 0);
	return cursor;
}

int isBallInFrame(ball_t* ball)
{
	if (ball->pos.X_ < 0 || ball->pos.X_ > SCREEN_SIZE * 2 || ball->pos.Y_ < 0 || ball->pos.Y_ > SCREEN_SIZE)
		return FALSE;
	return TRUE;
}

collider_t* findCollistion(ball_t* ball, level_t* level , char* isColliderHit , int* colliderIndex)
{
	int i = 0;
	collider_t* collider;
	iterator_t itr = initIterator(&(level->list));
	while(collider = (collider_t*)nextElement(&itr))
	{
		vec3 centerColl;
		vec3 collDir;
		vec3 closestPoint;
		if (isColliderHit[i])
		{	
			i++;
			continue;
		}
		centerColl = collider->getCenter(collider);
		collDir = vec3Minus(centerColl, ball->pos);
		closestPoint = vec3Plus(vec3multyScalar(vec3Normalize(collDir) , ball->radius) , ball->pos);
		if (collider->isOverlap(collider, closestPoint))
		{
			*colliderIndex = i;
			return collider;
		}
		i++;
	}
	return NULL;
}

void runLevel(level_t* level , buffer_screen_t* screen)
{
	vec3 cursor = {64 , 32};
	vec3 direction;
	int colliderCount = level->list.size;
	ball_t ball = initBall(initVec2(0,0) , 5);
	char* isColliderHit = (char*)lmalloc(sizeof(char) * colliderCount);
	int index;
	for (index = 0; index < colliderCount; index++)
	{
		isColliderHit[index] = FALSE;
	}
	startOver:
	lcdg_clear_buffer(screen , 1);
	drawColliders(level , screen , isColliderHit);
	drawCursor(cursor, screen);
	initLCDG();
	lcdg_writeFromBuffer(screen);

	cursor = ball.pos = getSelectedCursor(cursor, NULL, screen, level,isColliderHit);
	lcdg_clear_buffer(screen, 0);
	drawColliders(level, screen , isColliderHit);
	drawCursor(cursor, screen);
	drawBall(&ball, screen);
	lcdg_writeFromBuffer(screen);
	direction = getSelectedCursor(cursor, &ball, screen, level,isColliderHit);
	direction = vec3Normalize(vec3Minus(direction , ball.pos));
	while (isBallInFrame(&ball))
	{
		collider_t* hitCollider;
		int colliderIndex;
	    ball.pos = vec3Plus(ball.pos, direction);
		hitCollider = findCollistion(&ball, level , isColliderHit , &colliderIndex);
		if (hitCollider)
		{
			vec3 normal;
			isColliderHit[colliderIndex] = TRUE;
			colliderCount--;
			normal = hitCollider->getNormal(hitCollider, ball.pos);
			direction = vec3Normalize(vec3Minus(direction, vec3multyScalar(normal, dotVec3(normal, direction) * 2)));
			ball.pos = vec3Plus(ball.pos, direction);
		}
		lcdg_clear_buffer(screen, 0);
		drawColliders(level, screen,isColliderHit);
		drawBall(&ball, screen);
		lcdg_writeFromBuffer(screen);
	}
	if (!colliderCount)
	{
		lfree(isColliderHit);
		return;
	}
	{
		int i;
		for (i = 0; i < level->list.size; i++)
		{
			isColliderHit[i] = FALSE;
		}
		colliderCount = level->list.size;
	}
	goto startOver;
}

void addCircleCollider(level_t* level, vec3 pos, float radius)
{
	circle_collider_t* circle = (circle_collider_t*)lmalloc(sizeof(circle_collider_t));
    initCircleCol(pos, radius , circle);
	addCollider(level, (collider_t*)(circle));
}

void addTriangle(level_t* level, vec3 point1, vec3 point2, vec3 point3)
{
	triangle_collider_t* triangle = (triangle_collider_t*)lmalloc(sizeof(triangle_collider_t));
	*triangle = initTriangleCol(point1, point2, point3);
	addCollider(level, (collider_t*)triangle);
}

void addSquare(level_t* level, vec3 point1, vec3 point2, vec3 point3, vec3 point4)
{
	square_collider_t* square = (square_collider_t*)lmalloc(sizeof(square_collider_t));
	*square = initSquareCol(point1, point2, point3, point4);
	addCollider(level, (collider_t*)square);
}

void addRect(level_t* level, vec3 center , int height , int width , float angle)
{
	square_collider_t* square = (square_collider_t*)lmalloc(sizeof(square_collider_t));
	*square = initSquare(center, height, width, angle);
	addCollider(level, (collider_t*)square);
}