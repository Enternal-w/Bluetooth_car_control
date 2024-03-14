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

u8 x3=0;//窗帘的状态
u8 x4=0;//门的状态
u8 x2=0;//风扇 关着的
void USART1_Init(void);
void USART1_IRQHandler(void);
void USART2_Init(void);
void USART2_IRQHandler(void);
void showstate(void);
void chuang_close(void);
void chuang_open(void);
void change(void);
void light_control(char RBG);
void door_open(void);
void door_close(void);
void beepup(void);


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
	else if(a[1]==0x11||a[1]==0x12||a[1]==0x13||a[1]==0x14)
		LCD_ShowString(30,40,240,72,24,"LED has Open");
	
	if(a[2]==0x20)
		LCD_ShowString(30,80,240,72,24,"Fan has Closed");     //2x是风扇
	else if(a[2]==0x21)
		LCD_ShowString(30,80,240,72,24,"Fan has Open");
	
	if(a[3]==0x00||a[3]==0x30)
		LCD_ShowString(30,120,240,72,24,"Curtain has Closed");     //3x是窗帘
	else if(a[3]==0x11||a[3]==0x31)
		LCD_ShowString(30,120,240,72,24,"Curtain has Open");
	
	if(a[4]==0x00||a[4]==0x40)
		LCD_ShowString(30,160,240,72,24,"Door has Closed");     //4x是门
	else if(a[4]==0x11||a[4]==0x41)
		LCD_ShowString(30,160,240,72,24,"Door has Open");
	
}


void USART1_IRQHandler(void)
{
	u8 Res;
	if(USART_GetITStatus(USART1,USART_IT_RXNE))
	{
		Res=USART_ReceiveData(USART1);
		
		if(Res==0x10||Res==0x11||Res==0x12||Res==0x13||Res==0x14||Res==0x15)
		{
			a[1]=Res;
		}	
		else if(Res==0x20||Res==0x21)
		{
			a[2]=Res;
		}
		
		else if(Res==0x30||Res==0x31)
		{
			a[3]=Res;
		}	
		
		else if(Res==0x40||Res==0x41)
		{
			a[4]=Res;
		}	
		
		else if(Res==0x51||Res==0x52||Res==0x53)
		{
			a[5]=Res;
		}	
		
	}
	change();
	//showstate();
}

void USART2_IRQHandler(void)
{
	u8 Res;
	if(USART_GetITStatus(USART2,USART_IT_RXNE))
	{
		Res=USART_ReceiveData(USART2);
		
		if(Res==0x10||Res==0x11||Res==0x12||Res==0x13||Res==0x14||Res==0x15)
		{
			a[1]=Res;
		}	
		else if(Res==0x20||Res==0x21)
		{
			a[2]=Res;
		}
		else if(Res==0x30||Res==0x31)
		{
			a[3]=Res;
		}	
		
		else if(Res==0x40||Res==0x41)
		{
			a[4]=Res;
		}	
		
		else if(Res==0x51||Res==0x52||Res==0x53)
		{
			a[5]=Res;
		}	
	}
	change();
	//showstate();
}

void chuang_open()
{
	int i;
	for(i=1;i<=600;i++)//2048转4圈，那么一圈就是2048/4=512     
	{
		GPIO_SetBits(GPIOF,GPIO_Pin_1);
		GPIO_ResetBits(GPIOF,GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4);
		delay_us(2000);
		
		GPIO_SetBits(GPIOF,GPIO_Pin_2);
		GPIO_ResetBits(GPIOF,GPIO_Pin_1|GPIO_Pin_3|GPIO_Pin_4);
		delay_us(2000);
		
		GPIO_SetBits(GPIOF,GPIO_Pin_3);
		GPIO_ResetBits(GPIOF,GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_4);
		delay_us(2000);
		
		GPIO_SetBits(GPIOF,GPIO_Pin_4);
		GPIO_ResetBits(GPIOF,GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3);
		delay_us(2000);
	}
	GPIO_ResetBits(GPIOF,GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4);
	beepup();
}

