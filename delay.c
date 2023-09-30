/*----------------------------------------------------------------------------------
              
Filename: delay.c
Controller:89c51(8051 family)
Oscillator: 11.0592 MHz
Author: engrckbk
website: www.techcottageinc.com
 ----------------------------------------------------------------------------------*/
#include <reg51.h>
#include <delay.h>



void delay_us(unsigned int us_count)
 {	 do
      {
		 us_count--;
	   }while(us_count);
   }



/*-----------------------------------------------------------------------------------*/
void delay_ms(unsigned int ms_count)
 {
        while(ms_count!=0)
		 {
	        delay_us(112);	 
			 ms_count--;
			}

   }


/* -----------------------------------------------------------------------------------*/
void delay_sec(unsigned char sec_count)
 {


	 while(sec_count!=0)
	  {
	     delay_ms(1000);	
		 sec_count--;
		}
 }
/* -----------------------------------------------------------------------------------*/
void delay_min(unsigned char min_count)
 {


	 while(min_count!=0)
	  {
	     delay_sec(60);	
		 min_count--;
		}
  }