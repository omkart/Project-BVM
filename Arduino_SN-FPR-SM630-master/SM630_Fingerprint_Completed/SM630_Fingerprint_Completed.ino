#include <LiquidCrystal.h>
#define RIGHT_BUTTON_MENU 0
#define UP_BUTTON_MENU 1
#define DOWN_BUTTON_MENU 2
#define LEFT_BUTTON_MENU 3
#define SELECT_BUTTON_MENU 4

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

int analogPin = A0;
int buzzerPin = 11;      // the number of the LED pin
int adc_key_old;
int NUM_KEYS = 5;
int key=-1;
int adc_key_val[5] ={30, 150, 360, 535, 760};

// Variables will change:
int buttonPressed;
int currentState = 0;

/**********************************************
Command Code & Respond Code For Add Fingerprint
**********************************************/
byte addFingerPrint0[] = {0x4D, 0x58, 0x10, 0x03, 0x40, 0x00, 0x00, 0xF8}; // 8 bytes
byte addFingerPrint1[] = {0x4D, 0x58, 0x10, 0x03, 0x40, 0x00, 0x01, 0xF9}; // 8 bytes
byte addFingerPrint2[] = {0x4D, 0x58, 0x10, 0x03, 0x40, 0x00, 0x02, 0xFA}; // 8 bytes
byte addFingerPrint3[] = {0x4D, 0x58, 0x10, 0x03, 0x40, 0x00, 0x03, 0xFB}; // 8 bytes
byte addFingerPrint4[] = {0x4D, 0x58, 0x10, 0x03, 0x40, 0x00, 0x04, 0xFC}; // 8 bytes

byte operationSuccessAdd[] = {0x4D, 0x58, 0x30, 0x02, 0x40, 0x31, 0x48}; // 7 bytes
byte timeOutAdd[] = {0x4D, 0x58, 0x30, 0x02, 0x40, 0x33, 0x4A}; // 7 bytes
byte processFailureAdd[] = {0x4D, 0x58, 0x30, 0x02, 0x40, 0x34, 0x4B}; // 7 bytes
byte parameterErrorAdd[] = {0x4D, 0x58, 0x30, 0x02, 0x40, 0x35, 0x4C}; // 7 bytes

/*************************************************
Command Code & Respond Code For Delete Fingerprint
*************************************************/
byte deleteFingerPrint0[] = {0x4D, 0x58, 0x10, 0x03, 0x42, 0x00, 0x00, 0xFA}; // 8 bytes
byte deleteFingerPrint1[] = {0x4D, 0x58, 0x10, 0x03, 0x42, 0x00, 0x01, 0xFB}; // 8 bytes
byte deleteFingerPrint2[] = {0x4D, 0x58, 0x10, 0x03, 0x42, 0x00, 0x02, 0xFC}; // 8 bytes
byte deleteFingerPrint3[] = {0x4D, 0x58, 0x10, 0x03, 0x42, 0x00, 0x03, 0xFD}; // 8 bytes
byte deleteFingerPrint4[] = {0x4D, 0x58, 0x10, 0x03, 0x42, 0x00, 0x04, 0xFE}; // 8 bytes

byte operationSuccessDelete[] = {0x4D, 0x58, 0x30, 0x02, 0x42, 0x31, 0x4A}; // 7 bytes
byte parameterErrorDelete [] = {0x4D, 0x58, 0x30, 0x02, 0x42, 0x35, 0x4E}; // 7 bytes

/*************************************************
Command Code & Respond Code For Search Fingerprint
*************************************************/
byte searchFingerPrint[] = {0x4D, 0x58, 0x10, 0x05, 0x44, 0x00, 0x00, 0x00, 0x05, 0x03}; // 10 bytes

