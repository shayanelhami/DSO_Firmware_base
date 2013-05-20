/******************** (C) COPYRIGHT 2009 e-Design Co.,Ltd. *********************
* File Name: Funtion.h   Author: bure   Hardware:  DS0201V1.1   Version: Ver 1.0
*******************************************************************************/
#ifndef __Funtion_h
#define __Funtion_h

#define KEYCODE_VOID    0
#define KEYCODE_PLAY    1
#define KEYCODE_MANU    2
#define KEYCODE_UP      3
#define KEYCODE_DOWN    4
#define KEYCODE_LEFT    5
#define KEYCODE_RIGHT   6

#define X_SIZE    300
#define Y_SIZE    200
#define MIN_X     1
#define MIN_Y     19
#define MAX_X     (X_SIZE + MIN_X)
#define MAX_Y     (Y_SIZE + MIN_Y)

#define SYNC_MODE         0
#define Y_SENSITIVITY     1
#define X_SENSITIVITY     2
#define Y_POSITION        3
#define MEASUR_KIND       4
#define POWER_INFOMATION  5
#define TRIG_SENSITIVITY  6
#define TRIG_SLOPE        7
#define INPUT_ATTENUATOR  8
#define SAVE_WAVE_CURVE   9
#define LOAD_WAVE_CURVE   10
#define OUTPUT_FREQUENCY  11
#define X_VERNIER_2       12
#define X_VERNIER_1       13
#define X_POSITION        14
#define RUNNING_STATUS    15
#define DELTA_T           16
#define Y_VERNIER_2       17
#define Y_VERNIER_1       18
#define TRIG_LEVEL        19
#define VERNIERS          20
#define WINDOW_AREA       21

#define RUN                     0
#define HOLD                    1
#define RISING                  0


extern char                 Test[2]; 
extern unsigned short       Scan_Buffer[0x1000]; 
extern unsigned char        View_Buffer[300], Ref_Buffer[304];
extern unsigned char        Signal_Buffer[300], Key_Buffer;
extern unsigned const char  Item_V[20][11], Item_T[22][12];
extern unsigned const int   V_Scale[20],  T_Scale[22], Fout_ARR[16];
extern unsigned const char  V_Unit[4][3], T_Unit[4][3];
extern unsigned const short Scan_PSC[22], Scan_ARR[22];

extern unsigned short Delay_Counter, X_Counter;  
extern unsigned char  Counter_20mS, Key_Repeat_Counter, Key_Wait_Counter; 
extern unsigned char  Cursor_Counter,Erase_Note, Frame, Stop, Type, Sync;

extern unsigned int   vb_Sum, Battery;
extern unsigned short temp, Lcd_X, Lcd_Y;

extern unsigned char  Update[22];
extern unsigned char  SectorBuff[512];
extern unsigned char  FileNum[4];

extern unsigned short Item_Index[20];
extern unsigned short vt, v0, v1, v2, t0, t1, t2;    
extern unsigned char  Hide_vs, Hide_vt, Hide_v0, Hide_v1, Hide_v2;
extern unsigned char  Hide_t1, Hide_t2, Hide_Ref, Item;
extern           int  Frequency, Cycle, Duty;//, Vram, Vavg, Vpp, DCV;

void Delayms(unsigned short mS);
void Scan_Wave(void);
void Update_Item(void);
void Sync_Trig(void);
void Signal_Process(void);
void Erase_Draw(void);
void Measure_Wave(void);
void Battery_Detect(void);
void Test_Display(int Var);

#endif
/******************************** END OF FILE *********************************/
