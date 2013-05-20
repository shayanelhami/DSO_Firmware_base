/******************** (C) COPYRIGHT 2009 e-Design Co.,Ltd. *********************
* File Name: Function.c    Author: bure    Hardware: DS0201V1.1   Version: V1.0
*******************************************************************************/

#include "Function.h"
#include "Lcd.h"
#include "Calculate.h"
#include "stm32f10x_lib.h"
#include "HW_V1_Config.h"

//------------Vertical Range parameters------------

unsigned const char Item_V[20][11] = // Vertical Range String
// 0，        1，        2，        3，        4，       5，        6，
//  7，         8，        9,   探头衰减×1 时  
 {"10mV/Div", "20mV/Div", "50mV/Div", "0.1V/Div", "0.2V/Div", "0.5V/Div", " 1V/Div ", " 2V/Div ", " 5V/Div ", " 10V/Div",
//    10，       11，      12，       13，       14，      15，       16，       17，        18，       19，  探头衰减×10时    
"0.2V/Div", "0.5V/Div", " 1V/Div ", " 2V/Div ", " 5V/Div ", " 10V/Div", " 20V/Div", " 50V/Div", "100V/Div", " -GND-  "};

unsigned const int V_Scale[20] = // Vertical Range parameters
// 0，  1，   2，   3，   4，    5，    6，    7，     8，     9,
// Used x1 probe  
 {400, 800, 2000, 4000, 8000, 20000, 40000, 80000, 200000, 400000,
// 10，   11，   12，   13，    14，    15，    16，     17，   18， 19，  Used x10 probe    
8000, 20000, 40000, 80000, 200000, 400000, 800000, 2000000, 4000000, 0};

unsigned short  Km[20] =        // Vertical Range parameter K = Km/4069
// 0,    1,   2,   3,    4,   5,   6,    7,   8,   9,   10,  11,  12,   13,
// 14,  15,   16,  17,  18, 19
 {2956, 1478, 591, 296, 1114, 446, 223, 1157, 463, 231, 1452, 581, 290, 1082, 433, 216, 1048, 419, 210, 231};

//------------Horizontal Range parameters------------      

unsigned const char Item_T[22][12] = // Horizontal Range String
// 0           1           2            3           4           5           6 
// 7           8          9           10
 {" 1uS/Div ", " 2uS/Div ", " 5uS/Div ", " 10uS/Div", " 20uS/Div", " 50uS/Div", "100uS/Div", "200uS/Div", "500uS/Div", " 1mS/Div ", " 2mS/Div ",
//    11          12          13           14          15          16          17         18          19          20          21
" 5mS/Div ", " 10mS/Div", " 20mS/Div", " 50mS/Div", " 0.1S/Div", " 0.2S/Div", " 0.5S/Div", "  1S/Div ", "  2S/Div ", "  5S/Div ", " 10S/Div "};

unsigned const int T_Scale[22] = // Horizontal Range parameters 
// 0           1           2            3           4           5           6 
// 7           8          9           10
 {1167, 1167, 1167, 1167, 1167, 2000, 4000, 8000, 20000, 40000, 80000,
//    11          12          13           14          15          16          17         18          19          20          21
200000, 400000, 800000, 2000000, 4000000, 8000000, 20000000, 40000000, 80000000, 200000000, 400000000};

unsigned const short Scan_PSC[22] = // Horizontal Range prescale
// 0           1           2            3           4           5           6 
// 7           8          9           10
 {11,                           /*
                                 * 40n
                                 */ 11,
                                /*
                                 * 80n
                                 */ 11,
                                /*
                                 * 200n
                                 */ 11,
                                /*
                                 * 400n
                                 */ 11,
                                /*
                                 * 800n
                                 */ 15,
                                /*
                                 * 2u
                                 */ 15,
                                /*
                                 * 4u
                                 */ 15,
                                /*
                                 * 8u
                                 */ 15,
                                /*
                                 * 20u
                                 */ 15,
                                /*
                                 * 40u
                                 */ 15,
                                /*
                                 * 80u
                                 */
//    11          12          13           14          15          16          17         18          19          20          21
   511,                         /*
                                 * 200u
                                 */ 511,
                                /*
                                 * 400u
                                 */ 511,
                                /*
                                 * 800u
                                 */ 511,
                                /*
                                 * 2m
                                 */ 511,
                                /*
                                 * 4m
                                 */ 511,
                                /*
                                 * 8m
                                 */ 255,
                                /*
                                 * 20m
                                 */ 511,
                                /*
                                 * 40m
                                 */ 1023,
                                /*
                                 * 80m
                                 */ 511,
                                /*
                                 * 0.2S
                                 */ 511,
                                /*
                                 * 0.4S
                                 */ };