byte FingerPrint0[] = {0x4D, 0x58, 0x30, 0x04, 0x44, 0x39, 0x00, 0x00, 0x56}; // 10 bytes
byte FingerPrint1[] = {0x4D, 0x58, 0x30, 0x04, 0x44, 0x39, 0x00, 0x01, 0x57}; // 10 bytes
byte FingerPrint2[] = {0x4D, 0x58, 0x30, 0x04, 0x44, 0x39, 0x00, 0x02, 0x58}; // 10 bytes
byte FingerPrint3[] = {0x4D, 0x58, 0x30, 0x04, 0x44, 0x39, 0x00, 0x03, 0x59}; // 10 bytes
byte FingerPrint4[] = {0x4D, 0x58, 0x30, 0x04, 0x44, 0x39, 0x00, 0x04, 0x5A}; // 10 bytes

byte operationSuccessSearch[] = {0x4D, 0x58, 0x30, 0x02, 0x44, 0x31, 0x4C}; // 7 bytes
byte timeOutSearch[] = {0x4D, 0x58, 0x30, 0x02, 0x44, 0x33, 0x4E}; // 7 bytes
byte parameterErrorSearch[] = {0x4D, 0x58, 0x30, 0x02, 0x44, 0x35, 0x50}; // 7 bytes
byte processFailureSearch[] = {0x4D, 0x58, 0x30, 0x02, 0x44, 0x34, 0x4F}; // 7 bytes
byte noMatchFinger[] = {0x4D, 0x58, 0x30, 0x02, 0x44, 0x3A, 0x55}; // 7 bytes

/**********************************************************
Command Code & Respond Code For Empty Database Fingerprint
**********************************************************/
byte emptyFingerPrint[] = {0x4D, 0x58, 0x10, 0x01, 0x46, 0xFC}; // 6 bytes

byte operationSuccessEmpty[] = {0x4D, 0x58, 0x30, 0x02, 0x46, 0x31, 0x4E}; // 7 bytes

/**********************************************************
Command Code & Respond Code For Search Database Fingerprint
**********************************************************/
byte databaseFingerPrint0[] = {0x4D, 0x58, 0x10, 0x03, 0x4B, 0x00, 0x00,  0x03}; // 8 bytes command code
byte databaseFingerPrint1[] = {0x4D, 0x58, 0x10, 0x03, 0x4B, 0x00, 0x01,  0x04}; // 8 bytes command code
byte databaseFingerPrint2[] = {0x4D, 0x58, 0x10, 0x03, 0x4B, 0x00, 0x02,  0x05}; // 8 bytes command code
byte databaseFingerPrint3[] = {0x4D, 0x58, 0x10, 0x03, 0x4B, 0x00, 0x03,  0x06}; // 8 bytes command code
byte databaseFingerPrint4[] = {0x4D, 0x58, 0x10, 0x03, 0x4B, 0x00, 0x04,  0x07}; // 8 bytes command code

byte operationSuccessDatabase[] = {0x4D, 0x58, 0x30, 0x02, 0x4B, 0x37, 0x59}; // found, 7 bytes
byte operationFailureDatabase[] = {0x4D, 0x58, 0x30, 0x02, 0x4B, 0x38, 0x5A}; // not found, 7 bytes
byte notInRange[] = {0x4D, 0x58, 0x30, 0x02, 0x4B, 0x35, 0x57}; // not in range, 7 bytes

/**********************************************************
Fixed Respond Code
**********************************************************/
byte respondCorrect[] = {0x4D, 0x58, 0x30, 0x01, 0x01, 0xD7}; // 6 bytes
byte respond[6],operation[7],operationOK[7],operationFound[9],a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,z;

void setup ()
{
  Serial.begin(57600);
  lcd.begin(16, 2);

  menu();
  adc_key_old = analogRead(analogPin);       // store the unpress key value
  
  pinMode(buzzerPin, OUTPUT);      
}

void menu ()
{
  lcd.clear();
  lcd.setCursor(0,0);  
  lcd.print("FPrint Reader");
  lcd.setCursor(0,1);
  lcd.print("Mode : Up/Down");
}

