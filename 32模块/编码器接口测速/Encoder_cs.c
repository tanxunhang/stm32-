#include "stm32f10x.h"                  // Device header

void Encoder_cs_Init(void){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;		//GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;				//定义结构体变量
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;     //时钟分频，选择不分频，此参数用于配置滤波器时钟，不影响时基单元功能
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; //计数器模式，选择向上计数
	TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1;					//计数周期，即ARR的值
	TIM_TimeBaseInitStructure.TIM_Prescaler = 1 - 1;				//预分频器，即PSC的值
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;            //重复计数器，高级定时器才会用到
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure);  
	
	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel=TIM_Channel_1;
	TIM_ICInitStructure.TIM_ICFilter=0xF;
	TIM_ICInitStructure.TIM_ICPolarity=TIM_ICPolarity_Rising;
	TIM_ICInit(TIM4,&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel=TIM_Channel_2;
	TIM_ICInitStructure.TIM_ICFilter=0xF;
	TIM_ICInitStructure.TIM_ICPolarity=TIM_ICPolarity_Rising;
	TIM_ICInit(TIM4,&TIM_ICInitStructure);

	TIM_EncoderInterfaceConfig(TIM4,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);
	
	TIM_Cmd(TIM4,ENABLE);
}

int16_t Encoder_cs_Get(void){
	int16_t temp;
	temp=TIM_GetCounter(TIM4);
	TIM_SetCounter(TIM4,0);
	return temp;	
}
