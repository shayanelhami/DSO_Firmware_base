/******************** (C) COPYRIGHT 2009 e-Design Co.,Ltd. *********************
* File Name: lcd.c      Author: bure      Hardware: DS0201V1.1     Version: V1.0
*******************************************************************************/
#include "Function.h"
#include "lcd.h"
#include "stm32f10x_it.h"
#include "HW_V1_Config.h"
#include "usb_lib.h"
#include "msd.h"

unsigned const short Char_Dot[760] = // 744
 {
   0x0000, 0x0000, 0x0000, 0x001C, 0x0020, 0x0040, 0x0040, 0x0040, /*
                                                                    * " No
                                                                    * used
                                                                    */
   0x0040, 0x0040, 0x0020, 0x001C, 0x0000, 0x0000, 0x0000, 0x0000, /*
                                                                    * # No
                                                                    * used
                                                                    */
   0x0000, 0x0000, 0x0000, 0xE000, 0x1000, 0x0800, 0x0800, 0x0800, /*
                                                                    * $ No
                                                                    * used
                                                                    */
   0x0C30, 0x0C60, 0x00C0, 0x0180, 0x0300, 0x0630, 0x0C30, 0x0000, /*
                                                                    * %    
                                                                    */
   0x0800, 0x0800, 0x1000, 0xE000, 0x0000, 0x0000, 0x0000, 0x0000, /*
                                                                    * & 
                                                                    */
   0x2010, 0x2010, 0x2010, 0x2010, 0x2010, 0x2010, 0x3FF0, 0x0000, /*
                                                                    * ' Empty 
                                                                    * bottom
                                                                    */
   0x0000, 0x0000, 0x0FC0, 0x1FE0, 0x3030, 0x2010, 0x0000, 0x0000, /*
                                                                    * (    
                                                                    */
   0x0000, 0x0000, 0x2010, 0x3030, 0x1FE0, 0x0FC0, 0x0000, 0x0000, /*
                                                                    * )    
                                                                    */
   0x0000, 0x18C0, 0x0D80, 0x0700, 0x0700, 0x0D80, 0x18C0, 0x0000,
/**  ¡Á*/
   0x0000, 0x0100, 0x0100, 0x07C0, 0x07C0, 0x0100, 0x0100, 0x0000, /*
                                                                    * +    
                                                                    */
   0x0000, 0x0000, 0x0000, 0xFFFC, 0x0000, 0x0000, 0x0000, 0x0000, /*
                                                                    * , No
                                                                    * used
                                                                    */
   0x0000, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0000,
/*-    */
   0x0000, 0x0000, 0x0000, 0x0030, 0x0030, 0x0000, 0x0000, 0x0000, /*
                                                                    * .    
                                                                    */
   0x0030, 0x0060, 0x00C0, 0x0180, 0x0300, 0x0600, 0x0C00, 0x0000, /*
                                                                    * /    
                                                                    */
   0x1FE0, 0x3FF0, 0x2090, 0x2310, 0x2410, 0x3FF0, 0x1FE0, 0x0000, /*
                                                                    * 0    
                                                                    */
   0x0000, 0x0810, 0x1810, 0x3FF0, 0x3FF0, 0x0010, 0x0010, 0x0000, /*
                                                                    * 1    
                                                                    */
   0x1070, 0x30F0, 0x2190, 0x2310, 0x2610, 0x3830, 0x1830, 0x0000, /*
                                                                    * 2    
                                                                    */
   0x1020, 0x3030, 0x2210, 0x2210, 0x2210, 0x3FF0, 0x1DE0, 0x0000, /*
                                                                    * 3    
                                                                    */
   0x0300, 0x0700, 0x0D00, 0x1910, 0x3FF0, 0x3FF0, 0x0110, 0x0000, /*
                                                                    * 4    
                                                                    */
   0x3E20, 0x3E30, 0x2210, 0x2210, 0x2310, 0x23F0, 0x21E0, 0x0000, /*
                                                                    * 5    
                                                                    */
   0x0FE0, 0x1FF0, 0x3210, 0x2210, 0x2210, 0x03F0, 0x01E0, 0x0000, /*
                                                                    * 6    
                                                                    */
   0x3000, 0x3000, 0x20F0, 0x21F0, 0x2300, 0x3E00, 0x3C00, 0x0000, /*
                                                                    * 7    
                                                                    */
   0x1DE0, 0x3FF0, 0x2210, 0x2210, 0x2210, 0x3FF0, 0x1DE0, 0x0000, /*
                                                                    * 8    
                                                                    */
   0x1C00, 0x3E10, 0x2210, 0x2210, 0x2230, 0x3FE0, 0x1FC0, 0x0000, /*
                                                                    * 9    
                                                                    */
   0x0000, 0x0000, 0x0000, 0x0C60, 0x0C60, 0x0000, 0x0000, 0x0000, /*
                                                                    * :    
                                                                    */
   0x0040, 0x0040, 0x0040, 0x0040, 0x0040, 0x0040, 0x0040, 0x0040, /*
                                                                    * ; No
                                                                    * used
                                                                    */
   0x0000, 0x0100, 0x0380, 0x06C0, 0x0C60, 0x1830, 0x1010, 0x0000, /*
                                                                    * <    
                                                                    */
   0x0240, 0x0240, 0x0240, 0x0240, 0x0240, 0x0240, 0x0240, 0x0000,
/*=    */
   0x0000, 0x1010, 0x1830, 0x0C60, 0x06C0, 0x0380, 0x0100, 0x0000, /*
                                                                    * >    
                                                                    */
   0x1800, 0x3800, 0x2000, 0x23B0, 0x27B0, 0x3C00, 0x1800, 0x0000, /*
                                                                    * ?    
                                                                    */
   0x0FC0, 0x1020, 0x2790, 0x2490, 0x27D0, 0x1050, 0x0F80, 0x0000, /*
                                                                    * @    
                                                                    */
   0x07F0, 0x0FF0, 0x1900, 0x3100, 0x1900, 0x0FF0, 0x07F0, 0x0000, /*
                                                                    * A    
                                                                    */
   0x2010, 0x3FF0, 0x3FF0, 0x2210, 0x2210, 0x3FF0, 0x1DE0, 0x0000, /*
                                                                    * B    
                                                                    */
   0x0FC0, 0x1FE0, 0x3030, 0x2010, 0x2010, 0x3030, 0x1860, 0x0000, /*
                                                                    * C    
                                                                    */
   0x2010, 0x3FF0, 0x3FF0, 0x2010, 0x3030, 0x1FE0, 0x0FC0, 0x0000, /*
                                                                    * D    
                                                                    */
   0x2010, 0x3FF0, 0x3FF0, 0x2210, 0x2710, 0x3030, 0x3870, 0x0000, /*
                                                                    * E    
                                                                    */
   0x2010, 0x3FF0, 0x3FF0, 0x2210, 0x2700, 0x3000, 0x3800, 0x0000, /*
                                                                    * F    
                                                                    */
   0x0FC0, 0x1FE0, 0x3030, 0x2110, 0x2110, 0x31E0, 0x19F0, 0x0000, /*
                                                                    * G    
                                                                    */
   0x3FF0, 0x3FF0, 0x0200, 0x0200, 0x0200, 0x3FF0, 0x3FF0, 0x0000, /*
                                                                    * H    
                                                                    */
   0x0000, 0x0000, 0x2010, 0x3FF0, 0x3FF0, 0x2010, 0x0000, 0x0000, /*
                                                                    * I    
                                                                    */
   0x00E0, 0x00F0, 0x0010, 0x2010, 0x3FF0, 0x3FE0, 0x2000, 0x0000, /*
                                                                    * J    
                                                                    */
   0x2010, 0x3FF0, 0x3FF0, 0x0300, 0x0F00, 0x3CF0, 0x3070, 0x0000, /*
                                                                    * K    
                                                                    */
   0x2010, 0x3FF0, 0x3FF0, 0x2010, 0x0010, 0x0030, 0x0030, 0x0000, /*
                                                                    * L    
                                                                    */
   0x3FF0, 0x3FF0, 0x1C00, 0x0E00, 0x1C00, 0x3FF0, 0x3FF0, 0x0000, /*
                                                                    * M    
                                                                    */
   0x3FF0, 0x3FF0, 0x1C00, 0x0E00, 0x0700, 0x3FF0, 0x3FF0, 0x0000, /*
                                                                    * N    
                                                                    */
   0x0FC0, 0x1FE0, 0x3030, 0x2010, 0x3030, 0x1FE0, 0x0FC0, 0x0000, /*
                                                                    * O    
                                                                    */
   0x2010, 0x3FF0, 0x3FF0, 0x2210, 0x2200, 0x3E00, 0x1C00, 0x0000, /*
                                                                    * P    
                                                                    */
   0x1FE0, 0x3FF0, 0x2010, 0x2070, 0x2030, 0x3FF8, 0x1FE8, 0x0000, /*
                                                                    * Q    
                                                                    */
   0x2010, 0x3FF0, 0x3FF0, 0x2200, 0x2300, 0x3FF0, 0x1CF0, 0x0000, /*
                                                                    * R    
                                                                    */
   0x1860, 0x3C70, 0x2610, 0x2210, 0x2310, 0x39F0, 0x18E0, 0x0000, /*
                                                                    * S    
                                                                    */
   0x0000, 0x3800, 0x3010, 0x3FF0, 0x3FF0, 0x3010, 0x3800, 0x0000, /*
                                                                    * T    
                                                                    */
   0x3FE0, 0x3FF0, 0x0010, 0x0010, 0x0010, 0x3FF0, 0x3FE0, 0x0000, /*
                                                                    * U    
                                                                    */
   0x3F80, 0x3FC0, 0x0060, 0x0030, 0x0060, 0x3FC0, 0x3F80, 0x0000, /*
                                                                    * V    
                                                                    */
   0x3FC0, 0x3FF0, 0x0070, 0x01C0, 0x0070, 0x3FF0, 0x3FC0, 0x0000, /*
                                                                    * W    
                                                                    */
   0x3030, 0x3CF0, 0x0FC0, 0x0300, 0x0FC0, 0x3CF0, 0x3030, 0x0000, /*
                                                                    * X    
                                                                    */
   0x0000, 0x3C00, 0x3E10, 0x03F0, 0x03F0, 0x3E10, 0x3C00, 0x0000, /*
                                                                    * Y    
                                                                    */
   0x3870, 0x30F0, 0x2190, 0x2310, 0x2610, 0x3C30, 0x3870, 0x0000, /*
                                                                    * Z    
                                                                    */
   0x0010, 0x0030, 0x0050, 0x0190, 0x0310, 0x0C10, 0x3FF0, 0x0000, /*
                                                                    * [ Delta
                                                                    */
   0x0800, 0x0800, 0x0800, 0x0800, 0x0800, 0x0800, 0x0800, 0x0800, /*
                                                                    * \ No
                                                                    * used
                                                                    */
   0x0000, 0x0000, 0x0000, 0xFC7C, 0x0280, 0x0280, 0x0100, 0x0100, /*
                                                                    * ] No
                                                                    * used
                                                                    */
   0x0200, 0x0400, 0x0C00, 0x1FF0, 0x3FF0, 0x0C00, 0x0400, 0x0200, /*
                                                                    * ^
                                                                    * rising
                                                                    * edge
                                                                    */
   0x0100, 0x0080, 0x00C0, 0x3FE0, 0x3FF0, 0x00C0, 0x0080, 0x0100,
/*_ falling edge*/
   0x2010, 0x2010, 0x2010, 0x2010, 0x2010, 0x2010, 0x2010, 0x2010, /*
                                                                    * ` Empty 
                                                                    * body
                                                                    */
   0x00E0, 0x05F0, 0x0510, 0x0510, 0x07E0, 0x03F0, 0x0010, 0x0000, /*
                                                                    * a    
                                                                    */
   0x2010, 0x3FF0, 0x3FE0, 0x0210, 0x0210, 0x03F0, 0x01E0, 0x0000, /*
                                                                    * b    
                                                                    */
   0x03E0, 0x07F0, 0x0410, 0x0410, 0x0410, 0x0630, 0x0220, 0x0000, /*
                                                                    * c    
                                                                    */
   0x01E0, 0x03F0, 0x0610, 0x2410, 0x3FF0, 0x3FF0, 0x0010, 0x0000, /*
                                                                    * d    
                                                                    */
   0x03E0, 0x07F0, 0x0510, 0x0510, 0x0510, 0x0730, 0x0320, 0x0000, /*
                                                                    * e    
                                                                    */
   0x0000, 0x0210, 0x1FF0, 0x3FF0, 0x2210, 0x3000, 0x1800, 0x0000, /*
                                                                    * f    
                                                                    */
   0x03C8, 0x07EC, 0x0424, 0x0424, 0x03FC, 0x07F8, 0x0400, 0x0000, /*
                                                                    * g    
                                                                    */
   0x2010, 0x3FF0, 0x3FF0, 0x0220, 0x0400, 0x07F0, 0x03F0, 0x0000, /*
                                                                    * h    
                                                                    */
   0x0000, 0x0000, 0x0410, 0x37F0, 0x37F0, 0x0010, 0x0000, 0x0000, /*
                                                                    * i    
                                                                    */
   0x0000, 0x0018, 0x001C, 0x0404, 0x37FC, 0x37F8, 0x0400, 0x0000, /*
                                                                    * j    
                                                                    */
   0x2010, 0x3FF0, 0x3FF0, 0x2180, 0x03C0, 0x0670, 0x0430, 0x0000, /*
                                                                    * k    
                                                                    */
   0x0000, 0x0000, 0x2010, 0x3FF0, 0x3FF0, 0x0010, 0x0000, 0x0000, /*
                                                                    * l    
                                                                    */
   0x07F0, 0x07F0, 0x0600, 0x03F0, 0x0600, 0x07F0, 0x07F0, 0x0000, /*
                                                                    * m    
                                                                    */
   0x0400, 0x07F0, 0x03F0, 0x0400, 0x0400, 0x07F0, 0x03F0, 0x0000, /*
                                                                    * n    
                                                                    */
   0x03E0, 0x07F0, 0x0410, 0x0410, 0x0410, 0x07F0, 0x03E0, 0x0000, /*
                                                                    * o    
                                                                    */
   0x0404, 0x07FC, 0x03FC, 0x0444, 0x0440, 0x07C0, 0x0380, 0x0000, /*
                                                                    * p    
                                                                    */
   0x0380, 0x07C0, 0x0440, 0x0444, 0x07FC, 0x03FC, 0x0404, 0x0000, /*
                                                                    * q    
                                                                    */
   0x0410, 0x07F0, 0x03F0, 0x0610, 0x0400, 0x0600, 0x0300, 0x0000, /*
                                                                    * r    
                                                                    */
   0x0000, 0x0220, 0x0730, 0x0590, 0x0490, 0x06F0, 0x0260, 0x0000, /*
                                                                    * s    
                                                                    */
   0x0400, 0x0400, 0x1FE0, 0x3FF0, 0x0410, 0x0430, 0x0020, 0x0000, /*
                                                                    * t    
                                                                    */
   0x07E0, 0x07F0, 0x0010, 0x0010, 0x07E0, 0x07F0, 0x0010, 0x0000, /*
                                                                    * u    
                                                                    */
   0x0000, 0x07C0, 0x07E0, 0x0030, 0x0030, 0x07E0, 0x07C0, 0x0000, /*
                                                                    * v    
                                                                    */
   0x07E0, 0x07F0, 0x0030, 0x00E0, 0x0030, 0x07F0, 0x07E0, 0x0000, /*
                                                                    * w    
                                                                    */
   0x0410, 0x0630, 0x03E0, 0x01C0, 0x03E0, 0x0630, 0x0410, 0x0000, /*
                                                                    * x    
                                                                    */
   0x0780, 0x07C4, 0x0044, 0x0044, 0x0044, 0x07F8, 0x07F0, 0x0000, /*
                                                                    * y    
                                                                    */
   0x0630, 0x0670, 0x04D0, 0x0590, 0x0710, 0x0630, 0x0430, 0x0000, /*
                                                                    * z    
                                                                    */
   0x0780, 0x0480, 0x3CF0, 0x2010, 0x2FD0, 0x2FD0, 0x2FD0, 0x2FD0, /*
                                                                    * {
                                                                    * Battery 
                                                                    * head
                                                                    */
   0x2FD0, 0x2FD0, 0x2FD0, 0x2FD0, 0x2FD0, 0x2FD0, 0x2FD0, 0x2FD0, /*
                                                                    * |
                                                                    * Battery 
                                                                    * body
                                                                    */
   0x2FD0, 0x2FD0, 0x2FD0, 0x2FD0, 0x2FD0, 0x2010, 0x3FF0, 0x0000, /*
                                                                    * }
                                                                    * Battery 
                                                                    * bottom
                                                                    */
   0x0780, 0x0480, 0x3CF0, 0x2010, 0x2010, 0x2010, 0x2010, 0x2010 /*
                                                                   * ~ Empty
                                                                   * head
                                                                   */
};