void chuang_close()
{
	int i;
	
	for(i=1;i<=500;i++)//2048转4圈，那么一圈就是2048/4=512     
	{
		GPIO_SetBits(GPIOF,GPIO_Pin_4);
		GPIO_ResetBits(GPIOF,GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3);
		delay_us(2000);
		
		GPIO_SetBits(GPIOF,GPIO_Pin_3);
		GPIO_ResetBits(GPIOF,GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_4);
		delay_us(2000);
		
		GPIO_SetBits(GPIOF,GPIO_Pin_2);
		GPIO_ResetBits(GPIOF,GPIO_Pin_1|GPIO_Pin_3|GPIO_Pin_4);
		delay_us(2000);
		
		GPIO_SetBits(GPIOF,GPIO_Pin_1);
		GPIO_ResetBits(GPIOF,GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4);
		delay_us(2000);

	}
	GPIO_ResetBits(GPIOF,GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4);
	beepup();

}


void door_close()
{
	int i;
	for(i=1;i<=128;i++) //2048转4圈，那么一圈就是2048/4=512     
	{
		GPIO_SetBits(GPIOG,GPIO_Pin_2);
		GPIO_ResetBits(GPIOG,GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5);
		delay_us(2000);
		
		GPIO_SetBits(GPIOG,GPIO_Pin_3);
		GPIO_ResetBits(GPIOG,GPIO_Pin_2|GPIO_Pin_4|GPIO_Pin_5);
		delay_us(2000);
		
		GPIO_SetBits(GPIOG,GPIO_Pin_4);
		GPIO_ResetBits(GPIOG,GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_5);
		delay_us(2000);
		
		GPIO_SetBits(GPIOG,GPIO_Pin_5);
		GPIO_ResetBits(GPIOG,GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4);
		delay_us(2000);
	}
	GPIO_ResetBits(GPIOG,GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5);
	beepup();
}

void door_open()
{
	int i;
	for(i=1;i<=128;i++) //2048转4圈，那么一圈就是2048/4=512     
	{
		GPIO_SetBits(GPIOG,GPIO_Pin_5);
		GPIO_ResetBits(GPIOG,GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4);
		delay_us(2000);
		
		GPIO_SetBits(GPIOG,GPIO_Pin_4);
		GPIO_ResetBits(GPIOG,GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_5);
		delay_us(2000);
		
		GPIO_SetBits(GPIOG,GPIO_Pin_3);
		GPIO_ResetBits(GPIOG,GPIO_Pin_2|GPIO_Pin_4|GPIO_Pin_5);
		delay_us(2000);
		
		GPIO_SetBits(GPIOG,GPIO_Pin_2);
		GPIO_ResetBits(GPIOG,GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5);
		delay_us(2000);
	}
	GPIO_ResetBits(GPIOG,GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5);
	beepup();

}


void change()
{
	if(a[1]==0x10||a[1]==0x11||a[1]==0x12||a[1]==0x13||a[1]==0x14||a[1]==0x15) //打开灯
			light_control(a[1]);
	
	if(a[2]==0x20&&x2==1)//关闭风扇		
			PEout(1)=1,x2=0,beepup();
	else if(a[2]==0x21&&x2==0)//打开风扇
			PEout(1)=0,x2=1,beepup();
		
	if(a[3]==0x30&&x3==1)//拉住窗帘
	{		
		x3=0;
		a[3]=0x00;
		chuang_close();
	}
	else if(a[3]==0x31&&x3==0)//拉开窗帘
	{		
		x3=1;
		a[3]=0x11;
		chuang_open();
	}	

	
		if(a[4]==0x40&&x4==1)//关门		
	{
			a[4]=0x00;
			x4=0;//x=0标记电机在锁门的状态，横的;
			delay_ms(1000);
			door_close();
	}
	else if(a[4]==0x41&&x4==0) //开门
	{
		a[4]=0x11;
		x4=1;         //x=1标记 电机在开门的状态，竖的
		door_open();
	}
	

	if(a[5]==0x51)//夜间回家模式
	{
		a[1]=0x11;
		a[2]=0x21;
		a[3]=0x30;
		a[5]=0x00;
	}
	if(a[5]==0x52)//白天回家模式
	{
		a[1]=0x10;
		a[2]=0x21;
		a[3]=0x31;
		a[5]=0x00;
	}
	if(a[5]==0x53)//离家模式
	{
		a[1]=0x10;
		a[2]=0x20;
		a[3]=0x31;
		a[5]=0x00;
	}
	//showstate();
}


