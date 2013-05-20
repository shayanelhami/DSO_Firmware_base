/******************** (C) COPYRIGHT 2009 e-Design Co.,Ltd. *********************
* File Name: lcd.h      Author: bure      Hardware: DS0201V1.1     Version: V1.0
*******************************************************************************/
#ifndef __LCD_H
#define __LCD_H

//============================== ��ʾ��ɫ���� ==================================

#define WHITE                   0xFFFF	//��ɫ��B = F800, G = 07E0, R = 001F
#define PANEL                   0xFFE0	//��ɫ��B = F800, G = 07E0, R = 0000
#define RED                     0x001F	//��ɫ��B = 0000, G = 0000, R = 001F
#define GRN                     0x07E0	//��ɫ��B = 0000, G = 07E0, R = 0000
#define YEL                     0x07FF	//��ɫ��B = 0000, G = 07E0, R = 001F
#define GRID                    0x738E	//��ɫ��B = 7000, G = 0380, R = 000E
#define CURVE                   0x0F8E	//���̣�B = 0001, G = 0780, R = 000E 
#define MODEL                   0xC05E	//��ɫ��B = C000, G = 0040, R = 001E
#define LINE                    0xE79F	//��ɫ��B = E000, G = 0780, R = 001F
//#define NOTE                    0x073E	//���ƣ�B = 0000, G = 0720, R = 001F
#define BLACK                   0x0000	//��ɫ��B = 0000, G = 0000, R = 0000

//================================= �������� ===================================

#define PRN		0
#define INV		1

//========================== LCDģ����ʾ��غ������� ===========================

void LCD_WR_REG(unsigned short Reg, unsigned short Data);
void LCD_Initial(void); 
void Point_SCR(unsigned short x0, unsigned short y0);
void Set_Pixel(unsigned short Color);
void Clear_Screen(unsigned short Color);
void Display_Frame(void);						
void Display_Grid(void); 


void Display_Str(unsigned short xi, unsigned short yi, unsigned short Color, unsigned char Mode, unsigned const char *s);
void Display_Logo(unsigned short Dot_x, unsigned short Dot_y);
void Erase_Wave(unsigned short Color);
void Erase_SEG(unsigned short i, unsigned short y1, unsigned short y2);
void Draw_SEG(unsigned short i, unsigned short y1, unsigned short y2, unsigned short Color);						
void Draw_View_Area(void);
void Erase_View_Area(void);

void Erase_Vn(unsigned short v);
void Erase_Vi(unsigned short v);
void Erase_Ti(unsigned short t);
void Draw_v0(void);
void Draw_vt(void);
void Draw_vi(unsigned short vi);
void Draw_ti(unsigned short ti);
void Erase_Dot_v0(void);
void Erase_Dot_vt(void);
void Erase_Dot_vi(unsigned short vi);
void Erase_Dot_ti(unsigned short ti);
void Draw_Dot_v0(void);
void Draw_Dot_vt(void);
void Draw_Dot_vi(unsigned short vi);
void Draw_Dot_ti(unsigned short ti);

#endif

/********************************* END OF FILE ********************************/