unsigned const int Logo_Dot[512] = {

   0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
   0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,

   0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
   0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,

};

/*******************************************************************************
 LCD_WR_REG: Set LCD Register  Input: Register addr., Data
*******************************************************************************/
void            LCD_WR_REG(unsigned short Reg, unsigned short Data)
{
   LDC_DATA_OUT = Reg; // Reg. Addr.

   LCD_RS_LOW(); // RS=0,Piont to Index Reg.

   LCD_nWR_ACT(); // WR Cycle from 1 -> 0 -> 1

   LCD_RS_HIGH(); // RS=1,Piont to object Reg.

   LDC_DATA_OUT = Data; // Reg. Data 

   LCD_nWR_ACT(); // WR Cycle from 1 -> 0 -> 1

}
/*******************************************************************************
 LCD  initializtion 
*******************************************************************************/

void            LCD_Initial(void)
{
   // ------------------------- Reset LCD Driver ---------------------//
   LCD_DATA_BUS_OUT(); // Set LCD Data Bus as Output Mode

   LCD_nWR_HIGH();
   LCD_nRD_HIGH();
   LCD_RS_HIGH(); // RS=1        

   LCD_nRST_HIGH();
   Delayms(1); // RST=1£¬Delay 1ms      

   LCD_nRST_LOW();
   Delayms(1); // RST=0 Reset LCD£¬Delay 1ms      

   LCD_nRST_HIGH();
   Delayms(5); // RST=1£¬Delay 5 ms  
   // ----------------ST7781 Internal Register Initial------------//         

   LCD_WR_REG(0x00FF, 0x0001);
   LCD_WR_REG(0x00F3, 0x0008);
   LDC_DATA_OUT = 0x00F3;
   LCD_RS_LOW();
   LCD_nWR_ACT(); // Read Parameter      

   LCD_RS_HIGH();
   // -------------------Display Control Setting-----------------//
   LCD_WR_REG(0x0001, 0x0100); // Output Direct 

   LCD_WR_REG(0x0002, 0x0700); // Line Inversion

   LCD_WR_REG(0x0003, 0x0030); // BGR=0,ID=11

   LCD_WR_REG(0x0008, 0x0302); // Porch Setting

   LCD_WR_REG(0x0009, 0x0000); // Scan Cycle

   LCD_WR_REG(0x000A, 0x0000); // FMARK off
   // ----------------power Control Register Initial------------//

   LCD_WR_REG(0x0010, 0x0790); // Power Control1

   LCD_WR_REG(0x0011, 0x0005); // Power Control2

   LCD_WR_REG(0x0012, 0x0000); // Power Control3

   LCD_WR_REG(0x0013, 0x0000); // Power Control4

   Delayms(100); // Delay 100ms   

   LCD_WR_REG(0x0010, 0x12B0); // Power Control1

   Delayms(50); // delay 50ms

   LCD_WR_REG(0x0011, 0x0007); // Power Control2

   Delayms(50); // delay 50ms

   LCD_WR_REG(0x0012, 0x008C); // Power Control3

   LCD_WR_REG(0x0013, 0x1700); // Power Control4

   LCD_WR_REG(0x0029, 0x001A); // VCOMH Setting

   Delayms(50); // delay 50ms
   // ------------------GAMMA Cluster Setting-------------------//

   LCD_WR_REG(0x0030, 0x0000);
   LCD_WR_REG(0x0031, 0x0507);
   LCD_WR_REG(0x0032, 0x0003);
   LCD_WR_REG(0x0035, 0x0200);
   LCD_WR_REG(0x0036, 0x0106);
   LCD_WR_REG(0x0037, 0x0000);
   LCD_WR_REG(0x0038, 0x0507);
   LCD_WR_REG(0x0039, 0x0104);
   LCD_WR_REG(0x003C, 0x0200);
   LCD_WR_REG(0x003D, 0x0005);
   // ------------------Display Windows 240*320-----------------//
   LCD_WR_REG(0x0050, 0x0000);
   LCD_WR_REG(0x0051, 0x00EF);
   LCD_WR_REG(0x0052, 0x0000);
   LCD_WR_REG(0x0053, 0x013F);
   // --------------------Frame Rate Setting--------------------//
   LCD_WR_REG(0x0060, 0xA700);
   LCD_WR_REG(0x0061, 0x0001);
   LCD_WR_REG(0x0090, 0x0033);
   LCD_WR_REG(0x0007, 0x0133);
   Delayms(50); // delay 50ms

}

