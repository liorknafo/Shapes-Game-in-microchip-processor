#include "LAllocator.h"
#include <stdio.h>
#define HEIGHT 3
#define SIZE_TREE_CONTROL (((1 << (HEIGHT * 3)) - 1) / 7)
#define SIZE_DATA (1 << ((HEIGHT + 1) * 3))
#define SIZE_DATA_CONTROL (1 << ((HEIGHT * 3)))
#include "DrawLCD.h"

#include <string.h>

typedef struct 
{
	unsigned char data[2];
}bitControl_t;

char getIsUsed(bitControl_t* bitControl, int index)
{
	int value = (bitControl->data[0] | bitControl->data[1] << 8) &0xffff;
	return (value & (1 << (index * 2))) == 0 ? 0 : 1;
}

char getIsEnd(bitControl_t* bitControl, int index)
{
	int value = (bitControl->data[0] |(bitControl->data[1] << 8)) & 0xffff;
	return (value & (1 << (index * 2 + 1))) == 0 ? 0 : 1;
}

void setIsUsed(bitControl_t* bitControl, int index, char value)
{
	int theValue = (bitControl->data[0] |(bitControl->data[1] << 8)) & 0xffff;
	if (value)
	{
		theValue |= (1 << (index * 2));
	}
	else
	{
		theValue &= ~(1 << (index * 2));
	}
	bitControl->data[0] = theValue & 0xff;
	bitControl->data[1] = (theValue >> 8) & 0xff;
}

void setIsEnd(bitControl_t* bitControl, int index, char value)
{
	int theValue = (bitControl->data[0] |(bitControl->data[1] << 8)) & 0xffff;
	if (value)
	{
		theValue |= (1 << (index * 2 + 1));
	}
	else
	{
		theValue &= ~(1 << (index * 2 + 1));
	}
	bitControl->data[0] = theValue & 0xff;
	bitControl->data[1] = (theValue >> 8) & 0xff;
}

unsigned char isInit = 0;
unsigned char treeControl[SIZE_TREE_CONTROL];
bitControl_t controlData[SIZE_DATA_CONTROL];
unsigned int data[SIZE_DATA];

void initBitContol(bitControl_t* bitControl)
{
	bitControl->data[0] = 0;
	bitControl->data[1] = 0;
}

void initDataControl()
{
	int i;
	for (i = 0; i < SIZE_TREE_CONTROL; i++)
	{
		treeControl[i] = 0;
	}
	for (i = 0; i < SIZE_DATA_CONTROL; i++)
	{
		initBitContol(controlData + i);
	}
	isInit = 1;
}

char getUsedMap(bitControl_t* bitControl)
{
	char out = bitControl->data[0] & 0x55;
	out |= (bitControl->data[1] & 0x55) << 1;
	return out;
}

int findFirstIndexFree(int startDataIndex, char startBit)
{
	int i;
	for (i = startBit; i < 8; i++)
	{
		if (!getIsUsed(controlData + startDataIndex, i))
		{
			return i;
		}
	}
	return -1;
}

char findIfFree(int startDataIndex, int* startBit, int size)
{
	int i = 0;
	while (i < size && i + startDataIndex < SIZE_DATA_CONTROL)
	{
		char bits = (i + *startBit)% 8;
		if (getIsUsed(controlData + startDataIndex + (i + *startBit) / 8, bits))
		{
			*startBit += i;
			return 0;
		}
		i += 4;
	}
	return 1;
}

int findIndexFree(int startDataIndex , int size)
{
	int i = 0;
	char flag = 1;
	while (i < 8)
	{
		int index;
		if ((index = findFirstIndexFree(startDataIndex, i)) != -1)
		{
			if (findIfFree(startDataIndex, &index, size))
				return startDataIndex * 8 + index;
			i = index;
		}
		else
			return -1;
	}
	return -1;
	
}

void updateTreeControl(int index)
{
	int i = (index / 8) + SIZE_TREE_CONTROL - (SIZE_DATA_CONTROL / 8);
	int height = HEIGHT - 1;

	if (!(~getUsedMap(controlData + index)))
	{
		treeControl[i] |= 1 << (index % 8);
	}
	else
	{
		treeControl[i] &= ~(1 << (index % 8));
	}

	while (1)
	{
		int childIndex = i - ((1 << (height * 3)) - 1)/7;
		if (!((~treeControl[i]) & 0xff))
		{
			treeControl[(i - 1) / 8] |= 1 << (childIndex % 8);
		}
		else
		{
			treeControl[(i - 1) / 8] &= ~(1 << (childIndex % 8));
		}
		i--;
		i /= 8;
		height--;
		if (i == 0)
			return;
	}
}

void* allcate(int indexFree, int size)
{
	char bits = indexFree % 8;
	int index = indexFree / 8;
	int lastIndex = index;
	int i;
	for (i = 0; i < size; i++)
	{
		bits = (indexFree + i) % 8;
		index = (indexFree + i) / 8;
		setIsUsed(controlData + index, bits , 1);
		if (lastIndex != index)
		{
			updateTreeControl(lastIndex);
			lastIndex = index;
		}
	}
	updateTreeControl(index);
	setIsEnd(controlData + index , bits , 1);
	return data + indexFree;
}

void* lmalloc(int size)
{
	int i, height = HEIGHT;
	int node_num = 0;
	if (!isInit)
	{
		initDataControl();
	}
	for (i = 0; i < 8; i++)
	{
		char node = treeControl[node_num];
		if (!(node & (1 << i)))
		{
			if (height > 1)
			{
				node_num = node_num * 8 + i + 1;
				i = -1;
				height--;
				continue;
			}
			else
			{
				int j;
				for(j = 0 ; j < 8 ; j++)
				{
					int dataIndex = node_num * 8 + i + 1 + j - SIZE_TREE_CONTROL;
					bitControl_t* startData = controlData + dataIndex;
					int indexFree = findIndexFree(dataIndex, size);
					if (indexFree != -1)
					{
						return allcate(indexFree, size);
					}
				}
				height++;
				node_num = (node_num - 1)/8 + 1;	
			}
		}
	}
	return 0;
}

void freeIndexStart(int startIndex)
{
	int i = 0 , lastBitCont = startIndex / 8;
	if(startIndex != 0 && getIsUsed(controlData + startIndex - 1 , 7))
		setIsEnd(controlData +  + ((startIndex - 1) / 8), (startIndex - 1) % 8, 1);
	while (!getIsEnd(controlData + ((i + startIndex) / 8), (startIndex + i) % 8))
	{
		if (i + startIndex > SIZE_DATA)
			return;
		setIsUsed(controlData + ((i + startIndex) / 8), (startIndex + i) % 8 , 0);
		if (lastBitCont != (startIndex + i) / 8)
		{
			updateTreeControl(lastBitCont);
			lastBitCont = (startIndex + i) / 8;
		}
		i++;
	}
	updateTreeControl((startIndex + i) / 8);
	setIsEnd(controlData + ((i + startIndex) / 8), (startIndex + i) % 8, 0);
}

void lfree(void* allocation)
{
	int startIndex = ((unsigned int*)allocation) - data;
	if (startIndex < 0 || startIndex >= SIZE_DATA)
		return;
	freeIndexStart(startIndex);
}