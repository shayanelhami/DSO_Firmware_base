/******************** (C) COPYRIGHT 2009 e-Design Co.,Ltd. *********************
* File Name: HW_V1_config.c   Author: bure   Hardware: DS0201V1.1  Version: V1.0
*******************************************************************************/
#include "Function.h"
#include "stm32f10x_it.h"
#include "stm32f10x_systick.h"
#include "HW_V1_Config.h"
#include "usb_lib.h"
#include "msd.h"

unsigned char   Key_Status_Last = 0;

ErrorStatus     HSEStartUpStatus;

/*******************************************************************************
* System Clock Configuration  
*******************************************************************************/
void            Set_System(void)
{
   RCC_DeInit();
   RCC_HSEConfig(RCC_HSE_ON);
   HSEStartUpStatus = RCC_WaitForHSEStartUp();
   if (HSEStartUpStatus == SUCCESS)
   {
      FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
      FLASH_SetLatency(FLASH_Latency_2); // Flash 2 wait state

      RCC_HCLKConfig(RCC_SYSCLK_Div1); // HCLK = SYSCLK 

      RCC_PCLK2Config(RCC_HCLK_Div1); // PCLK2 = HCLK 

      RCC_PCLK1Config(RCC_HCLK_Div2); // PCLK1 = HCLK/2

      RCC_ADCCLKConfig(RCC_PCLK2_Div4); // ADCCLK = PCLK2/4 = 18MHz

      RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9); // PLLCLK = 72 MHz

      RCC_PLLCmd(ENABLE);
      while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
      {
      }
      RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
      while (RCC_GetSYSCLKSource() != 0x08)
      {
      }
   }
   RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA, ENABLE);

   RCC_APB2PeriphClockCmd(
          RCC_APB2Periph_ADC1 | RCC_APB2Periph_ADC2 | RCC_APB2Periph_GPIOA |
        RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD |
   RCC_APB2Periph_GPIOE | RCC_APB2Periph_AFIO | RCC_APB2Periph_TIM1, ENABLE);

   RCC_APB1PeriphClockCmd(
           RCC_APB1Periph_TIM2 | RCC_APB1Periph_TIM3 | RCC_APB1Periph_TIM4 |
                            RCC_APB1Periph_SPI2, ENABLE);

   RCC_USBCLKConfig(RCC_USBCLKSource_PLLCLK_1Div5); // USBCLK = 48MHz

   RCC_APB1PeriphClockCmd(RCC_APB1Periph_USB, ENABLE); // Enable USB clock

}
/*******************************************************************************
* Interrupt vector Configuration   
*******************************************************************************/
void            NVIC_Configuration(void)
{
   NVIC_InitTypeDef NVIC_InitStructure;

//  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0000);
   NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x4000);

   NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

   NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN_RX0_IRQChannel;
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
   NVIC_Init(&NVIC_InitStructure);

   NVIC_InitStructure.NVIC_IRQChannel = USB_HP_CAN_TX_IRQChannel;
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
   NVIC_Init(&NVIC_InitStructure);
   /*
    * enabling TIM1 interrupt 
    */
   NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQChannel;
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
   NVIC_Init(&NVIC_InitStructure);
   /*
    * enabling TIM3 interrupt 
    */
   NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQChannel;
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
   NVIC_Init(&NVIC_InitStructure);
   /*
    * enabling DMA interrupt 
    */
   NVIC_InitStructure.NVIC_IRQChannel = DMAChannel2_IRQChannel;
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
   NVIC_Init(&NVIC_InitStructure);
   /*
    * SysTick NVIC_SystemHandlerPriorityConfig(SystemHandler_SysTick, 1, 0);
    * SysTick_ITConfig(ENABLE); SysTick_CounterCmd(SysTick_Counter_Enable);
    */
}