unsigned const short Scan_ARR[22] = // Horizontal Range period
// 0           1           2            3           4           5           6 
// 7           8          9           10
 {6,                            /*
                                 * 40n
                                 */ 6,
                                /*
                                 * 80n
                                 */ 6,
                                /*
                                 * 200n
                                 */ 6,
                                /*
                                 * 400n
                                 */ 6,
                                /*
                                 * 800n
                                 */ 8,
                                /*
                                 * 2u
                                 */ 17,
                                /*
                                 * 4u
                                 */ 35,
                                /*
                                 * 8u
                                 */ 89,
                                /*
                                 * 20u
                                 */ 179,
                                /*
                                 * 40u
                                 */ 359,
                                /*
                                 * 80u
                                 */
//    11          12          13           14          15          16          17         18          19          20          21
   1124,                        /*
                                 * 200u
                                 */ 1124,
                                /*
                                 * 400u
                                 */ 1124,
                                /*
                                 * 800u
                                 */ 1124,
                                /*
                                 * 2m
                                 */ 1124,
                                /*
                                 * 4m
                                 */ 1124,
                                /*
                                 * 8m
                                 */ 5624,
                                /*
                                 * 20m
                                 */ 5624,
                                /*
                                 * 40m
                                 */ 5624,
                                /*
                                 * 80m
                                 */ 28124,
                                /*
                                 * .2
                                 */ 28124,
                                /*
                                 * .4
                                 */ };

unsigned const short Ks[22] =   // Horizontal Range coefficient
// 0     1     2    3    4    5    6    7    8    9    10   11   12   13   14 
// 15   16   17   18   19   20   21 
 {29860, 14930, 5972, 2986, 1493, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024};

//------------Output Signal parameters------------      

unsigned const char Item_F[16][7] = // Output Signal frequency string
// 0       1        2        3         4        5        6        7       8
//       9       10       11       12       13       14       15  
 {" 1MHz ", "500KHz", "200KHz", "100KHz", " 50KHz", " 20KHz", " 10KHz", " 5KHZ ", " 2KHz ", " 1KHz ", " 500Hz", " 200Hz", " 100Hz", " 50Hz ", " 20 Hz", " 10Hz "};

unsigned const int Fout_PSC[16] = // Output Signal prescale
// 0       1        2        3         4        5        6        7       8
//       9       10       11       12       13       14       15  
 {0, 0, 0, 0, 0, 0, 0, 0, 4 - 1, 4 - 1, 16 - 1, 16 - 1, 64 - 1, 64 - 1, 128 - 1, 128 - 1};

unsigned const int Fout_ARR[16] = // Output Signal period 
// 0       1        2        3         4        5        6        7       8
//       9       10       11       12       13       14       15  
 {72 - 1, 144 - 1, 360 - 1, 720 - 1, 1440 - 1, 3600 - 1, 7200 - 1, 14400 - 1, 9000 - 1, 18000 - 1, 9000 - 1, 22500 - 1, 11250 - 1, 22500 - 1, 28125 - 1, 56250 - 1};

//-----------------------------------------------------------------------------

