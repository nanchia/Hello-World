#include "dis.h"
unsigned char  CODE[10]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,};
unsigned char  TAB[10]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,};

void disp0(unsigned char *p)                      
{
	unsigned char i;
	IIC_Start();
	IIC_Send_Byte(0x40);  
	IIC_Stop();

	IIC_Start();
	IIC_Send_Byte(0xc0);  
	for(i=0;i<4;i++)  
	{
	IIC_Send_Byte(*p);

	p++;
	}
	IIC_Stop();

	IIC_Start();
	IIC_Send_Byte(0x8C); 
	IIC_Stop();

}

void disp(unsigned char add, unsigned char value)
{
	IIC_Start();
	IIC_Send_Byte(0x44);    //设置数据。固定地址。
	IIC_Stop();

	IIC_Start();
	IIC_Send_Byte(add);			//设置地址。

	IIC_Send_Byte(CODE[value]);	//传输显示数据。
	IIC_Stop();

	IIC_Start();
	IIC_Send_Byte(0x8C);		//控制显示。
	IIC_Stop();

}
