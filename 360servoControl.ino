#include <Servo.h>
#include "WProgram.h"

class servo360
{
  private:
    uint8_t pin;
    uint8_t low, mid, high;
    int8_t spd;
    Servo S;
    
  public:
    servo360(void);
    void attach(uint8_t _pin);
    void tune(uint8_t _low, uint8_t _mid, uint8_t _high);
    void write(int8_t newSpd);
};

servo360::servo360(void)
{
  spd = 0;
  low=0, mid=90, high=180;
}  
  
void servo360::attach(uint8_t _pin)
{
  pin = _pin;
}

// Use this if the servo midpoint is not at 90deg
void servo360::tune(uint8_t _low, uint8_t _mid, uint8_t _high)
{
  low  = _low;
  mid  = _mid;
  high = _high;
}

void servo360::write(int8_t newSpd)
{
  if (newSpd == spd) return;
  
  if (spd == 0)
    S.attach(pin);
  
  spd = newSpd;

  if (spd == 0)
  {
    S.detach();
    return;
  }

  uint8_t u;
  if (spd < 0)
    u = map(spd, -100,   0, low, mid);
  else
    u = map(spd,    0, 100, mid, high);    
  S.write(u);
}



servo360 s;
  
void setup() 
{ 
  Serial.begin(9600);
  s.attach(9);
  s.tune(0,80,180);
    
  Serial.println("READY!");  
} 


void loop()
{
  static int8_t x=0;
  
  if (!Serial.available())
    return;

  switch (Serial.read())
  {
     case 'a': x--;break;
     case 's': x++;break;     
  }

  s.write(x);

  Serial.print("x:");
  Serial.print(x);
}
