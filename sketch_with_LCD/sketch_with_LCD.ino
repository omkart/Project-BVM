#include <LiquidCrystal.h>

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

LiquidCrystal lcd(12, 11, 9, 7, 5, 4);


void setup() {

    lcd.begin(16, 2);
    
  Serial.begin(9600);
  mySerial.begin(57600);
  lcd.setCursor(3,0);
  //Serial.println("Welcome to biometric Voting Machine.....");
  lcd.print("..WELCOME..");
  
 // delay(1000);
    lcd.setCursor(2,1);
   if(finger.verifyPassword())
   {
    //Serial.println("Biometric Machine Connected");
    lcd.print("BVM Detected");
    delay(5000);
    lcd.clear();
    spacing();
   }

   else 
   {
   // Serial.println("Failed to detect Biometric Machine");
     lcd.print("BVM NOT FOUND");
    delay(5000);
    lcd.clear();
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
 // lcd.print("Please Select one of the Options");
  //Serial.println("Please Select one of the Options.");
  lcd.clear();
 // Serial.println("1. Biometric Enrollment                 2. Proceed For VOTING");
  lcd.setCursor(0,0);
 
  lcd.print("1.ENROLLMENT");
  lcd.setCursor(0,1);
  lcd.print("2.VOTING");
  
 // lcd.clear();
 // Serial.println("Enter the passcode to check the results");
  
// spacing();
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
    //Serial.println("Invalid Input");
    lcd.clear();
    lcd.print("INVALID INPUT");
    First_Menu();
  }
}

void Enrollment()
{
  lcd.clear();
    lcd.setCursor(0,0);
   //Serial.println("Biometric Enrollment");
  // Serial.println("Enter your id number");
  lcd.print("ENROLLMENT..");
    lcd.setCursor(0,1);
  lcd.print("Enter ID:");
    id();
    lcd.clear();
   // Serial.println("Please place your finger");
   lcd.print("PLACE FINGER...");
   while(!getFingerprintEnroll()); 
}

void VOTING()
{
  spacing();
  //Serial.println("Please place your finger to check in the database");
  lcd.clear();
  lcd.print("Place Finger...");
while(1){
getFingerprintID();
}
}

void Passcode()
{
 // Serial.println("1.Show Results                          2.Clear Results database");
 // Serial.println("3.Clear the enrollment database         4.Exit");
 lcd.clear();
 lcd.print("1.RESULTS");
 lcd.setCursor(0,1);
lcd.print("4.EXIT");
  results_menu=user_input();
  lcd.clear();
  switch(results_menu)
  {
    case 1:
    Serial.println("Showing the results of votes counted uptill now");
            show_results();
            break;
    case 2:
  //  Serial.println("Clearing the database...");
             
            clear_results();
            break;
    case 3: 
           // Serial.println("Clearing the voters identification database....");
           
             clear_voters();
           
            break;
    case 4:
    
  //  Serial.println("Exiting....");
         lcd.print("EXITING...");
           delay(1000);
             First_Menu();
            break;
    default:
         // Serial.println("Invalid Input ,Exiting the Results menu");
         lcd.print("INVALID INPUT");
         lcd.setCursor(0,1);
         lcd.print("EXITING...");
         delay(1000);
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
 
  lcd.print("Results Cleared.");
           delay(1000);
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
 lcd.print("Database Cleared.");
          delay(1000);
  First_Menu();
}

uint8_t getFingerprintEnroll()
{
    int p = -1;
 //Serial.print("Waiting for valid finger to enroll"); 
 
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
  


//Serial.println("Remove finger");
lcd.clear();
lcd.print("Remove Finger");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
    lcd.setCursor(0,1);
    lcd.print("ID : ");
      lcd.setCursor(6,1);
    lcd.print(id_no);
    delay(2200);
  //Serial.print("ID "); Serial.println(id_no);
  p = -1;
  lcd.clear();
  lcd.print("Place finger");
  lcd.setCursor(0,1);
  lcd.print("Again");
 // Serial.println("Place same finger again");
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
  //Serial.print("Creating model for #");  Serial.println(((id_no)-1));
  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    //Serial.println("Prints matched!");
    lcd.clear();
    lcd.print("Prints Matched");
    
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
   // Serial.println("Fingerprints did not match");
     // lcd.setCursor(0,1);
     lcd.clear();
    lcd.print("Prints not Matched");
    delay(2000);

    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }   


  //Serial.print("ID "); Serial.println(id_no);
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
delay(2000);
 // spacing();
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
     // Serial.println("Image taken");
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
    //  Serial.println("Image converted");
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

   lcd.clear();
   lcd.print("Found match.");
   delay(1000);
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
        //spacing();
        //Serial.println("Sorry your name not in the voters list.");
        lcd.clear();
        lcd.print("VOTER not found");
           delay(1000);
        First_Menu(); 
    }
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }   



  // found a match!
  //Serial.print("Found ID #"); Serial.print(finger.fingerID); 
 // Serial.print(" with confidence of "); Serial.println(finger.confidence); 


  
}

}


void start_voting()
{
  // spacing();
  // Serial.print("Found ID #"); Serial.print(finger.fingerID); 
  // Serial.print(" with confidence of "); Serial.println(finger.confidence); 
  lcd.clear();
 lcd.print("ID :");
 lcd.setCursor(5,0);
 lcd.print(finger.fingerID);
  lcd.setCursor(0,1);
  lcd.print("CON:");
  lcd.setCursor(6,1);
  lcd.print(finger.confidence);
     delay(2000);

  
   if(((finger.fingerID)+16)==EEPROM.length())
   {
    Serial.println("ID number out of bounds");
    
   }
   if(EEPROM.read((finger.fingerID)+16)==finger.fingerID)
   {
    //Serial.println("Sorry You have already voted");
    lcd.clear();
    lcd.print("ALREADY VOTED !");
       delay(1000);
    First_Menu();
   }
   else{
   EEPROM.write((finger.fingerID)+16,finger.fingerID);
   spacing();
 /* Serial.println("Select your party");
  Serial.println("1.BJP");
  Serial.println("2.Shivsena");
  Serial.println("3.Congress");Serial.println("4.MNS");*/
  lcd.clear();
  lcd.print("Select Party");
  delay(2200);
  lcd.clear();
  lcd.print("1.BMT");
  lcd.setCursor(7,0);
  lcd.print("2.BTC");
   lcd.setCursor(0,1);
  lcd.print("3.MNC");
   lcd.setCursor(7,1);
  lcd.print("4.SKP");
  
  vote=user_input();
  if(vote>4)
  {
    //Serial.println("Invalid input");
    lcd.clear();
    lcd.print("INVALID INPUT..");
       delay(1000);
  }
  else
  {
    //Serial.println("Your response has been recorded");
    lcd.clear();
    lcd.print("RESPONSE RECORDED");
    delay(1000);
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

  
   //Serial.println("1.Add more users                2.Exit");
   lcd.clear();
   lcd.print("1.Add Voters");
   lcd.setCursor(0,1);
   lcd.print("2.Exit");
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
 // Serial.println("Invalid input");
  lcd.clear();
  lcd.print("INVALID INPUT..");
  delay(2000);
  First_Menu();
 }
  
}



