void loop()
{
  buttonPressed = analogRead(analogPin);     // read ADC value
  buttonPressed = get_key(buttonPressed);
  delay(500);
/************************************************************
Function: Main Menu Display Operation Fingerprint
Flow    : Count Down & Count Up & Back Using Right Button
************************************************************/
  if (currentState == 0 && buttonPressed == DOWN_BUTTON_MENU || currentState == 2 && buttonPressed == UP_BUTTON_MENU
      || currentState == 6 && buttonPressed == RIGHT_BUTTON_MENU || currentState == 7 && buttonPressed == RIGHT_BUTTON_MENU
      || currentState == 8 && buttonPressed == RIGHT_BUTTON_MENU || currentState == 9 && buttonPressed == RIGHT_BUTTON_MENU
      || currentState == 10 && buttonPressed == RIGHT_BUTTON_MENU)
  {
    currentState = 1;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Add Fingerprint");
  }

  else if (currentState == 1 && buttonPressed == DOWN_BUTTON_MENU || currentState == 3 && buttonPressed == UP_BUTTON_MENU
          || currentState == 11 && buttonPressed == RIGHT_BUTTON_MENU || currentState == 12 && buttonPressed == RIGHT_BUTTON_MENU
          || currentState == 13 && buttonPressed == RIGHT_BUTTON_MENU || currentState == 14 && buttonPressed == RIGHT_BUTTON_MENU
          || currentState == 15 && buttonPressed == RIGHT_BUTTON_MENU)
  {
    currentState = 2;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Delete");
    lcd.setCursor(0,1);
    lcd.print("Fingerprint");
  }
  
  else if (currentState == 2 && buttonPressed == DOWN_BUTTON_MENU || currentState == 4 && buttonPressed == UP_BUTTON_MENU)
  {
    currentState = 3;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Search");
    lcd.setCursor(0,1);
    lcd.print("Fingerprint");
  }
  
  else if (currentState == 3 && buttonPressed == DOWN_BUTTON_MENU || currentState == 5 && buttonPressed == UP_BUTTON_MENU)
  {
    currentState = 4;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Empty Database");
  }
  
  else if (currentState == 4 && buttonPressed == DOWN_BUTTON_MENU || currentState == 16 && buttonPressed == RIGHT_BUTTON_MENU
           || currentState == 17 && buttonPressed == RIGHT_BUTTON_MENU || currentState == 18 && buttonPressed == RIGHT_BUTTON_MENU
           || currentState == 19 && buttonPressed == RIGHT_BUTTON_MENU || currentState == 20 && buttonPressed == RIGHT_BUTTON_MENU)
          
  {
    currentState = 5;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Search Database");
  }
/////////////////////////////////////////////////////////////
/************************************************************
Function: Add Fingerprint Address 1 - Address 5
Flow    : Count Down & Count Up
************************************************************/
  else if (currentState == 1 && buttonPressed == SELECT_BUTTON_MENU || currentState == 7 && buttonPressed == UP_BUTTON_MENU)
  {
    currentState = 6;
    lcd.setCursor(0,1);
    lcd.print("User ID 1");
  }
  
  else if (currentState == 6 && buttonPressed == DOWN_BUTTON_MENU || currentState == 8 && buttonPressed == UP_BUTTON_MENU)
  {
    currentState = 7;
    lcd.setCursor(0,1);
    lcd.print("User ID 2");
  }

  else if (currentState == 7 && buttonPressed == DOWN_BUTTON_MENU || currentState == 9 && buttonPressed == UP_BUTTON_MENU)
  {
    currentState = 8;
    lcd.setCursor(0,1);
    lcd.print("User ID 3");
  }

  else if (currentState == 8 && buttonPressed == DOWN_BUTTON_MENU || currentState == 10 && buttonPressed == UP_BUTTON_MENU)
  {
    currentState = 9;
    lcd.setCursor(0,1);
    lcd.print("User ID 4");
  }
  
  else if (currentState == 9 && buttonPressed == DOWN_BUTTON_MENU)
  {
    currentState = 10;
    lcd.setCursor(0,1);
    lcd.print("User ID 5");
  }
/////////////////////////////////////////////////////////////
/************************************************************
Function: Select Add Fingeprint
Flow    : None
************************************************************/
  else if (currentState == 6 && buttonPressed == SELECT_BUTTON_MENU)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Adding Finger");
    lcd.setCursor(0,1);
    lcd.print("print in process");
  
    currentState = 0;
    addFinger0();
    correctRespond();
    operationSuccesful();
  }
  
  else if (currentState == 7 && buttonPressed == SELECT_BUTTON_MENU)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Adding Finger");
    lcd.setCursor(0,1);
    lcd.print("print in process");

    currentState = 0;
    addFinger1();
    correctRespond();
    operationSuccesful();
  }

  else if (currentState == 8 && buttonPressed == SELECT_BUTTON_MENU)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Adding Finger");
    lcd.setCursor(0,1);
    lcd.print("print in process");

    currentState = 0;
    addFinger2();
    correctRespond();
    operationSuccesful();
  }

  else if (currentState == 9 && buttonPressed == SELECT_BUTTON_MENU)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Adding Finger");
    lcd.setCursor(0,1);
    lcd.print("print in process");

    currentState = 0;
    addFinger3();
    correctRespond();
    operationSuccesful();
  }

  else if (currentState == 10 && buttonPressed == SELECT_BUTTON_MENU)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Adding Finger");
    lcd.setCursor(0,1);
    lcd.print("print in process");

    currentState = 0;
    addFinger4();
    correctRespond();
    operationSuccesful();
  }
