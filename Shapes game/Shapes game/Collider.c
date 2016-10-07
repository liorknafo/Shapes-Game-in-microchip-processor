#include "Collider.h"
#include "LAllocator.h"

#ifndef BOOL
#define BOOL
#define TRUE 1
#define FALSE 0
#endif

void initCircleCol(vec3 pos , float radius , circle_collider_t* out)
{
	out->collider.isOverlap = isOverlapCircle;
	out->collider.drawCollider = drawCircleCol;
	out->collider.getCenter = getCenterCircle;
	out->collider.getNormal = getNormalCircle;

	out->position = pos;
	out->radius = radius;	
}
triangle_collider_t initTriangleCol(vec3 point1 , vec3 point2 , vec3 point3)
{
	triangle_collider_t out;
	out.collider.isOverlap = isOverlapTriangle;
	out.collider.drawCollider = drawTriangleCol;
	out.collider.getCenter = getCenterTriangle;
	out.collider.getNormal = getNormalTriangle;
	
	out.points[0] = point1;
	out.points[1] = point2;
	out.points[2] = point3;
	
	return out;
}
square_collider_t initSquareCol(vec3 point1 , vec3 point2 , vec3 point3 , vec3 point4)
{
	square_collider_t out;
	out.collider.isOverlap = isOverlapSquare;
	out.collider.drawCollider = drawSquareCol;
	out.collider.getCenter = getCenterSquare;
	out.collider.getNormal = getNormalSquare;

	out.points[0] = point1;
	out.points[1] = point2;
	out.points[2] = point3;
	out.points[3] = point4;
	return out;
}

int isOverlapCircle(collider_t* collider , vec3 point)
{
	circle_collider_t* circle = (circle_collider_t*)collider;
	if(vec3length(vec3Minus(circle->position , point)) < circle->radius)
	{
		return TRUE;
	}
	return FALSE;
}

int overlap(vec3 point1 , vec3 point2 , vec3 point3)
{
	float res = (point2.X_ - point1.X_) * (point3.Y_ - point1.Y_) - (point2.Y_ - point1.Y_) * (point3.X_ - point1.X_);
	if(res < 0)
		return FALSE;
	return TRUE;
}

int isOverlapTriangle(collider_t* collider , vec3 point)
{
	triangle_collider_t* triangle = (triangle_collider_t*)collider;
	int i;
	for(i = 0 ; i < 3 ; i++)
	{
		if(overlap(point, triangle->points[i] , triangle->points[(i + 1)%3]))
			return FALSE;	
	}
	return TRUE;
}
int isOverlapSquare(collider_t* collider , vec3 point)
{
	square_collider_t* square = (square_collider_t*)collider;
	int i;
	for(i = 0 ; i < 4 ; i++)
	{
		if(overlap(point, square->points[i] , square->points[(i + 1)%4]))
			return FALSE;	
	}
	return TRUE;
}

void drawCircleCol(collider_t* collider , buffer_screen_t* screen)
{
	circle_collider_t* circle = (circle_collider_t*)collider;
	float i = 0;
	float angleStep = (float)asin(1/circle->radius);
	int pr = (int)((i * 100) / (2 * M_PI));
	for(i = 0 ; i < M_PI * 2 ; i += angleStep)
	{
		int x = (int)(cos(i) * circle->radius);
		int y = (int)(sin(i) * circle->radius);
		x += (int)circle->position.X_;
		y += (int)circle->position.Y_;
		lcdg_writeXYtoBuffer(screen , x , y);
	}
}

void drawLine(buffer_screen_t* screen, vec3 point1, vec3 point2)
{
	int x1, y1 , x2 , y2;
	int i;
	x1 = (int)point1.X_;
	y1 = (int)point1.Y_;

	x2 = (int)point2.X_;
	y2 = (int)point2.Y_;

	if (x1 == x2)
	{
		int dirY = y2 - y1;
		if(dirY != 0)
			dirY = abs(dirY) / dirY;
		for (i = y1; i != y2; i += dirY)
		{
			lcdg_writeXYtoBuffer(screen, x1, i);
		}
		return;
	}
	if(abs(x1 - x2) > abs(y1 - y2)){
		int dirX = x2 - x1;
		int index;
		float defrence = (float)(y2 - y1) / (x2 - x1);
		dirX = abs(dirX) / dirX;
		for (index = 0 ,i = x1; i != x2; i += dirX , index++)
		{
			lcdg_writeXYtoBuffer(screen, i, y1 + (int)(defrence * index * dirX));
		}
	}
	else
	{
		int dirY = y2 - y1;
		int index;
		float defrence = (float)(x2 - x1) / (y2 - y1);
		dirY = abs(dirY) / dirY;
		for (index = 0, i = y1; i != y2; i += dirY, index++)
		{
			lcdg_writeXYtoBuffer(screen, x1 + (int)(defrence * index * dirY) ,i);
		}
	}
}

