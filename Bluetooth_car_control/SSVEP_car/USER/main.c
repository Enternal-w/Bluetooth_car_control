#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "string.h"
#include "lcd.h"
#include "usart.h"
#define uint unsigned int

char a[128];
//u8 q[64];



void USART1_Init(void);
void USART1_IRQHandler(void);
void USART2_Init(void);
void USART2_IRQHandler(void);
void showstate(void);


void USART1_Init()
{
	GPIO_InitTypeDef GPIO_InitStrue;
	USART_InitTypeDef USART_InitStrue;
	NVIC_InitTypeDef NVIC_InitStrue;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);

	GPIO_InitStrue.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStrue.GPIO_Pin=GPIO_Pin_9;
	GPIO_InitStrue.GPIO_Speed=GPIO_Speed_10MHz;
	GPIO_Init(GPIOA,&GPIO_InitStrue);
	
	GPIO_InitStrue.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_InitStrue.GPIO_Pin=GPIO_Pin_10;
	GPIO_InitStrue.GPIO_Speed=GPIO_Speed_10MHz;
	GPIO_Init(GPIOA,&GPIO_InitStrue);
	
	USART_InitStrue.USART_BaudRate=38400;
	USART_InitStrue.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_InitStrue.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
	USART_InitStrue.USART_Parity=USART_Parity_No;
	USART_InitStrue.USART_StopBits=USART_StopBits_1;
	USART_InitStrue.USART_WordLength=USART_WordLength_8b; 
	
	USART_Init(USART1,&USART_InitStrue);
	USART_Cmd(USART1,ENABLE); 
	
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);//打开接受中断
	
	NVIC_InitStrue.NVIC_IRQChannel=USART1_IRQn;
	NVIC_InitStrue.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStrue.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStrue.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&NVIC_InitStrue);
}



void USART2_Init()
{
	GPIO_InitTypeDef GPIO_InitStrue;
	USART_InitTypeDef USART_InitStrue;
	NVIC_InitTypeDef NVIC_InitStrue;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	
	//USART2_TX   GPIOA.2
  GPIO_InitStrue.GPIO_Pin = GPIO_Pin_2; //PA2
  GPIO_InitStrue.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStrue.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOA, &GPIO_InitStrue);//初始化GPIO2 
  
	//USART2_RX	  GPIOA.3初始化
  GPIO_InitStrue.GPIO_Pin = GPIO_Pin_3;//PA3
  GPIO_InitStrue.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(GPIOA, &GPIO_InitStrue);//初始化GPIOA3 
	
	USART_InitStrue.USART_BaudRate=9600;//串口2 用来接串口屏幕
	USART_InitStrue.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_InitStrue.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
	USART_InitStrue.USART_Parity=USART_Parity_No;
	USART_InitStrue.USART_StopBits=USART_StopBits_1;
	USART_InitStrue.USART_WordLength=USART_WordLength_8b; 
	USART_Init(USART2,&USART_InitStrue);
	USART_Cmd(USART2,ENABLE); 
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);//打开接受中断
	
	NVIC_InitStrue.NVIC_IRQChannel=USART2_IRQn;       //屏幕的响应中断
	NVIC_InitStrue.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStrue.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStrue.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&NVIC_InitStrue);
	
}

 

void showstate()
{
	LCD_Clear(WHITE);
	POINT_COLOR=BLACK;
	
	if(a[1]==0x10)
		LCD_ShowString(30,40,240,72,24,"LED has Closed");     //1x是灯
	else if(a[1]==0x11)
		LCD_ShowString(30,40,240,72,24,"LED has Open");
	
	if(a[2]==0x20)
		LCD_ShowString(30,80,240,72,24,"Fan has Closed");     //2x是风扇
	else if(a[2]==0x21)
		LCD_ShowString(30,80,240,72,24,"Fan has Open");
}


void USART1_IRQHandler(void)
{
	u8 Res;
	if(USART_GetITStatus(USART1,USART_IT_RXNE))
	{
		Res=USART_ReceiveData(USART1);
		
		if(Res==0x10||Res==0x11)
		{
			a[1]=Res;
		}	
		
		else if(Res==0x20||Res==0x21)
		{
			a[2]=Res;
		}	
	}
	showstate();
}

void USART2_IRQHandler(void)
{
	u8 Res;
	
	if(USART_GetITStatus(USART2,USART_IT_RXNE))
	{
		Res=USART_ReceiveData(USART2);
		if(Res==0x10||Res==0x11)
		{
			a[1]=Res;
		}	
		else if(Res==0x20||Res==0x21)
		{
			a[2]=Res;
		}	
	}
	showstate();
}





int main(void)
{	 
	vu8 key=0;	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	USART1_Init();	
	USART2_Init();	
	delay_init();	    	 //延时函数初始化化	  
 	LED_Init();			     //LED端口初始化
	KEY_Init();          //初始化与按键连接的硬件接口 	 //初始化蜂鸣器端口
	LCD_Init();
	LED0=0;			         //先点亮红灯
	LCD_Clear(WHITE);
	POINT_COLOR=BLACK;

	while(1)
	{
		if(a[1]==0x11)//打开灯
			PEout(5)=0;
		else if(a[1]==0x10)//关闭灯
			PEout(5)=1;
		
		if(a[2]==0x20)//关闭风扇		
			PEout(1)=1;
		else if(a[2]==0x21)//打开风扇
			PEout(1)=0;
		
	}
}