void beepup()
{
	delay_ms(200);
	PBout(5)=0;
	delay_ms(200);
	PBout(5)=1;
}
void light_control(char RBG)
{
	
	if(RBG==0x10) //全关
	{
		GPIO_SetBits(GPIOB,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2);
	}
	else if(RBG==0x11) //白灯
	{
		GPIO_ResetBits(GPIOB,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2);
	}
	else if(RBG==0x12)//红灯
	{
		GPIO_ResetBits(GPIOB,GPIO_Pin_0);
		GPIO_SetBits(GPIOB,GPIO_Pin_1|GPIO_Pin_2);
	}
	
	else if(RBG==0x13)//绿灯
	{
		GPIO_ResetBits(GPIOB,GPIO_Pin_2);
		GPIO_SetBits(GPIOB,GPIO_Pin_0|GPIO_Pin_1);
	}
	
	else if(RBG==0x14)//蓝灯
	{
		GPIO_ResetBits(GPIOB,GPIO_Pin_1);
		GPIO_SetBits(GPIOB,GPIO_Pin_0|GPIO_Pin_2);
	}
	
	else if(RBG==0x15)//炫彩
	{
		GPIO_ResetBits(GPIOB,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2);
		delay_ms(500);
		GPIO_ResetBits(GPIOB,GPIO_Pin_0);
		GPIO_SetBits(GPIOB,GPIO_Pin_1|GPIO_Pin_2);
		delay_ms(500);
		GPIO_ResetBits(GPIOB,GPIO_Pin_2);
		GPIO_SetBits(GPIOB,GPIO_Pin_0|GPIO_Pin_1);
		delay_ms(500);
		GPIO_ResetBits(GPIOB,GPIO_Pin_1);
		GPIO_SetBits(GPIOB,GPIO_Pin_0|GPIO_Pin_2);
		delay_ms(500);
	}
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
	x4=0;    //上来的状态是关门的状态   一定要 先开开门，然后再把门锁住
	x3=0;//上来窗帘是拉住的
	x2=0;//风扇 关着的
	LCD_Clear(WHITE);
	POINT_COLOR=BLACK;
	a[1]=0x10,a[2]=0x20,a[3]=0x00,a[4]=0x00; //上来默认是关门的状态
	//showstate();
	while(1)
	{
		change();
	}		
	
}
//while(1){
//		key=KEY_Scan(0);	//得到键值
//  	if(key)                  //在接受字符之前，通过按钮来进行放置或者开始拍照等。。。
//		{						   
//			switch(key)
//			{				 
//				case WKUP_PRES:	//拍照
//		
//					break;
//				case KEY2_PRES:	//对左手进逆时针微调。用来使手变平。
//		
//				break;
//				case KEY1_PRES:	//控制俩手抓同时闭合，按钮太少了。。。。不然可以分别控制最好，看能不能外加几个按键。					
//					R_shun_90();
//					break;
//				case KEY0_PRES:	//对右手进行顺时针微调，用来使手抓变平
//		
//				break;
//			}
//		}
//		else delay_ms(10);  
//	}

