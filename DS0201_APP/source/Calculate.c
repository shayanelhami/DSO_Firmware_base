/******************** (C) COPYRIGHT 2009 e-Design Co.,Ltd. *********************
* File Name: Calculate.c  Author: kewei & bure  Hardware: DS0201V1.1 Version: Ver 1.0
*******************************************************************************/

#include "Calculate.h"

/*******************************************************************************
 Int32String_sign: Change signed int to String  
*******************************************************************************/
void            Int32String_sign(I32STR_RES * r, int n, int e)
{
   int             i, m, c, fixlen;
   char           *p = r->str;

   fixlen = e + 2;
   if (n == 0)
   {
      *p++ = ' ';
      *p++ = '0';
      *p++ = '.';
      *p++ = '0';
      *p++ = '0';
      *p = 0;
      r->decPos = 0;
      r->len = p - r->str;
      return;
   }
   if (n > 0)
      *p++ = ' ';
   else
   {
      *p++ = '-';
      n = -n;
   }
   m = n;
   i = 0;
   c = 5;
   while (m >= 10)
   {
      m /= 10;
      if (++i > e)
         c *= 10;
   }
   r->decPos = i / 3;
   if (i >= e)
      n += c;
   switch (i)
   {
   case 9:
      *p++ = '0' + n / 1000000000;
      if (--e == 0)
         break;
      n %= 1000000000;
      *p++ = '.', i = 0;
   case 8:
      *p++ = '0' + n / 100000000;
      if (--e == 0)
         break;
      n %= 100000000;
   case 7:
      *p++ = '0' + n / 10000000;
      if (--e == 0)
         break;
      n %= 10000000;
   case 6:
      *p++ = '0' + n / 1000000;
      if (--e == 0)
         break;
      n %= 1000000;
      if (i)
         *p++ = '.', i = 0;
   case 5:
      *p++ = '0' + n / 100000;
      if (--e == 0)
         break;
      n %= 100000;
   case 4:
      *p++ = '0' + n / 10000;
      if (--e == 0)
         break;
      n %= 10000;
   case 3:
      *p++ = '0' + n / 1000;
      if (--e == 0)
         break;
      n %= 1000;
      if (i)
         *p++ = '.', i = 0;
   case 2:
      *p++ = '0' + n / 100;
      if (--e == 0)
         break;
      n %= 100;
   case 1:
      *p++ = '0' + n / 10;
      if (--e == 0)
         break;
      n %= 10;
   case 0:
      *p++ = '0' + n;
   }
   while (p < r->str + fixlen)
      *p++ = ' ';
   *p = 0;
   r->len = p - r->str;
}
/*******************************************************************************
 Int32String: Change signed int to String  
*******************************************************************************/
void            Int32String(I32STR_RES * r, int n, int e)
{
   int             i, m, c, fixlen;
   char           *p = r->str;

   fixlen = e + 2;
   if (n == 0)
   {
      *p++ = '0';
      *p++ = '.';
      *p++ = '0';
      *p++ = '0';
      *p++ = '0';
      *p = 0;
      r->decPos = 0;
      r->len = p - r->str;
      return;
   }
   // if ( n > 0 )
   // *p++ = ' ';
   // else {
   // *p++ = '-';
   // n = -n;
   // }
   m = n;
   i = 0;
   c = 5;
   while (m >= 10)
   {
      m /= 10;
      if (++i > e)
         c *= 10;
   }
   r->decPos = i / 3;
   if (i >= e)
      n += c;
   switch (i)
   {
   case 9:
      *p++ = '0' + n / 1000000000;
      if (--e == 0)
         break;
      n %= 1000000000;
      *p++ = '.', i = 0;
   case 8:
      *p++ = '0' + n / 100000000;
      if (--e == 0)
         break;
      n %= 100000000;
   case 7:
      *p++ = '0' + n / 10000000;
      if (--e == 0)
         break;
      n %= 10000000;
   case 6:
      *p++ = '0' + n / 1000000;
      if (--e == 0)
         break;
      n %= 1000000;
      if (i)
         *p++ = '.', i = 0;
   case 5:
      *p++ = '0' + n / 100000;
      if (--e == 0)
         break;
      n %= 100000;
   case 4:
      *p++ = '0' + n / 10000;
      if (--e == 0)
         break;
      n %= 10000;
   case 3:
      *p++ = '0' + n / 1000;
      if (--e == 0)
         break;
      n %= 1000;
      if (i)
         *p++ = '.', i = 0;
   case 2:
      *p++ = '0' + n / 100;
      if (--e == 0)
         break;
      n %= 100;
   case 1:
      *p++ = '0' + n / 10;
      if (--e == 0)
         break;
      n %= 10;
   case 0:
      *p++ = '0' + n;
   }
   while (p < r->str + fixlen)
      *p++ = ' ';
   *p = 0;
   r->len = p - r->str;
}
/*******************************************************************************
 Char_to_Str: Change a byte to ASCII   Input£ºchar  output£ºString  
*******************************************************************************/
void            Char_to_Str(unsigned char *p, unsigned char n)
{
   *p++ = '0' + n / 100;
   n %= 100;
   *p++ = '0' + n / 10;
   n %= 10;
   *p++ = '0' + n;
   *p = 0;
}
/*******************************************************************************
 Char_to_Hex: Change a byte to Hex  Input£ºchar  output£ºString 
*******************************************************************************/
void            Char_to_Hex(unsigned char *p, unsigned char n)
{
   if (n / 16 > 9)
      *p++ = 'A' + (n / 16 - 10);
   else
      *p++ = '0' + n / 16;
   n %= 16;
   if (n > 9)
      *p++ = 'A' + (n - 10);
   else
      *p++ = '0' + n;
   *p++ = ' ';
   *p = 0;
}
/********************************* END OF FILE ********************************/
