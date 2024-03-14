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

u8 x3=0;//������״̬
u8 x4=0;//�ŵ�״̬
u8 x2=0;//���� ���ŵ�
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
	
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);//�򿪽����ж�
	
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
  GPIO_InitStrue.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
  GPIO_Init(GPIOA, &GPIO_InitStrue);//��ʼ��GPIO2 
  
	//USART2_RX	  GPIOA.3��ʼ��
  GPIO_InitStrue.GPIO_Pin = GPIO_Pin_3;//PA3
  GPIO_InitStrue.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
  GPIO_Init(GPIOA, &GPIO_InitStrue);//��ʼ��GPIOA3 
	
	USART_InitStrue.USART_BaudRate=9600;//����2 �����Ӵ�����Ļ
	USART_InitStrue.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_InitStrue.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
	USART_InitStrue.USART_Parity=USART_Parity_No;
	USART_InitStrue.USART_StopBits=USART_StopBits_1;
	USART_InitStrue.USART_WordLength=USART_WordLength_8b; 
	USART_Init(USART2,&USART_InitStrue);
	USART_Cmd(USART2,ENABLE); 
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);//�򿪽����ж�
	
	NVIC_InitStrue.NVIC_IRQChannel=USART2_IRQn;       //��Ļ����Ӧ�ж�
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
		LCD_ShowString(30,40,240,72,24,"LED has Closed");     //1x�ǵ�
	else if(a[1]==0x11||a[1]==0x12||a[1]==0x13||a[1]==0x14)
		LCD_ShowString(30,40,240,72,24,"LED has Open");
	
	if(a[2]==0x20)
		LCD_ShowString(30,80,240,72,24,"Fan has Closed");     //2x�Ƿ���
	else if(a[2]==0x21)
		LCD_ShowString(30,80,240,72,24,"Fan has Open");
	
	if(a[3]==0x00||a[3]==0x30)
		LCD_ShowString(30,120,240,72,24,"Curtain has Closed");     //3x�Ǵ���
	else if(a[3]==0x11||a[3]==0x31)
		LCD_ShowString(30,120,240,72,24,"Curtain has Open");
	
	if(a[4]==0x00||a[4]==0x40)
		LCD_ShowString(30,160,240,72,24,"Door has Closed");     //4x����
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
	for(i=1;i<=600;i++)//2048ת4Ȧ����ôһȦ����2048/4=512     
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
	
	for(i=1;i<=500;i++)//2048ת4Ȧ����ôһȦ����2048/4=512     
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
	for(i=1;i<=128;i++) //2048ת4Ȧ����ôһȦ����2048/4=512     
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
	for(i=1;i<=128;i++) //2048ת4Ȧ����ôһȦ����2048/4=512     
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
	if(a[1]==0x10||a[1]==0x11||a[1]==0x12||a[1]==0x13||a[1]==0x14||a[1]==0x15) //�򿪵�
			light_control(a[1]);
	
	if(a[2]==0x20&&x2==1)//�رշ���		
			PEout(1)=1,x2=0,beepup();
	else if(a[2]==0x21&&x2==0)//�򿪷���
			PEout(1)=0,x2=1,beepup();
		
	if(a[3]==0x30&&x3==1)//��ס����
	{		
		x3=0;
		a[3]=0x00;
		chuang_close();
	}
	else if(a[3]==0x31&&x3==0)//��������
	{		
		x3=1;
		a[3]=0x11;
		chuang_open();
	}	

	
		if(a[4]==0x40&&x4==1)//����		
	{
			a[4]=0x00;
			x4=0;//x=0��ǵ�������ŵ�״̬�����;
			delay_ms(1000);
			door_close();
	}
	else if(a[4]==0x41&&x4==0) //����
	{
		a[4]=0x11;
		x4=1;         //x=1��� ����ڿ��ŵ�״̬������
		door_open();
	}
	

	if(a[5]==0x51)//ҹ��ؼ�ģʽ
	{
		a[1]=0x11;
		a[2]=0x21;
		a[3]=0x30;
		a[5]=0x00;
	}
	if(a[5]==0x52)//����ؼ�ģʽ
	{
		a[1]=0x10;
		a[2]=0x21;
		a[3]=0x31;
		a[5]=0x00;
	}
	if(a[5]==0x53)//���ģʽ
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
	
	if(RBG==0x10) //ȫ��
	{
		GPIO_SetBits(GPIOB,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2);
	}
	else if(RBG==0x11) //�׵�
	{
		GPIO_ResetBits(GPIOB,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2);
	}
	else if(RBG==0x12)//���
	{
		GPIO_ResetBits(GPIOB,GPIO_Pin_0);
		GPIO_SetBits(GPIOB,GPIO_Pin_1|GPIO_Pin_2);
	}
	
	else if(RBG==0x13)//�̵�
	{
		GPIO_ResetBits(GPIOB,GPIO_Pin_2);
		GPIO_SetBits(GPIOB,GPIO_Pin_0|GPIO_Pin_1);
	}
	
	else if(RBG==0x14)//����
	{
		GPIO_ResetBits(GPIOB,GPIO_Pin_1);
		GPIO_SetBits(GPIOB,GPIO_Pin_0|GPIO_Pin_2);
	}
	
	else if(RBG==0x15)//�Ų�
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
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	USART1_Init();	
	USART2_Init();	
	delay_init();	    	 //��ʱ������ʼ����	  
 	LED_Init();			     //LED�˿ڳ�ʼ��
	KEY_Init();          //��ʼ���밴�����ӵ�Ӳ���ӿ� 	 //��ʼ���������˿�
	LCD_Init();
	x4=0;    //������״̬�ǹ��ŵ�״̬   һ��Ҫ �ȿ����ţ�Ȼ���ٰ�����ס
	x3=0;//������������ס��
	x2=0;//���� ���ŵ�
	LCD_Clear(WHITE);
	POINT_COLOR=BLACK;
	a[1]=0x10,a[2]=0x20,a[3]=0x00,a[4]=0x00; //����Ĭ���ǹ��ŵ�״̬
	//showstate();
	while(1)
	{
		change();
	}		
	
}
//while(1){
//		key=KEY_Scan(0);	//�õ���ֵ
//  	if(key)                  //�ڽ����ַ�֮ǰ��ͨ����ť�����з��û��߿�ʼ���յȡ�����
//		{						   
//			switch(key)
//			{				 
//				case WKUP_PRES:	//����
//		
//					break;
//				case KEY2_PRES:	//�����ֽ���ʱ��΢��������ʹ�ֱ�ƽ��
//		
//				break;
//				case KEY1_PRES:	//��������ץͬʱ�պϣ���ť̫���ˡ���������Ȼ���Էֱ������ã����ܲ�����Ӽ���������					
//					R_shun_90();
//					break;
//				case KEY0_PRES:	//�����ֽ���˳ʱ��΢��������ʹ��ץ��ƽ
//		
//				break;
//			}
//		}
//		else delay_ms(10);  
//	}

