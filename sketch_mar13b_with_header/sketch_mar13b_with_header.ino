/*
Project Prepared by Mitesh Khedekar and Omkar Tulaskar.
This Project does not involve use of LCD. We use the Serial monitor for 
user interaction.
Hardware used:      Arduino Uno
                    R305 fingerprint Sensor Module    

Problems in this code : large no. of global variables are being used.
                                   
*/

#include "Arduino.h"
#ifdef __AVR__
  #include <SoftwareSerial.h>
#endif

#define FINGERPRINT_OK 0x00
#define FINGERPRINT_PACKETRECIEVEERR 0x01
#define FINGERPRINT_NOFINGER 0x02
#define FINGERPRINT_IMAGEFAIL 0x03
#define FINGERPRINT_IMAGEMESS 0x06
#define FINGERPRINT_FEATUREFAIL 0x07
#define FINGERPRINT_NOMATCH 0x08
#define FINGERPRINT_NOTFOUND 0x09
#define FINGERPRINT_ENROLLMISMATCH 0x0A
#define FINGERPRINT_BADLOCATION 0x0B
#define FINGERPRINT_DBRANGEFAIL 0x0C
#define FINGERPRINT_UPLOADFEATUREFAIL 0x0D
#define FINGERPRINT_PACKETRESPONSEFAIL 0x0E
#define FINGERPRINT_UPLOADFAIL 0x0F
#define FINGERPRINT_DELETEFAIL 0x10
#define FINGERPRINT_DBCLEARFAIL 0x11
#define FINGERPRINT_PASSFAIL 0x13
#define FINGERPRINT_INVALIDIMAGE 0x15
#define FINGERPRINT_FLASHERR 0x18
#define FINGERPRINT_INVALIDREG 0x1A
#define FINGERPRINT_ADDRCODE 0x20
#define FINGERPRINT_PASSVERIFY 0x21

#define FINGERPRINT_STARTCODE 0xEF01

#define FINGERPRINT_COMMANDPACKET 0x1
#define FINGERPRINT_DATAPACKET 0x2
#define FINGERPRINT_ACKPACKET 0x7
#define FINGERPRINT_ENDDATAPACKET 0x8

#define FINGERPRINT_TIMEOUT 0xFF
#define FINGERPRINT_BADPACKET 0xFE

#define FINGERPRINT_GETIMAGE 0x01
#define FINGERPRINT_IMAGE2TZ 0x02
#define FINGERPRINT_REGMODEL 0x05
#define FINGERPRINT_STORE 0x06
#define FINGERPRINT_LOAD 0x07
#define FINGERPRINT_UPLOAD 0x08
#define FINGERPRINT_DELETE 0x0C
#define FINGERPRINT_EMPTY 0x0D
#define FINGERPRINT_VERIFYPASSWORD 0x13
#define FINGERPRINT_HISPEEDSEARCH 0x1B
#define FINGERPRINT_TEMPLATECOUNT 0x1D



//modified part

#define FINGERPRINT_UPLOADIMAGE 0x0A
#define FINGERPRINT_DOWNLOADIMAGE 0x0B
//#define FINGERPRINT_DEBUG 

#define DEFAULTTIMEOUT 5000  // milliseconds


class Adafruit_Fingerprint {
 public:
#ifdef __AVR__
  Adafruit_Fingerprint(SoftwareSerial *);
#endif
  Adafruit_Fingerprint(HardwareSerial *);

  void begin(uint16_t baud);

  boolean verifyPassword(void);
  uint8_t getImage(void);
  uint8_t image2Tz(uint8_t slot = 1);
  uint8_t createModel(void);

  uint8_t emptyDatabase(void);
  uint8_t storeModel(uint16_t id);
  uint8_t loadModel(uint16_t id);
  uint8_t getModel(void);
  uint8_t deleteModel(uint16_t id);
  uint8_t fingerFastSearch(void);
  uint8_t getTemplateCount(void);
  
  uint8_t uploadimage(uint16_t id);
  uint8_t downloadimage(uint16_t id);
  
  void writePacket(uint32_t addr, uint8_t packettype, uint16_t len, uint8_t *packet);
  uint8_t getReply(uint8_t packet[], uint16_t timeout=DEFAULTTIMEOUT);

