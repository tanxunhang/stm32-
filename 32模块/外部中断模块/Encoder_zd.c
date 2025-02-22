#include "stm32f10x.h"                  // Device header

extern int num; //extern 穿透文件传递变量

void Encoder_zd_Init(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	GPIO_InitTypeDef B;
	B.GPIO_Mode=GPIO_Mode_IPU;
	B.GPIO_Pin=GPIO_Pin_1|GPIO_Pin_0;
	B.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&B);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource0);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource1);
	
	EXTI_InitTypeDef EXIT_InitStructure;
	
	EXIT_InitStructure.EXTI_Line=EXTI_Line0;
	EXIT_InitStructure.EXTI_LineCmd=ENABLE;
	EXIT_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
	EXIT_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling; 
	EXTI_Init(&EXIT_InitStructure);
	
	EXIT_InitStructure.EXTI_Line=EXTI_Line1;
	EXIT_InitStructure.EXTI_LineCmd=ENABLE;
	EXIT_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
	EXIT_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling; 
	EXTI_Init(&EXIT_InitStructure);
	
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_InitStructure.NVIC_IRQChannel=EXTI0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel=EXTI1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&NVIC_InitStructure);
	
	
}

void EXTI1_IRQHandler(void){
 	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0)==0){
		num++;
	}
	EXTI_ClearITPendingBit(EXTI_Line1);
}
void EXTI0_IRQHandler(void){
 	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)==0){
		num--;
	}
	EXTI_ClearITPendingBit(EXTI_Line0);
}