/////////////////////////////////////////////////////////////
/************************************************************
Function: Delete Fingerprint Address 1 - Address 5
Flow    : Count Down & Count Up
************************************************************/
  else if (currentState == 2 && buttonPressed == SELECT_BUTTON_MENU || currentState == 12 && buttonPressed == UP_BUTTON_MENU)
  {
    currentState = 11;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Del Fingerprint");
    lcd.setCursor(0,1);
    lcd.print("User ID 1");
  }
  
  else if (currentState == 11 && buttonPressed == DOWN_BUTTON_MENU || currentState == 13 && buttonPressed == UP_BUTTON_MENU)
  {
    currentState = 12;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Del Fingerprint");
    lcd.setCursor(0,1);
    lcd.print("User ID 2");
  }

  else if (currentState == 12 && buttonPressed == DOWN_BUTTON_MENU || currentState == 14 && buttonPressed == UP_BUTTON_MENU)
  {
    currentState = 13;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Del Fingerprint");
    lcd.setCursor(0,1);
    lcd.print("User ID 3");
  }

  else if (currentState == 13 && buttonPressed == DOWN_BUTTON_MENU || currentState == 15 && buttonPressed == UP_BUTTON_MENU)
  {
    currentState = 14;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Del Fingerprint");
    lcd.setCursor(0,1);
    lcd.print("User ID 4");
  }
  
  else if (currentState == 14 && buttonPressed == DOWN_BUTTON_MENU)
  {
    currentState = 15;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Del Fingerprint");
    lcd.setCursor(0,1);
    lcd.print("User ID 5");
  }