  uint16_t fingerID, confidence, templateCount;

 private: 
  uint32_t thePassword;
  uint32_t theAddress;
    uint8_t recvPacket[20];

  Stream *mySerial;
#ifdef __AVR__
  SoftwareSerial *swSerial;
#endif
  HardwareSerial *hwSerial;
};

/////////////////////////////////////////////////////////////


//#include "Adafruit_Fingerprint.h"
#ifdef __AVR__
    #include <util/delay.h>
    #include <SoftwareSerial.h>
#endif

//#define FINGERPRINT_DEBUG

#ifdef __AVR__
Adafruit_Fingerprint::Adafruit_Fingerprint(SoftwareSerial *ss) {
  thePassword = 0;
  theAddress = 0xFFFFFFFF;

  hwSerial = NULL;
  swSerial = ss;
  mySerial = swSerial;
}
#endif

Adafruit_Fingerprint::Adafruit_Fingerprint(HardwareSerial *ss) {
  thePassword = 0;
  theAddress = 0xFFFFFFFF;

#ifdef __AVR__
  swSerial = NULL;
#endif
  hwSerial = ss;
  mySerial = hwSerial;
}

void Adafruit_Fingerprint::begin(uint16_t baudrate) {
  delay(1000);  // one second delay to let the sensor 'boot up'

  if (hwSerial) hwSerial->begin(baudrate);
#ifdef __AVR__
  if (swSerial) swSerial->begin(baudrate);
#endif
}

boolean Adafruit_Fingerprint::verifyPassword(void) {
  uint8_t packet[] = {FINGERPRINT_VERIFYPASSWORD, 
                      (thePassword >> 24), (thePassword >> 16),
                      (thePassword >> 8), thePassword};
  writePacket(theAddress, FINGERPRINT_COMMANDPACKET, 7, packet);
  uint8_t len = getReply(recvPacket);
  
  if ((len == 1) && (recvPacket[0] == FINGERPRINT_ACKPACKET) && (recvPacket[1] == FINGERPRINT_OK))
    return true;

/*
  Serial.print("\nGot packet type "); Serial.print(packet[0]);
  for (uint8_t i=1; i<len+1;i++) {
    Serial.print(" 0x");
    Serial.print(packet[i], HEX);
  }
  */
  return false;
}

uint8_t Adafruit_Fingerprint::getImage(void) {
  uint8_t packet[] = {FINGERPRINT_GETIMAGE};
  writePacket(theAddress, FINGERPRINT_COMMANDPACKET, 3, packet);
  uint8_t len = getReply(recvPacket);
  
  if ((len != 1) && (recvPacket[0] != FINGERPRINT_ACKPACKET))
   return -1;
  return recvPacket[1];
}

uint8_t Adafruit_Fingerprint::image2Tz(uint8_t slot) {
  uint8_t packet[] = {FINGERPRINT_IMAGE2TZ, slot};
  writePacket(theAddress, FINGERPRINT_COMMANDPACKET, sizeof(packet)+2, packet);
  uint8_t len = getReply(recvPacket);
  
  if ((len != 1) && (recvPacket[0] != FINGERPRINT_ACKPACKET))
   return -1;
  return recvPacket[1];
}


uint8_t Adafruit_Fingerprint::createModel(void) {
  uint8_t packet[] = {FINGERPRINT_REGMODEL};
  writePacket(theAddress, FINGERPRINT_COMMANDPACKET, sizeof(packet)+2, packet);
  uint8_t len = getReply(recvPacket);
  
  if ((len != 1) && (recvPacket[0] != FINGERPRINT_ACKPACKET))
   return -1;
  return recvPacket[1];
}


uint8_t Adafruit_Fingerprint::storeModel(uint16_t id) {
  uint8_t packet[] = {FINGERPRINT_STORE, 0x01, id >> 8, id & 0xFF};
  writePacket(theAddress, FINGERPRINT_COMMANDPACKET, sizeof(packet)+2, packet);
  uint8_t len = getReply(recvPacket);
  
  if ((len != 1) && (recvPacket[0] != FINGERPRINT_ACKPACKET))
   return -1;
  return recvPacket[1];
}
    
