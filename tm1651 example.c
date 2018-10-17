#include <reg52.h>//头文件
#include "intrins.h"//包含_nop_()指令头文件

#define nop _nop_();_nop_();_nop_();_nop_();_nop_();//宏定义
//#define nop __nop();__nop();__nop();__nop();__nop();     keil开发中使用。注意nop前面有两个_.

/********************定义控制端口**********************/
sbit CLK=P3^3; //定义CLK
sbit DIO=P3^2; //定义DIO


/********************定义数据*************************/
unsigned char code CODE[10]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,};//共阳数码管0~9字型码
unsigned char code TAB[10]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,};//数码管不显示

/********************延时函数，延时nms******************/
void delay_nms(unsigned int n)
{
unsigned int i;
while(n--)
   for(i=0;i<550;i++);
}

/********************Start函数*************************/
void I2CStart()
{
DIO=1;
CLK=1;
nop;
DIO=1;
nop;
DIO=0;
nop;
CLK=0;
}

/********************Stop函数*************************/
void I2CStop()
{
    CLK=0;
nop;
nop;
DIO=0;
nop;
nop;
CLK=1;
nop;
nop;
nop;
DIO=1;
nop;
CLK=0;
DIO=0;
}

/***************发送8bit数据，从低位开始**************/
void I2CWritebyte(unsigned char oneByte)
{
  unsigned char i;
  for(i=0;i<8;i++)
  {
    CLK=0;
if(oneByte&0x01) 
  DIO=1;
else 
  DIO=0;
nop;
    CLK=1;
    oneByte=oneByte>>1;
  }
  //8位数据传送完 
CLK=0;//判断芯片发过来的ACK应答信号
nop;
while(DIO==1);
nop;
CLK=1;
nop;
}

/***************读按键程序**************/
unsigned char read_key()
{
unsigned char rekey,i;
I2CStart();
I2CWritebyte(0x42); //写读键指令0x42
DIO=1;
for(i=0;i<8;i++)
{
CLK=0;
nop;
nop;
rekey=rekey>>1;   //先读低位
nop;
nop;
CLK=1;
if(DIO) 
  rekey=rekey|0x80;
else 
  rekey=rekey|0x00;
nop;
}
CLK=0;  //判断芯片发过来的ACK应答信号
nop;
nop;
while(DIO==1);
nop;
nop;
CLK=1;
nop;
nop;
I2CStop();

return rekey;
}

/************显示函数，地址自加一************/
void disp0(unsigned char *p)                      
{
unsigned char i;
I2CStart();
I2CWritebyte(0x40);  //数据命令设置：地址自动加1，写数据到显示寄存器
I2CStop();

I2CStart();
I2CWritebyte(0xc0);  //地址命令设置：初始地址为00H
for(i=0;i<4;i++)  //发送4字节数据到显存
{
I2CWritebyte(*p);
p++;
}
I2CStop();

I2CStart();
I2CWritebyte(0x8C); //显示控制命令：开显示，脉冲宽度为11/16.
I2CStop();

}

/************显示函数，固定地址写数据************/
void disp(unsigned char add, unsigned char value)
{
I2CStart();
I2CWritebyte(0x44); //数据命令设置：固定地址，写数据到显示寄存器
I2CStop();

I2CStart();
I2CWritebyte(add);//地址命令设置：写入add对应地址

I2CWritebyte(CODE[value]);//给add地址写数据
I2CStop();

I2CStart();
I2CWritebyte(0x8C);//显示控制命令：开显示，脉冲宽度为11/16.
I2CStop();

}

/************按键处理函数，按键数据低位在前高位在后************/
void key_process()
{
unsigned char temp;
temp=read_key();  //读取按键返回值
if(temp!=0xff)
{
  disp0(TAB);  //清屏
  switch(temp)
  {
    case 0xf7 : disp(0xc0,1);break;  //K1与SG1对应按键按下,显示1
case 0xf6 : disp(0xc1,2);break;  //K1与SG2对应按键按下,显示2
case 0xf5 : disp(0xc2,3);break;  //K1与SG3对应按键按下,显示3
case 0xf4 : disp(0xc3,4);break;  //K1与SG4对应按键按下,显示4
case 0xf3 : disp(0xc0,5);break;  //K1与SG5对应按键按下,显示5
case 0xf2 : disp(0xc1,6);break;  //K1与SG6对应按键按下,显示6
case 0xf1 : disp(0xc2,7);break;  //K1与SG7对应按键按下,显示7
default   : break;
  }
}
}

void main()
{

disp0(CODE);      //上电数码管显示0~3
delay_nms(1);
while(1)     //按键后显示按键内容
{
key_process();
delay_nms(100);
}
}