/*******************************************************************************
* GPIO Configuration   
*******************************************************************************/
void            GPIO_Config(void)
{
   GPIOA_CRL = 0x88888080; /*
                            * GPIOA Bit0-7  |||||||+----- Nib0  Ain   Analog
                            * input ||||||+------ Nib1  NC    Pull up input
                            * |||||+------- Nib2  Vbat  Analog input  
                            * ||||+-------- Nib3  NC    Pull up input  
                            * |||+--------- Nib4  KP    Pull up input  
                            * ||+---------- Nib5  KL    Pull up input  
                            * |+----------- Nib6  KU    Pull up input  
                            * +------------ Nib7  KR    Pull up input
                            */
   GPIOA_CRH = 0x8884488B; /*
                            * GPIOA Bit8-15  |||||||+----- Nib8  MCO
                            * Alternate output ||||||+------ Nib9  NC    Pull 
                            * up input |||||+------- Nib10 NC    Pull up
                            * input   ||||+-------- Nib11 D-    Input  
                            * |||+--------- Nib12 D+    Input   ||+---------- 
                            * Nib13 TMS   Pull up input   |+----------- Nib14 
                            * TCK   Pull up input   +------------ Nib15 TDI
                            * Pull up input
                            */
   GPIOA_ODR = 0x0FFFF;

   GPIOB_CRL = 0x88888411; /*
                            * GPIOB Bit0-7  |||||||+----- Nib0  A     Low
                            * speed output ||||||+------ Nib1  D     Low
                            * speed output |||||+------- Nib2  BT1   Input  
                            * ||||+-------- Nib3  TDO   Pull up input  
                            * |||+--------- Nib4  TRST  Pull up input  
                            * ||+---------- Nib5  NC    Pull up input  
                            * |+----------- Nib6  NC    Pull up input  
                            * +------------ Nib7  NC    Pull up input
                            */
   GPIOB_CRH = 0xBBB1B488; /*
                            * GPIOB Bit8-15  |||||||+----- Nib8  NC    Pull
                            * up input ||||||+------ Nib9  NC    Pull up
                            * input |||||+------- Nib10 Vusb  Input  
                            * ||||+-------- Nib11 V0    Alternate output  
                            * |||+--------- Nib12 SDCS  Low speed output  
                            * ||+---------- Nib13 SCK   Alternate output  
                            * |+----------- Nib14 MISO  Alternate output  
                            * +------------ Nib15 MOSI  Alternate output
                            */
   GPIOB_ODR = 0x0FFFF;

   GPIOC_CRL = 0x84118881; /*
                            * GPIOB Bit0-7  |||||||+----- Nib0  nCLS  Low
                            * speed output ||||||+------ Nib1  NC    Pull up
                            * input |||||+------- Nib2  NC    Pull up input  
                            * ||||+-------- Nib3  NC    Pull up input  
                            * |||+--------- Nib4  C     Low speed output  
                            * ||+---------- Nib5  B     Low speed output  
                            * |+----------- Nib6  NC    Input   +------------
                            * Nib7  NC    Pull up input
                            */
   GPIOC_CRH = 0x88888884; /*
                            * GPIOB Bit8-15  |||||||+----- Nib8  NC    Input
                            * ||||||+------ Nib9  NC    Pull up input
                            * |||||+------- Nib10 NC    Pull up input  
                            * ||||+-------- Nib11 NC    Pull up input  
                            * |||+--------- Nib12 NC    Pull up input  
                            * ||+---------- Nib13 NC    Pull up input  
                            * |+----------- Nib14 NC    Pull up input  
                            * +------------ Nib15 NC    Pull up input
                            */
   GPIOC_ODR = 0x0FFFF;

   GPIOD_CRL = 0x38338838; /*
                            * GPIOB Bit0-7  |||||||+----- Nib0  NC    Pull up 
                            * input ||||||+------ Nib1  RS    Hight speed
                            * output |||||+------- Nib2  NC    Pull up input
                            *  ||||+-------- Nib3  NC    Pull up input  
                            * |||+--------- Nib4  nRD   Hight speed output  
                            * ||+---------- Nib5  nWR   Hight speed output  
                            * |+----------- Nib6  NC    Pull up input  
                            * +------------ Nib7  nCS   Hight speed output
                            */
   GPIOD_CRH = 0x888B8884; /*
                            * GPIOB Bit8-15  |||||||+----- Nib8  NC    Pull
                            * up input ||||||+------ Nib9  KD    Pull up
                            * input |||||+------- Nib10 SDDT  Pull up input  
                            * ||||+-------- Nib11 KM    Pull up input  
                            * |||+--------- Nib12 Fout  Alternate output  
                            * ||+---------- Nib13 NC    Pull up input  
                            * |+----------- Nib14 NC    Pull up input  
                            * +------------ Nib15 NC    Pull up input
                            */
   GPIOD_ODR = 0x0FF7F;

   GPIOE_CRL = 0x33333333; // Bit0-7  Hight speed output£®LCD_DB0-7 £©

   GPIOE_CRH = 0x33333333; // Bit8-15 Hight speed output£®LCD_DB8-15£©

   AFIO_MAPR = 0x00001200; /*
                            * AF remap and debug I/O configuration register
                            * Bit0-31  ||||||++----- Reset value
                            * |||||+------- Bit9   TIM2_CH3_CH4 remap to
                            * PB10,PB11 ||||+-------- Bit12  TIM4_CH1
                            * remap to PD12 ++++--------- Reset value 
                            */
}