//read a fingerprint template from flash into Char Buffer 1
uint8_t Adafruit_Fingerprint::loadModel(uint16_t id) {
    uint8_t packet[] = {FINGERPRINT_LOAD, 0x01, id >> 8, id & 0xFF};
    writePacket(theAddress, FINGERPRINT_COMMANDPACKET, sizeof(packet)+2, packet);
    uint8_t len = getReply(recvPacket);
    
    if ((len != 1) && (recvPacket[0] != FINGERPRINT_ACKPACKET))
        return -1;
    return recvPacket[1];
}

//transfer a fingerprint template from Char Buffer 1 to host computer
uint8_t Adafruit_Fingerprint::getModel(void) {
    uint8_t packet[] = {FINGERPRINT_UPLOAD, 0x01};
    writePacket(theAddress, FINGERPRINT_COMMANDPACKET, sizeof(packet)+2, packet);
    uint8_t len = getReply(recvPacket);
    
    if ((len != 1) && (recvPacket[0] != FINGERPRINT_ACKPACKET))
        return -1;
    return recvPacket[1];
}
    
uint8_t Adafruit_Fingerprint::deleteModel(uint16_t id) {
    uint8_t packet[] = {FINGERPRINT_DELETE, id >> 8, id & 0xFF, 0x00, 0x01};
    writePacket(theAddress, FINGERPRINT_COMMANDPACKET, sizeof(packet)+2, packet);
    uint8_t len = getReply(recvPacket);
        
    if ((len != 1) && (recvPacket[0] != FINGERPRINT_ACKPACKET))
        return -1;
    return recvPacket[1];
}

uint8_t Adafruit_Fingerprint::emptyDatabase(void) {
  uint8_t packet[] = {FINGERPRINT_EMPTY};
  writePacket(theAddress, FINGERPRINT_COMMANDPACKET, sizeof(packet)+2, packet);
  uint8_t len = getReply(recvPacket);
  
  if ((len != 1) && (recvPacket[0] != FINGERPRINT_ACKPACKET))
   return -1;
  return recvPacket[1];
}

uint8_t Adafruit_Fingerprint::fingerFastSearch(void) {
  fingerID = 0xFFFF;
  confidence = 0xFFFF;
  // high speed search of slot #1 starting at page 0x0000 and page #0x00A3 
  uint8_t packet[] = {FINGERPRINT_HISPEEDSEARCH, 0x01, 0x00, 0x00, 0x00, 0xA3};
  writePacket(theAddress, FINGERPRINT_COMMANDPACKET, sizeof(packet)+2, packet);
  uint8_t len = getReply(recvPacket);
  
  if ((len != 1) && (recvPacket[0] != FINGERPRINT_ACKPACKET))
   return -1;

  fingerID = recvPacket[2];
  fingerID <<= 8;
  fingerID |= recvPacket[3];
  
  confidence = recvPacket[4];
  confidence <<= 8;
  confidence |= recvPacket[5];
  
  return recvPacket[1];
}

uint8_t Adafruit_Fingerprint::getTemplateCount(void) {
  templateCount = 0xFFFF;
  // get number of templates in memory
  uint8_t packet[] = {FINGERPRINT_TEMPLATECOUNT};
  writePacket(theAddress, FINGERPRINT_COMMANDPACKET, sizeof(packet)+2, packet);
  uint8_t len = getReply(recvPacket);
  
  if ((len != 1) && (recvPacket[0] != FINGERPRINT_ACKPACKET))
   return -1;

  templateCount = recvPacket[2];
  templateCount <<= 8;
  templateCount |= recvPacket[3];
  
  return recvPacket[1];
}







uint8_t Adafruit_Fingerprint::downloadimage(uint16_t id)
{
   uint8_t packet[] = {FINGERPRINT_DOWNLOADIMAGE};
  writePacket(theAddress, FINGERPRINT_COMMANDPACKET, sizeof(packet)+2, packet);
  uint8_t len = getReply(recvPacket);
  
  if ((len != 1) && (recvPacket[0] != FINGERPRINT_ACKPACKET))
   return -1;
  return recvPacket[1];
}


