#include "sys.h"
#include "stdio.h"
#include "dis.h"
#include "delay.h"
int main(void)
{ 
	IIC_Init();
	while(1)
	{
	disp0(CODE); 	
	delay_ms(2000);
	}                                                         
}
