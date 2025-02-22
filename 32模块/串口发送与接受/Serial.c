#include "stm32f10x.h"                  // Device header
#include "stdio.h"

uint8_t Serial_RxData;
uint8_t Serial_RxFlag;

void Serial_Init(){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef A;
 	A.GPIO_Mode = GPIO_Mode_AF_PP;
	A.GPIO_Speed = GPIO_Speed_50MHz;
	A.GPIO_Pin = GPIO_Pin_9;
 	GPIO_Init(GPIOA, &A);
	
	A.GPIO_Mode = GPIO_Mode_IPU;
	A.GPIO_Speed = GPIO_Speed_50MHz;
	A.GPIO_Pin = GPIO_Pin_10;
 	GPIO_Init(GPIOA, &A);
	
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate=9600;
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode=USART_Mode_Tx|USART_Mode_Rx;
	USART_InitStructure.USART_Parity=USART_Parity_No;
	USART_InitStructure.USART_StopBits=USART_StopBits_1;
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;
	USART_Init(USART1,&USART_InitStructure);
	
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel=USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_Cmd(USART1,ENABLE); 
}

void Serial_SendByte(uint8_t Byte){
	USART_SendData(USART1,Byte);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
}

void Serial_SendArray(uint8_t *Array,uint16_t Length){
	uint16_t i;
	for(i=0;i<Length;i++){
		Serial_SendByte(Array[i]);
	}
}

void Serial_SendString(char *String){
	uint8_t i;
	for(i=0;String[i]!=0;i++){
		Serial_SendByte(String[i]);
	}
}

uint32_t Serial_Pow(uint32_t X,uint32_t Y){
	uint32_t result=1;
	while(Y--){
		result*=X;
	}
	return result;
}

void Serial_SendNumber(uint32_t Number,uint8_t Length){
	uint8_t i;
	for(i=0;i<Length;i++){
		Serial_SendByte(Number/Serial_Pow(10,Length-i-1)%10+'0');
	}
}

int fputc(int ch,FILE *f){
	Serial_SendByte(ch);
	return ch;
}

uint8_t Serial_GetRxFlag(void){
	if(Serial_RxFlag==1){
		Serial_RxFlag=0;
		return 1;
	}
	return 0;
}

uint8_t Serial_GetRxData(void){
	return Serial_RxData;
}

void USART1_IRQHandler(void){
	if(USART_GetFlagStatus(USART1,USART_IT_RXNE)==SET){
		Serial_RxData=USART_ReceiveData(USART1);
		Serial_RxFlag=1;
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
	}
}
