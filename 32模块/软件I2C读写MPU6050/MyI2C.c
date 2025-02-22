#include "stm32f10x.h"                  // Device header
#include "DeLay.h"

void MyI2C_W_SCL(uint8_t BitValue){
	GPIO_WriteBit(GPIOA,GPIO_Pin_9,(BitAction)BitValue);
	Delay_us(10);
}

void MyI2C_W_SDA(uint8_t BitValue){
	GPIO_WriteBit(GPIOA,GPIO_Pin_8,(BitAction)BitValue);
	Delay_us(10);
}

uint8_t MyI2C_R_SDA(void){
	uint8_t BitValue;
	BitValue = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8);
	Delay_us(10);
	return BitValue;
}

void MyI2C_Init(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef A;
 	A.GPIO_Mode = GPIO_Mode_Out_OD;
	A.GPIO_Speed = GPIO_Speed_50MHz;
	A.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;
 	GPIO_Init(GPIOA,&A);
	
	GPIO_SetBits(GPIOA,GPIO_Pin_8|GPIO_Pin_9);
}

void MyI2C_Start(){
	MyI2C_W_SDA(1);
	MyI2C_W_SCL(1);
	MyI2C_W_SDA(0);
	MyI2C_W_SCL(0);
}

void MyI2C_Stop(){
	MyI2C_W_SDA(0);
	MyI2C_W_SCL(1);
	MyI2C_W_SDA(1);
}

void MyI2C_SendBity(uint8_t Byte){
	uint8_t i;
	for(i=0;i<8;i++){
		MyI2C_W_SDA(Byte & (0x80>>i));
		MyI2C_W_SCL(1);
		MyI2C_W_SCL(0 );
	}
}

uint8_t MyI2C_ReceiveByte(void){
	uint8_t Byte=0x00,i;	
	MyI2C_W_SDA(1);
	for(i=0;i<8;i++){
		MyI2C_W_SCL(1);
		if(MyI2C_R_SDA()==1) {Byte |= (0x80>>i);}
		MyI2C_W_SCL(0);
	}
	return Byte;
}

void MyI2C_SendAck(uint8_t AckByte){
	MyI2C_W_SDA(AckByte);
	MyI2C_W_SCL(1);
	MyI2C_W_SCL(0);
	
}

uint8_t MyI2C_ReceiveAck(void){
	uint8_t AckByte;	
	MyI2C_W_SDA(1);
	MyI2C_W_SCL(1);
	AckByte=MyI2C_R_SDA();
	MyI2C_W_SCL(0);
	return AckByte;
}
