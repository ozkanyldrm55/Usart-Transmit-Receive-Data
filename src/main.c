#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

char str[50];
uint32_t i = 0 , a = 0;
char gelen_veri[21] = {0};

void Delay(uint32_t time)
{
	while(time)
	time--;
}


void UsartPuts(USART_TypeDef* USARTx , volatile char *s) // char karakter sayisi kadar donderiyor
{
	while(*s)
	{
		while(USART_GetFlagStatus(USARTx , USART_FLAG_TXE) == RESET);
		USART_SendData(USARTx, *s);
		s++;
	}
}


void USART2_IRQHandler(void){
	/* okunan data registeri bos ise yani herhangi bir veri gelmediyse bekle , RXEN = RX Data register not empty */
	while(USART_GetFlagStatus(USART2 , USART_FLAG_RXNE) == RESET);
	char tmp = USART_ReceiveData(USART2);

	USART_SendData(USART2 , tmp);
	//gelen_veri[i] = tmp;
	i++;

	if(tmp == '1')
	{
		GPIO_SetBits(GPIOD , GPIO_Pin_12);
		GPIO_ResetBits(GPIOD , GPIO_Pin_13);
		GPIO_ResetBits(GPIOD , GPIO_Pin_14);
		GPIO_ResetBits(GPIOD , GPIO_Pin_15);
	}
	if(tmp == '2')
	{
		GPIO_SetBits(GPIOD , GPIO_Pin_13);
		GPIO_ResetBits(GPIOD , GPIO_Pin_12);
		GPIO_ResetBits(GPIOD , GPIO_Pin_14);
		GPIO_ResetBits(GPIOD , GPIO_Pin_15);
	}
	if(tmp == '3')
	{
		GPIO_SetBits(GPIOD , GPIO_Pin_14);
		GPIO_ResetBits(GPIOD , GPIO_Pin_12);
		GPIO_ResetBits(GPIOD , GPIO_Pin_13);
		GPIO_ResetBits(GPIOD , GPIO_Pin_15);
	}
	if(tmp == '4')
	{
		GPIO_SetBits(GPIOD , GPIO_Pin_15);
		GPIO_ResetBits(GPIOD , GPIO_Pin_12);
		GPIO_ResetBits(GPIOD , GPIO_Pin_14);
		GPIO_ResetBits(GPIOD , GPIO_Pin_13);
	}





}

int main(void)
{

			GPIO_InitTypeDef 	GPIO_InitStructure;
			USART_InitTypeDef 	USART_InitStructure;

			RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE); // transmitter tx A2 GPIOya bagli usartta
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE); //GPIODya clock verdik,AHB1 hangi clock hattina bagli oldugu


			GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
			GPIO_InitStructure.GPIO_Pin=GPIO_Pin_12 | GPIO_Pin_13 |GPIO_Pin_14 | GPIO_Pin_15 ;
			GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
			GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;
			GPIO_Init(GPIOD, &GPIO_InitStructure); // D portu Out olarak tanimlanmis

			GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF; //Alternatif fonksiyonlar(input,output,adc disinda baska sey oldugu)
			GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2 | GPIO_Pin_3;
			GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
			GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;

			GPIO_Init(GPIOA,&GPIO_InitStructure);

			GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2);//USARTtan bilgiyi GPIOya atacaz,CPUya tanitmamiz lazim
			GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);

			USART_InitStructure.USART_BaudRate=115200; // Saniye içinde hat üzerinden kaç tane bit gönderilmesi gerektigi
			USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
			USART_InitStructure.USART_Mode=USART_Mode_Rx | USART_Mode_Tx;;
			USART_InitStructure.USART_Parity=USART_Parity_No;
			USART_InitStructure.USART_StopBits=USART_StopBits_1;
			USART_InitStructure.USART_WordLength=USART_WordLength_8b;
			USART_Init(USART2,&USART_InitStructure);

			USART_Cmd(USART2,ENABLE);
			//USART_ITConfig(USART2, USART_IT_TXE, ENABLE);
			USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

			NVIC_InitTypeDef NVIC_InitStructure;

			NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

			NVIC_Init(&NVIC_InitStructure);




  while (1)
  {
	  USART2_IRQHandler();

  }
}




/*
 * Callback used by stm32f4_discovery_audio_codec.c.
 * Refer to stm32f4_discovery_audio_codec.h for more info.
 */
void EVAL_AUDIO_TransferComplete_CallBack(uint32_t pBuffer, uint32_t Size){
  /* TODO, implement your code here */
  return;
}

/*
 * Callback used by stm324xg_eval_audio_codec.c.
 * Refer to stm324xg_eval_audio_codec.h for more info.
 */
uint16_t EVAL_AUDIO_GetSampleCallBack(void){
  /* TODO, implement your code here */
  return -1;
}