/*******************************************************************************
*  SPI Configuration
*******************************************************************************/
void            SPI_Config(void)
{
   SPI_InitTypeDef SPI_InitStructure;

   SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
   SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
   SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
   SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
   SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
   SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
   SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
   SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
   SPI_InitStructure.SPI_CRCPolynomial = 7;
   SPI_Init(SPI2, &SPI_InitStructure); // SPI2 Config

   SPI_Cmd(SPI2, ENABLE); // SPI2 enable 

}
/*******************************************************************************
* DMA configuration 
*******************************************************************************/
void            DMA_Configuration(void)
{
   DMA_ISR = 0x00000000; /*
                          * DMA interrupt status register |||||||+---GIF1 =0
                          * |||||||+---TCIF1=0 |||||||+---HTIF1=0
                          * |||||||+---TEIF1=0 |++++++----Reset value
                          * +----------Reserved
                          */
   DMA_IFCR = 0x00000000; /*
                           * DMA interrupt flag clear register
                           * |||||||+---CGIF1 =0 |||||||+---CTCIF1=0
                           * |||||||+---CHTIF1=0 |||||||+---CTEIF1=0
                           * |++++++----Reset value +----------Reserved
                           */
   DMA_CCR1 = 0x00003580; /*
                           * 0011 0101 1000 0000   ||||       |||| |||| ||||
                           * |||+--EN = 0 ||||       |||| |||| ||||
                           * ||+---TCIE=0 ||||       |||| |||| ||||
                           * |+----HTIE=0 ||||       |||| |||| ||||
                           * +-----TEIE=0 ||||       |||| |||| |||+-------DIR 
                           * =0 ||||       |||| |||| ||+--------CIRC=0 ||||
                           *     |||| |||| |+---------PINC=0  ||||       |||| 
                           * |||| +----------MINC=1 ||||       ||||
                           * ||++------------PSIZE=01 ||||       ||||
                           * ++--------------MSIZE=01 ||||
                           * ||++-----------------PL=11 ||||
                           * |+-------------------MEM2MEM=0
                           * ++++-------+--------------------Reserved
                           */

   DMA_CNDTR1 = 0x00001000; // DMA channel1 number of data register=4096*2
                            // Bytes  

   DMA_CPAR1 = ADC1_DR_ADDR; // DMA channel1 peripheral address register 

   DMA_CMAR1 = (u32) & Scan_Buffer[0]; // DMA channel1 memory address
                                       // register

   DMA_CCR1 |= 0x00000001; // DMA channel1 EN = 1 

}
/*******************************************************************************
* ADC configuration 
*******************************************************************************/
void            ADC_Configuration(void)
{
   ADC2_CR1 = 0x00000000;
   ADC1_CR1 = 0x00000000; /*
                           * ADC control register 1
                           * ||||||++---AWDCH[4:0]=00000 
                           * ||||||+----EOC=AWD=JEOC=0 |||||+-----Nib8_SCAN=0
                           * ||||++-----Reset value |||+-------DUALMOD=0
                           * ||+--------Reset value  ++---------Reserved
                           */
   ADC2_CR2 = 0x00100000;
   ADC1_CR2 = 0x00100100; /*
                           * ADC control register 2 |||||||+---ADON=0
                           * |||||||+---CONT=0 |||||||+---CAL=0
                           * |||||||+---RSTCAL=0 ||||||+----Reserved
                           * |||||+-----ALIGN=0 & DMA=1 ||||+------Reset
                           * value |||+-------EXTSEL=000 ||+--------EXTTRIG=1 
                           * 
                           * ++---------Reserved
                           */
   ADC2_SQR1 = 0x00000000;
   ADC1_SQR1 = 0x00000000; /*
                            * ADC regular sequence register 1
                            * ||++++++---SQ13-16=00000 ++---------Reserved
                            */
   ADC2_SQR3 = 0x00000004;
   ADC1_SQR3 = 0x00000000; /*
                            * ADC regular sequence register 3
                            * ||||||++---SQ1=00000 +++++++----SQ2-6=00000
                            * +----------Reserved
                            */
   ADC2_SMPR2 = 0x00001000;
   ADC1_SMPR2 = 0x00000000; /*
                             * ADC sample time register 2   
                             * |||||||+---SMP01=001(3Bits)   
                             * ||||+------SMP04=001(3Bits)
                             * |+++++++---SMP11-17=000(3Bits*7)
                             * ++---------Reserved
                             */
   ADC1_CR2 |= 0x00000001;
   ADC2_CR2 |= 0x00000001; // ADON=1£∫ADC1°¢ADC2 Start

   ADC1_CR2 |= 0x00000008;
   while (ADC1_CR2 & 0x00000008); // ADC1 Initialize calibration register

   ADC2_CR2 |= 0x00000008;
   while (ADC2_CR2 & 0x00000008); // ADC2 Initialize calibration register

   ADC1_CR2 |= 0x00000004;
   while (ADC1_CR2 & 0x00000004); // ADC1 calibration

   ADC2_CR2 |= 0x00000004;
   while (ADC2_CR2 & 0x00000004); // ADC2 calibration

}
/*******************************************************************************
* Timer configuration 
*******************************************************************************/
void            Timer_Configuration(void)
{
//---------------TIM1 used in ADC-------------------
   Set_Base(Item_Index[2]);
   TIM1_CR1 = 0x0094; /*
                       * 0000 0000 1001 0100   |||| |||| |||| |||+---CEN=0
                       * |||| |||| |||| ||+----UDIS=0 |||| |||| ||||
                       * |+-----URS=1  |||| |||| |||| +------OPM=0  |||| |||| 
                       * |||+--------DIR=1 |||| |||| |++---------CMS=00 ||||
                       * |||| +-----------ARPE=1 |||| ||++-------------CKD=00
                       * ++++-++---------------Reserved
                       */
   TIM1_RCR = 0x0000; /*
                       * 0000 0000 0000 0001   Repetition counter register
                       * |||| |||| ++++ ++++---Repetition Counter Value=1
                       * ++++-++++-------------Reserved
                       */
   TIM1_CCER = 0x0001; /*
                        * 0000 0000 0000 0001   |||| |||| |||| |||+---CC1E=1
                        * |||| |||| |||| ||+----CC1P=0 |||| |||| ||||
                        * |+-----CC1NE=0 |||| |||| |||| +------CC1NP=0
                        * ++++-++++-++++--------Reset value
                        */
   TIM1_CCMR1 = 0x0078; /*
                         * 0000 0000 0111 1100   |||| |||| ||||
                         * ||++---CC1S=00 |||| |||| |||| |+-----OC1FE=1 ||||
                         * |||| |||| +------OC1PE=1  |||| ||||
                         * |+++--------0C1M=111 |||| |||| +-----------OC1CE=0
                         * ++++-++++-------------Reset value
                         */
   TIM1_BDTR = 0x8000; /*
                        * 1000 0000 0000 0000   |+++-++++-++++-++++---Reset
                        * value  +---------------------MOE=0
                        */
   TIM1_DIER = 0x4200; /*
                        * 0100 0011 0000 0000  DMA/Interrupt enable register
                        * |     ||        +----CC1IE=0 |
                        * |+-------------UDE=1 |     +--------------CC1DE=1
                        * +--------------------TDE=1
                        */
   TIM1_CR1 |= 0x0001; // CEN=1, TIMER1 Enable

//-----------------TIM2_CH4 used in Vertical offset PWM-------------------  
   TIM2_PSC = 0;
   TIM2_ARR = 3839; // 72MHz/3840=18.75KHz 

   Set_Y_Pos(v0);
   TIM2_CR1 = 0x0084; /*
                       * 0000 0000 1000 0100   |||| |||| |||| |||+---CEN=0
                       * |||| |||| |||| ||+----UDIS=0 |||| |||| ||||
                       * |+-----URS=1  |||| |||| |||| +------OPM=0  |||| |||| 
                       * |||+--------DIR=0 |||| |||| |++---------CMS=00 ||||
                       * |||| +-----------ARPE=1 |||| ||++-------------CKD=00
                       * ++++-++---------------Reserved
                       */
   TIM2_CCER = 0x3000; /*
                        * 0011 0000 0000 0000   |||| ++++-++++-++++---Reset
                        * value |||+------------------CC4E=1
                        * ||+-------------------CC4P=1
                        * |+--------------------CC4NE=0
                        * +---------------------CC4NP=0
                        */
   TIM2_CCMR2 = 0x7C00; /*
                         * 0111 1100 0000 0000   |||| |||| ++++-++++---Reset
                         * value |||| ||++-------------CC4S=00 ||||
                         * |+---------------OC4FE=1 ||||
                         * +----------------OC4PE=1 
                         * |+++------------------0C4M=111
                         * +---------------------OC4CE=0
                         */
   TIM2_CR1 |= 0x0001; // CEN=1, TIMER2 Enable 

//-----------------TIM3 used in system-------------------  
   TIM3_PSC = 9;
   TIM3_ARR = 7199; // 1mS =£®PSC+1£©°¡£®ARR+1£©/£®72MHz/2£©£®uS£©

   TIM3_CCR1 = 3600; // duty=50%

   TIM3_CR1 = 0x0084; /*
                       * 0000 0000 1000 0100   |||| |||| |||| |||+---CEN=0
                       * |||| |||| |||| ||+----UDIS=0 |||| |||| ||||
                       * |+-----URS=1  |||| |||| |||| +------OPM=0  |||| |||| 
                       * |||+--------DIR=0 |||| |||| |++---------CMS=00 ||||
                       * |||| +-----------ARPE=1 |||| ||++-------------CKD=00
                       * ++++-++---------------Reserved
                       */
   TIM3_DIER = 0x0002; /*
                        * 0000 0000 0000 0010   |         +----CC1IE=1
                        * +--------------CC1DE=1
                        */
   TIM3_CR1 |= 0x0001; // CEN=1, TIMER3 Enable 

//------------------TIM4_CH1 used in Reference Output----------------------
   TIM4_PSC = 7;
   TIM4_ARR = Fout_ARR[Item_Index[11]]; // Cycle

   TIM4_CCR1 = (Fout_ARR[Item_Index[11]] + 1) / 2; // Duty=50%

   TIM4_CR1 = 0x0084; /*
                       * 0000 0000 1000 0100   |||| |||| |||| |||+---CEN=0
                       * |||| |||| |||| ||+----UDIS=0 |||| |||| ||||
                       * |+-----URS=1  |||| |||| |||| +------OPM=0  |||| |||| 
                       * |||+--------DIR=0 |||| |||| |++---------CMS=00 ||||
                       * |||| +-----------ARPE=1 |||| ||++-------------CKD=00
                       * ++++-++---------------Reserved
                       */
   TIM4_CCER = 0x0001; /*
                        * 0000 0000 0000 0001   |||| |||| |||| |||+---CC1E=1
                        * |||| |||| |||| ||+----CC1P=0 |||| |||| ||||
                        * |+-----CC1NE=0 |||| |||| |||| +------CC1NP=0
                        * ++++-++++-++++--------Reset value
                        */
   TIM4_CCMR1 = 0x0078; /*
                         * 0000 0000 0111 1100   |||| |||| ||||
                         * ||++---CC1S=00 |||| |||| |||| |+-----OC1FE=1 ||||
                         * |||| |||| +------OC1PE=1  |||| ||||
                         * |+++--------0C1M=111 |||| |||| +-----------OC1CE=0
                         * ++++-++++-------------Reset value
                         */
   TIM4_CR1 |= 0x0001; // CEN=1, TIMER4 Enable

}
/*******************************************************************************
* Key Scan & Process    Output£∫KeyBuff=Key Code
*******************************************************************************/
char            KeyScan(void)
{
   char            Key_Status_Now = 0, KeyCode = 0;

   if ((~GPIOA_IDR) & 0x0010)
      Key_Status_Now |= 0x01; // KEY_PLAY_ON

   if ((~GPIOD_IDR) & 0x0800)
      Key_Status_Now |= 0x02; // KEY_MENU_ON

   if ((~GPIOA_IDR) & 0x0040)
      Key_Status_Now |= 0x04; // KEY_UP_ON

   if ((~GPIOD_IDR) & 0x0200)
      Key_Status_Now |= 0x08; // KEY_DOWN_ON

   if ((~GPIOA_IDR) & 0x0020)
      Key_Status_Now |= 0x10; // KEY_LEFT_ON

   if ((~GPIOA_IDR) & 0x0080)
      Key_Status_Now |= 0x20; // KEY_RIGHT_ON

   if (Key_Status_Now & (~Key_Status_Last))
   {

      Key_Wait_Counter = 25; // Push on <500mS, No auto repeat

      if (Key_Status_Now & 0x01)
         KeyCode = KEYCODE_PLAY; // KeyCode(Play) 

      if (Key_Status_Now & 0x02)
         KeyCode = KEYCODE_MANU; // KeyCode(Manu) 

      if (Key_Status_Now & 0x04)
         KeyCode = KEYCODE_UP; // KeyCode(Up) 

      if (Key_Status_Now & 0x08)
         KeyCode = KEYCODE_DOWN; // KeyCode(Down) 

      if (Key_Status_Now & 0x10)
         KeyCode = KEYCODE_LEFT; // KeyCode(Left) 

      if (Key_Status_Now & 0x20)
         KeyCode = KEYCODE_RIGHT; // KeyCode(Right) 

   } else
   {

      if (Key_Status_Now & Key_Status_Last)
      {
         if ((Key_Wait_Counter | Key_Repeat_Counter) == 0)
         {

            if (Key_Status_Now & 0x01)
               KeyCode = KEYCODE_PLAY; // KeyCode(Play)

            if (Key_Status_Now & 0x02)
               KeyCode = KEYCODE_MANU; // KeyCode(Manu) 

            if (Key_Status_Now & 0x04)
               KeyCode = KEYCODE_UP; // KeyCode(Up) 

            if (Key_Status_Now & 0x08)
               KeyCode = KEYCODE_DOWN; // KeyCode(Down) 

            if (Key_Status_Now & 0x10)
               KeyCode = KEYCODE_LEFT; // KeyCode(Left) 

            if (Key_Status_Now & 0x20)
               KeyCode = KEYCODE_RIGHT; // KeyCode(Right)

            Key_Repeat_Counter = 3; // Auto repeat each 60mS

         }
      } else
         Key_Wait_Counter = 25;
   }
   Key_Status_Last = Key_Status_Now;
   return KeyCode;
}
/*******************************************************************************
* Write a Byte to MiniSD Card      Input£∫Data
*******************************************************************************/
void            MSD_WriteByte(u8 Data)
{
   while (SPI_GetFlagStatus(SPI2, SPI_FLAG_TXE) == RESET);
   SPI_SendData(SPI2, Data);
   while (SPI_GetFlagStatus(SPI2, SPI_FLAG_BSY) == RESET);
}

