#include "LCDG_Display.h"

#ifdef _WIN32
#include <windows.h>
HDC mydc;
void lcdg_writeXYtoBuffer(buffer_screen_t* screen , int x , int y)
{
	cell_t* cell;
	unsigned char* line;
	if (x < 0 || x >= SCREEN_SIZE * 2 || y < 0 || y >= SCREEN_SIZE)
		return;
	cell = &screen->cells[x / 8][y / 8];
	line = &cell->line[x % 8];
	*line |= 1 << (y % 8);
}
void initLCDG()
{
	HWND myconsole = GetConsoleWindow();
	mydc = GetDC(myconsole);
}
void lcdg_clear_buffer(buffer_screen_t* screen , char start)
{
	int i , j ,k;
	for(i = 0 ; i < 8 ; i++)
	{
		for(j = 0 ; j < 16 ; j++)
		{
			for(k = 0 ; k < 8 ; k++)
			{
				screen->cells[j][i].line[k] = 0;
			}
		}
	}
}
void lcdg_writeFromBuffer(buffer_screen_t* screen)
{
	int i , j , k , bit;
	for(i = 0 ; i < 8 ; i++)
	{
		for(j = 0 ; j < 16 ; j++)
		{
			int lcd = (j/8) + 1; 
			for(k = 0 ; k < 8 ; k++)
			{
				for (bit = 0; bit < 8; bit++)
				{
					if (screen->cells[j][i].line[k] & (1 << bit))
					{
						SetPixel(mydc, j * 8 + k, i * 8 + bit, RGB(0, 0, 0));
					}
					else
					{
						SetPixel(mydc, j * 8 + k, i * 8 + bit, RGB(255, 255, 255));
					}
				}
			}
		}
	}
}

#else
#include <p32xxxx.h>
void initLCDGPortD(void);
void initLCDGPortB(void);
void initLCDGPortE(void);
void initLCDGPortF(void);
void initLCDGPortG(void);

void delay(int x);
void initLcd(void);
void writeXY(int x,int y,int lcd_cs);
void writeLcd(unsigned int num,int lcd_cs);
void delay_slow(void);
void clearLCDG();
void busy_lcdg();
void initLCDG()
{
	initLCDGPortD();
	initLCDGPortB();
	initLCDGPortE();
	initLCDGPortF();
	initLCDGPortG();
	initLcd();
}
void lcdg_writeXYtoBuffer(buffer_screen_t* screen , int x , int y)
{
	if (x < 0 || x >= SCREEN_SIZE * 2 || y < 0 || y >= SCREEN_SIZE)
		return;
	cell_t* cell = &screen->cells[x / 8][y / 8];
	char* line = &cell->line[x % 8];
	*line |= 1 << (y % 8);
}
void lcdg_clear_buffer(buffer_screen_t* screen , char start)
{
	int i , j ,k;
	for(i = 0 ; i < 8 ; i++)
	{
		for(j = 0 ; j < 16 ; j++)
		{
			for(k = 0 ; k < 8 ; k++)
			{
				screen->cells[j][i].line[k] = 0;
			}
		}
	}
}
void lcdg_writeFromBuffer(buffer_screen_t* screen)
{
	int i , j , k;
	for(i = 0 ; i < 8 ; i++)
	{
		for(j = 0 ; j < 16 ; j++)
		{
			int lcd = (j/8) + 1; 
			for(k = 0 ; k < 8 ; k++)
			{
				writeXY(j * 8 + k ,i , lcd);	
				writeLcd(screen->cells[j][i].line[k],lcd);
			}
			busy_lcdg();
		}
	}
}

void clearLCDG()
{
	int i,j;
	for(i = 0;i <8;i++)
	{
		for(j = 0;j < 64;j++)
		{
			writeXY(j,i,1);
			writeLcd(0x00,1);
			writeXY(j,i,2);
			writeLcd(0x00,2);
		}
	}	
}

