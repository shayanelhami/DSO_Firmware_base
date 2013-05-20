/******************** (C) COPYRIGHT 2009 e-Design Co.,Ltd. *********************
* File Name: main.c       Author: bure   Hardware: DS0201V1.1   Version: Ver 1.0
*******************************************************************************/

#include "Function.h"
#include "Lcd.h"
#include "Calculate.h"
#include "stm32f10x_lib.h"
#include "usb_lib.h"
#include "HW_V1_Config.h"
#include "msd.h"
#include "File.h"
#include "string.h"

int PositionY=0, PreviousPositionY = 0;
int PositionX = 0, PreviousPositionX = 0;
I32STR_RES numX, numY;
unsigned char Message[30] = "Pos:_____,_____" ;
unsigned char *ClearMessage = "                    " ;

void displayPoints(void);

void displayPoints(void)
{
  int i;

  Display_Str(PreviousPositionX, PreviousPositionY, GRN, PRN, ClearMessage);

  Int32String_sign(&numX, PositionX, 4);
  Int32String_sign(&numY, PositionY, 4);
  
  for(i=0;i<numX.len;i++)  Message[4+i] = numX.str[i];  
  for(i=0;i<numY.len;i++)  Message[10+i] = numY.str[i];
  
  Display_Str(PositionX, PositionY, GRN, PRN, Message);
  PreviousPositionX = PositionX;
  PreviousPositionY = PositionY;
}