/*******************************************************************************
Point_SCR: Set display position   Input: X, Y 
*******************************************************************************/
void            Point_SCR(unsigned short x0, unsigned short y0)
{
   LCD_WR_REG(0x0020, y0);
   LCD_WR_REG(0x0021, x0);
   LDC_DATA_OUT = 0x0022; // DRAM Reg.      

   LCD_RS_LOW();
   LCD_nWR_ACT(); // WR Cycle from 1 -> 0 -> 1

   LCD_nWR_ACT(); // WR Cycle from 1 -> 0 -> 1

   LCD_RS_HIGH();
}
/*******************************************************************************
 Set_Pixel: Set a Pixel  Input: Color
*******************************************************************************/
void            Set_Pixel(unsigned short Color)
{
   LDC_DATA_OUT = Color; // Color Data

   LCD_nWR_ACT(); // WR Cycle from 1 -> 0 -> 1

}
/*******************************************************************************
 Clear Screen 
*******************************************************************************/
void            Clear_Screen(unsigned short Color)
{
   unsigned int    i;

   Point_SCR(0, 0); // X_pos=0£¬Y_pos=0

   for (i = 0; i < 240 * 320; ++i)
      Set_Pixel(Color);
}
/*******************************************************************************
 Display Frame 
*******************************************************************************/
void            Display_Frame(void)
{
   unsigned short  i, j;

   for (i = 0; i < 320; ++i)
   {
      Point_SCR(i, 0);
      for (j = 0; j < MIN_Y - 1; ++j)
         Set_Pixel(PANEL);
   }
   for (i = 0; i < 320; ++i)
   {
      Point_SCR(i, MAX_Y + 3);
      for (j = MAX_Y + 3; j < 240; ++j)
         Set_Pixel(PANEL);
   }
}
/*******************************************************************************
 Display Grid
*******************************************************************************/
void            Display_Grid(void)
{
   unsigned short  i, j;

   for (j = MIN_Y; j <= MAX_Y; j += 25)
      for (i = MIN_X; i < MAX_X + 1; ++i)
      {
         Point_SCR(i, j);
         Set_Pixel(GRID);
      }
   for (i = MIN_X; i <= MAX_X; i += 25)
   {
      Point_SCR(i, MIN_Y);
      for (j = MIN_Y; j <= MAX_Y; ++j)
         Set_Pixel(GRID);
   }
   for (i = MIN_X + 5; i < MAX_X; i += 5)
   {
      Point_SCR(i, MIN_Y + 1);
      Set_Pixel(GRID);
      Point_SCR(i, MAX_Y - 1);
      Set_Pixel(GRID);
      Point_SCR(i, ((MIN_Y + MAX_Y) / 2 + 1));
      Set_Pixel(GRID);
      Point_SCR(i, ((MIN_Y + MAX_Y) / 2 - 1));
      Set_Pixel(GRID);
   }
   for (j = MIN_Y + 5; j < MAX_Y; j += 5)
   {
      Point_SCR(MIN_X + 1, j);
      Set_Pixel(GRID);
      Point_SCR(MAX_X - 1, j);
      Set_Pixel(GRID);
      Point_SCR(((MIN_X + MAX_X) / 2 + 1), j);
      Set_Pixel(GRID);
      Point_SCR(((MIN_X + MAX_X) / 2 - 1), j);
      Set_Pixel(GRID);
   }
}
/*******************************************************************************
 Erase segment
*******************************************************************************/
void            Erase_SEG(unsigned short i, unsigned short y1, unsigned short y2)
{
   unsigned short  j, Tmp;

   Tmp = y2;
   if (y1 > y2)
   {
      y2 = y1;
      y1 = Tmp;
   }
   if (y2 >= MAX_Y)
      y2 = MAX_Y - 1;
   if (y1 <= MIN_Y)
      y1 = MIN_Y + 1;
   for (j = y1; j <= y2; ++j)
   {

      Point_SCR(i + MIN_X, j);
      Set_Pixel(BLACK);

      if (j > 97 + MIN_Y)
      {
         if (j < 103 + MIN_Y)
         {
            if (i % 5 == 0)
            {
               Point_SCR(i + MIN_X, j);
               Set_Pixel(GRID);
            }
         }
      }
      if (i > 147)
      {
         if (i < 153)
         {
            if ((j - MIN_Y) % 5 == 0)
            {
               Point_SCR(i + MIN_X, j);
               Set_Pixel(GRID);
            }
         }
      }
      if ((j - MIN_Y) % 25 == 0)
      {
         Point_SCR(i + MIN_X, j);
         Set_Pixel(GRID);
      }
      if (i % 25 == 0)
      {
         Point_SCR(i + MIN_X, j);
         Set_Pixel(GRID);
      }
      if (t1 == i + 1)
      {
         if (Hide_t1 == 0)
         {
            if ((j - MIN_Y) % 3 == 0)
            {
               Point_SCR(i + MIN_X, j);
               Set_Pixel(WHITE);
            }
         }
      }
      if (t2 == i + 1)
      {
         if (Hide_t2 == 0)
         {
            if ((j - MIN_Y) % 3 == 0)
            {
               Point_SCR(i + MIN_X, j);
               Set_Pixel(WHITE);
            }
         }
      }
      if (Hide_v1 == 0)
      {
         if (v1 == j)
         {
            if (i % 5 == 0)
            {
               Point_SCR(i + MIN_X, j);
               Set_Pixel(WHITE);
            }
         }
      }
      if (Hide_v2 == 0)
      {
         if (v2 == j)
         {
            if (i % 5 == 0)
            {
               Point_SCR(i + MIN_X, j);
               Set_Pixel(WHITE);
            }
         }
      }
      if (Hide_vt == 0)
      {
         if (vt == j)
         {
            if (i % 2 == 0)
            {
               Point_SCR(i + MIN_X, j);
               Set_Pixel(YEL);
            }
         }
      }
   }
}