uint8_t Adafruit_Fingerprint::uploadimage(uint16_t id)
{
   uint8_t packet[] = {FINGERPRINT_UPLOADIMAGE, 0x01, id >> 8, id & 0xFF};
    writePacket(theAddress, FINGERPRINT_COMMANDPACKET, sizeof(packet)+2, packet);
    uint8_t len = getReply(recvPacket);
    
    if ((len != 1) && (recvPacket[0] != FINGERPRINT_ACKPACKET))
        return -1;
    return recvPacket[1];
}




void Adafruit_Fingerprint::writePacket(uint32_t addr, uint8_t packettype, 
               uint16_t len, uint8_t *packet) {
#ifdef FINGERPRINT_DEBUG
  Serial.print("---> 0x");
  Serial.print((uint8_t)(FINGERPRINT_STARTCODE >> 8), HEX);
  Serial.print(" 0x");
  Serial.print((uint8_t)FINGERPRINT_STARTCODE, HEX);
  Serial.print(" 0x");
  Serial.print((uint8_t)(addr >> 24), HEX);
  Serial.print(" 0x");
  Serial.print((uint8_t)(addr >> 16), HEX);
  Serial.print(" 0x");
  Serial.print((uint8_t)(addr >> 8), HEX);
  Serial.print(" 0x");
  Serial.print((uint8_t)(addr), HEX);
  Serial.print(" 0x");
  Serial.print((uint8_t)packettype, HEX);
  Serial.print(" 0x");
  Serial.print((uint8_t)(len >> 8), HEX);
  Serial.print(" 0x");
  Serial.print((uint8_t)(len), HEX);
#endif
 
#if ARDUINO >= 100
  mySerial->write((uint8_t)(FINGERPRINT_STARTCODE >> 8));
  mySerial->write((uint8_t)FINGERPRINT_STARTCODE);
  mySerial->write((uint8_t)(addr >> 24));
  mySerial->write((uint8_t)(addr >> 16));
  mySerial->write((uint8_t)(addr >> 8));
  mySerial->write((uint8_t)(addr));
  mySerial->write((uint8_t)packettype);
  mySerial->write((uint8_t)(len >> 8));
  mySerial->write((uint8_t)(len));
#else
  mySerial->print((uint8_t)(FINGERPRINT_STARTCODE >> 8), BYTE);
  mySerial->print((uint8_t)FINGERPRINT_STARTCODE, BYTE);
  mySerial->print((uint8_t)(addr >> 24), BYTE);
  mySerial->print((uint8_t)(addr >> 16), BYTE);
  mySerial->print((uint8_t)(addr >> 8), BYTE);
  mySerial->print((uint8_t)(addr), BYTE);
  mySerial->print((uint8_t)packettype, BYTE);
  mySerial->print((uint8_t)(len >> 8), BYTE);
  mySerial->print((uint8_t)(len), BYTE);
#endif
  
  uint16_t sum = (len>>8) + (len&0xFF) + packettype;
  for (uint8_t i=0; i< len-2; i++) {
#if ARDUINO >= 100
    mySerial->write((uint8_t)(packet[i]));
#else
    mySerial->print((uint8_t)(packet[i]), BYTE);
#endif
#ifdef FINGERPRINT_DEBUG
    Serial.print(" 0x"); Serial.print(packet[i], HEX);
#endif
    sum += packet[i];
  }
#ifdef FINGERPRINT_DEBUG
  //Serial.print("Checksum = 0x"); Serial.println(sum);
  Serial.print(" 0x"); Serial.print((uint8_t)(sum>>8), HEX);
  Serial.print(" 0x"); Serial.println((uint8_t)(sum), HEX);
#endif
#if ARDUINO >= 100
  mySerial->write((uint8_t)(sum>>8));
  mySerial->write((uint8_t)sum);
#else
  mySerial->print((uint8_t)(sum>>8), BYTE);
  mySerial->print((uint8_t)sum, BYTE);
#endif
}


