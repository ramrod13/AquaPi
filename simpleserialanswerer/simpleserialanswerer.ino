///////////////////////////////////////////////////////////////////////////
//
// This program implements a ring buffer and periodically reads from a DHT11
// It records those data and provides a temperature data averaged over some window.
//
//
//


#include <dht.h>
dht DHT;

#define DHT11_PIN 2
#define RING_BUF_SIZE 60
int humidity=0;
int temperature =0;
long previousMillis = 0;        // will store last time LED was updated
long interval = 15000;  
int head;  //first, oldest value
int tail;  //last, newest value in buffer
int fillcount=0;
int hum[RING_BUF_SIZE];
int tem[RING_BUF_SIZE];
   float tempout=0;
   float humout=0;

void setup()
{
  // initialize the serial communication:
  Serial.begin(9600);
  // initialize the ledPin as an output:
  pinMode(13, OUTPUT);
  checkDHT11();
  for(int i=0;i<RING_BUF_SIZE;i++){  //fill buffer with initial values
    hum[i]=humidity;
    tem[i]=temperature;
  }
  fillcount=0;
}

void loop() {
   unsigned long currentMillis = millis();
   if(currentMillis - previousMillis > interval) {
        // save the last time you blinked the LED 
        previousMillis = currentMillis;   
        checkDHT11();
        updateBuffers();
     }   
  byte command;    //declare a variable to hold our command
  if(Serial.available()){    //check and see if a message has been sent
    command=Serial.read();    //if so, read the message into our variable
    switch( command){        //run different commands based on the value of the message, send a message for undefined commands
      case 'h':              //do something when an "h" is received
        calculateValues();
        Serial.print("Humidity, ");
        Serial.println(humout);
//        readHumidity();
        //checkDHT11();
        break;
      case 't':              //do something when an "h" is received
        calculateValues();
        Serial.print("Temperature, ");
        Serial.println(tempout);
        //checkDHT11();
//        readHumidity();
        break;
      case 'c':
        checkDHT11();
        break;
      case 'p':
        Serial.print("Tail: ");
        Serial.print(tail);
        Serial.print("\tHead: ");
        Serial.print(head);
        Serial.print("\tRING_BUF_SIZE: ");
        Serial.println(RING_BUF_SIZE);
        for(int i= 0;i < RING_BUF_SIZE; i++){
          Serial.print(tem[i]);
          Serial.print(", ");
        }
        Serial.println();
        for(int i= 0;i < RING_BUF_SIZE; i++){
          Serial.print(hum[i]);
          Serial.print(", ");
        }
        Serial.println();
        break;
        case 'm':
          Serial.println(calcVal(4));
          break;
      default:               //send a message for undefined commands
        Serial.print("Nothing to do for: ");
        Serial.println(char(command));
        break;      
    }
  }
  //delay(2000);
}

void updateBuffers(){
     tail+=1 ; //%RING_BUF_SIZE
     hum[tail%RING_BUF_SIZE]=DHT.humidity;
     tem[tail%RING_BUF_SIZE]=DHT.temperature; 
     if(fillcount > RING_BUF_SIZE){
       head+=1;
     }
     fillcount++;
}
float calcVal(int num){
  float tempo=0;
  float humo=0;
  int ttail=tail;
   for(int i=ttail;i>ttail-num;i--){
     tempo+=tem[i%RING_BUF_SIZE];
     humo+=hum[i%RING_BUF_SIZE];
   }
   tempout=tempo/(num);
//   humout=humo/(num);
}
void calculateValues(){
   float tempo=0;
   float humo=0;
   for(int i=tail;i>head;i--){
     tempo+=tem[i%RING_BUF_SIZE];
     humo+=hum[i%RING_BUF_SIZE];
   }
   tempout=tempo/(tail-head);
   humout=humo/(tail-head);
}

void checkDHT11(){
   int chk = DHT.read11(DHT11_PIN);
     //Serial.println("DHT11, \t");
//  int chk = DHT.read11(DHT11_PIN);
  switch (chk)
  {
    case DHTLIB_OK:  
		//Serial.print("OK,\t"); 
		break;
    case DHTLIB_ERROR_CHECKSUM: 
		Serial.print("Checksum error,\t"); 
		break;
    case DHTLIB_ERROR_TIMEOUT: 
		Serial.print("Time out error,\t"); 
		break;
    default: 
		Serial.print("Unknown error,\t"); 
		break;
  }
  humidity=DHT.humidity;
  temperature=DHT.temperature;
  
}

void readHumidity(){
  
  
  
  
}