/************************************************************
Function: Select Del Fingeprint
Flow    : None
************************************************************/
  else if (currentState == 11 && buttonPressed == SELECT_BUTTON_MENU)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Deleting Finger");
    lcd.setCursor(0,1);
    lcd.print("print in process");

    currentState = 0;
    deleteFinger0();
    correctRespond();
    operationSuccesful();
  }
  
  else if (currentState == 12 && buttonPressed == SELECT_BUTTON_MENU)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Deleting Finger");
    lcd.setCursor(0,1);
    lcd.print("print in process");

    currentState = 0;
    deleteFinger1();
    correctRespond();
    operationSuccesful();
  }

  else if (currentState == 13 && buttonPressed == SELECT_BUTTON_MENU)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Deleting Finger");
    lcd.setCursor(0,1);
    lcd.print("print in process");

    currentState = 0;
    deleteFinger2();
    correctRespond();
    operationSuccesful();
  }

  else if (currentState == 14 && buttonPressed == SELECT_BUTTON_MENU)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Deleting Finger");
    lcd.setCursor(0,1);
    lcd.print("print in process");

    currentState = 0;
    deleteFinger3();
    correctRespond();
    operationSuccesful();
  }

  else if (currentState == 15 && buttonPressed == SELECT_BUTTON_MENU)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Deleting Finger");
    lcd.setCursor(0,1);
    lcd.print("print in process");

    currentState = 0;
    deleteFinger4();
    correctRespond();
    operationSuccesful();
  }
/////////////////////////////////////////////////////////////
/************************************************************
Function: Search Fingerprint Address 1 - Address 5
Flow    : None
************************************************************/
  else if (currentState == 3 && buttonPressed == SELECT_BUTTON_MENU)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Searching Finger");
    lcd.setCursor(0,1);
    lcd.print("print in process");

    currentState = 0;
    searchFinger();
    correctRespond();
    operationSuccesful();
  }
/////////////////////////////////////////////////////////////
/************************************************************
Function: Empty Database
Flow    : None
************************************************************/
  else if (currentState == 4 && buttonPressed == SELECT_BUTTON_MENU)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Empty Database");
    lcd.setCursor(0,1);
    lcd.print("in process");

    delay(1000);
    currentState = 0;
    emptyFinger();
    correctRespond();
    operationSuccesful();
  }
/////////////////////////////////////////////////////////////
/************************************************************
Function: Search Database Fingerprint Address 1 - Address 5
Flow    : Count Down & Count Up
************************************************************/
  else if (currentState == 5 && buttonPressed == SELECT_BUTTON_MENU || currentState == 17 && buttonPressed == UP_BUTTON_MENU)
  {
    currentState = 16;
    lcd.setCursor(0,1);
    lcd.print("User ID 1");
  }
  
  else if (currentState == 16 && buttonPressed == DOWN_BUTTON_MENU || currentState == 18 && buttonPressed == UP_BUTTON_MENU)
  {
    currentState = 17;
    lcd.setCursor(0,1);
    lcd.print("User ID 2");
  }

  else if (currentState == 17 && buttonPressed == DOWN_BUTTON_MENU || currentState == 19 && buttonPressed == UP_BUTTON_MENU)
  {
    currentState = 18;
    lcd.setCursor(0,1);
    lcd.print("User ID 3");
  }

  else if (currentState == 18 && buttonPressed == DOWN_BUTTON_MENU || currentState == 20 && buttonPressed == UP_BUTTON_MENU)
  {
    currentState = 19;
    lcd.setCursor(0,1);
    lcd.print("User ID 4");
  }
  
  else if (currentState == 19 && buttonPressed == DOWN_BUTTON_MENU)
  {
    currentState = 20;
    lcd.setCursor(0,1);
    lcd.print("User ID 5");
  }