uint8_t Adafruit_Fingerprint::getReply(uint8_t packet[], uint16_t timeout) {
    uint8_t reply[20], idx;
  uint16_t timer=0;
  
  idx = 0;
#ifdef FINGERPRINT_DEBUG
  Serial.print("<--- ");
#endif
while (true) {
    while (!mySerial->available()) {
      delay(1);
      timer++;
      if (timer >= timeout) return FINGERPRINT_TIMEOUT;
    }
    // something to read!
    reply[idx] = mySerial->read();
#ifdef FINGERPRINT_DEBUG
    Serial.print(" 0x"); Serial.print(reply[idx], HEX);
#endif
    if ((idx == 0) && (reply[0] != (FINGERPRINT_STARTCODE >> 8)))
      continue;
    idx++;
    
    // check packet!
    if (idx >= 9) {
      if ((reply[0] != (FINGERPRINT_STARTCODE >> 8)) ||
          (reply[1] != (FINGERPRINT_STARTCODE & 0xFF)))
          return FINGERPRINT_BADPACKET;
      uint8_t packettype = reply[6];
      //Serial.print("Packet type"); Serial.println(packettype);
      uint16_t len = reply[7];
      len <<= 8;
      len |= reply[8];
      len -= 2;
      //Serial.print("Packet len"); Serial.println(len);
      if (idx <= (len+10)) continue;
      packet[0] = packettype;      
      for (uint8_t i=0; i<len; i++) {
        packet[1+i] = reply[9+i];
      }
#ifdef FINGERPRINT_DEBUG
      Serial.println();
#endif
      return len;
    }
  }
}















#include <SoftwareSerial.h>
#include<EEPROM.h>
uint8_t menu1;
 uint8_t id_no= 0;
uint8_t enrollment_menu;
uint8_t results_menu;
uint8_t vote;


int repeat=0;
//int voted_id[]={};
int address=0;

uint8_t BJP=0,Congress=0,Shivsena=0,MNS=0;
SoftwareSerial mySerial(2,3);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
void setup() {


  Serial.begin(9600);
  mySerial.begin(57600);
  Serial.println("Welcome to biometric Voting Machine.....");
  delay(1000);

   if(finger.verifyPassword())
   {
    Serial.println("Biometric Machine Connected");
    spacing();
   }

   else 
   {
    Serial.println("Failed to detect Biometric Machine");
    while(1);
   }
}

void spacing()
{
   int i=0;
    while(i<=3)
    {
      Serial.println(" ");
      i++;
    }
}

uint8_t user_input()                //function to get the input from the user
{
  uint8_t num = 0;
  boolean validnum = false; 
  while (1) {
    while (! Serial.available());
    char c = Serial.read();
    if (isdigit(c)) 
    {
       num *= 10;
       num += c - '0';
       validnum = true;
    } 
    else if (validnum) 
    {
      return num;
    }
  }
}


void id()
{
 
 // Serial.println(id_no);
//  id_no++;

id_no=user_input();
  
}
void loop() {

First_Menu();

 
}

void First_Menu()
{
  Serial.println("Please Select one of the Options.");
  Serial.println("1. Biometric Enrollment                 2. Proceed For VOTING");
  Serial.println("Enter the passcode to check the results");
  
  spacing();
  menu1=user_input();

  if(menu1==1)
  {
     Enrollment();
  }
  else if(menu1==2)
  {
    VOTING();
  }
  else if(menu1==32)
  {
    Passcode();
  }
  else
  {
    Serial.println("Invalid Input");
    First_Menu();
  }
}

void Enrollment()
{
   Serial.println("Biometric Enrollment");
   Serial.println("Enter your id number");
    id();
    Serial.println("Please place your finger");
   while(!getFingerprintEnroll()); 
}

void VOTING()
{
  spacing();
  Serial.println("Please place your finger to check in the database");
while(1){
getFingerprintID();
}
}

void Passcode()
{
  Serial.println("1.Show Results                          2.Clear Results database");
  Serial.println("3.Clear the enrollment database         4.Exit");
  results_menu=user_input();
  switch(results_menu)
  {
    case 1:
    Serial.println("Showing the results of votes counted uptill now");
            show_results();
            break;
    case 2:
    Serial.println("Clearing the database...");
            clear_results();
            break;
    case 3: 
            Serial.println("Clearing the voters identification database....");
           
             clear_voters();
           
            break;
    case 4:
    
    Serial.println("Exiting....");
             First_Menu();
            break;
    default:
          Serial.println("Invalid Input ,Exiting the Results menu");
         First_Menu();
          break;                        
  }
  
}


