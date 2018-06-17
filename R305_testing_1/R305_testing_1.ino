#include <LiquidCrystal.h>

#include <SoftwareSerial.h>
#define DEBUG 0

SoftwareSerial mySerial(2, 3); // RX, TX

/************************************************************************
Communication Packet
************************************************************************/
byte addFingerPrint0[] = {0xEF01, 0xFFFFFFFF, 01, 03, 01, 05 }; 
//byte receiveCorrect[] = {0x4D, 0x58, 0x30, 0x01, 0x01, 0xD7}; 
byte receiveCorrect[] = {0xEF01, 0xFFFFFFFF, 0x07, 0x03, 0x00, 0x0A }; 
byte parameterErrorAdd[] = {0xEF01, 0xFFFFFFFF, 0x07, 0x03, 0x01 ,0x0B }; 
byte processFailureAdd[] = {0xEF01, 0xFFFFFFFF, 0x07, 0x03, 0x02 ,0x0C}; 
byte timeOutAdd[] = {0xEF01, 0xFFFFFFFF, 0x07, 0x03, 0x03 ,0x0D}; 

/************************************************************************
Variables
************************************************************************/
byte respond[10], operation[10], operationSecond[10]; 
byte a,b,c,d,e,f,g,h;
int A = 0;

void setup() 
{
  mySerial.begin(57600);
  Serial.begin(9600);
}

void loop() 
{
  if(A == 0)
  {
    addFinger0();
    if(correctRespond())
      operationStart();
    delay(1000); // a delay for next operation 
  }
} 

void addFinger0() {
 
  Serial.println("Add fingerprint at ID #0");
  mySerial.write(addFingerPrint0, sizeof(addFingerPrint0));
  Serial.println("Requested");
  
}

bool correctRespond()
{
    Serial.println("CorrectRespond");
  while(mySerial.available() <= 0) {}; //wait for incoming data
  delay(10); // give some time for all incoming bytes to be stored into serial buffer
  while(mySerial.available() > 6) // this is to make sure removal of respond from previous command
    mySerial.read();
     
  memset(respond, '\0', 6); // clear respond buffer
  mySerial.readBytes(respond, 6); //store the remaining data to respond buffer 
  
#if DEBUG  
  for(byte x=0; x<sizeof(respond); x++)
    Serial.print(respond[x], HEX);
  Serial.println();
#endif
 
  a = memcmp(receiveCorrect, respond, sizeof(receiveCorrect));
  if (a == 0)
  {
    Serial.println("Receive Correct");
    return 1;
  }
  else
  {
    Serial.println("Error!");
    return 0;
  }
}

void operationStart()
{
  while(mySerial.available() <= 0) {}; //wait for incoming data
  memset(operation, '\0', 7); // clear operation buffer
  mySerial.readBytes(operation, 7); //store data

#if DEBUG  
  for(byte x=0; x<6; x++)
    Serial.print(operation[x], HEX);
  Serial.println();
#endif
b = memcmp(receiveCorrect, operation, sizeof(receiveCorrect));
  c = memcmp(processFailureAdd, operation, sizeof(processFailureAdd));
  d = memcmp(parameterErrorAdd, operation, sizeof(parameterErrorAdd));
  e = memcmp(timeOutAdd, operation, sizeof(timeOutAdd)); 

  if (b == 0) 
  {
    Serial.println("First Fingerprint Scanned"); 
    while(mySerial.available() <= 0) {}; //wait for incoming data
    memset(operation, '\0', 7); //clear operation buffer
    mySerial.readBytes(operation, 7); //store data to buffer

#if DEBUG  
  for(byte x=0; x<6; x++)
    Serial.print(operation[x], HEX);
  Serial.println();
#endif
     
    f = memcmp(receiveCorrect, operation, sizeof(receiveCorrect));
    g = memcmp(processFailureAdd, operation, sizeof(processFailureAdd)); 
    h = memcmp(timeOutAdd, operation, sizeof(timeOutAdd));
    
    if (f == 0)
    {
      Serial.println("Second Fingerprint Scanned");
      Serial.println("Fingerprint Added"); 
      A = 1;
    }
    else 
    {
      if (g == 0)
        Serial.println("Fingerprint Process Failure"); 
      else if (h == 0) 
        Serial.println("Time Out");
        
      Serial.println("Starting new operation!");
    }
      
  }

  else
  {
    if (c == 0) 
     Serial.println("Fingerprint Process Failure");
  
    else if (d == 0) 
     Serial.println("Parameter Error");
  
    else if (e == 0) 
     Serial.println("Time Out");
     
    Serial.println("Starting new operation!");
  }
}

