#include "outputAnalog.h"

#ifdef _WIN32

#include <Windows.h>

void setAnalogValue(int value , int channel)
{
	HWND myconsole = GetConsoleWindow();
	HDC mydc1 = GetDC(myconsole);
	int i , j;
	for(j = 0 ; j < 1 ; j++)
	{
		for(i = 0 ; i < 0x100 ; i++)
		{
			if(0xff - i == value)
				SetPixel(mydc1 , 255 + j , i , RGB(0,0,0));
			else
				SetPixel(mydc1 , 255 + j , i , RGB(0xff,0xff,0xff));
		}
	}
}

#else
#include <p32xxxx.h>

void initPortD(void);
void initPortE(void);
void initPortF(void);

void setAnalogValue(int value , int channel)
{
	int td = TRISD, te = TRISE , tf = TRISF;
	int pd = PORTD, pe = PORTE , pf = PORTF;
	initPortD();
	initPortE();
	initPortF();

	PORTF = channel;
	PORTE = value;
	PORTDbits.RD4 = 1;
	PORTDbits.RD4 = 0;

	/*TRISD = td;
	TRISE = te;
	TRISF = tf;

	PORTD = pd;
	PORTE = pe;
	PORTF = pf;*/
}

void initPortD(void)
{
	unsigned int portMap;

	portMap = TRISD;
	portMap &= 0xFFFFFF4F;
	TRISD = portMap;
	PORTDbits.RD4 = 0;
	PORTDbits.RD5 = 0;
}

void initPortE(void)
{
	unsigned int portMap;

	portMap = TRISE;
	portMap &= 0xFFFFFF00;
	TRISE = portMap;
	PORTE = 0x00;
}

void initPortF(void)
{
	unsigned int portMap;

	portMap = TRISF;
	portMap &= 0xFFFFFEF8;
	TRISF = portMap;
	PORTFbits.RF8 = 1;
}
#endif