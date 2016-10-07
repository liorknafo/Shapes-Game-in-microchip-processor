#ifndef __COLLIDER__H
#define __COLLIDER__H
#include "Mat3d.h"
#include "LCDG_Display.h"

typedef struct collider
{
	int (*isOverlap)(struct collider* collider , vec3 point);
	vec3 (*getNormal)(struct collider* collider, vec3 point);
	void (*drawCollider)(struct collider* collider , buffer_screen_t* screen);
	vec3 (*getCenter)(struct collider*);
}collider_t;

typedef struct 
{
	collider_t collider;
	vec3 position;
	float radius;
}circle_collider_t;

typedef struct 
{
	collider_t collider;
	vec3 points[3];
}triangle_collider_t;

typedef struct 
{
	collider_t collider;
	vec3 points[4];
}square_collider_t;

void initCircleCol(vec3 pos , float radius , circle_collider_t* out);
triangle_collider_t initTriangleCol(vec3 point1 , vec3 point2 , vec3 point3);
square_collider_t initSquareCol(vec3 point1 , vec3 point2 , vec3 point3 , vec3 point4);
square_collider_t initSquare(vec3 center , int height , int width , float angle);

int isOverlapCircle(collider_t* collider , vec3 point);
int isOverlapTriangle(collider_t* collider , vec3 point);
int isOverlapSquare(collider_t* collider , vec3 point);

void drawCircleCol(collider_t* collider , buffer_screen_t* screen);
void drawTriangleCol(collider_t* collider , buffer_screen_t* screen);
void drawSquareCol(collider_t* collider , buffer_screen_t* screen);

vec3 getCenterCircle(collider_t* collider);
vec3 getCenterTriangle(collider_t* collider);
vec3 getCenterSquare(collider_t* collider);

vec3 getNormalCircle(collider_t* collider, vec3 point);
vec3 getNormalTriangle(collider_t* collider, vec3 point);
vec3 getNormalSquare(collider_t* collider, vec3 point);

void freeCollider(void* collider);

#endif