void drawTriangleCol(collider_t* collider , buffer_screen_t* screen)
{
	triangle_collider_t* triangle = (triangle_collider_t*)collider;
	int i;
	for(i = 0 ; i < 3 ; i++)
	{
		drawLine(screen , triangle->points[i] , triangle->points[(i + 1)%3]);
	}
}
void drawSquareCol(collider_t* collider , buffer_screen_t* screen)
{
	square_collider_t* square = (square_collider_t*)collider;
	int i;
	for(i = 0 ; i < 4 ; i++)
	{
		drawLine(screen , square->points[i] , square->points[(i + 1)%4]);
	}
}

vec3 getCenterCircle(collider_t* collider)
{
	circle_collider_t* circle = (circle_collider_t*)collider;
	return circle->position;
}
vec3 getCenterTriangle(collider_t* collider)
{
	vec3 out = initVec2(0,0);
	triangle_collider_t* triangle = (triangle_collider_t*)collider;
	int i;
	for(i = 0 ; i < 3 ; i++)
	{
		out = vec3Plus(out,triangle->points[i]);
	}
	out = vec3multyScalar(out , 1/3.0f);
	return out;
}
vec3 getCenterSquare(collider_t* collider)
{
	vec3 out = initVec2(0,0);
	square_collider_t* square = (square_collider_t*)collider;
	int i;
	for(i = 0 ; i < 4 ; i++)
	{
		out = vec3Plus(out,square->points[i]);
	}
	out = vec3multyScalar(out , 1/4.0f);
	return out;
}

vec3 getNormalCircle(collider_t* collider, vec3 point)
{
	circle_collider_t* circle = (circle_collider_t*)collider;
	vec3 out = vec3Normalize(vec3Minus(point, circle->position));
	return out;
}

void getTheClosestLinePair(vec3* res1, vec3* res2, vec3* arr , vec3 checkPoint, int size)
{
	int i;
	float minDistance = 200;
	for (i = 0; i < size ; i++)
	{
		float dis = vec3length(crossVec3(vec3Normalize(vec3Minus(arr[i], arr[(i + 1)%size])), vec3Minus(arr[i], checkPoint)));
		if (dis < minDistance)
		{
			*res1 = arr[i];
			*res2 = arr[(i + 1)%size];
			minDistance = dis;
		}
	}
}

vec3 getNormalFromPair(vec3 point1, vec3 point2, vec3 point)
{
	vec3 dir1 = vec3Normalize(vec3Minus(point2, point1));
	vec3 dir2 = vec3Normalize(vec3Minus(point, point1));
	float arg = dotVec3(dir1, dir2);
	dir2 = vec3Normalize(vec3Minus(dir2, vec3multyScalar(dir1, arg)));
	return dir2;
}

vec3 getNormalTriangle(collider_t* collider, vec3 point)
{
	triangle_collider_t* triangle = (triangle_collider_t*)collider;
	vec3 point1, point2;
	getTheClosestLinePair(&point1, &point2, triangle->points, point, 3);
	return getNormalFromPair(point1, point2, point);
}
vec3 getNormalSquare(collider_t* collider, vec3 point)
{
	square_collider_t* square = (square_collider_t*)collider;
	vec3 point1, point2;
	getTheClosestLinePair(&point1, &point2, square->points, point, 4);
	return getNormalFromPair(point1, point2, point);
}

void freeCollider(void* collider)
{
	lfree(collider);
}

square_collider_t initSquare(vec3 center , int height , int width , float angle)
{
	int i;
	square_collider_t out;
	vec3 points[4];
	mat3 rot;
	points[0] = initVec2( width / 2 ,  height/ 2);
	points[1] = initVec2( width / 2 , -height/ 2);
	points[2] = initVec2(-width / 2 ,-height / 2);
	points[3] = initVec2(-width / 2 , height / 2);
		
	rot = rotateMat3(initVec3(0,0,1),angle * M_PI / 180);
	for(i = 0 ; i < 4 ; i++)
	{
		points[i] = vec3Plus(center , multyMat3vec(rot , points[i]));
	}

	out = initSquareCol(points[0] ,points[1] ,points[2] ,points[3]);
	return out;
}