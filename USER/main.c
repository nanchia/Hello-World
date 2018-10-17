#include "sys.h"
#include "stdio.h"
#include "dis.h"
#include "delay.h"
int main(void)
{ 
	IIC_Init();
	while(1)
	{
	 disp(0xc0,1); 	
	//delay_ms(2000);
	}                                                         
}
