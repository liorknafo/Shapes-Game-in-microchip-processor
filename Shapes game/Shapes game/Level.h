#ifndef __LEVEL__H
#define __LEVEL__H
#include "Collider.h"
#include "list.h"

typedef struct
{
	list_t list;
}level_t; 

level_t initLevel();
void addCollider(level_t* level , collider_t* collider);
void freeColliders(level_t* level);
void runLevel(level_t* level , buffer_screen_t* screen);
void addCircleCollider(level_t* level, vec3 pos, float radius);
void addTriangle(level_t* level, vec3 point1, vec3 point2, vec3 point3);
void addSquare(level_t* level, vec3 point1, vec3 point2, vec3 point3, vec3 point4);
void addRect(level_t* level, vec3 center , int height , int width , float angle);
#endif