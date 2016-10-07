#ifndef __LCDG_DISPLAY__H
#define __LCDG_DISPLAY__H

#define SCREEN_SIZE 64

typedef struct
{
	unsigned char line[8]; 
}cell_t;

typedef struct
{
	cell_t cells[16][8];
}buffer_screen_t;

void initLCDG();
void lcdg_clear_buffer(buffer_screen_t* screen , char start);
void lcdg_writeXYtoBuffer(buffer_screen_t* screen , int x , int y);
void lcdg_writeFromBuffer(buffer_screen_t* screen);

#endif