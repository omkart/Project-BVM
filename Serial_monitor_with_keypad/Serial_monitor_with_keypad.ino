//#include <LiquidCrystal.h>

/*
Project Prepared by Mitesh Khedekar and Omkar Tulaskar.
This Project does not involve use of LCD. We use the Serial monitor for 
user interaction.
Hardware used:      Arduino Uno
                    R305 fingerprint Sensor Module    

Problems in this code : large no. of global variables are being used.
                                   
*/
#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>
#include<EEPROM.h>
#include<Wire.h>
#include <Keypad.h>
#define Password_Lenght 7 

char Data[Password_Lenght]; // 6 is the number of chars it can hold + the null char = 7
char Master[Password_Lenght] = "123456"; 
byte data_count = 0, master_count = 0;

char id[10];

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
//define the cymbols on the buttons of the keypads
char Keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {24,26,28,30}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {32,34, 36, 38};

char menu1;
 char id_no;
uint8_t enrollment_menu;
uint8_t results_menu;
uint8_t vote;


int repeat=0;
//int voted_id[]={};
int address=0;

uint8_t BJP=0,Congress=0,Shivsena=0,MNS=0;
Keypad customKeypad = Keypad( makeKeymap(Keys), rowPins, colPins, ROWS, COLS); 

SoftwareSerial mySerial(10,11);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

//LiquidCrystal lcd(12, 11, 8, 7, 5, 4);


void setup() {
  

//    lcd.begin(16, 2);
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

char user_input()                //function to get the input from the user
{
 
   while(1)
{
   char customKey = customKeypad.getKey();
   if (customKey!=NO_KEY){
    Serial.println(customKey);
   
     if(customKey=='B')
   {
       //lasti=i;
    break;
  
   }
   else
   {
     
  //input[i]=customKey;
//  Serial.print("input :");
  // Serial.println(input[i]);
Serial.println(customKey);
 return customKey;
 
   }
  }
  
}
}


void get_id()
{
 
int data_count_1=0;
//  id_no++;
while(1){
char customKey = customKeypad.getKey();
  if (customKey) // makes sure a key is actually pressed, equal to (customKey != NO_KEY)
  {
    if(customKey=='A')
{
  break;
}
else{
    id[data_count_1] = customKey; // store char into data array
  
    data_count_1++; // increment data array by 1 to store new char, also keep track of the number of chars entered
  }
  } 
}
Serial.print(id);
Serial.println("");
}
void loop() {

First_Menu();

 
}

void First_Menu()
{
 // lcd.print("Please Select one of the Options");
  Serial.println("Please Select one of the Options.");
 Serial.println("1. Biometric Enrollment                 2. Proceed For VOTING");
  /*lcd.setCursor(0,0);
 
  lcd.print("1.Enrollment");
  lcd.setCursor(0,1);
  lcd.print("2.VOTING");
  
  delay(5000);
  
  lcd.clear();*/
  Serial.println("Enter the passcode to check the results");
  
  spacing();
  menu1=(int)user_input();
  Serial.println(menu1);


  if(menu1=='1')
  {
     Enrollment();
  }
  else if(menu1=='2')
  {
    VOTING();
  }
  else if(menu1=='A')
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
    get_id();
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
  Serial.print("ID :"); Serial.println(id);
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



















