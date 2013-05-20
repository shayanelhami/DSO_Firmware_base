/******************** (C) COPYRIGHT 2009 e-Design Co.,Ltd. *********************
* File Name: file.c      Author: bure      Hardware: DS0201V1.1    Version: V1.0
*******************************************************************************/
#include "msd.h"
#include "File.h"
#include "Function.h"
#include "Calculate.h"
#include "Lcd.h"

unsigned int    DiskStart, Hidden_Sec, Root_Addr, ClusterNum, File_Addr;
unsigned short  RsvdSecCnt, FAT_Size16, FAT_Size32;
unsigned short  SecPerClus, DirFliePtr, DirFlieSec, DirSecNum, MaxNum;

/*******************************************************************************
 MSD_FAT_Info: Get SD card infomation   Input£ºNONE   Return£º0x00=success
*******************************************************************************/
char            FAT_Info(void)
{
   char            Rvalue = 0xFF;

   DiskStart = 0;
   Rvalue = MSD_ReadBlock(SectorBuff, 0, 512);
   if (Rvalue != 0)
      return Rvalue;
   if ((SectorBuff[0] != 0xEB) && (SectorBuff[0] != 0xE9))
   {
      DiskStart = (SectorBuff[0x1C9] << 24) + (SectorBuff[0x1C8] << 16) + (SectorBuff[0x1C7] << 8) + (SectorBuff[0x1C6]);
      Rvalue = MSD_ReadBlock(SectorBuff, DiskStart << 9, 512);
      if (Rvalue != 0)
         return Rvalue;
   }
   SecPerClus = SectorBuff[0x0D];
   RsvdSecCnt = (SectorBuff[0x0F] << 8) + (SectorBuff[0x0E]); // 0x0001

   FAT_Size16 = (SectorBuff[0x17] << 8) + (SectorBuff[0x16]); // 0x001F

   Hidden_Sec = (SectorBuff[0x1F] << 24) + (SectorBuff[0x1E] << 16) + (SectorBuff[0x1D] << 8) + (SectorBuff[0x1C]);
   FAT_Size32 = (SectorBuff[0x25] << 8) + (SectorBuff[0x24]);
   if (FAT_Size16 == 0)
      Root_Addr = (DiskStart + RsvdSecCnt + 2 * FAT_Size32) * 512;
   else
      Root_Addr = (DiskStart + RsvdSecCnt + 2 * FAT_Size16) * 512; // (61+1+1F+1F)*512=14000

   return 0;
}
/*******************************************************************************
 Open_File: Open a File   Input£ºnumber of The filename  Return£º0x00=success 
*******************************************************************************/
char            Open_File(unsigned char *n)
{
   char            Rvalue = 0xFF;
   unsigned short  i, SectorCnt;

   DirFliePtr = 0;
   DirFlieSec = 0;
   if (FAT_Size16 == 0)
      DirSecNum = 0;
   else
      DirSecNum = 0x20;

   Rvalue = MSD_ReadBlock(SectorBuff, Root_Addr, 512);
   if (Rvalue != 0)
   {
      return 0xFF;
   }
   for (SectorCnt = 0; SectorCnt < DirSecNum; ++SectorCnt)
   {
      Rvalue = MSD_ReadBlock(SectorBuff, Root_Addr + SectorCnt * 512, 512);
      if (Rvalue != 0)
      {
         return Rvalue;
      }
      for (i = 0; i < 512; i += 32)
      {
         if ((SectorBuff[i] == 'F') && (SectorBuff[i + 1] == 'I') && (SectorBuff[i + 2] == 'L') && (SectorBuff[i + 3] == 'E'))
         {
            if ((SectorBuff[i + 8] == 'D') && (SectorBuff[i + 9] == 'A') && (SectorBuff[i + 10] == 'T'))
            {
               if ((SectorBuff[i + 4] == *n) && (SectorBuff[i + 5] == *(n + 1)) && (SectorBuff[i + 6] == *(n + 2)))
               {
                  DirFliePtr = i;
                  DirFlieSec = SectorCnt;
                  if (FAT_Size16 == 0)
                     SectorBuff[i + 0x15] = SectorBuff[i + 0x14] = 0;
                  ClusterNum = ((SectorBuff[i + 0x15] << 24) + (SectorBuff[i + 0x14] << 16) + (SectorBuff[i + 0x1B] << 8) + (SectorBuff[i + 0x1A]));
                  File_Addr = Root_Addr + (DirSecNum + (ClusterNum - 2) * SecPerClus) * 512;
                  return 0;
               }
            }
         }
      }
   }
   return 0xFF;
}
/*******************************************************************************
 Read_File: Read a File   Input£ºNONE    Return£º0x00=success 
*******************************************************************************/
char            Read_File(void)
{
   return MSD_ReadBlock(SectorBuff, File_Addr, 512);
}
/*******************************************************************************
 Write_File: Write File   Input£ºNONE    Return£º0x00=success
*******************************************************************************/
char            Write_File(void)
{
   char            Rvalue = 0xFF;

   Rvalue = MSD_WriteBlock(SectorBuff, File_Addr, 512);
   if (Rvalue != 0)
      return Rvalue;
   return Rvalue;
}

/******************************** END OF FILE *********************************/