unsigned short  Scan_Buffer[0x1000];
unsigned char   Signal_Buffer[300];
unsigned char   View_Buffer[300];
unsigned char   Ref_Buffer[304] =
{100, 116, 130, 144, 157, 167, 175, 181, 185, 185, 184, 179, 173, 164, 153, 141, 128, 114, 100, 86, 73, 60, 49, 40, 33, 27, 24, 24, 25, 29,
   35, 43, 52, 63, 75, 87, 100, 112, 124, 135, 145, 153, 160, 164, 167, 167, 166, 163, 157, 150, 142, 133, 122, 111, 100, 89, 79, 70, 61, 54,
   48, 44, 42, 42, 43, 46, 50, 57, 64, 72, 81, 90, 100, 109, 118, 126, 133, 139, 144, 147, 149, 149, 148, 146, 142, 137, 130, 124, 116, 108,
   100, 93, 85, 79, 73, 68, 64, 61, 60, 60, 61, 63, 66, 70, 75, 81, 87, 93, 100, 106, 112, 117, 122, 125, 128, 130, 131, 131, 131, 129,
   126, 123, 119, 115, 110, 105, 100, 96, 91, 88, 84, 81, 79, 78, 77, 78, 78, 80, 82, 84, 87, 90, 93, 97, 100, 103, 106, 108, 110, 112,
   113, 114, 114, 113, 113, 112, 110, 109, 107, 105, 104, 102, 100, 99, 97, 96, 96, 95, 95, 95, 95, 96, 96, 97, 97, 98, 99, 99, 100, 100,
   100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
   100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 115, 129, 141, 151, 160, 165, 169, 170, 169, 167, 162, 156, 148, 139, 130,
   120, 110, 100, 91, 82, 75, 68, 63, 59, 57, 56, 56, 58, 61, 64, 69, 75, 81, 87, 94, 100, 106, 112, 117, 121, 125, 127, 129, 130, 130,
   128, 127, 124, 121, 117, 113, 109, 104, 100, 96, 92, 88, 85, 83, 81, 80, 79, 79, 80, 81, 83, 85, 88, 91, 94, 97, 100, 103, 106, 108,
108, 108, 6, 8};

//------------------------------------------Power ON Difinition------------------------------------------------
unsigned short  Item_Index[20] = {0, 6, 8, 0, 0, 4, 8, 0, 0, 1, 1, 9, 0, 0, 4096, 0, 0, 0, 0, 0};
unsigned short  vt = 140, v0 = 69, v1 = 199, v2 = 40, t0 = 150, t1 = 68, t2 = 233;
unsigned char   Hide_vs = 1, Hide_vt = 0, Hide_v0 = 1, Hide_v1 = 0, Hide_v2 = 0;
unsigned char   Hide_t1 = 0, Hide_t2 = 0, Hide_Ref = 0, Item = 4;

//------------------------------------------------------------------------------------------------------------
int             Frequency, Cycle, Duty;

unsigned char   Update[22] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0};
unsigned const char V_Unit[4][3] = {"uV", "mV", "V ", "kV"};
unsigned const char T_Unit[4][3] = {"nS", "uS", "mS", "S "};
unsigned const char F_Unit[4][4] = {"Hz", "Hz", "KC", "MC"};
unsigned const char Battery_Status[5][4] = {"~`'", "~`}", "~|}", "{|}", "USB"};
unsigned const short Battery_Color[5] = {RED, YEL, GRN, GRN, GRN};
unsigned const char MODE_Unit[4][5] = {"AUTO", "NORM", "SIGN", "NONE"};
unsigned const char TEST_Unit[3][8] = {" FREQN ", " CYCLE ", "  DUTY "};
unsigned char   FileNum[4] = "000";

unsigned short  Delay_Counter = 0, X_Counter = 0, First_Edge, Last_Edge;
unsigned short  temp = 0;
unsigned char   Counter_20mS = 0, Key_Repeat_Counter = 0, Key_Wait_Counter = 0,
                Cursor_Counter = 0, Type;
unsigned char   Key_Buffer = 0, Toggle = 0, Sync = 0, Erase_Note = 0, Frame = 0,
                Stop = 0;
unsigned int    vb_Sum = 3600 * 64, Battery;

I32STR_RES      Num;

/*******************************************************************************
 delayms: delay   Input: Number of millisecond
*******************************************************************************/
void            Delayms(u16 mS)
{
   Delay_Counter = mS;
   while (Delay_Counter)
   {
   };
}

/*******************************************************************************
 Sync_Trig: Trigger & Synchronization 
*******************************************************************************/
void            Sync_Trig(void)
{
   int             Vs;

   for (t0 = 150; t0 < (0x1000 - X_SIZE); ++t0)
   {
      if (t0 >= (0x1000 - DMA_CNDTR1))
         break;
      Vs = 120 + (Km[Item_Index[Y_SENSITIVITY]] * (2048 - Scan_Buffer[t0])) / 4096;
      if (Item_Index[TRIG_SLOPE] == 0)
      { // rising edge   

         if (Vs < (vt - Item_Index[TRIG_SENSITIVITY]) && (Sync == 0))
            Sync = 1;
         if (Vs > (vt + Item_Index[TRIG_SENSITIVITY]) && (Sync == 1))
            Sync = 2;
      } else
      { // falling edge 

         if (Vs > (vt + Item_Index[TRIG_SENSITIVITY]) && (Sync == 0))
            Sync = 1;
         if (Vs < (vt - Item_Index[TRIG_SENSITIVITY]) && (Sync == 1))
            Sync = 2;
      }
      if (Sync == 2)
         break;
   }
   X_Counter = 0;
   if (t0 >= (0x1000 - X_SIZE))
      Sync = 3; // Fail

}

