/*
  I AM A ROBOT!!!
  Fader & Flasher classes for LEDs for eyes, chest & beacon.
 */

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
  Fader(int pin, int fade, int fSpeed, int bright)
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

class Flasher
{
  // Class Member Variables
  // These are initialized at startup
  int ledPin;      // the number of the LED pin
  long OnTime;     // milliseconds of on-time
  long OffTime;    // milliseconds of off-time
 
  // These maintain the current state
  int ledState;                 // ledState used to set the LED
  unsigned long previousMillis;   // will store last time LED was updated
 
  // Constructor - creates a Flasher 
  // and initializes the member variables and state
  public:
  Flasher(int pin, long on, long off)
  {
  ledPin = pin;
  pinMode(ledPin, OUTPUT);     
    
  OnTime = on;
  OffTime = off;
  
  ledState = LOW; 
  previousMillis = 0;
  }
 
  void Update(unsigned long currentMillis)
  {
    if((ledState == HIGH) && (currentMillis - previousMillis >= OnTime))
    {
      ledState = LOW;  // Turn it off
      previousMillis = currentMillis;  // Remember the time
      digitalWrite(ledPin, ledState);  // Update the actual LED
    }
    else if ((ledState == LOW) && (currentMillis - previousMillis >= OffTime))
    {
      ledState = HIGH;  // turn it on
      previousMillis = currentMillis;   // Remember the time
      digitalWrite(ledPin, ledState);   // Update the actual LED
    }
  }
};

class Toggler
{
  // Class Member Variables
  // These are initialized at startup
  int led1Pin;      // the number of the first LED pin
  int led2Pin;      // the number of the second LED pin
  long OnTime;     // milliseconds of on-time
  long OffTime;    // milliseconds of off-time
 
  // These maintain the current state
  int ledState;                 // ledState used to set the LED
  unsigned long previousMillis;   // will store last time LED was updated
 
  // Constructor - creates a Flasher 
  // and initializes the member variables and state
  public:
  Toggler(int pin1, int pin2, long on, long off)
  {
  led1Pin = pin1;
  led2Pin = pin2;
  pinMode(led1Pin, OUTPUT);     
  pinMode(led2Pin, OUTPUT);
    
  OnTime = on;
  OffTime = off;
  
  ledState = LOW; 
  previousMillis = 0;
  }
 
  void Update(unsigned long currentMillis)
  {
    if((ledState == HIGH) && (currentMillis - previousMillis >= OnTime))
    {
      ledState = LOW;  // Turn it off
      previousMillis = currentMillis;  // Remember the time
      digitalWrite(led1Pin, ledState);  // Update the actual LED
      digitalWrite(led2Pin, HIGH);      // toggle the other LED on
    }
    else if ((ledState == LOW) && (currentMillis - previousMillis >= OffTime))
    {
      ledState = HIGH;  // turn it on
      previousMillis = currentMillis;   // Remember the time
      digitalWrite(led2Pin, LOW);       // Toggle the other LED OFF
      digitalWrite(led1Pin, ledState);   // Update the actual LED
    }
  }
};


Fader led1(0, 1, 1, 35);  // pin, fadeIncrement, fadeSpeed [millis], maxBrightness (<=255)
Flasher led2 (4, 750, 250); // pin, ontime, offtime [millis]
Toggler led3 (3, 2, 250, 250); // pin1, pin2, ontime, offtime [millis]


void setup() 
{ 

  
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
  led2.Update(currentMillis);
  led3.Update(currentMillis);

} 
 
void loop()
{
}