void show_results()
{
      
Serial.println(EEPROM.get(address,BJP));
      
Serial.println( EEPROM.get(address+2,Shivsena));


 Serial.println(EEPROM.get(address+4,Congress));

   Serial.println(EEPROM.get(address+8,MNS));

   First_Menu();
}

void clear_results()
{

  for(int i=0;i<=10;i++)
  {
    EEPROM.write(i,0);
    
  }
    First_Menu();
}

void clear_voters()
{
  int i;
for(i=16;i<40;i++)
{
  EEPROM.write(i,0);
}
uint8_t  p=finger.emptyDatabase();
  switch(p){
    case  FINGERPRINT_OK: Serial.println("SUCCESS...");
      break;
    case FINGERPRINT_DBCLEARFAIL: Serial.println("Failed to clear the database");
    break;
    case FINGERPRINT_PACKETRECIEVEERR: Serial.println("Error receiving the packet");
    break;
      
  }

  First_Menu();
}

uint8_t getFingerprintEnroll()
{
    int p = -1;
  Serial.print("Waiting for valid finger to enroll"); 
 
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      //Serial.println(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      break;
    default:
      Serial.println("Unknown error");
      break;
    }
  }

    p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }
  


Serial.println("Remove finger");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  Serial.print("ID "); Serial.println(id_no);
  p = -1;
  Serial.println("Place same finger again");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.print(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      break;
    default:
      Serial.println("Unknown error");
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }
  
  // OK converted!
  Serial.print("Creating model for #");  Serial.println(((id_no)-1));
  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println("Prints matched!");
    
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("Fingerprints did not match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }   


  Serial.print("ID "); Serial.println(id_no);
  p = finger.storeModel(id_no);
  if (p == FINGERPRINT_OK) {
    Serial.println("Stored!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not store in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }   

  spacing();
  Second_menu_1();
 
}


uint8_t getFingerprintID() {

   

   int p = -1;
 // Serial.print("Waiting for valid finger to enroll as #"); Serial.println(id);

  while (p != FINGERPRINT_OK)
  {
  //uint8_t p = finger.getImage();
  p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      //Serial.println("No finger detected");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK success!

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }
  
  // OK converted!
  
   //Searching the database
  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println("Found a print match!");
    repeat=0;
    start_voting();
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("Did not find a match");
    repeat++;
    if(repeat==5)
    {
        spacing();
        Serial.println("Sorry your name not in the voters list.");
        First_Menu(); 
    }
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }   



  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID); 
  Serial.print(" with confidence of "); Serial.println(finger.confidence); 


  
}

}


void start_voting()
{
   spacing();
   Serial.print("Found ID #"); Serial.print(finger.fingerID); 
   Serial.print(" with confidence of "); Serial.println(finger.confidence); 
   if(((finger.fingerID)+16)==EEPROM.length())
   {
    Serial.println("ID number out of bounds");
    
   }
   if(EEPROM.read((finger.fingerID)+16)==finger.fingerID)
   {
    Serial.println("Sorry You have already voted");
    First_Menu();
   }
   else{
   EEPROM.write((finger.fingerID)+16,finger.fingerID);
   spacing();
  Serial.println("Select your party");
  Serial.println("1.BJP");
  Serial.println("2.Shivsena");
  Serial.println("3.Congress");Serial.println("4.MNS");
  vote=user_input();
  if(vote>4)
  {
    Serial.println("Invalid input");
    
  }
  else
  {
    Serial.println("Your response has been recorded");
    switch(vote)
    {
      case 1: BJP++;
      EEPROM.write(address,BJP);
              break;
      case 2: Shivsena++;
      EEPROM.write(address+2,Shivsena);
              break;
      case 3: Congress++;
      EEPROM.write(address+4,Congress);
              break;
      case 4: MNS++;
      EEPROM.write(address+8,MNS);
              break;                      
    }
  }

  First_Menu();
   }
  
}

void Second_menu_1()
{

  
   Serial.println("1.Add more users                2.Exit");
  enrollment_menu=user_input();

 if(enrollment_menu==2)
 {
  //go to the main menu
  First_Menu();
 }
 else if(enrollment_menu==1)
 {
    Enrollment();
 }
 else
 {
  Serial.println("Invalid input");
 }
  
}



