/************************************************************
Function: Search Database Fingerprint Address 1 - Address 5
Flow    : None
************************************************************/
  else if (currentState == 16 && buttonPressed == SELECT_BUTTON_MENU)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Searching Data");
    lcd.setCursor(0,1);
    lcd.print("base in process");

    currentState = 0;
    databaseFinger0();
    correctRespond();
    operationSuccesful();
  }
  
  else if (currentState == 17 && buttonPressed == SELECT_BUTTON_MENU)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Searching Data");
    lcd.setCursor(0,1);
    lcd.print("base in process");

    currentState = 0;
    databaseFinger1();
    correctRespond();
    operationSuccesful();
  }

  else if (currentState == 18 && buttonPressed == SELECT_BUTTON_MENU)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Searching Data");
    lcd.setCursor(0,1);
    lcd.print("base in process");

    currentState = 0;
    databaseFinger2();
    correctRespond();
    operationSuccesful();
  }

  else if (currentState == 19 && buttonPressed == SELECT_BUTTON_MENU)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Searching Data");
    lcd.setCursor(0,1);
    lcd.print("base in process");

    currentState = 0;
    databaseFinger3();
    correctRespond();
    operationSuccesful();
  }

  else if (currentState == 20 && buttonPressed == SELECT_BUTTON_MENU)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Searching Data");
    lcd.setCursor(0,1);
    lcd.print("base in process");

    currentState = 0;
    databaseFinger4();
    correctRespond();
    operationSuccesful();
  }
/////////////////////////////////////////////////////////////
}

int get_key(unsigned int input)
{
  int k;

    for (k = 0; k < NUM_KEYS; k++)
    {
      if (input < adc_key_val[k])
      {

      return k;
      }
    }
    
  if (k >= NUM_KEYS)
    k = -1;     // No valid key pressed

    return k;
}

/*************************************************************
Add Fingerprint Function
*************************************************************/
void addFinger0(){
  // write the 8bytes array of the command code
  Serial.write(addFingerPrint0, sizeof(addFingerPrint0));
  delay(1000);
}

void addFinger1(){
  // write the 8bytes array of the command code
  Serial.write(addFingerPrint1, sizeof(addFingerPrint1));
  delay(1000);
}

void addFinger2(){
  // write the 8bytes array of the command code
  Serial.write(addFingerPrint2, sizeof(addFingerPrint2));
  delay(1000);
}

void addFinger3(){
  // write the 8bytes array of the command code
  Serial.write(addFingerPrint3, sizeof(addFingerPrint3));
  delay(1000);
}

void addFinger4(){
  // write the 8bytes array of the command code
  Serial.write(addFingerPrint4, sizeof(addFingerPrint4));
  delay(1000);
}

/*************************************************************
Delete Fingerprint Function
*************************************************************/
void deleteFinger0(){
  // write the 8bytes array of the command code
  Serial.write(deleteFingerPrint0, sizeof(deleteFingerPrint0));
  delay(1000);
}

void deleteFinger1(){
  // write the 8bytes array of the command code
  Serial.write(deleteFingerPrint1, sizeof(deleteFingerPrint1));
  delay(1000);
}

void deleteFinger2(){
  // write the 8bytes array of the command code
  Serial.write(deleteFingerPrint2, sizeof(deleteFingerPrint2));
  delay(1000);
}

void deleteFinger3(){
  // write the 8bytes array of the command code
  Serial.write(deleteFingerPrint3, sizeof(deleteFingerPrint3));
  delay(1000);
}

void deleteFinger4(){
  // write the 8bytes array of the command code
  Serial.write(deleteFingerPrint4, sizeof(deleteFingerPrint4));
  delay(1000);
}

/*************************************************************
Search Fingerprint Function
*************************************************************/
void searchFinger(){
  // write the 8bytes array of the command code
  Serial.write(searchFingerPrint, sizeof(searchFingerPrint));
  delay(1000);
}

/*************************************************************
Empty Database Fingerprint Function
*************************************************************/
void emptyFinger(){
  // write the 8bytes array of the command code
  Serial.write(emptyFingerPrint, sizeof(emptyFingerPrint));
  delay(1000);
}

/*************************************************************
Search Database Fingerprint Function
*************************************************************/
void databaseFinger0(){
  // write the 8bytes array of the command code
  Serial.write(databaseFingerPrint0, sizeof(databaseFingerPrint0));
  delay(1000);
}

void databaseFinger1(){
  // write the 8bytes array of the command code
  Serial.write(databaseFingerPrint1, sizeof(databaseFingerPrint1));
  delay(1000);
}

