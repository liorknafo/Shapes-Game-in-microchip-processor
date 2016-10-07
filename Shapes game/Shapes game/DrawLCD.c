#include "DrawLCD.h"
#include <stdio.h>

#ifdef _WIN32
#include <stdlib.h>
void displayLCD(char* title , char* description)
{
	system("cls");
	printf("%s\n", title);
	printf("%s\n", description);
}

#else
#include <p32xxxx.h>
void busy()
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

void initLCD()
{
	unsigned int portMap;
	char control[6]={0x38,0x38,0x38,0x0e,0x06,0x1};
	int i;
	
	AD1PCFG |= 0x800f; //Select PORTB to be digital port input
	CNCONbits.ON = 0; //Change Notice Module On bit CN module is disabled
	CNEN |= 0x3C;	
	CNPUE |=0x3C;  	//Set RB0 - RB3 as inputs with weak pull-up
	CNCONbits.ON = 1;// 1 = CN module is enabled
	
	portMap = TRISG;
	portMap &= 0xFFFF7FFF;
	TRISG = portMap;
	PORTGbits.RG15 = 0;//buzzer=0ff
	portMap = TRISB;
	portMap &= 0xFFFF7FFF;
	TRISB = portMap;

	
	portMap = TRISF;
	portMap &= 0xFFFFFEF8;
	TRISF = portMap;
	PORTFbits.RF8 = 1;
    
	portMap = TRISE;
	portMap &= 0xFFFFFF00;
	TRISE = portMap;
	       
	portMap = TRISD;
	portMap &= 0xFFFFFFCF;
	TRISD = portMap;

	
	PORTDbits.RD5 = 0;//w/r
	PORTBbits.RB15 = 0;//rs
	PORTF = 0x00;
	for(i = 0;i < 6;i++)
	{
	 	PORTE=control[i];
        PORTDbits.RD4=1;//enable=1
        PORTDbits.RD4=0;//enable=0
		busy();
	}
	PORTFbits.RF8 = 1;
	PORTDbits.RD5 = 0;//w/r
	PORTBbits.RB15 = 1;//rs
	PORTF = 0x00;
}

void displayLCD(char* title , char* description)
{
	char* string = title;
	int i;
	initLCD();
	for(i = 0;i < strlen(title);i++)
    {
	  	PORTE=string[i];
        PORTDbits.RD4=1;//enable=1
        PORTDbits.RD4=0;//enable=0
		busy();
    }   
	PORTFbits.RF8 = 1;
	PORTDbits.RD5 = 0;//w/r
	PORTBbits.RB15 = 0;//rs
	PORTF = 0x00;
	 
	PORTDbits.RD5 = 0;//w/r
	PORTBbits.RB15 = 0;//rs
	PORTF = 0x00;
	PORTE=0xc0;
    PORTDbits.RD4=1;//enable=1
    PORTDbits.RD4=0;//enable=0
	busy();
	PORTFbits.RF8 = 1;
	PORTDbits.RD5 = 0;//w/r
	PORTBbits.RB15 = 1;//rs
	PORTF = 0x00;
	string = description;
	for(i = 0;i < strlen(description);i++)
    {
	  	PORTE=string[i];
        PORTDbits.RD4=1;//enable=1
        PORTDbits.RD4=0;//enable=0
		busy();
    }   
}
#endif