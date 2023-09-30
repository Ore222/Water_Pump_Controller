#include <reg51.h> 
#include "lcd.h"	
#include "delay.h" 
//----------------------------------- definition ---------------------------------------------------------------
#define ON 0
#define OFF 1
#define HIGH 0
#define LOW 1

#define _pumppausetime 15  // pump pause time when well water is low (minutes).
#define _pumppausedelay 1//240	// pump pause monitoring delay approximate time (seconds times 2).

volatile _autoflag;
volatile _lowbuzzflag;
volatile _stopdispflag;
volatile _pumponflag;
volatile _highsenseflag;
volatile _autoidledispflag;
volatile _manualidledispflag;
volatile _dispflagfull;				  
volatile _dispflagokay;
volatile _dispflagerr;
volatile _dispflaglow;
int count;

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

	count = 0;
	_autoflag = HIGH;
	_lowbuzzflag = HIGH;
	_stopdispflag = HIGH;
	_autoidledispflag = HIGH;
	_manualidledispflag = HIGH;
	_highsenseflag = HIGH;
	_pumponflag = HIGH;
	_dispflagfull = HIGH;
	_dispflagokay = HIGH;
	_dispflagerr = HIGH;
	_dispflaglow = HIGH;
	
//--------------------------------------------------------------------------------------------
	LCD_Init();
	LCD_DisplayString("    JABALTech   ");
	LCD_GoToLineTwo();
	LCD_DisplayString(" Pump Controller");
	delay_sec(1);
	LCD_Clear();
	LCD_DisplayString(" Powering on ");
	delay_ms(500);
	LCD_GoToXY(2, 14);
	LCD_DataWrite('.');
	delay_ms(500);
	LCD_GoToXY(2, 15);
	LCD_DataWrite('.');
	delay_ms(500);
	LCD_GoToXY(2, 16);
	LCD_DataWrite('.');
	_buzzer = ON;
	delay_ms(100);
	_buzzer = OFF;
	LCD_Clear();
	if (_asswitch == LOW){
		LCD_GoToLineOne();
		LCD_DisplayString(" Automatic Start");
		LCD_GoToLineTwo();
		LCD_DisplayString("    disabled    ");
		delay_sec(1);}
	else{
		LCD_GoToLineOne();
		LCD_DisplayString(" Automatic Start");
		LCD_GoToLineTwo();
		LCD_DisplayString("     enabled    ");
		delay_sec(1);}
		LCD_Clear();
}
//=========================================================================================
void waterleveldisp(){			   	
				if (_highsensor == HIGH && _lowsensor == HIGH && _dispflagfull == HIGH){ // water level is on top sensor
				LCD_GoToLineOne();
				LCD_DisplayString("Water Level:Full");
				_dispflagfull = LOW;
				_dispflaglow = HIGH;
				_dispflagokay = HIGH;
				_dispflagerr = HIGH;
				_autoidledispflag = HIGH;
				_manualidledispflag = HIGH;}

				if (_highsensor == LOW && _lowsensor == HIGH && _dispflagokay == HIGH){ // water level is below high sensor
				LCD_GoToLineOne();
				LCD_DisplayString("Water Level:Okay");
  				_dispflagokay = LOW;
				_dispflaglow = HIGH;
				_dispflagfull = HIGH;
				_dispflagerr = HIGH;
				_autoidledispflag = HIGH;
				_manualidledispflag = HIGH;}

				if (_highsensor == HIGH && _lowsensor == LOW && _dispflagerr == HIGH){ // sensor malfunction
				LCD_GoToLineOne();
				LCD_DisplayString("Water Level:Err.");
  				_dispflagerr = LOW;
				_dispflaglow = HIGH;
				_dispflagfull = HIGH;
				_dispflagokay = HIGH;
				_autoidledispflag = HIGH;
				_manualidledispflag = HIGH;}

				if (_highsensor == LOW && _lowsensor == LOW && _dispflaglow == HIGH){ // water level is below low sensor
				LCD_GoToLineOne();
				LCD_DisplayString("Water Level:Low!");
  				_dispflaglow = LOW;
				_dispflagokay = HIGH;
				_dispflagfull = HIGH;
				_dispflagerr = HIGH;
				_autoidledispflag = HIGH;
				_manualidledispflag = HIGH;}
}

void status_manualidle(){
	waterleveldisp();

	if (_manualidledispflag == HIGH)
	{
		if (_highsensor == HIGH && _pumponflag == HIGH){
			LCD_GoToLineTwo();
			LCD_DisplayString("----------------");}

		if (_highsensor == HIGH && _stopdispflag == LOW){
			LCD_GoToLineTwo();
			LCD_DisplayString("----------------");}

		if (_highsensor != HIGH && _pumponflag == HIGH){  												// water level is not full
			LCD_GoToLineTwo();
			LCD_DisplayString("  Press START   ");}
		_manualidledispflag = LOW;
		_autoidledispflag = HIGH;
	}
}

void status_autoidle(){
	waterleveldisp();
	if (_autoidledispflag == HIGH)
	{
		if (_pumponflag == HIGH)
		{
			LCD_GoToLineTwo();
			LCD_DisplayString("  Monitoring... ");
		}
		_autoidledispflag = LOW;
		_manualidledispflag = HIGH;}
}

