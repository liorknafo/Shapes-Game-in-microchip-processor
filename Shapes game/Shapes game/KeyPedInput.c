#include "KeyPedInput.h"

#ifdef _WIN32
#include <conio.h> 
        
char getKeyPressed()
{
	if (_kbhit())
	{
		char out = _getch();
		return out;
	}
}
void getKeyPedIInput(int* x , int* y)
{
	if (_kbhit())
	{
		char res = _getch();
		if (res <= '9' && res > '0')
		{
			res -= '1';
			*y = res / 3;
			*x = res % 3;
			return;
		}
	}
	*x = -1;
	*y = -1;
}

char GetChar()
{
	return _getch();
}

#else

#include <p32xxxx.h>
int bitsToNumber(int bits);
void KeyinitPortB(void);
void print_led(char x);
void KeyinitPortD(void);
void KeyinitPortE(void);
void KeyinitPortF(void);
void KeyinitPortG(void);
void delay3(void);

int num_led,i,keyVal,column;           
char getKeyPressed()
{
	int x, y;
	getKeyPedIInput(&x, &y);
	if (x == -1)
		return -1;
	if (x == 3)
	{
		return 'A' + y;
	}
	if (y == 3)
	{
		switch (x)
		{
		case 0:
			return '*';
		case 1:
			return '0';
		case 2:
			return '#';
		}
	}
	return '1' + y * 3 + x;
}
void getKeyPedIInput(int* y , int* x)
{
	char flag=0;
	int RUN_ZERO[4] = {0xee,0xdd,0xbb,0x77};
  	KeyinitPortB();
	KeyinitPortD();
	KeyinitPortE();
	KeyinitPortF();
	KeyinitPortG();
	PORTG = 0x00;
 	PORTF = 0x07;
    flag=0; 
	column = 0;   
    while(column != 4)
    {	
		PORTG = 0x00;
        PORTF = 0x07;
		PORTE = RUN_ZERO[column];
       	delay(100);
		keyVal = PORTB & 0x0F;
        if(keyVal != 0x0f)
        {
           	flag=1;
           	break;
        }					    
        column++;  
	}
	if(!flag)
	{
		*x = -1;
		*y = -1;
		return;
	}
	*x = (keyVal ^ 0xf) & 0xf;
	*y = ((RUN_ZERO[column] ^ 0xf0) & 0xf0) >> 4;	
	
	*x = bitsToNumber(*x);
	*y = bitsToNumber(*y);
}

char GetChar()
{
	int x , y;
	do{
		getKeyPedIInput(&y , &x);
	}
	while(x == -1);
	if(x == 3)
	{
		return 'A' + y;
	}
	if(y == 3)
	{	
		switch(x)
		{
			case 0:
				return '*';
			case 1:
				return '0';
			case 2:
				return '#';
		}
	}
	return '1' + y * 3 + x;
}
   
int bitsToNumber(int bits)
{
	int num = 0;
	while(bits != 1)
	{
		bits /= 2;
		num++;
	}
	return num;
}
   
void print_led(char x)
{
	PORTF=4;
	PORTE=x;
	PORTDbits.RD4=1;
	PORTDbits.RD4=0;
}
void KeyinitPortB(void)
{             
	unsigned int portMap;
    portMap = TRISB;
	portMap &= 0xFFFF7FFF;
    portMap |= 0xF;
	TRISB = portMap;

	AD1PCFG |= 0x800f; //Select PORTB to be digital port input
	CNCONbits.ON = 0; //Change Notice Module On bit CN module is disabled
	CNEN |= 0x3C;	
	CNPUE |=0x3C;  	//Set RB0 - RB3 as inputs with weak pull-up
	CNCONbits.ON = 1;// 1 = CN module is enabled
}

void KeyinitPortD(void)
{
	unsigned int portMap;
	portMap = TRISD;
	portMap &= 0xFFFFFFCF;
	TRISD = portMap;
}
void KeyinitPortE(void)
{
	unsigned int portMap;
	portMap = TRISE;
	portMap &= 0xFFFFFF00;
	TRISE = portMap;
	PORTE = 0x00;
}

void KeyinitPortF(void)
{
	unsigned int portMap;
	portMap = TRISF;
	portMap &= 0xFFFFFEF8;
	TRISF = portMap;
	PORTFbits.RF8 = 1;
}

void KeyinitPortG(void)
{
	unsigned int portMap;
	portMap = TRISG;
	portMap &= 0xFFFFFFFC;
	TRISG = portMap;
	PORTG = 0x00;
}
#endif
   