void initLCDGPortD(void)
{
	unsigned int portMap;

	portMap = TRISD;
	portMap &= 0xFFFFFF4F;
	TRISD = portMap;
	PORTDbits.RD4 = 0;
	PORTDbits.RD5 = 0;
	PORTDbits.RD7 = 0;
}

void initLCDGPortB(void)
{
	unsigned int portMap;

	portMap = TRISB;
	portMap &= 0xFFFF7FFF;
	TRISB = portMap;
	PORTBbits.RB15 = 0;
}

void initLCDGPortE(void)
{
	unsigned int portMap;

	portMap = TRISE;
	portMap &= 0xFFFFFF00;
	TRISE = portMap;
	PORTE = 0x00;
}

void initLCDGPortF(void)
{
	unsigned int portMap;

	portMap = TRISF;
	portMap &= 0xFFFFFEF8;
	TRISF = portMap;
	PORTFbits.RF8 = 1;
}
void initLCDGPortG()
{
	unsigned int portMap;

	portMap = TRISG;
	portMap &= 0xFFFF7fFf;
	TRISG = portMap;
    PORTGbits.RG15=0;
	PORTFbits.RF8 = 1;
}

void delay(int x)
{
	unsigned int i;

	for(i=0;i<x;i++);
}


void initLcd(void)
{	
	int CONTROL[4] = {0x40,0xB8,0xFF,0x3F};
	int i;

	PORTDbits.RD5 = 0;
	PORTBbits.RB15 = 0;
	PORTF = 0x01;
	PORTDbits.RD7 = 0;
	PORTDbits.RD7 = 1;
	PORTF = 0x02;
	PORTDbits.RD7 = 0;
	PORTDbits.RD7 = 1;
	PORTFbits.RF8 = 1;

	for(i = 0;i < 4;i++)
	{
		PORTE = CONTROL[i];
		PORTF = 0x01;
		PORTDbits.RD4 = 1;//enable=1
    	PORTDbits.RD4 = 0;//enable=0
		
		busy_lcdg();
		PORTF = 0x02;
		PORTDbits.RD4 = 1;//enable=1
    	PORTDbits.RD4 = 0;//enable=0
		busy_lcdg();	
	}
	PORTFbits.RF8 = 1;
}

void writeXY(int x,int y,int lcd_cs)
{
	PORTDbits.RD5 = 0;
	PORTBbits.RB15 = 0;
	PORTF = lcd_cs;
	PORTE = x;//0x40+ x;
	PORTDbits.RD4 = 1;//enable=1
    PORTDbits.RD4 = 0;//enable=0
	busy_lcdg();
	PORTE =0xb8+ y;
	PORTDbits.RD4 = 1;//enable=1
    PORTDbits.RD4 = 0;//enable=0
	busy_lcdg();
	PORTFbits.RF8 = 1;
}

void writeLcd(unsigned int num,int lcd_cs)
{
	int i;

	PORTDbits.RD5 = 0;
	PORTBbits.RB15 = 1;
	PORTF = lcd_cs;
	PORTE = num;
    PORTDbits.RD4 = 1;//enable=1
    PORTDbits.RD4 = 0;//enable=0
	busy_lcdg();
	PORTFbits.RF8 = 1;
}
void delay_slow(void)
{
	unsigned int i;

	for(i=0;i<64;i++);
}

void busy_lcdg()
{
	int portMap;
    char RD,RS;
    int STATUS_TRISE;
    RD=PORTDbits.RD5;
    RS=PORTBbits.RB15;
    STATUS_TRISE=TRISE;
	PORTDbits.RD5 = 1;//w/r
	PORTBbits.RB15 = 0;//rs 
    portMap = TRISE;
	portMap |= 0x80;
	TRISE = portMap;
	do
    {
    	PORTDbits.RD4=1;//enable=1
     	PORTDbits.RD4=0;//enable=0
    }
   	while(PORTEbits.RE7); // BF ????? ???
    PORTDbits.RD5=RD; 
    PORTBbits.RB15=RS;
    TRISE=STATUS_TRISE;   
}
#endif