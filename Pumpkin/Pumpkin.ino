
/*
* Wind Switch with Flickering LED
* by Electrick_RWM (dragoonx@gmail.com)
* Date Oct 18 2007 
* Makes the LED on Pin 9 glow sloftly and when Switch or "Wind" Switch is toggled it flickers!  
* After the LED flickers it returns to its normal glowing loop until blown on again 
*
* Thanks to TodBot for the Spooky Arduino Projects! 
* Without his tutorials I would have never been able to create this project
* http://www.arduino.cc
*/

int ledPin = 4;             // choose the pin for the LED
int ledCandle = 0;           // choose the pin for the LED
int inputPin = 2;            // choose the input pin (For Wind Switch)
int val = 0;                 // variable for reading the pin status
int max_count = 60;          // counter for the flicker table
int max_count_Normal = 64;   // counter for normal glowing table
int count = 0;
int countTwo = 0;
byte Norm_Glow_table[] = {   10, 10, 10, 10, 20, 30, 30, 30, 40, 50, 60, 70, 80, 90,100, // The table for our normal glowing
                           110,120,130,140,150,160,170,180,190,200,
                           210,220,230,240,250,250,250,250,250,250,
                           240,230,220,210,200,190,180,170,160,150,
                           140,130,120,110,100, 90, 80, 70, 60, 50,
                            40, 30, 30, 30, 20, 10, 10, 10, 10 };
byte Flicker_table[] = {      10, 10, 20, 30, 30, 30, 40, 50, 60, 70, 80, 70, 70,     // the table that tells us how to flicker
                           60, 60, 50, 50, 50, 60, 70, 80, 90, 100,
                           120,140,160,240,250,100,150,250,250,140,
                           240,230,220,100, 80,70,70, 70, 80, 80,
                           140,130,120,110,200,210,220,220,100, 90,
                            40, 30, 30, 30, 20, 10, 10 };
class Fader
{
  // Class Member Variables
  // initialised at startup
  int ledPin;     // the number of the LED pin
  int fadeAmount;  // how many points to fade the LED by
  int maxBright; // max brightness to fade to (max 255)

  // These maintain the current state
  unsigned long previousMillis;   // will store last time LED was updated  
  int brightness; // brightness used to set LED
  int fadeSpeed;  // fader speed

  // Constructor - creates a Fader object
  // and initialises the vairables and state
  public:
  Fader(int pin, int fade, long fSpeed, int bright)
  {
    ledPin = pin;
    pinMode(ledPin, OUTPUT);

    fadeAmount = fade;
    brightness = 0;
    maxBright = bright;
    fadeSpeed = fSpeed;
    previousMillis = 0;
  }

  void Update(unsigned long currentMillis)
  {

     if(currentMillis - previousMillis >= fadeSpeed)
    {
      if (brightness >= 0 && brightness <= maxBright)  {
        // Check we haven't overflowed the int variable.
        analogWrite(ledPin, brightness);  // set the LED brightness
      }
      
      brightness = brightness + fadeAmount; // increase/decrease brighness for next time

      // reverse fade direction at each end by +/- fadeAmount to decrement or increment
      if (brightness <= 0 || brightness >= maxBright) {
        fadeAmount = -fadeAmount;
      }

      previousMillis = currentMillis;  // Remember the time
      
    }
    
  
  }
};
                            
Fader led1(1, 25, 40, 250);

void setup() {
 pinMode(ledPin, OUTPUT);      // declare LED as output
 pinMode(ledCandle, OUTPUT);   // declare Wind led as output
 pinMode(inputPin, INPUT);     // declare Wind Switch as input
 // Timer0 is already used for millis() - we'll just interrupt somewhere
 // in the middle and call the "Compare A" function below
 OCR0A = 0xAF;
 TIMSK |= _BV(OCIE0A);
} 
 
// Interrupt is called once a millisecond
SIGNAL(TIMER0_COMPA_vect) 
{
  unsigned long currentMillis = millis();

  led1.Update(currentMillis); 
//  led2.Update(currentMillis);
//  led3.Update(currentMillis);

}

void loop(){
 val = digitalRead(inputPin);  // read input value
 if (val == LOW) {            // check if the input is HIGH
   analogWrite(ledCandle, Norm_Glow_table[count]);  // Runs through the Norm_Glow_table 
   count++;                                         // We stay in this loop till the switch is activaled
   if(count > max_count_Normal )
     count = 0;    // Helps makes sure our next normal glow doesnt start in an arbitrary place on the table
 } else {
   digitalWrite(ledPin, HIGH); // turn LED ON ~ Means our wind Switch worked
   delay(30);
   digitalWrite(ledPin, LOW);
   for ( int i=0; i <= 60; i++) {  // This for loop runs untill the flicker table finishes
   analogWrite(ledCandle, Flicker_table[countTwo]);
     countTwo++;
   if(countTwo > max_count ){
     countTwo = 0;  // Helps makes sure our next flicker doesnt start in an arbitrary place on the table
   }
   delay(40);  // the delay for our flicker, make it faster to to make it flicker a little more violently
   }
 }
 delay(40);    // the delay for our normal glow 
}