void databaseFinger2(){
  // write the 8bytes array of the command code
  Serial.write(databaseFingerPrint2, sizeof(databaseFingerPrint2));
  delay(1000);
}

void databaseFinger3(){
  // write the 8bytes array of the command code
  Serial.write(databaseFingerPrint3, sizeof(databaseFingerPrint3));
  delay(1000);
}

void databaseFinger4(){
  // write the 8bytes array of the command code
  Serial.write(databaseFingerPrint4, sizeof(databaseFingerPrint4));
  delay(1000);
}

/*************************************************************
Received Respond Code Function
*************************************************************/
void correctRespond() {
  // Now, reply contains all 6 bytes
  for(byte x=0; x<6; x++)
  {
    while(Serial.available() == 0) {}; // Wait for a byte
    respond[x] = Serial.read();
    
    a = memcmp(respondCorrect, respond, sizeof(respondCorrect));

    if (a==0) {
      }
  
    else {
      }
  }
  delay(1000);
}

/*************************************************************
Operation Function
*************************************************************/
void operationSuccesful() {
  // Now, reply contains all 7 bytes
  for(byte x=0; x<7; x++)
  {
    while(Serial.available() == 0) {}; // Wait for a byte
    operation[x] = Serial.read();
    
    // Compare 7 bytes of received respond code with the add fingerprint operation of the fingerprint reader
    b = memcmp(operationSuccessAdd, operation, sizeof(operationSuccessAdd));
    c = memcmp(processFailureAdd, operation, sizeof(processFailureAdd));
    d = memcmp(parameterErrorAdd, operation, sizeof(parameterErrorAdd));
    e = memcmp(timeOutAdd, operation, sizeof(timeOutAdd));

    // Compare 7 bytes of received respond code with the delete fingerprint operation of the fingerprint reader    
    f = memcmp(operationSuccessDelete, operation, sizeof(operationSuccessDelete));
    g = memcmp(parameterErrorDelete, operation, sizeof(parameterErrorDelete));

    // Compare 7 bytes of received respond code with the search fingerprint operation of the fingerprint reader    
    h = memcmp(operationSuccessSearch, operation, sizeof(operationSuccessSearch));
    i = memcmp(parameterErrorSearch, operation, sizeof(parameterErrorSearch));
    j = memcmp(processFailureSearch, operation, sizeof(processFailureSearch));
    k = memcmp(timeOutSearch, operation, sizeof(timeOutSearch));
    l = memcmp(noMatchFinger, operation, sizeof(noMatchFinger));

    // Compare 7 bytes of received respond code with the empty database operation of the fingerprint reader    
    m = memcmp(operationSuccessEmpty, operation, sizeof(operationSuccessEmpty));

    // Compare 7 bytes of received respond code with the search database operation of the fingerprint reader    
    n = memcmp(operationSuccessDatabase, operation, sizeof(operationSuccessDatabase));
    o = memcmp(operationFailureDatabase, operation, sizeof(operationFailureDatabase));
    p = memcmp(notInRange, operation, sizeof(notInRange));
    
/*************************************************************
Add Fingerprint Operation
*************************************************************/
    if (b==0) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Adding Fprint");
      lcd.setCursor(0, 1);
      lcd.print("Operation Success");
      delay (1500);
      
      ////////////////////////////////////////////////////////////////
      for(byte y=0; y<9; y++)
      {
        while(Serial.available() == 0) {}; // Wait for a byte
        operationOK[y] = Serial.read();
        
        q = memcmp(operationSuccessAdd, operationOK, sizeof(operationSuccessAdd));
        r = memcmp(processFailureAdd, operationOK, sizeof(processFailureAdd));
        
        if (q==0) {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Fingerprint");
          lcd.setCursor(0, 1);
          lcd.print("Added");
          buzzerOK();
        }
          
        else if (r==0) {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Not Done");
          buzzerNotOK();
        }
      }
      ///////////////////////////////////////////////////
    }
    
   else if (c==0 || j==0) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Process Failure");
      buzzerNotOK();
      }
        
    else if (d==0 || g==0 || i==0) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Parameter Error");
      buzzerNotOK();
      }

    else if (e==0 || k==0) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Time Out");
      buzzerNotOK();
      }
