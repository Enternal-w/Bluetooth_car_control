#include "led.h"

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ���������ɣ��������������κ���;
//ALIENTEKս��STM32������
//LED��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/2
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) �������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	   

//��ʼ��PB5��PE5Ϊ�����.��ʹ���������ڵ�ʱ��		    
//LED IO��ʼ��
void LED_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOE|RCC_APB2Periph_GPIOF, ENABLE);	 //ʹ��PB,PE�˿�ʱ��
// PE0 PE1 PB5 PE5 ���ĸ�io�ڿ��� �����ת��	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;				 //PF.6�˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOF, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB.5
	GPIO_SetBits(GPIOF,GPIO_Pin_6);		//PB5 �����
	
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;	    		 //LED1-->PE.5 �˿�����, �������
	GPIO_Init(GPIOF, &GPIO_InitStructure);	  				 //������� ��IO���ٶ�Ϊ50MHz
	GPIO_SetBits(GPIOF,GPIO_Pin_7); 						 //PF.7 ����� 
 
	
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;   //PE.0����
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	GPIO_SetBits(GPIOE,GPIO_Pin_0);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;		//PE.1����   ��������ĵ��
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	GPIO_SetBits(GPIOE,GPIO_Pin_1);						
		
	

 ////////////////////////////////////////////////////////F�ڣ�~���Ƶ�ŷ�
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOF, &GPIO_InitStructure);	  				 //PF0 ENA �����ϵ�pwm �����ŷ�
	GPIO_SetBits(GPIOF,GPIO_Pin_0);
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_Init(GPIOF, &GPIO_InitStructure);	  				 //PF1 IN1
	GPIO_SetBits(GPIOF,GPIO_Pin_1);
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOF, &GPIO_InitStructure);	  				 //PF2 IN2 
	GPIO_SetBits(GPIOF,GPIO_Pin_2);
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_Init(GPIOF, &GPIO_InitStructure);	  				 //PF3��ENB
	GPIO_SetBits(GPIOF,GPIO_Pin_3);
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;	
	GPIO_Init(GPIOF, &GPIO_InitStructure);	  				 //PF4	IN1
	GPIO_SetBits(GPIOF,GPIO_Pin_4);
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_Init(GPIOF, &GPIO_InitStructure);	  				 //PF5  IN2
	GPIO_SetBits(GPIOF,GPIO_Pin_5);
}
 