/*******************************************************************************
* Read a Byte to MiniSD Card     Return: Data
*******************************************************************************/
unsigned char   MSD_ReadByte(void)
{
   unsigned char   Data = 0;

   while (SPI_GetFlagStatus(SPI2, SPI_FLAG_TXE) == RESET);
   SPI_SendData(SPI2, DUMMY);
   while (SPI_GetFlagStatus(SPI2, SPI_FLAG_RXNE) == RESET);
   Data = SPI_ReceiveData(SPI2);
   return Data;
}

/*******************************************************************************
 Battery Detect 
*******************************************************************************/
void            Battery_Detect(void)
{
   unsigned int    vb;

   vb = (123 * (*((vu16 *) (ADC1_DR_ADDR + 2)))) >> 10;
   Battery = vb_Sum / 64;
   vb_Sum = vb_Sum + vb - Battery;
   Update[5] = 1;
   if (Battery > 360)
      Item_Index[5] = 3;
   else if (Battery > 340)
      Item_Index[5] = 2;
   else if (Battery > 320)
      Item_Index[5] = 1;
   else
      Item_Index[5] = 0;
}
/*******************************************************************************
 Set Vertical Range 
*******************************************************************************/
void            Set_Range(char Range)
{

   switch (Range)
   {
   case 0: // 10mV °¡1/Div   

   case 1: // 20mV °¡1/Div   

   case 2: // 50mV °¡1/Div   

   case 3: // 0.1V °¡1/Div   

      RANGE_A_HIGH();
      RANGE_B_HIGH();
      RANGE_C_HIGH();
      RANGE_D_HIGH();
      break;
   case 4: // 0.2V °¡1/Div µ    

   case 5: // 0.5V °¡1/Div   

   case 6: // 1V °¡1/Div   

      RANGE_A_HIGH();
      RANGE_B_HIGH();
      RANGE_C_HIGH();
      RANGE_D_LOW();
      break;
   case 7: // 2V °¡1/Div    

   case 8: // 5V °¡1/Div

   case 9: // 10V °¡1/Div

      RANGE_A_LOW();
      RANGE_B_HIGH();
      RANGE_C_HIGH();
      RANGE_D_LOW();
      break;
   case 10: // 0.2V/Div £®20mV °¡10/Div£© 

   case 11: // 0.5V/Div £®50mV °¡10/Div£©

   case 12: // 1V/Div £®0.1V °¡10/Div£©

      RANGE_A_HIGH();
      RANGE_B_LOW();
      RANGE_C_HIGH();
      RANGE_D_LOW();
      break;
   case 13: // 2V/Div £®0.2V °¡10/Div£©

   case 14: // 5V/Div £®0.5V °¡10/Div£©

   case 15: // 10V/Div £®  1V °¡10/Div£©

      RANGE_A_LOW();
      RANGE_B_LOW();
      RANGE_C_HIGH();
      RANGE_D_LOW();
      break;
   case 16: // 20V/Div £® 2V °¡10/Div£©

   case 17: // 50V/Div £® 5V °¡10/Div£©

   case 18: // 100V/Div £®10V °¡10/Div£©

      RANGE_A_HIGH();
      RANGE_B_HIGH();
      RANGE_C_LOW();
      RANGE_D_LOW();
      break;
   default: // Connecte To GND

      RANGE_A_LOW();
      RANGE_B_LOW();
      RANGE_C_LOW();
      RANGE_D_LOW();
      break;
   }
}