/*******************************************************************************
 Signal_Process: Signal Process 
*******************************************************************************/
void            Signal_Process(void)
{
   int             i, p, q;
   int             Vs, Vr;

   if (Sync == 3)
      t0 = 150; // If SYNC Fail

   p = t0 + Item_Index[X_POSITION] - 4096;
   for (i = X_Counter; i < (X_SIZE); ++i)
   {
      Sync = 5;
      q = p + (i * 1024) / Ks[Item_Index[X_SENSITIVITY]];
      if (q < 0)
      {
         q = 0;
         Item_Index[X_POSITION]++;
      }
      if (q >= 0x1000)
      {
         q = 0x1000 - 1;
         Item_Index[X_POSITION]--;
      }
      if (q >= (0x1000 - DMA_CNDTR1))
         break;
      X_Counter = i + 1;
      Vr = Km[Item_Index[Y_SENSITIVITY]] * (Scan_Buffer[q + 1] - Scan_Buffer[q]) / 4096;
      Vs = (Km[Item_Index[Y_SENSITIVITY]] * (2048 - Scan_Buffer[q])) / 4096 + 120
         - (((i * 1024) % Ks[Item_Index[X_SENSITIVITY]]) * Vr) / Ks[Item_Index[X_SENSITIVITY]];
      if (Vs > MAX_Y)
         Vs = MAX_Y;
      else if (Vs < MIN_Y)
         Vs = MIN_Y;
      Signal_Buffer[i] = Vs;
      Sync = 4; // Process finish

   }
   if (DMA_CNDTR1 == 0)
   {
      Measure_Wave();
      if (Item_Index[RUNNING_STATUS] == RUN)
         ADC_Start();
   }
}

/*******************************************************************************
 Erase_Draw: Erase and Draw the new curve    
*******************************************************************************/
void            Erase_Draw(void)
{
   unsigned short  i;
   unsigned char   y1, y2, y3, y4, y5, y6;

   y1 = View_Buffer[0];
   y3 = Signal_Buffer[0];
   y5 = Ref_Buffer[0];

   for (i = 0; i < X_Counter; ++i)
   {
      y2 = View_Buffer[i];
      Erase_SEG(i, y1, y2);
      y1 = y2;
      y4 = Signal_Buffer[i];
      Draw_SEG(i, y3, y4, CURVE);
      y3 = y4;
      View_Buffer[i] = y4;
      y6 = Ref_Buffer[i];
      if (Hide_Ref == 0)
         Draw_SEG(i, y5, y6, MODEL);
      y5 = y6;
   }

   if (X_Counter >= X_SIZE - 1)
   { // frame finish

      Stop = 1;
      X_Counter = 0;
      Battery_Detect();
      if ((((Frame + 2) * X_SIZE) + t0 + Item_Index[14]) < 0x1000)
         Frame++;
      else
      {
         Frame = 0;
         Sync = 0;
      }
      Delay_Counter = 100;
      if (Item_Index[0] != 4)
         Sync = 0; // Resume

      else
         Erase_Wave(CURVE);
   } else
   {
      Sync = 2;
      Stop = 0;
   }
}
/*******************************************************************************
 Scan_Wave: SYNC mode Process 
*******************************************************************************/