void            main(void)
{
   //unsigned int    i;
/*--------------initialization-----------*/

   Set_System();
   NVIC_Configuration();
   GPIO_Config();
   MSD_Init();
   Get_Medium_Characteristics();
   USB_Init();
   DMA_Configuration();
   ADC_Configuration();
   Timer_Configuration();
   LCD_Initial();
/*
   Clear_Screen(BLACK);
   //Display_Logo(30, 150);

//----------Power ON Information----------

   Display_Str(8, 87, GRN, PRN, "         System Initialize...");
   Display_Str(8, 55, GRN, PRN, "            Please Wait");
   //Display_Str(8, 23, YEL, PRN, " DS0201 Oscilloscope Firmware Ver 1.1 ");

   Delay_Counter = 2000;
   while (Delay_Counter)
   {
      Battery_Detect();
      for (i = 0; i < 300; ++i)
         Signal_Buffer[i] = 120;
      for (i = 0; i < 0x1000; ++i)
         Scan_Buffer[i] = 1940;
   };
*/
/*--------Screen initialization--------*/

   Clear_Screen(BLACK);
   displayPoints();
   //Display_Grid();
   //Display_Frame();

   while(1) 
   {
     if (Key_Buffer !=0 )
     {
      if (Key_Buffer == KEYCODE_UP)
      {
        PositionY +=5;         
      }
      else if (Key_Buffer == KEYCODE_DOWN)
      {
        PositionY -=5;         
      }
      else if (Key_Buffer == KEYCODE_LEFT)
      {
        PositionX -=5;         
      }
      else if (Key_Buffer == KEYCODE_RIGHT)
      {
        PositionX +=5;         
      }

      displayPoints();
      Key_Buffer = 0;
     }
   }
/*------------Main Loop------------*/
   /*
   while (1)
   {
      Update_Item();
      Scan_Wave();
      if (Key_Buffer != 0)
      {
         Update[Item] = 1;
         if (Key_Buffer == KEYCODE_PLAY)
         {
            Update[15] = 1;
            if (Item_Index[RUNNING_STATUS] == RUN)
               Item_Index[RUNNING_STATUS] = HOLD;
            else
            {
               ADC_Start();
               Item_Index[RUNNING_STATUS] = RUN;
               Sync = 0;
            }
         }
         switch (Item)
         {
         case SYNC_MODE:
            if (Key_Buffer == KEYCODE_LEFT)
               Item = TRIG_LEVEL;
            if (Key_Buffer == KEYCODE_RIGHT)
               Item = Y_SENSITIVITY;
            if (Key_Buffer == KEYCODE_DOWN)
            {
               if (Item_Index[SYNC_MODE] > 0)
                  Item_Index[SYNC_MODE]--;
               else
                  Item_Index[SYNC_MODE] = 3;
               ADC_Start();
               Sync = 0;
               Frame = 0;
               Item_Index[RUNNING_STATUS] = RUN;
               Update[RUNNING_STATUS] = 1;
            }
            if (Key_Buffer == KEYCODE_UP)
            {
               if (Item_Index[SYNC_MODE] < 3)
                  Item_Index[SYNC_MODE]++;
               else
                  Item_Index[SYNC_MODE] = 0;
               ADC_Start();
               Sync = 0;
               Frame = 0;
               Item_Index[RUNNING_STATUS] = RUN;
               Update[RUNNING_STATUS] = 1;
            }
            break;

         case Y_SENSITIVITY:
            if (Key_Buffer == KEYCODE_LEFT)
               Item = SYNC_MODE;
            if (Key_Buffer == KEYCODE_RIGHT)
               Item = X_SENSITIVITY;
            if (Key_Buffer == KEYCODE_UP)
            {
               Erase_Wave(MODEL);
               ADC_Start();
               Sync = 0;
               if ((Item_Index[INPUT_ATTENUATOR] == 0) && (Item_Index[Y_SENSITIVITY] < 9))
                  Item_Index[Y_SENSITIVITY]++; // x1 probe

               if ((Item_Index[INPUT_ATTENUATOR] == 1) && (Item_Index[Y_SENSITIVITY] < 18))
                  Item_Index[Y_SENSITIVITY]++; // x10 probe

            }
            if (Key_Buffer == KEYCODE_DOWN)
            {
               Erase_Wave(MODEL);
               ADC_Start();
               Sync = 0;
               if ((Item_Index[INPUT_ATTENUATOR] == 0) && (Item_Index[Y_SENSITIVITY] > 0))
                  Item_Index[Y_SENSITIVITY]--; // x1 probe

               if ((Item_Index[INPUT_ATTENUATOR] == 1) && (Item_Index[Y_SENSITIVITY] > 11))
                  Item_Index[Y_SENSITIVITY]--; // x10 probe

            }
            if (Item_Index[Y_SENSITIVITY] != Ref_Buffer[302])
               Hide_Ref = 1;
            break;

         case X_SENSITIVITY:
            if (Key_Buffer == KEYCODE_LEFT)
               Item = Y_SENSITIVITY;
            if (Key_Buffer == KEYCODE_RIGHT)
               Item = Y_POSITION;
            if ((Key_Buffer == KEYCODE_UP) && (Item_Index[X_SENSITIVITY] < 21))
            {
               Item_Index[X_SENSITIVITY]++;
               Erase_Wave(MODEL);
               ADC_Start();
               Sync = 0;
            }
            if ((Key_Buffer == KEYCODE_DOWN) && (Item_Index[X_SENSITIVITY] > 0))
            {
               Item_Index[X_SENSITIVITY]--;
               Erase_Wave(MODEL);
               ADC_Start();
               Sync = 0;
            }
            if (Item_Index[X_SENSITIVITY] != Ref_Buffer[303])
               Hide_Ref = 1;
            break;

         case Y_POSITION:
            if (Key_Buffer == KEYCODE_LEFT)
               Item = X_SENSITIVITY;
            if (Key_Buffer == KEYCODE_RIGHT)
               Item = MEASUR_KIND;
            if ((Key_Buffer == KEYCODE_UP) && (v0 < MAX_Y))
            {
               Erase_Wave(MODEL);
               Erase_Wave(CURVE);
               Erase_Dot_v0();
               Erase_Vn(v0);
               v0++;
               Hide_v0 = 0;
            }
            if ((Key_Buffer == KEYCODE_DOWN) && (v0 > (MIN_Y + 1)))
            {
               Erase_Wave(MODEL);
               Erase_Wave(CURVE);
               Erase_Dot_v0();
               Erase_Vn(v0);
               v0--;
               Hide_v0 = 0;
            }
            if (Key_Buffer == KEYCODE_MANU)
               Hide_v0 = 1 - Hide_v0;
            break;

         case MEASUR_KIND:
            if (Key_Buffer == KEYCODE_LEFT)
               Item = Y_POSITION;
            if (Key_Buffer == KEYCODE_RIGHT)
            {
               Hide_vs = 0;
               Item = TRIG_SENSITIVITY;
            }
            if (Key_Buffer == KEYCODE_UP)
            {
               if (Item_Index[MEASUR_KIND] < 2)
                  Item_Index[MEASUR_KIND]++;
               else
                  Item_Index[MEASUR_KIND] = 0;
            }
            if (Key_Buffer == KEYCODE_DOWN)
            {
               if (Item_Index[MEASUR_KIND] > 0)
                  Item_Index[MEASUR_KIND]--;
               else
                  Item_Index[MEASUR_KIND] = 2;
            }
            break;

         case POWER_INFOMATION:
            if (Key_Buffer == KEYCODE_LEFT)
               Item = MEASUR_KIND;
            if (Key_Buffer == KEYCODE_DOWN)
               Item = TRIG_SENSITIVITY;
            break;

         case TRIG_SENSITIVITY:
            if (Key_Buffer == KEYCODE_UP)
            {
               Hide_vs = 1;
               Item = MEASUR_KIND;
               Update[Y_VERNIER_2] = 1;
            }
            if (Key_Buffer == KEYCODE_DOWN)
            {
               Item = TRIG_SLOPE;
               Hide_vs = 1;
            }
            if ((Key_Buffer == KEYCODE_RIGHT) && ((Item_Index[TRIG_SENSITIVITY] + vt) < (MAX_Y - 1))
                && ((vt - Item_Index[TRIG_SENSITIVITY]) > (MIN_Y + 3)))
            {
               Item_Index[TRIG_SENSITIVITY]++;
            }
            if ((Key_Buffer == KEYCODE_LEFT) && (Item_Index[TRIG_SENSITIVITY] > 0))
            {
               temp = vt;
               vt = temp + Item_Index[TRIG_SENSITIVITY];
               Erase_Vn(vt);
               vt = temp - Item_Index[TRIG_SENSITIVITY];
               Erase_Vn(vt);
               vt = temp;
               Item_Index[TRIG_SENSITIVITY]--;
            }
            break;

         case TRIG_SLOPE:
            if (Key_Buffer == KEYCODE_UP)
            {
               Hide_vs = 0;
               Item = TRIG_SENSITIVITY;
            }
            if (Key_Buffer == KEYCODE_DOWN)
               Item = INPUT_ATTENUATOR;
            if ((Key_Buffer == KEYCODE_LEFT) || (Key_Buffer == KEYCODE_RIGHT))
            {
               Item_Index[TRIG_SLOPE] = 1 - Item_Index[TRIG_SLOPE];
            }
            break;

         case INPUT_ATTENUATOR:
            if (Key_Buffer == KEYCODE_UP)
               Item = TRIG_SLOPE;
            if (Key_Buffer == KEYCODE_DOWN)
            {
               if (SD_Card_ON() == 0)
                  Item = SAVE_WAVE_CURVE;
               else
                  Item = OUTPUT_FREQUENCY;
            }
            if ((Key_Buffer == KEYCODE_LEFT) || (Key_Buffer == KEYCODE_RIGHT))
            {
               Erase_Wave(MODEL);
               Erase_Wave(CURVE);
               Item_Index[INPUT_ATTENUATOR] = 1 - Item_Index[INPUT_ATTENUATOR]; // x1 
                                                                                // / 
                                                                                // x10 
                                                                                // probe

            }
            if ((Item_Index[INPUT_ATTENUATOR] == 0) && (Item_Index[Y_SENSITIVITY] > 9))
               Item_Index[Y_SENSITIVITY] -= 9;
            if ((Item_Index[INPUT_ATTENUATOR] == 1) && (Item_Index[Y_SENSITIVITY] < 10))
               Item_Index[Y_SENSITIVITY] += 9;
            break;

         case SAVE_WAVE_CURVE:
            if (SD_Card_ON() == 0)
            {
               if (Key_Buffer == KEYCODE_UP)
                  Item = INPUT_ATTENUATOR;
               if (Key_Buffer == KEYCODE_DOWN)
                  Item = LOAD_WAVE_CURVE;
               if ((Key_Buffer == KEYCODE_RIGHT) && (Item_Index[SAVE_WAVE_CURVE] < 255))
                  Item_Index[SAVE_WAVE_CURVE]++;
               if ((Key_Buffer == KEYCODE_LEFT) && (Item_Index[SAVE_WAVE_CURVE] > 1))
                  Item_Index[SAVE_WAVE_CURVE]--;
               if (Key_Buffer == KEYCODE_MANU)
               {
                  Update[SAVE_WAVE_CURVE] = 0;
                  Cursor_Counter = 0;
                  Char_to_Str(FileNum, Item_Index[SAVE_WAVE_CURVE]);
                  if (FAT_Info() == 0)
                  {
                     if (Open_File(FileNum) == 0)
                     {
                        for (i = 0; i < 300; ++i)
                        {
                           SectorBuff[i + 2] = View_Buffer[i];
                        }
                        SectorBuff[0] = 0;
                        SectorBuff[1] = 0;
                        SectorBuff[302] = (unsigned char) Item_Index[Y_SENSITIVITY];
                        SectorBuff[303] = (unsigned char) Item_Index[X_SENSITIVITY];
                        if (Write_File() == 0)
                        {
                           Item_Index[LOAD_WAVE_CURVE] = Item_Index[SAVE_WAVE_CURVE];
                           if (Item_Index[SAVE_WAVE_CURVE] < 255)
                           {
                              Item_Index[SAVE_WAVE_CURVE]++;
                           }
                           Update[SAVE_WAVE_CURVE] = 1;
                        } else
                           Display_Str(89, 2, WHITE, PRN, "!FileWriteErr!");
                     } else
                        Display_Str(89, 2, WHITE, PRN, "!File NoPlace!");
                  } else
                     Display_Str(89, 2, WHITE, PRN, "!Micro SD Err!");
               }
            } else
            {
               Item = OUTPUT_FREQUENCY;
            }
            break;

         case LOAD_WAVE_CURVE:
            if (SD_Card_ON() == 0)
            {
               if (Key_Buffer == KEYCODE_UP)
                  Item = SAVE_WAVE_CURVE;
               if (Key_Buffer == KEYCODE_DOWN)
                  Item = OUTPUT_FREQUENCY;
               if ((Key_Buffer == KEYCODE_RIGHT) && (Item_Index[LOAD_WAVE_CURVE] < 255))
                  Item_Index[LOAD_WAVE_CURVE]++;
               if ((Key_Buffer == KEYCODE_LEFT) && ((Item_Index[LOAD_WAVE_CURVE]) > 1))
                  Item_Index[LOAD_WAVE_CURVE]--;
               if (Key_Buffer == KEYCODE_MANU)
               {
                  Erase_Wave(MODEL);
                  Update[LOAD_WAVE_CURVE] = 0;
                  Cursor_Counter = 0;
                  Char_to_Str(FileNum, Item_Index[LOAD_WAVE_CURVE]);
                  if (FAT_Info() == 0)
                  {
                     if (Open_File(FileNum) == 0)
                     {
                        if (Read_File() == 0)
                        {
                           for (i = 0; i < 300; ++i)
                           {
                              Ref_Buffer[i] = SectorBuff[i + 2];
                           }
                           if (SectorBuff[302] < 12)
                           {
                              Item_Index[Y_SENSITIVITY] = SectorBuff[302];
                           } else
                           {
                              Item_Index[Y_SENSITIVITY] = 4;
                           }
                           if (SectorBuff[303] < 22)
                           {
                              Item_Index[X_SENSITIVITY] = SectorBuff[303];
                           } else
                           {
                              Item_Index[X_SENSITIVITY] = 8;
                           }
                        } else
                           Display_Str(89, 2, WHITE, PRN, "!File ReadErr!");
                     } else
                        Display_Str(89, 2, WHITE, PRN, "!File NoFound!");
                  } else
                     Display_Str(89, 2, WHITE, PRN, "!Micro SD Err!");
                  Hide_Ref = 0;
               }
            } else
               Item = OUTPUT_FREQUENCY;
            break;

         case OUTPUT_FREQUENCY:
            if (Key_Buffer == KEYCODE_UP)
            {
               if (SD_Card_ON() == 0)
                  Item = LOAD_WAVE_CURVE;
               else
                  Item = INPUT_ATTENUATOR;
            }
            if (Key_Buffer == KEYCODE_DOWN)
               Item = X_VERNIER_2;
            if ((Key_Buffer == KEYCODE_RIGHT) && (Item_Index[OUTPUT_FREQUENCY] < 15))
               Item_Index[OUTPUT_FREQUENCY]++;
            if ((Key_Buffer == KEYCODE_LEFT) && (Item_Index[OUTPUT_FREQUENCY] > 0))
               Item_Index[OUTPUT_FREQUENCY]--;
            break;

         case X_VERNIER_2:
            if (Key_Buffer == KEYCODE_UP)
               Item = OUTPUT_FREQUENCY;
            if (Key_Buffer == KEYCODE_DOWN)
               Item = X_VERNIER_1;
            if (Key_Buffer == KEYCODE_MANU)
            {
               Hide_t2 = 1 - Hide_t2;
               Erase_Ti(t2);
            }
            if ((Key_Buffer == KEYCODE_RIGHT) && (t2 < MAX_X))
            {
               Erase_Dot_ti(t2);
               Erase_Ti(t2);
               t2++;
               Hide_t1 = 0;
               Hide_t2 = 0;
            }
            if ((Key_Buffer == KEYCODE_LEFT) && (t2 > MIN_X + 1))
            {
               Erase_Dot_ti(t2);
               Erase_Ti(t2);
               t2--;
               Hide_t1 = 0;
               Hide_t2 = 0;
            }
            Update[DELTA_T] = 1;
            Update[VERNIERS] = 1;
            break;

         case X_VERNIER_1:
            if (Key_Buffer == KEYCODE_UP)
               Item = X_VERNIER_2;
            if (Key_Buffer == KEYCODE_DOWN)
            {
               Item = X_POSITION;
               Draw_View_Area();
            }
            if (Key_Buffer == KEYCODE_MANU)
            {
               Hide_t1 = 1 - Hide_t1;
               Erase_Ti(t1);
            }
            if ((Key_Buffer == KEYCODE_RIGHT) && (t1 < MAX_X))
            {
               Erase_Dot_ti(t1);
               Erase_Ti(t1);
               t1++;
               Hide_t1 = 0;
               Hide_t2 = 0;
            }
            if ((Key_Buffer == KEYCODE_LEFT) && (t1 > MIN_X + 1))
            {
               Erase_Dot_ti(t1);
               Erase_Ti(t1);
               t1--;
               Hide_t1 = 0;
               Hide_t2 = 0;
            }
            Update[DELTA_T] = 1;
            Update[VERNIERS] = 1;
            break;

         case X_POSITION:
            if (Key_Buffer == KEYCODE_UP)
            {
               Item = X_VERNIER_1;
               Erase_View_Area();
            }
            if (Key_Buffer == KEYCODE_DOWN)
            {
               Item = Y_VERNIER_2;
               Update[Y_VERNIER_2] = 1;
               Erase_View_Area();
            }
            if (Key_Buffer == KEYCODE_RIGHT)
            {
               Stop = 0;
               Item_Index[X_POSITION]++;
               Draw_View_Area();
            }
            if (Key_Buffer == KEYCODE_LEFT)
            {
               Stop = 0;
               Item_Index[X_POSITION]--;
               Draw_View_Area();
            }
            Update[VERNIERS] = 1;
            break;

         case RUNNING_STATUS:
            if (Key_Buffer == KEYCODE_UP)
               Item = X_POSITION;
            if (Key_Buffer == KEYCODE_LEFT)
               Item = Y_VERNIER_2;
            break;

         case Y_VERNIER_2:
            if (Key_Buffer == KEYCODE_RIGHT)
            {
               Item = X_POSITION;
               Draw_View_Area();
            }
            if (Key_Buffer == KEYCODE_LEFT)
               Item = Y_VERNIER_1;
            if (Key_Buffer == KEYCODE_MANU)
            {
               Hide_v2 = 1 - Hide_v2;
               Erase_Vi(v2);
            }
            if ((Key_Buffer == KEYCODE_UP) && (v2 < MAX_Y))
            {
               Erase_Dot_vi(v2);
               Erase_Vi(v2);
               v2++;
            }
            if ((Key_Buffer == KEYCODE_DOWN) && (v2 > MIN_Y + 1))
            {
               Erase_Dot_vi(v2);
               Erase_Vi(v2);
               v2--;
            }
            Update[VERNIERS] = 1;
            break;

         case Y_VERNIER_1:
            if (Key_Buffer == KEYCODE_RIGHT)
               Item = Y_VERNIER_2;
            if (Key_Buffer == KEYCODE_LEFT)
               Item = TRIG_LEVEL;
            if (Key_Buffer == KEYCODE_MANU)
            {
               Hide_v1 = 1 - Hide_v1;
               Erase_Vi(v1);
            }
            if ((Key_Buffer == KEYCODE_UP) && (v1 < MAX_Y))
            {
               Erase_Dot_vi(v1);
               Erase_Vi(v1);
               v1++;
            }
            if ((Key_Buffer == KEYCODE_DOWN) && (v1 > MIN_Y + 1))
            {
               Erase_Dot_vi(v1);
               Erase_Vi(v1);
               v1--;
            }
            Update[VERNIERS] = 1;
            break;

         case TRIG_LEVEL:
            if (Key_Buffer == KEYCODE_RIGHT)
               Item = Y_VERNIER_1;
            if (Key_Buffer == KEYCODE_LEFT)
               Item = SYNC_MODE;
            if (Key_Buffer == KEYCODE_MANU)
               Hide_vt = 1 - Hide_vt;
            if ((Key_Buffer == KEYCODE_UP) && (vt < (MAX_Y - Item_Index[TRIG_SENSITIVITY])))
            {
               Erase_Dot_vt();
               Erase_Vn(vt);
               vt++;
            }
            if ((Key_Buffer == KEYCODE_DOWN) && (v1 > (MIN_Y + 5 + Item_Index[TRIG_SENSITIVITY])))
            {
               Erase_Dot_vt();
               Erase_Vn(vt);
               vt--;
            }
            break;
         }
         Key_Buffer = 0;
      }
   }
*/
}
/********************************* END OF FILE ********************************/
