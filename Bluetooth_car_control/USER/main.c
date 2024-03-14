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
void change(void);



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
	
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);//�򿪽����ж�
	
	NVIC_InitStrue.NVIC_IRQChannel=USART1_IRQn;
	NVIC_InitStrue.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStrue.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStrue.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&NVIC_InitStrue);
}



//void USART2_Init()
//{
//	GPIO_InitTypeDef GPIO_InitStrue;
//	USART_InitTypeDef USART_InitStrue;
//	NVIC_InitTypeDef NVIC_InitStrue;
//	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
//	
//	//USART2_TX   GPIOA.2
//  GPIO_InitStrue.GPIO_Pin = GPIO_Pin_2; //PA2
//  GPIO_InitStrue.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_InitStrue.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
//  GPIO_Init(GPIOA, &GPIO_InitStrue);//��ʼ��GPIO2 
//  
//	//USART2_RX	  GPIOA.3��ʼ��
//  GPIO_InitStrue.GPIO_Pin = GPIO_Pin_3;//PA3
//  GPIO_InitStrue.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
//  GPIO_Init(GPIOA, &GPIO_InitStrue);//��ʼ��GPIOA3 
//	
//	USART_InitStrue.USART_BaudRate=9600;//����2 �����Ӵ�����Ļ
//	USART_InitStrue.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
//	USART_InitStrue.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
//	USART_InitStrue.USART_Parity=USART_Parity_No;
//	USART_InitStrue.USART_StopBits=USART_StopBits_1;
//	USART_InitStrue.USART_WordLength=USART_WordLength_8b; 
//	USART_Init(USART2,&USART_InitStrue);
//	USART_Cmd(USART2,ENABLE); 
//	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);//�򿪽����ж�
//	
//	NVIC_InitStrue.NVIC_IRQChannel=USART2_IRQn;       //��Ļ����Ӧ�ж�
//	NVIC_InitStrue.NVIC_IRQChannelCmd=ENABLE;
//	NVIC_InitStrue.NVIC_IRQChannelPreemptionPriority=1;
//	NVIC_InitStrue.NVIC_IRQChannelSubPriority=1;
//	NVIC_Init(&NVIC_InitStrue);
//	
//}

 



void USART1_IRQHandler(void)
{
	u8 Res;
	if(USART_GetITStatus(USART1,USART_IT_RXNE))
	{
		Res=USART_ReceiveData(USART1);
		a[0]=Res;
	}
	//showstate();
}

//void USART2_IRQHandler(void)
//{
//	u8 Res;
//	if(USART_GetITStatus(USART2,USART_IT_RXNE))
//	{
//		Res=USART_ReceiveData(USART2);
//		
//		a[0]=Res;
//	}
//		//showstate();
//}





//void change()
//{
//	if(a[1]==0x10||a[1]==0x11||a[1]==0x12||a[1]==0x13||a[1]==0x14||a[1]==0x15); //�򿪵�
//			
//}



/*
ENA = PF1
IN1 = PF2
IN2 = PF3

ENB = PF4
IN3 = PF5
IN4 = PF6
*/

void backward()
{
	PFout(1) = 1;
	PFout(5) = 1;
	
	PFout(3) = 1;
	PFout(4) = 0; //����
	
	PFout(7) = 1;
	PFout(8) = 0; //����
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);

	
	PFout(1) = 0, PFout(5) = 0;
}

void turn_right()
{
	PFout(1) = 1, PFout(5) = 1;
	PFout(3) = 0,PFout(4) = 1; //����
	PFout(7) = 1,PFout(8) = 0; //����
	delay_ms(600);
	PFout(1) = 0, PFout(5) = 0;
}

void turn_left()
{
	PFout(1) = 1, PFout(5) = 1;
	PFout(3) = 1,PFout(4) = 0; //����
	PFout(7) = 0,PFout(8) = 1; //����
	delay_ms(600);
	PFout(1) = 0, PFout(5) = 0;
}

void forward()
{
	PFout(1) = 1, PFout(5) = 1;
	PFout(3) = 0,PFout(4) = 1; //����
	PFout(7) = 0,PFout(8) = 1; //����
	
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	
	PFout(1) = 0, PFout(5) = 0;
}




int main(void)
{	 
//	vu8 key=0;	
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	USART1_Init();	
//	USART2_Init();	
	delay_init();	    	 //��ʱ������ʼ����	  
 	LED_Init();			     //LED�˿ڳ�ʼ��
	

	a[0]=0x50;
	//a[2]=0x20,a[3]=0x00,a[4]=0x00; //����Ĭ���ǹ��ŵ�״̬
	//showstate();
	while(1)
	{
		if(a[0]==0x10)
			forward(),a[0]=0x50;
		else if(a[0]==0x20)
			turn_right(),a[0]=0x50;
		else if(a[0]==0x30)
			turn_left(),a[0]=0x50;
		else if(a[0]==0x40)
			backward(),a[0]=0x50;
	}		
	
}