void            Scan_Wave(void) // SYNC mode: AUTO、NORM、SIGN、NONE 
 {

   // ---------------------AUTO--------------------------
   if (Item_Index[SYNC_MODE] == 0)
   {
      Frame = 0;
      if (Item_Index[RUNNING_STATUS] == RUN)
      {
         Stop = 0;
         if ((Sync == 0) || (Sync == 1))
            Sync_Trig();
      } else
      { // in 'HOLD' status

         Sync = 2;
         X_Counter = 0;
      }
      if ((Sync == 2) || (Sync == 3))
         Signal_Process();
      if ((Sync > 3) && (Delay_Counter == 0))
         Erase_Draw();
   }
//--------------------NORM-----------------------------
   if (Item_Index[SYNC_MODE] == 1)
   {
      Frame = 0;
      if (Item_Index[RUNNING_STATUS] == RUN)
      {
         Stop = 0;
         if ((Sync == 0) || (Sync == 1))
            Sync_Trig();
      } else
      { // in 'HOLD' status

         Sync = 2;
         X_Counter = 0;
      }
      if (Sync == 3)
         Erase_Wave(CURVE);
      if (Sync == 2)
         Signal_Process();
      if ((Sync > 3) && (Delay_Counter == 0))
         Erase_Draw();
   }
//--------------------SIGN--------------------------
   if (Item_Index[SYNC_MODE] == 2)
   {
      Frame = 0;
      if (Item_Index[RUNNING_STATUS] == RUN)
      { // in 'RUN' status

         Stop = 0;
         if ((Sync == 0) || (Sync == 1))
            Sync_Trig();
      } else
      { // in 'HOLD' status

         Sync = 2;
         X_Counter = 0;
      }
      if (Sync == 3)
         Erase_Wave(CURVE);
      if (Sync == 2)
         Signal_Process();
      if ((Sync > 3) && (Delay_Counter == 0))
         Erase_Draw();
      Item_Index[RUNNING_STATUS] = HOLD;
      Update[RUNNING_STATUS] = 1;
   }
//-----------------------NONE------------------------
   if (Item_Index[SYNC_MODE] == 3)
   {
      Frame = 0;
      X_Counter = 0;
      if (Item_Index[RUNNING_STATUS] == RUN)
      { // in 'RUN' status

         Sync = 3;
         Stop = 0;
      } else
      { // in 'HOLD' status

         Sync = 2;
      }
      if ((Sync == 2) || (Sync == 3))
         Signal_Process();

      if ((Sync > 3) && (Delay_Counter == 0))
         Erase_Draw();
   }
}
/*******************************************************************************
 Measure_Wave: Measure & Calculate frequenc, cycle, duty 
*******************************************************************************/
void            Measure_Wave(void)
{
   unsigned short  i = 0, Edge = 0, Trig = 0;
   unsigned int    Threshold1, Threshold2, Threshold3;
   int             Vm;

   First_Edge = 0;
   Last_Edge = 0;
   Threshold1 = 2048 - ((vt - Item_Index[TRIG_SENSITIVITY] - 120) * 4096)
      / Km[Item_Index[Y_SENSITIVITY]];
   Threshold2 = 2048 - ((vt + Item_Index[TRIG_SENSITIVITY] - 120) * 4096)
      / Km[Item_Index[Y_SENSITIVITY]];
   Threshold3 = 2048 - ((vt - 120) * 4096) / Km[Item_Index[Y_SENSITIVITY]];
   for (i = 0; i < 0x1000; ++i)
   {
      if ((Scan_Buffer[i] > Threshold1) && (Trig == 0))
         Trig = 1;
      if ((Scan_Buffer[i] < Threshold2) && (Trig == 1))
      {
         Trig = 0;
         if (First_Edge == 0)
         {
            First_Edge = i;
            Last_Edge = i;
            Edge = 0;
         } else
         {
            Last_Edge = i;
            Edge++;
         }
      }
   }
   if (Edge != 0)
   {
      Vm = 0;
      for (i = First_Edge; i < Last_Edge; ++i)
      {
         if (Scan_Buffer[i] < Threshold3)
            Vm++;
      }
      if (Type == PRN)
      {
         Frequency = (Edge * (1000000000 / T_Scale[Item_Index[X_SENSITIVITY]]) / (Last_Edge - First_Edge)) * 1000;
         Cycle = ((Last_Edge - First_Edge) * T_Scale[Item_Index[X_SENSITIVITY]]) / Edge;
         Duty = 100000 * Vm / (Last_Edge - First_Edge);
      }
   } else
   {
      Frequency = 0;
      Cycle = 0;
      Duty = 0;
   }
}
/*******************************************************************************
 Update_Item:  
*******************************************************************************/
void            Update_Item(void)
{
   unsigned short  Tmp;         // i, j, 

   if (Update[SYNC_MODE] != 0)
   {
      Update[SYNC_MODE] = Type;
      Display_Str(3, 224, GRN, Type, MODE_Unit[Item_Index[SYNC_MODE]]);
   }
   if (Update[Y_SENSITIVITY] != 0)
   {
      Update[Y_SENSITIVITY] = Type;
      Display_Str(40, 224, YEL, Type, Item_V[Item_Index[Y_SENSITIVITY]]);
      Update[TRIG_LEVEL] = 1;
      Update[Y_VERNIER_1] = 1;
      Set_Range(Item_Index[Y_SENSITIVITY]);
      Set_Y_Pos(v0);
   }
   if (Update[X_SENSITIVITY] != 0)
   {
      Update[X_SENSITIVITY] = Type;
      Display_Str(109, 224, YEL, Type, Item_T[Item_Index[X_SENSITIVITY]]);
      Display_Str(109, 224, YEL, Type, Item_T[Item_Index[X_SENSITIVITY]]);
      Set_Base(Item_Index[X_SENSITIVITY]);
      Update[DELTA_T] = 1;
   }
   if (Update[Y_POSITION] != 0)
   {
      Update[Y_POSITION] = Type;
      Display_Str(186, 224, MODEL, Type, "Y.POS");
      Draw_Dot_v0();
      if (Hide_v0)
         Erase_Vn(v0);
      else
         Draw_v0();
      Set_Y_Pos(v0);
      Update[VERNIERS] = 1;
   }
   if (Update[MEASUR_KIND] != 0)
   {
      Update[MEASUR_KIND] = Type;
      if (Type == 0)
         Display_Str(231, 224, WHITE, PRN, TEST_Unit[Item_Index[MEASUR_KIND]]);
      else
      {
         switch (Item_Index[4])
         {
         case 0: // Frequency

            if (Frequency == 0)
               Display_Str(231, 224, WHITE, INV, TEST_Unit[Item_Index[MEASUR_KIND]]);
            else
            {
               Int32String(&Num, Frequency, 4);
               Display_Str(231, 224, WHITE, Type, (unsigned const char *) Num.str);
               Display_Str(231 + 40, 224, WHITE, Type, F_Unit[Num.decPos]);
            }
            break;
         case 1: // Cycle

            if (Cycle == 0)
               Display_Str(231, 224, WHITE, INV, TEST_Unit[Item_Index[MEASUR_KIND]]);
            else
            {
               Int32String(&Num, Cycle, 4);
               Display_Str(231, 224, WHITE, Type, (unsigned const char *) Num.str);
               Display_Str(231 + 40, 224, WHITE, Type, T_Unit[Num.decPos]);
            }
            break;
         case 2: // Duty

            if (Duty == 0)
               Display_Str(231, 224, WHITE, INV, TEST_Unit[Item_Index[MEASUR_KIND]]);
            else
            {
               Int32String(&Num, Duty, 4);
               Display_Str(231, 224, WHITE, Type, (unsigned const char *) Num.str);
               Display_Str(231 + 40, 224, WHITE, Type, "% ");
            }
            break;
         }
      }
   }
   if (Update[POWER_INFOMATION] != 0)
   {
      Update[POWER_INFOMATION] = Type;
      if (Test_USB_ON())
         Display_Str(292, 224, Battery_Color[4], PRN, Battery_Status[4]);
      else
         Display_Str(292, 224, Battery_Color[Item_Index[5]], PRN,
                     Battery_Status[Item_Index[POWER_INFOMATION]]);
   }
   if (Update[TRIG_SENSITIVITY] != 0)
   {
      Update[TRIG_SENSITIVITY] = Type;
      Display_Str(303, 195, YEL, Type, "TR");
      if (Item == TRIG_SENSITIVITY)
      {
         Display_Str(88, 2, WHITE, Type, "Trig!Sensitive");
         temp = vt;
         for (vt = temp - 2; vt > (temp - Item_Index[6]); vt = vt - 2)
            Draw_vt();
         for (vt = (temp + 2); vt < (temp + Item_Index[6]); vt = vt + 2)
            Draw_vt();
         vt = temp;
      }
   }
   if (Update[TRIG_SLOPE] != 0)
   {
      Update[TRIG_SLOPE] = Type;
      if (Item_Index[TRIG_SLOPE] == RISING)
         Display_Str(303, 175, YEL, Type, "^S");
      else
         Display_Str(303, 175, YEL, Type, "_S");
      if (Item == TRIG_SLOPE)
      {
         if (Item_Index[TRIG_SLOPE] == 0)
            Display_Str(88, 2, WHITE, Type, "!Trig.Slope=!^!");
         else
            Display_Str(88, 2, WHITE, Type, "!Trig.Slope=!_!");
      }
   }
   if (Update[INPUT_ATTENUATOR] != 0)
   {
      Update[INPUT_ATTENUATOR] = Type;
      if (Item_Index[INPUT_ATTENUATOR] == 0)
         Display_Str(303, 155, YEL, Type, "*1");
      else
         Display_Str(303, 155, YEL, Type, "10");
      if (Item == INPUT_ATTENUATOR)
      {
         if (Item_Index[INPUT_ATTENUATOR] == 0)
            Display_Str(88, 2, WHITE, Type, "!INP.Scale!=*1!");
         else
            Display_Str(88, 2, WHITE, Type, "!INP.Scale=*10");
      }
      Update[Y_SENSITIVITY] = 1;
   }
   if (Update[SAVE_WAVE_CURVE] != 0)
   {
      Update[SAVE_WAVE_CURVE] = Type;
      Display_Str(303, 135, YEL, Type, "FS");
      if (Item == SAVE_WAVE_CURVE)
      {
         Char_to_Str(FileNum, Item_Index[SAVE_WAVE_CURVE]);
         Display_Str(88, 2, WHITE, Type, "!Save File");
         Display_Str(164, 2, WHITE, Type, FileNum);
         Display_Str(188, 2, WHITE, Type, " ");
      }
   }
   if (Update[LOAD_WAVE_CURVE] != 0)
   {
      Update[LOAD_WAVE_CURVE] = Type;
      Display_Str(303, 115, YEL, Type, "FL");
      if (Item == LOAD_WAVE_CURVE)
      {
         Char_to_Str(FileNum, Item_Index[LOAD_WAVE_CURVE]);
         Display_Str(88, 2, WHITE, Type, "!Load File");
         Display_Str(164, 2, WHITE, Type, FileNum);
         Display_Str(188, 2, WHITE, Type, " ");
      }
   }
   if (Update[OUTPUT_FREQUENCY] != 0)
   {
      Update[OUTPUT_FREQUENCY] = Type;
      TIM4_PSC = Fout_PSC[Item_Index[OUTPUT_FREQUENCY]];
      TIM4_ARR = Fout_ARR[Item_Index[OUTPUT_FREQUENCY]];
      TIM4_CCR1 = (Fout_ARR[Item_Index[OUTPUT_FREQUENCY]] + 1) / 2;
      Display_Str(303, 95, YEL, Type, "Fo");
      if (Item == 11)
      {
         Display_Str(88, 2, WHITE, Type, " Fout!=");
         Display_Str(140, 2, WHITE, Type, Item_F[Item_Index[OUTPUT_FREQUENCY]]);
         Display_Str(188, 2, WHITE, Type, " ");
      }
   }
   if (Update[X_VERNIER_2] != 0)
   {
      Update[X_VERNIER_2] = Type;
      Display_Str(303, 75, YEL, Type, "T2");
      if (Item == X_VERNIER_2)
      {
         Display_Str(88, 2, WHITE, Type, "!Time Cursor2 ");
         if (Hide_t1 == 0)
            Draw_ti(t1);
         Draw_Dot_ti(t1);
      }
   }
   if (Update[X_VERNIER_1] != 0)
   {
      Update[X_VERNIER_1] = Type;
      Display_Str(303, 55, YEL, Type, "T1");
      if (Item == X_VERNIER_1)
      {
         Display_Str(88, 2, WHITE, Type, "!Time Cursor1 ");
         if (Hide_t1 == 0)
            Draw_ti(t1);
         Draw_Dot_ti(t1);
      }
   }
   if (Update[X_POSITION] != 0)
   {
      Update[X_POSITION] = Type;
      Display_Str(303, 35, YEL, Type, "T0");
      if (Item == X_POSITION)
      {
         Display_Str(88, 2, WHITE, Type, "!X.POS Adjust ");
      }
   }
   if (Update[RUNNING_STATUS] != 0)
   {
      Update[RUNNING_STATUS] = 0;
      if (Item_Index[RUNNING_STATUS] == 0)
         Display_Str(284, 2, GRN, PRN, "!RUN!");
      else
         Display_Str(284, 2, RED, PRN, "HOLD");
   }
   if (Update[DELTA_T] != 0)
   {
      Update[DELTA_T] = 0;
      Int32String_sign(&Num, (t2 - t1) * T_Scale[Item_Index[X_SENSITIVITY]], 3); // 

      Display_Str(199, 2, YEL, PRN, "[T=");
      Display_Str(224, 2, YEL, PRN, (unsigned const char *) Num.str);
      Display_Str(224 + Num.len * 8, 2, YEL, PRN, " ");
      Display_Str(224 + 40, 2, YEL, PRN, T_Unit[Num.decPos]);
   }
   if (Update[Y_VERNIER_2] != 0)
   {
      Update[Y_VERNIER_2] = Type;
      if (Item != Y_VERNIER_2)
         Tmp = PRN;
      else
         Tmp = Type;
      if ((Item < 6) || (Item > 14))
      {
         Int32String_sign(&Num, (v1 - v2) * V_Scale[Item_Index[Y_SENSITIVITY]], 3); // 

         Display_Str(88, 2, WHITE, PRN, "V1");
         Display_Str(105, 2, WHITE, PRN, "-");
         Display_Str(114, 2, WHITE, Tmp, "V2");
         Display_Str(131, 2, WHITE, PRN, "=");
         Display_Str(139, 2, WHITE, PRN, (unsigned const char *) Num.str);
         Display_Str(139 + 40, 2, WHITE, PRN, V_Unit[Num.decPos]);
      }
   }
   if (Update[Y_VERNIER_1] != 0)
   {
      Update[Y_VERNIER_1] = Type;
      if (Item != Y_VERNIER_1)
         Tmp = PRN;
      else
         Tmp = Type;
      if ((Item < 6) || (Item > 14))
      {
         Int32String_sign(&Num, (v1 - v2) * V_Scale[Item_Index[Y_SENSITIVITY]], 3); // 

         Display_Str(88, 2, WHITE, Tmp, "V1");
         Display_Str(105, 2, WHITE, PRN, "-");
         Display_Str(139, 2, WHITE, PRN, (unsigned const char *) Num.str);
         Display_Str(139 + 40, 2, WHITE, PRN, V_Unit[Num.decPos]);
      }
   }
   if (Update[TRIG_LEVEL] != 0)
   {
      Update[TRIG_LEVEL] = Type;
      Update[VERNIERS] = 1;
      if (Item != TRIG_LEVEL)
         Tmp = PRN;
      else
         Tmp = Type;
      Int32String_sign(&Num, (vt - v0) * V_Scale[Item_Index[Y_SENSITIVITY]], 3); // 

      Display_Str(3, 2, YEL, Tmp, "Vt");
      Display_Str(20, 2, YEL, PRN, "=");
      Display_Str(28, 2, YEL, PRN, (unsigned const char *) Num.str);
      Display_Str(28 + 40, 2, YEL, PRN, V_Unit[Num.decPos]);
   }
   if (Hide_vs)
   {
      temp = vt;
      for (vt = temp - 2; vt >= (temp - Item_Index[TRIG_SENSITIVITY]); vt = vt - 2)
         Erase_Vn(vt);
      for (vt = (temp + 2); vt <= (temp + Item_Index[TRIG_SENSITIVITY]); vt = vt + 2)
         Erase_Vn(vt);
      vt = temp;
      Hide_vs = 0;
   }
   if (Update[VERNIERS])
   {
      Update[VERNIERS] = 0;
      if (Hide_t1)
         Erase_Ti(t1);
      else
         Draw_ti(t1);
      if (Hide_t2)
         Erase_Ti(t2);
      else
         Draw_ti(t2);
      if (Hide_v1)
         Erase_Vi(v1);
      else
         Draw_vi(v1);
      if (Hide_v2)
         Erase_Vi(v2);
      else
         Draw_vi(v2);
      if (Hide_vt)
         Erase_Vn(vt);
      else
         Draw_vt();
      Draw_Dot_vt();
      Draw_Dot_vi(v1);
      Draw_Dot_vi(v2);
      Draw_Dot_ti(t1);
      Draw_Dot_ti(t2);
   }
}
void            Test_Display(int Var)
{
   Int32String_sign(&Num, Var, 4);
   Display_Str(28, 60, YEL, PRN, (unsigned const char *) Num.str);
}
/******************************** END OF FILE *********************************/