/*************************************************************
Delete Fingerprint Operation
*************************************************************/
    else if (f==0) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Delete Fprint");
      lcd.setCursor(0, 1);
      lcd.print("Operation Success");
      buzzerOK();
      }
/*************************************************************
Search Fingerprint Operation
*************************************************************/
    else if (h==0) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Search Fprint");
      lcd.setCursor(0, 1);
      lcd.print("Operation Success");
      delay(1500);
      
      ////////////////////////////////////////////////////////////////
      for(byte y=0; y<9; y++)
      {
        while(Serial.available() == 0) {}; // Wait for a byte
        operationFound[y] = Serial.read();
        
        s = memcmp(FingerPrint0, operationFound, sizeof(FingerPrint0));
        t = memcmp(FingerPrint1, operationFound, sizeof(FingerPrint1));
        u = memcmp(FingerPrint2, operationFound, sizeof(FingerPrint2));
        v = memcmp(FingerPrint3, operationFound, sizeof(FingerPrint3));
        w = memcmp(FingerPrint4, operationFound, sizeof(FingerPrint4));
        z = memcmp(noMatchFinger, operationFound, sizeof(noMatchFinger));
        
        if (s==0) {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Fprint Founded");
          lcd.setCursor(0, 1);
          lcd.print("User ID 1");
          buzzerOK();
        }
          
        else if (t==0) {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Fprint Founded");
          lcd.setCursor(0, 1);
          lcd.print("User ID 2");
          buzzerOK();
        }
          
        else if (u==0) {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Fprint Founded");
          lcd.setCursor(0, 1);
          lcd.print("User ID 3");
          buzzerOK();
        }

        else if (v==0) {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Fprint Founded");
          lcd.setCursor(0, 1);
          lcd.print("User ID 4");
          buzzerOK();
        }

        else if (w==0) {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Fprint Founded");
          lcd.setCursor(0, 1);
          lcd.print("User ID 5");
          buzzerOK();
        }
        
        else if (z==0) {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("No Match Found");   
          lcd.setCursor(0, 1);
          lcd.print("Invalid Fprint");
          buzzerNotOK();
        }
      }
      ///////////////////////////////////////////////////
      }

    else if (l==0) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("No Matching Found");
      lcd.setCursor(0, 1);
      lcd.print("Invalid Fprint");
      buzzerNotOK();
    }
/*************************************************************
Empty Database Operation
*************************************************************/
    else if (m==0) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Empty Database");
      lcd.setCursor(0, 1);
      lcd.print("Operation Success");
      buzzerOK();
      }
/*************************************************************
Search Database Operation
*************************************************************/
    else if (n==0) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Database Found");
      buzzerOK();
      }
    
    else if (o==0) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Database Not");
      lcd.setCursor(0, 1);
      lcd.print("Found");
      buzzerNotOK();
      }
        
    else if (p==0) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Not In Range");
      buzzerNotOK();
      }
/*************************************************************
If other respond code received
*************************************************************/
    else {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Please Restart");
      }
  }
  delay(3000);
  menu();
}

void buzzerOK () {
  digitalWrite(buzzerPin, HIGH);  
  delay(500);
  
  digitalWrite(buzzerPin, LOW); 
  delay(250);

  digitalWrite(buzzerPin, HIGH);  
  delay(500);

  digitalWrite(buzzerPin, LOW);  
}

void buzzerNotOK () {
  digitalWrite(buzzerPin, HIGH);  
  delay(1000);
  
  digitalWrite(buzzerPin, LOW); 
  delay(500);

  digitalWrite(buzzerPin, HIGH);  
  delay(1000);

  digitalWrite(buzzerPin, LOW);  
}
