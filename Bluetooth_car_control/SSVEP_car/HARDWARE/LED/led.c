#include "led.h"

//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//LED驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/2
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	   

//初始化PB5和PE5为输出口.并使能这两个口的时钟		    
//LED IO初始化
void LED_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOE|RCC_APB2Periph_GPIOF, ENABLE);	 //使能PB,PE端口时钟
// PE0 PE1 PB5 PE5 这四个io口控制 电机的转动	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;				 //PF.6端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOF, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.5
	GPIO_SetBits(GPIOF,GPIO_Pin_6);		//PB5 输出高
	
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;	    		 //LED1-->PE.5 端口配置, 推挽输出
	GPIO_Init(GPIOF, &GPIO_InitStructure);	  				 //推挽输出 ，IO口速度为50MHz
	GPIO_SetBits(GPIOF,GPIO_Pin_7); 						 //PF.7 输出高 
 
	
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;   //PE.0配置
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	GPIO_SetBits(GPIOE,GPIO_Pin_0);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;		//PE.1配置   控制左面的电机
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	GPIO_SetBits(GPIOE,GPIO_Pin_1);						
		
	

 ////////////////////////////////////////////////////////F口！~控制电磁阀
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOF, &GPIO_InitStructure);	  				 //PF0 ENA 驱动上的pwm 左面电磁阀
	GPIO_SetBits(GPIOF,GPIO_Pin_0);
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_Init(GPIOF, &GPIO_InitStructure);	  				 //PF1 IN1
	GPIO_SetBits(GPIOF,GPIO_Pin_1);
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOF, &GPIO_InitStructure);	  				 //PF2 IN2 
	GPIO_SetBits(GPIOF,GPIO_Pin_2);
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_Init(GPIOF, &GPIO_InitStructure);	  				 //PF3，ENB
	GPIO_SetBits(GPIOF,GPIO_Pin_3);
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;	
	GPIO_Init(GPIOF, &GPIO_InitStructure);	  				 //PF4	IN1
	GPIO_SetBits(GPIOF,GPIO_Pin_4);
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_Init(GPIOF, &GPIO_InitStructure);	  				 //PF5  IN2
	GPIO_SetBits(GPIOF,GPIO_Pin_5);
}
 

