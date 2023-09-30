#include <reg51.h> 

#include "lcd.h"	
#include "delay.h" 
//----------------------------------- definition ---------------------------------------------------------------
#define ON 0
#define OFF 1

#define HIGH 0
#define LOW 1


//----------------------------------- Inputs  (Port 1) ----------------------------------------------------------
sbit _startswitch = P1^0;
sbit _stopswitch = P1^1;
sbit _asswitch = P1^2;
sbit _flowsensor = P1^3;
sbit _highsensor = P1^4;
sbit _lowsensor = P1^5;

//----------------------------------- Outputs (Port 3) ----------------------------------------------------------
sbit _relay = P2^0;
sbit _buzzer = P2^1;

//=================================== Initialization ==========================================

void init_peripheral()
{	
	P0 = 0x00;
	P1 = 0xFF; 
	P2 = 0xFF;
	
//--------------------------------------------------------------------------------------------
	LCD_Init();
	LCD_DisplayString("    JABALTech   ");
	LCD_GoToLineTwo();
	LCD_DisplayString(" Pump Controller");
	delay_sec(1);
	LCD_Clear();

//---------------------------------- Buzzer Test ---------------------------------------------
	LCD_DisplayString("  Buzzer Check  ");
	delay_sec(1);
	_buzzer = ON;
	delay_sec(1);
	_buzzer = OFF;
	delay_sec(1);
	_buzzer = ON;
	delay_sec(1);
	_buzzer = OFF;
	delay_sec(1);
	LCD_Clear();

//---------------------------------- Relay Test ---------------------------------------------
	LCD_DisplayString("  Relay Check   ");
	delay_sec(1);
	_relay = ON;
	delay_sec(1);
	_relay = OFF;
	delay_sec(1);
	_relay = ON;
	delay_sec(1);
	_relay = OFF;
	LCD_Clear();
}


//============================ Main Code ====================================================
void main() 
{
	init_peripheral();
	
	while(1){
				_buzzer = OFF;
				LCD_DisplayString("  Sensor Check  ");
				LCD_GoToLineTwo();
				LCD_DisplayString("   Mode Ready   ");
				delay_sec(1);
				
				while (_startswitch == HIGH)
				{
					LCD_DisplayString(" Start Switch OK");
					_buzzer = ON;
				} 
				
				while (_stopswitch != HIGH)
				{
					LCD_DisplayString(" Stop Switch OK ");
					_buzzer = ON;
				}
				
				while (_asswitch == HIGH)
				{
					LCD_DisplayString(" Auto Switch OK ");
					_buzzer = ON;
				} 
				
				while (_flowsensor == HIGH)
				{
					LCD_DisplayString(" Flow Sensor OK ");
					_buzzer = ON;
				}

				while (_highsensor == HIGH)
				{
					LCD_DisplayString(" High Sensor OK ");
					_buzzer = ON;
				}

				while (_lowsensor == HIGH)
				{
					LCD_DisplayString("  Low Sensor OK ");
					_buzzer = ON;
				}
			
			}				
	
}  