#include "list.h"

#ifndef NULL
#define NULL 0
#endif

node_t* initNode(void* data)
{
	node_t* out = (node_t*)lmalloc(sizeof(node_t));
	out->data = data;
	out->next = out->prev = NULL;
	return out;
}
list_t initList(void (*freeElement)(void*))
{
	list_t out;
	out.head = out.tail = NULL;
	out.size = 0;
	out.freeElement = freeElement;
	return out;
}
void addElement(list_t* list, void* data)
{
	if (list->head == NULL)
	{
		list->head = list->tail = initNode(data);
		list->size++;
		return;
	}
	list->tail->next = initNode(data);
	list->tail->next->prev = list->tail;
	list->tail = list->tail->next;
	list->size++;
}

void freeList(list_t* list)
{
	node_t* ptr = list->head;
	while (ptr)
	{
		if (!(list->freeElement))
			list->freeElement(ptr->data);
		ptr = ptr->next;
		lfree(ptr);
	}
	list->head = list->tail = NULL;
	list->size = 0;
}

iterator_t initIterator(list_t* list)
{
	iterator_t out;
	out.current = NULL;
	out.list = list;
	return out;
}
void* nextElement(iterator_t* iterator)
{
	if (!iterator->current)
	{
		iterator->current = iterator->list->head;
	}
	else
	{
		if (iterator->current->next)
			iterator->current = iterator->current->next;
		else
			return NULL;
	}
	if(iterator->current)
		return iterator->current->data;
	return NULL;
}
void removeCurrentElement(iterator_t* iterator)
{
	if (iterator->current == iterator->list->head)
	{
		iterator->list->head = iterator->list->head->next;
		iterator->list->head->prev = NULL;
		if (!(iterator->list->freeElement))
			iterator->list->freeElement(iterator->current->data);
		lfree(iterator->current);
		iterator->current = iterator->list->head;
	}
	else if (iterator->current == iterator->list->tail)
	{
		iterator->list->tail = iterator->list->tail->prev;
		iterator->list->tail->next = NULL;
		if (!(iterator->list->freeElement))
			iterator->list->freeElement(iterator->current->data);
		lfree(iterator->current);
		iterator->current = iterator->list->tail;
	}
	else
	{
		node_t* next = iterator->current->prev;
		node_t* prev = iterator->current->next;
		next->prev = prev;
		prev->next = next;
		if (!(iterator->list->freeElement))
			iterator->list->freeElement(iterator->current->data);
		lfree(iterator->current);
		iterator->current = next;
	}
}