#include "sys.h"
#include "stdio.h"
#include "dis.h"
#include "delay.h"
unsigned char i;
int main(void)
{ 
	 SystemInit();
	IIC_Init();
	while(1)
	{	
		disp(0xc2,i); 	
		disp(0xc3,i); 	
		delay_ms(200);
		i++;
		if(i==10) i=0;
	}                                                         
}