/*******************************************************************************
 Draw segment
*******************************************************************************/
void            Draw_SEG(unsigned short i, unsigned short y1, unsigned short y2, unsigned short Color)
{
   unsigned short  j, Tmp;

   Tmp = y2;
   if (y1 > y2)
   {
      y2 = y1;
      y1 = Tmp;
   }
   if (y2 >= MAX_Y)
      y2 = MAX_Y - 1;
   if (y1 <= MIN_Y)
      y1 = MIN_Y + 1;
   Point_SCR(i + MIN_X, y1);
   for (j = y1; j <= y2; ++j)
      Set_Pixel(Color);
}
/*******************************************************************************
 Erase Wave Curve
*******************************************************************************/
void            Erase_Wave(unsigned short Color)
{
   unsigned short  i;
   unsigned char   y1, y2;

   if (Color == CURVE)
      y1 = View_Buffer[0];
   else
      y1 = Ref_Buffer[0];

   for (i = 0; i < X_SIZE; ++i)
   {
      if (Color == CURVE)
         y2 = View_Buffer[i];
      else
         y2 = Ref_Buffer[i];
      Erase_SEG(i, y1, y2);
      y1 = y2;
   }
}
/*******************************************************************************
 Erase Vn/Vi/Ti vernier
*******************************************************************************/
void            Erase_Vn(unsigned short v)
{
   unsigned short  i;

   for (i = MIN_X; i < MAX_X; i += 2)
   {
      Point_SCR(i, v);
      if (((v - MIN_Y) % 25 == 0) || ((i - MIN_X) % 25 == 0))
      {
         Set_Pixel(GRID);
      } else
      {
         if (((v == ((MIN_Y + MAX_Y) / 2 + 1)) || (v == ((MIN_Y + MAX_Y) / 2 - 1))) && ((i - MIN_X) % 5 == 0))
         {
            Set_Pixel(GRID);
         } else
         {
            if (((i == ((MIN_X + MAX_X) / 2 + 1)) || (i == ((MIN_X + MAX_X) / 2 - 1))) && ((v - MIN_Y) % 5 == 0))
            {
               Set_Pixel(GRID);
            } else
            {
               if (((v == v1) || (v == v2)) && ((i - MIN_X) % 5 == 0))
               {
                  Set_Pixel(WHITE);
               } else
               {
                  if (((i == t1) || (i == t2)) && ((v - MIN_Y) % 3 == 0))
                     Set_Pixel(WHITE);
                  else
                     Set_Pixel(BLACK);
               }
            }
         }
      }
   }
}
void            Erase_Vi(unsigned short v)
{
   unsigned short  i;

   for (i = MIN_X; i < MAX_X; i += 5)
   {
      Point_SCR(i, v);
      if (((v - MIN_Y) % 25 == 0) || ((i - MIN_X) % 25 == 0))
      {
         Set_Pixel(GRID);
      } else
      {
         if (((v == ((MIN_Y + MAX_Y) / 2 + 1)) || (v == ((MIN_Y + MAX_Y) / 2 - 1))) && ((i - MIN_X) % 5 == 0))
         {
            Set_Pixel(GRID);
         } else
         {
            if (((i == ((MIN_X + MAX_X) / 2 + 1)) || (i == ((MIN_X + MAX_X) / 2 - 1))) && ((v - MIN_Y) % 5 == 0))
            {
               Set_Pixel(GRID);
            } else
            {
               if ((v == vt) && ((i - MIN_X) % 2 == 0))
               {
                  Set_Pixel(YEL);
               } else
               {
                  if (((i == t1) || (i == t2)) && ((v - MIN_Y) % 3 == 0))
                     Set_Pixel(WHITE);
                  else
                     Set_Pixel(BLACK);
               }
            }
         }
      }
   }
}
void            Erase_Ti(unsigned short t)
{
   unsigned short  j;

   if (t1 != t2)
   {
      for (j = MIN_Y + 3; j < MAX_Y; j += 3)
      {
         Point_SCR(t, j);
         if (((t - MIN_X) % 25 == 0) || ((j - MIN_Y) % 25 == 0))
         {
            Set_Pixel(GRID);
         } else
         {
            if (((t == ((MIN_X + MAX_X) / 2 + 1)) || (t == ((MIN_X + MAX_X) / 2 - 1))) && ((j - MIN_Y) % 5 == 0))
            {
               Set_Pixel(GRID);
            } else
            {
               if (((j == ((MIN_Y + MAX_Y) / 2 + 1)) || (j == ((MIN_Y + MAX_Y) / 2 - 1))) && ((t - MIN_X) % 5 == 0))
               {
                  Set_Pixel(GRID);
               } else
               {
                  if (((j == v1) || (j == v2)) && ((t - MIN_X) % 5 == 0))
                     Set_Pixel(WHITE);
                  else
                     Set_Pixel(BLACK);
               }
            }
         }
      }
   }
}
/*******************************************************************************
 Draw v0/vi/ti vernier
*******************************************************************************/
void            Draw_v0(void)
{
   unsigned short  i;

   for (i = MIN_X + 2; i < MAX_X; i += 2)
   {
      Point_SCR(i, v0);
      Set_Pixel(MODEL);
   }
}
void            Draw_vt(void)
{
   unsigned short  i;

   for (i = MIN_X + 2; i < MAX_X; i += 2)
   {
      Point_SCR(i, vt);
      Set_Pixel(YEL);
   }
}
void            Draw_vi(unsigned short vi)
{
   unsigned short  i;

   for (i = MIN_X + 5; i < MAX_X; i += 5)
   {
      Point_SCR(i, vi);
      Set_Pixel(LINE);
   }
}
void            Draw_ti(unsigned short ti)
{
   unsigned short  j;

   for (j = MIN_Y + 3; j < MAX_Y; j += 3)
   {
      Point_SCR(ti, j);
      Set_Pixel(LINE);
   }
}
/*******************************************************************************
 Erase  v0/vi/ti end point
*******************************************************************************/
void            Erase_Dot_v0(void)
{
   Point_SCR(MIN_X - 1, v0 - 2);
   Set_Pixel(BLACK);
   Point_SCR(MIN_X - 1, v0 + 2);
   Set_Pixel(BLACK);
   Point_SCR(MAX_X + 1, v0 - 2);
   Set_Pixel(BLACK);
   Point_SCR(MAX_X + 1, v0 + 2);
   Set_Pixel(BLACK);

   Point_SCR(MIN_X, v0 - 1);
   Set_Pixel(GRID);
   Point_SCR(MIN_X, v0 + 1);
   Set_Pixel(GRID);
   Point_SCR(MAX_X, v0 - 1);
   Set_Pixel(GRID);
   Point_SCR(MAX_X, v0 + 1);
   Set_Pixel(GRID);

   Point_SCR(MIN_X + 1, v0);
   if (((v0 - MIN_Y) % 25 == 0) || ((v0 - MIN_Y) % 5 == 0))
      Set_Pixel(GRID);
   else
      Set_Pixel(BLACK);
   Point_SCR(MAX_X - 1, v0);
   if (((v0 - MIN_Y) % 25 == 0) || ((v0 - MIN_Y) % 5 == 0))
      Set_Pixel(GRID);
   else
      Set_Pixel(BLACK);
}
void            Erase_Dot_vt(void)
{
   Point_SCR(MIN_X - 1, vt - 2);
   Set_Pixel(BLACK);
   Point_SCR(MIN_X - 1, vt + 2);
   Set_Pixel(BLACK);
   Point_SCR(MAX_X + 1, vt - 2);
   Set_Pixel(BLACK);
   Point_SCR(MAX_X + 1, vt + 2);
   Set_Pixel(BLACK);

   Point_SCR(MIN_X, vt - 1);
   Set_Pixel(GRID);
   Point_SCR(MIN_X, vt + 1);
   Set_Pixel(GRID);
   Point_SCR(MAX_X, vt - 1);
   Set_Pixel(GRID);
   Point_SCR(MAX_X, vt + 1);
   Set_Pixel(GRID);

   Point_SCR(MIN_X + 1, vt);
   if (((vt - MIN_Y) % 25 == 0) || ((vt - MIN_Y) % 5 == 0))
      Set_Pixel(GRID);
   else
      Set_Pixel(BLACK);
   Point_SCR(MAX_X - 1, vt);
   if (((vt - MIN_Y) % 25 == 0) || ((vt - MIN_Y) % 5 == 0))
      Set_Pixel(GRID);
   else
      Set_Pixel(BLACK);
}
void            Erase_Dot_vi(unsigned short vi)
{
   Point_SCR(MIN_X - 1, vi - 2);
   Set_Pixel(BLACK);
   Point_SCR(MIN_X - 1, vi + 2);
   Set_Pixel(BLACK);
   Point_SCR(MAX_X + 1, vi - 2);
   Set_Pixel(BLACK);
   Point_SCR(MAX_X + 1, vi + 2);
   Set_Pixel(BLACK);

   Point_SCR(MIN_X, vi - 1);
   Set_Pixel(GRID);
   Point_SCR(MIN_X, vi + 1);
   Set_Pixel(GRID);
   Point_SCR(MAX_X, vi - 1);
   Set_Pixel(GRID);
   Point_SCR(MAX_X, vi + 1);
   Set_Pixel(GRID);

   Point_SCR(MIN_X + 1, vi);
   if (((vi - MIN_Y) % 25 == 0) || ((vi - MIN_Y) % 5 == 0))
      Set_Pixel(GRID);
   else
      Set_Pixel(BLACK);
   Point_SCR(MAX_X - 1, vi);
   if (((vi - MIN_Y) % 25 == 0) || ((vi - MIN_Y) % 5 == 0))
      Set_Pixel(GRID);
   else
      Set_Pixel(BLACK);
}
void            Erase_Dot_ti(unsigned short ti)
{
   Point_SCR(ti - 2, MIN_Y - 1);
   Set_Pixel(BLACK);
   Point_SCR(ti + 2, MIN_Y - 1);
   Set_Pixel(BLACK);
   Point_SCR(ti - 2, MAX_Y + 1);
   Set_Pixel(BLACK);
   Point_SCR(ti + 2, MAX_Y + 1);
   Set_Pixel(BLACK);

   Point_SCR(ti - 1, MIN_Y);
   Set_Pixel(GRID);
   Point_SCR(ti + 1, MIN_Y);
   Set_Pixel(GRID);
   Point_SCR(ti - 1, MAX_Y);
   Set_Pixel(GRID);
   Point_SCR(ti + 1, MAX_Y);
   Set_Pixel(GRID);

   Point_SCR(ti, MIN_Y + 1);
   if (((ti - MIN_X) % 25 == 0) || ((ti - MIN_X) % 5 == 0))
      Set_Pixel(GRID);
   else
      Set_Pixel(BLACK);
   Point_SCR(ti, MAX_Y - 1);
   if (((ti - MIN_X) % 25 == 0) || ((ti - MIN_X) % 5 == 0))
      Set_Pixel(GRID);
   else
      Set_Pixel(BLACK);
}
/*******************************************************************************
 Draw v0/vt/vi/ti end point
*******************************************************************************/
void            Draw_Dot_v0(void)
{
   unsigned short  i, j;

   for (i = (MIN_X - 1); i < (MIN_X + 2); ++i)
   {
      Point_SCR(i, v0 - (1 + MIN_X - i));
      for (j = (v0 - (1 + MIN_X - i)); j < (v0 + 2 + MIN_X - i); ++j)
         Set_Pixel(CURVE);
   }
   for (i = (MAX_X - 1); i < (MAX_X + 2); ++i)
   {
      Point_SCR(i, v0 - (i + 1 - MAX_X));
      for (j = (v0 - (i - MAX_X) - 2); j < (v0 + (i - MAX_X + 1)); ++j)
         Set_Pixel(CURVE);
   }
}
void            Draw_Dot_vt(void)
{
   unsigned short  i, j;

   for (i = (MIN_X - 1); i < (MIN_X + 2); ++i)
   {
      Point_SCR(i, vt - (1 + MIN_X - i));
      for (j = (vt - (1 + MIN_X - i)); j < (vt + 2 + MIN_X - i); ++j)
         Set_Pixel(YEL);
   }
   for (i = (MAX_X - 1); i < (MAX_X + 2); ++i)
   {
      Point_SCR(i, vt - (i + 1 - MAX_X));
      for (j = (vt - (i - MAX_X) - 2); j < (vt + (i - MAX_X + 1)); ++j)
         Set_Pixel(YEL);
   }
}
void            Draw_Dot_vi(unsigned short vi)
{
   unsigned short  i, j;

   for (i = (MIN_X - 1); i < (MIN_X + 2); ++i)
   {
      Point_SCR(i, vi - (1 + MIN_X - i));
      for (j = vi - (1 + MIN_X - i); j < (vi + (MIN_X - i + 2)); ++j)
         Set_Pixel(LINE);
   }
   for (i = (MAX_X - 1); i < (MAX_X + 2); ++i)
   {
      Point_SCR(i, vi - (i + 1 - MAX_X));
      for (j = (vi - (i - MAX_X) - 2); j < (vi + (i - MAX_X + 1)); ++j)
         Set_Pixel(LINE);
   }
}
void            Draw_Dot_ti(unsigned short ti)
{
   unsigned short  i, j;

   for (j = (MIN_Y - 1); j < (MIN_Y + 2); ++j)
   {
      for (i = (ti - (MIN_Y - j) - 1); i < (ti + 2 + MIN_Y - j); ++i)
      {
         Point_SCR(i, j);
         Set_Pixel(LINE);
      }
   }
   for (j = (MAX_Y - 1); j < (MAX_Y + 2); ++j)
   {
      for (i = (ti - (j - MAX_Y) - 1); i < (ti + 2 + j - MAX_Y); ++i)
      {
         Point_SCR(i, j);
         Set_Pixel(LINE);
      }
   }
}
/*******************************************************************************
 Erase View Area indicator
*******************************************************************************/
void            Erase_View_Area(void)
{
   unsigned short  i;

   for (i = MIN_X + 2; i < MAX_X - 1; ++i)
   {
      Point_SCR(i, MIN_Y + 2);
      if ((i - MIN_X) % 25 == 0)
      {
         Set_Pixel(GRID);
         Set_Pixel(GRID);
         Set_Pixel(GRID);
         Set_Pixel(GRID);
      } else
      {
         if (((i - MIN_X) > 147) && ((i - MIN_X) < 153))
         {
            Set_Pixel(BLACK);
            Set_Pixel(BLACK);
            Set_Pixel(BLACK);
            Set_Pixel(GRID);
         } else
         {
            Set_Pixel(BLACK);
            Set_Pixel(BLACK);
            Set_Pixel(BLACK);
            Set_Pixel(BLACK);
         }
      }
   }
}
/*******************************************************************************
 Draw View Area indicator
*******************************************************************************/
void            Draw_View_Area(void)
{
   unsigned short  i, j;

   j = MIN_X + (((t0 + Item_Index[X_POSITION] - 4096) * X_SIZE) >> 12);
   for (i = MIN_X + 2; i < MAX_X - 1; ++i)
   {
      Point_SCR(i, MIN_Y + 2);
      if ((i > j) && (i < j + 22))
      {
         Set_Pixel(LINE);
         Set_Pixel(LINE);
         Set_Pixel(LINE);
         Set_Pixel(LINE);
      } else
      {
         Set_Pixel(BLACK);
         Set_Pixel(YEL);
         Set_Pixel(YEL);
         Set_Pixel(BLACK);
      }
   }
}
/*******************************************************************************
 Display_Str: Display String   Input: X¡¢Y, Color, Mode, String  
*******************************************************************************/
void            Display_Str(unsigned short x0, unsigned short y0, unsigned short Color, unsigned char Mode, unsigned const char *s)
{
   signed short    i, j, b;

   Point_SCR(x0, y0);
   for (j = 0; j < 14; ++j)
   {
      if (Mode == 0)
         Set_Pixel(BLACK); // Normal replace Display

      if (Mode == 1)
         Set_Pixel(Color); // Inverse replace Display

   }
   x0++;
   while (*s != 0)
   {
      unsigned const short *scanline = Char_Dot + ((*s - 0x22) * 8);

      for (i = 0; i < 8; ++i)
      {
         Point_SCR(x0 + i, y0);
         if ((*s == 0x20) || (*s == 0x21))
            b = 0x0000;
         else
            b = scanline[i];
         if ((*s == 0x21) && (i == 4))
            break;
         for (j = 0; j < 14; ++j)
         {
            if (b & 4)
            {
               if (Mode == 0)
                  Set_Pixel(Color);
               if (Mode == 1)
                  Set_Pixel(BLACK);
               if (Mode == 2)
                  Set_Pixel(Color);
               if (Mode == 3)
                  Set_Pixel(BLACK);
            } else
            {
               if (Mode == 0)
                  Set_Pixel(BLACK);
               if (Mode == 1)
                  Set_Pixel(Color);
               if (Mode == 2)
                  Set_Pixel(BLACK);
               if (Mode == 3)
                  Set_Pixel(Color);
            }
            b >>= 1;
         }
      }
      if (*s == 0x21)
         x0 += 4;
      else
         x0 += 8;
      ++s; // next character

   }
}
/*******************************************************************************
 Display_Logo: Display Logo   Input: X, Y 
*******************************************************************************/
void            Display_Logo(unsigned short Dot_x, unsigned short Dot_y)
{
   unsigned short  i, j, k, Color;
   int             b;

   for (i = 0; i < 256; ++i)
   {
      Point_SCR(Dot_x + i, Dot_y);
      for (j = 0; j < 2; ++j)
      {
         b = Logo_Dot[(i * 2) + j];
         for (k = 0; k < 32; ++k)
         {
            if (b & 0x80000000)
            {
               Color = 0x07E0; // Green?

            } else
               Color = 0;
            Set_Pixel(Color);
            b <<= 1;
         }
      }
   }

   for (i = 3; i < 317; ++i)
   {
      Point_SCR(i, 3);
      Set_Pixel(0xFFFF);
      Point_SCR(i, 236);
      Set_Pixel(0xFFFF);
   }

   Point_SCR(0, 0);
   for (j = 0; j < 240; ++j)
      Set_Pixel(0xFFFF);
   Point_SCR(3, 3);
   for (j = 3; j < 237; ++j)
      Set_Pixel(0xFFFF);
   Point_SCR(316, 3);
   for (j = 3; j < 237; ++j)
      Set_Pixel(0xFFFF);
   Point_SCR(319, 0);
   for (j = 0; j < 240; ++j)
      Set_Pixel(0xFFFF);

}

/******************************** END OF FILE *********************************/