void status_startpump(){
	waterleveldisp();
	_relay = ON;				// start water pump when START button is pressed
	_pumponflag = LOW;
	_stopdispflag = HIGH;
	_autoidledispflag = HIGH;
	_manualidledispflag = HIGH;
	_highsenseflag = HIGH;
	_lowbuzzflag = LOW;
	LCD_GoToLineTwo();
	LCD_DisplayString("  Pump Running  ");}

void status_stoppump(){
	waterleveldisp();
	_relay = OFF;				// stop water pump when START/STOP button is pressed
	_pumponflag = HIGH;
	_lowbuzzflag = HIGH;
	_autoidledispflag = HIGH;
	_manualidledispflag = HIGH;
	count = 0;
	if (_stopdispflag == HIGH)
	{
		LCD_GoToLineTwo();
		LCD_DisplayString("  Pump Stopped  ");
		delay_sec(1);
		_stopdispflag = LOW;
	}
}

void status_pausepump()
{
	_relay = OFF;
	_pumponflag = HIGH;
	count = 0;
	LCD_GoToLineOne();
	LCD_DisplayString(" Well water low!");
	LCD_GoToLineTwo();
	LCD_DisplayString("  Pump paused.  ");

	if (_asswitch == HIGH){
		delay_min(_pumppausetime);
		_autoidledispflag = HIGH;
		_manualidledispflag = HIGH;
		_dispflagfull = HIGH;
		_dispflaglow = HIGH;
		_dispflagokay = HIGH;
		_dispflagerr = HIGH;
		status_startpump();
	}
	else{
		while(_startswitch == LOW && _stopswitch == LOW){
			_buzzer = ON;
			delay_ms(500);
			_buzzer = OFF;
			delay_sec(1);
		}
			if (_startswitch == HIGH){
				_autoidledispflag = HIGH;
				_manualidledispflag = HIGH;
				_dispflagfull = HIGH;
				_dispflaglow = HIGH;
				_dispflagokay = HIGH;
				_dispflagerr = HIGH;
				status_startpump();
			}

			if (_stopswitch == HIGH){
				_autoidledispflag = HIGH;
				_manualidledispflag = HIGH;
				_dispflagfull = HIGH;
				_dispflaglow = HIGH;
				_dispflagokay = HIGH;
				_dispflagerr = HIGH;
				status_stoppump();
			}
		
	}
	
}


//=========================================================================================================================
void main() 
{
	init_peripheral();
	
	while(1){
			
			while (_asswitch == LOW)							// Autostart is disabled
			{
				
				status_manualidle();
				
				if (_pumponflag == LOW)								// For pump pause delay timing
				{
					delay_ms(500);
					count ++;
				}

				if (_startswitch == HIGH)
				{
					_buzzer = ON;
					delay_ms(100);
					_buzzer = OFF;
					delay_ms(100);
					_buzzer = ON;
					delay_ms(100);
					_buzzer = OFF;
					status_startpump();
				}

				if (_stopswitch == HIGH)
				{
					_buzzer = ON;
					delay_ms(250);
					_buzzer = OFF;
					status_stoppump();
				}


				if (count > _pumppausedelay && _flowsensor == LOW && _pumponflag == LOW)
				{
					_buzzer = ON;
					delay_sec(1);
					_buzzer = OFF;
					status_pausepump();
				}

				if (_highsensor == HIGH && _highsenseflag == HIGH)
				{
					delay_sec(1);
					_buzzer = ON;
					delay_ms(500);
					_buzzer = OFF;
					_highsenseflag = LOW;
					status_stoppump();
				}

				if (_lowsensor == LOW && _lowbuzzflag == HIGH)
				{
				_buzzer = ON;
				delay_sec(250);
				_buzzer = OFF;
				delay_sec(1);
				}
			}
	

			while (_asswitch == HIGH)								// Auto-start is enabled
			{	
				status_autoidle(); 

				if (_pumponflag == LOW)								// For pump pause delay timing
				{
					delay_ms(500);
					count ++;
				}
						
				if (_highsensor != HIGH && _autoflag == HIGH)
					{
						status_startpump();
						_autoflag = LOW;
					}

		
				if (_lowsensor != HIGH)
					{
						_autoflag = HIGH;
					}


				if (_startswitch == HIGH)
					{
						_buzzer = ON;
						delay_ms(100);
						_buzzer = OFF;
						delay_ms(100);
						_buzzer = ON;
						delay_ms(100);
						_buzzer = OFF;
						status_startpump();
					}

					if (_stopswitch == HIGH)
					{
						_buzzer = ON;
						delay_ms(250);
						_buzzer = OFF;
						status_stoppump();
					}
				
					if (count > _pumppausedelay && _flowsensor == LOW && _pumponflag == LOW)
					{
						_buzzer = ON;
						delay_sec(1);
						_buzzer = OFF;
						status_pausepump(); 
					}

					if (_highsensor == HIGH && _highsenseflag == HIGH)
				{
					
					delay_sec(1);
					_buzzer = ON;
					delay_ms(500);
					_buzzer = OFF;
					_highsenseflag = LOW;
					status_stoppump();
				}
			}				
	}
}  