/*******************************************************************************
 Set Horizontal time Base 
*******************************************************************************/
void            Set_Base(char Base)
{
   TIM1_PSC = Scan_PSC[Base];
   TIM1_ARR = Scan_ARR[Base];
   TIM1_CCR1 = (Scan_ARR[Base] + 1) / 2;
}
/*******************************************************************************
 Restart the ADC 
*******************************************************************************/
void            ADC_Start(void)
{
   DMA_CCR1 = 0x00000000;
   DMA_CMAR1 = (u32) & Scan_Buffer[0];
   DMA_CNDTR1 = 0x00001000;
   DMA_CCR1 |= 0x00000001;
}
/*******************************************************************************
 Set Y Position
*******************************************************************************/
void            Set_Y_Pos(unsigned short Y0)
{
   switch (Item_Index[1])
   {
   case 0:
      TIM2_CCR4 = ((240 - Y0) * 20) / 32 + 881;
      break;
   case 1:
      TIM2_CCR4 = ((240 - Y0) * 42) / 32 + 799;
      break;
   case 2:
      TIM2_CCR4 = ((240 - Y0) * 104) / 32 + 566;
      break;
   case 3:
      TIM2_CCR4 = ((240 - Y0) * 207) / 32 + 180;
      break;

   case 4:
      TIM2_CCR4 = ((240 - Y0) * 55) / 32 + 750;
      break;
   case 5:
      TIM2_CCR4 = ((240 - Y0) * 138) / 32 + 439;
      break;
   case 6:
      TIM2_CCR4 = ((240 - Y0) * 275) / 32 - 75;
      break;

   case 7:
      TIM2_CCR4 = ((240 - Y0) * 53) / 32 + 758;
      break;
   case 8:
      TIM2_CCR4 = ((240 - Y0) * 132) / 32 + 461;
      break;
   case 9:
      TIM2_CCR4 = ((240 - Y0) * 265) / 32 - 37;
      break;

   case 10:
      TIM2_CCR4 = ((240 - Y0) * 42) / 32 + 799;
      break;
   case 11:
      TIM2_CCR4 = ((240 - Y0) * 105) / 32 + 563;
      break;
   case 12:
      TIM2_CCR4 = ((240 - Y0) * 212) / 32 + 161;
      break;

   case 13:
      TIM2_CCR4 = ((240 - Y0) * 57) / 32 + 743;
      break;
   case 14:
      TIM2_CCR4 = ((240 - Y0) * 142) / 32 + 424;
      break;
   case 15:
      TIM2_CCR4 = ((240 - Y0) * 284) / 32 - 109;
      break;

   case 16:
      TIM2_CCR4 = ((240 - Y0) * 59) / 32 + 735;
      break;
   case 17:
      TIM2_CCR4 = ((240 - Y0) * 146) / 32 + 409;
      break;
   case 18:
      TIM2_CCR4 = ((240 - Y0) * 294) / 32 - 146;
      break;

   default:
      TIM2_CCR4 = 956;
      break;
   }
}
/*******************************************************************************
 Detect USB ON
*******************************************************************************/
char            Test_USB_ON(void)
{
   if (GPIOB_IDR & 0x0400)
      return 1;
   else
      return 0;
}

/*******************************************************************************
 Detect SD Card ON
*******************************************************************************/
char            SD_Card_ON(void)
{
   if (GPIOD_IDR & 0x0400)
      return 1;
   else
      return 0;
}

/******************************** END OF FILE *********************************/
