#ifndef __LIST_H
#define __LIST_H

#include "LAllocator.h"
									
typedef struct Node										
{															
	void* data;												
	struct Node* prev;									
	struct Node* next;									
}node_t;													
														
typedef struct												
{															
	node_t* head;										
	node_t* tail;										
	int size;												
	void(*freeElement)(void*);									
}list_t;													
								
typedef struct												
{															
	list_t* list;										
	node_t* current;										
}iterator_t;												
												
node_t* initNode(void* data);
list_t initList(void(*freeElement)(void*));
void addElement(list_t* list, void* data);
void freeList(list_t* list);
iterator_t initIterator(list_t* list);
void* nextElement(iterator_t* iterator);
void removeCurrentElement(iterator_t* iterator);
						
#endif
