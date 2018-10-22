#include <avr/sleep.h>
#include <avr/wdt.h>

#define LED 0

ISR(WDT_vect) 
  {
  wdt_disable();  
  }

void myWatchdogEnable(const byte interval) 
  {
  wdt_reset();
    
  MCUSR = 0;                          // reset various flags
  WDTCR |= 0b00011000;               // see docs, set WDCE, WDE
  WDTCR =  0b01000000 | interval;    // set WDIE, and appropriate delay
  
  ADCSRA &= ~_BV(ADEN);

  set_sleep_mode (SLEEP_MODE_PWR_DOWN); 
  sleep_bod_disable();
  sei();
  sleep_mode();     
  
  
  ADCSRA |= _BV(ADEN);
  
  } 

void setup()
{
pinMode (LED, OUTPUT);
}  

void loop(){

  digitalWrite (LED, HIGH); 
  delay (2000);    
  digitalWrite (LED, LOW);  

  for (int i = 0; i < 15; i++){
  myWatchdogEnable (0b000110);  // 1 second
  }

}  

// sleep bit patterns:
//  1 second:  0b000110
//  2 seconds: 0b000111
//  4 seconds: 0b100000
//  8 seconds: 0b100001
