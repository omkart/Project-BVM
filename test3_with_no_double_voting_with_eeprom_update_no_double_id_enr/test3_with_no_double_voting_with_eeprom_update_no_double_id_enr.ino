#include <EEPROM.h>

#include <LiquidCrystal.h>
#include<Wire.h>
#include <avr/pgmspace.h>
/*
Project Prepared by Mitesh Khedekar and Omkar Tulaskar.
This Project does not involve use of LCD. We use the Serial monitor for 
user interaction.
Hardware used:      Arduino Uno
                    R305 fingerprint Sensor Module    

Problems in this code : large no. of global variables are being used.
           t                        
*/
#define Password_Lenght 7
char Data[Password_Lenght]; // 6 is the number of chars it can hold + the null char = 7
char Master[Password_Lenght] = "123456"; 
byte data_count = 0, master_count = 0;
#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>
#include<EEPROM.h>
#include<Keypad.h>
int result_exit;
int menu1;
int lastaddress;


int  addr=(address+1000);
enrolled_id[];

uint8_t enrollment_menu;
uint8_t results_menu;
uint8_t vote;

int id;
PROGMEM
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
//define the symbols on the buttons of the keypads
char Keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {24,26,28,30}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {32,34, 36, 38};
Keypad customKeypad = Keypad( makeKeymap(Keys), rowPins, colPins, ROWS, COLS); 



int repeat=0;
//int voted_id[]={};
int address=0;

uint8_t BJP=0,Congress=0,Shivsena=0,MNS=0;

SoftwareSerial mySerial(10,11);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

LiquidCrystal lcd(12, 9, 8, 7,6, 5);


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


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

                                                                        // User Input //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int user_input()                //function to get the input from the user
{
 while(1)
{
   int customKey = customKeypad.getKey();
   if (customKey!=NO_KEY){
    Serial.println(customKey);
   
    if(customKey==66)
   {
       //lasti=i;
       break;
  
   }
   else
   {
     
  Serial.println(customKey-48);
   }
  return (customKey-48);
 
   
  }
  
}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

                                                                        // GET ID //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void get_id()
{
 id=0;

while(1){
 
int customKey = customKeypad.getKey();
  if (customKey) // makes sure a key is actually pressed, equal to (customKey != NO_KEY)
  {
    if(customKey==65)
{
  break;
}
else if(customKey==66)
{
  First_Menu();
}
else{
   id = (id*10)+ (customKey-48);
    lcd.setCursor(11,1);
    lcd.print(id);
   
  }
  } 
}
Serial.println(id);
}
void loop() {

First_Menu();

 
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

                                                                        // First Menu //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
  else if(menu1==17)
  {
     Serial.println("Password Mode selected");
    Password();
   
  }
  else
  {
    //Serial.println("Invalid Input");
    lcd.clear();
    lcd.print("INVALID INPUT");
    First_Menu();
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

                                                                        //Entering Password in Background  //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Password()
{
  while(1)
  {
    char customKey = customKeypad.getKey();
  if (customKey) // makes sure a key is actually pressed, equal to (customKey != NO_KEY)
  {
    Data[data_count] = customKey; // store char into data array
  
    data_count++; // increment data array by 1 to store new char, also keep track of the number of chars entered
  }

  if(data_count == Password_Lenght-1) // if the array index is equal to the number of expected chars, compare data to master
  {
    //lcd.clear();
   // lcd.setCursor(0, 0);
  //  Serial.println("Password is ");

    if(!strcmp(Data, Master)) // equal to (strcmp(Data, Master) == 0)
      {//Serial.println("Good");
      for(int i=0;i<8;i++)
      {
        Data[i]=0;
      }
      Passcode();}
    else
      Serial.println("Bad");
      First_Menu();
    delay(1000);// added 1 second delay to make sure the password is completely shown on screen before it gets cleared.
    
  }
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

                                                                        // Enrollment Menu //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void Enrollment()
{
 int id_repeat=0;
  lcd.clear();
    lcd.setCursor(0,0);
   //Serial.println("Biometric Enrollment");
  // Serial.println("Enter your id number");
  lcd.print("ENROLLMENT..");
    lcd.setCursor(0,1);
  lcd.print("Enter ID:");
    get_id();
   // check_same_id();

   for(int i=1000;i<=2000;i++)
   {
    if (EEPROM.read(i)==id;
    id_repeat=1;
    
   }
  
      if(addr==EEPROM.length())                                                      ///changed from address+16 to address+1000
   {
    Serial.println("ID number out of bounds");
    lcd.clear();
    lcd.print("ID number");
    lcd.setCursor(0,1);
    lcd.print("out of bounds");
   }
   else if(id_repeat==1)
   {
    lcd.clear();
    lcd.print("ID already");
    lcd.setCursor(0,1);
    lcd.print("Enrolled");
    First_Menu();
   }
   else 
   {
    addr=last_address;
    EEPROM.update(last_address,id);
    
    EEPROM.update((address+2000),addr);
    last_address=EEPROM.read(address+2000);
                                                                                  //////////////extra else added just now
    lcd.clear();
   // Serial.println("Please place your finger");
   lcd.print("PLACE FINGER...");
   while(!getFingerprintEnroll()); 
   }
}




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

                                                                        // VOTING Menu //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

                                                                        // Passcode Menu //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Passcode()
{
 // Serial.println("1.Show Results                          2.Clear Results database");
 // Serial.println("3.Clear the enrollment database         4.Exit");
 lcd.clear();
 lcd.print("1.RESULTS");
 lcd.setCursor(0,1);
lcd.print("2.EXIT");
  results_menu=user_input();
  lcd.clear();
  switch(results_menu)
  {
    case 1:
    Serial.println("Showing the results of votes counted uptill now");
            show_results();
            break;
    case 3:
  //  Serial.println("Clearing the database...");
             
            clear_results();
            break;
    case 4: 
           // Serial.println("Clearing the voters identification database....");
           
             clear_voters();
           
            break;
    case 2:
    
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


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

                                                                        //Show Results Menu //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void show_results()
{
      
//Serial.println(EEPROM.read(address));
lcd.clear();
lcd.print("BMT-");
lcd.setCursor(5,0);
lcd.print(EEPROM.read(address));
      
//Serial.println( EEPROM.read(address+2));
lcd.setCursor(8,0);
lcd.print("BTC-");
lcd.setCursor(13,0);
lcd.print(EEPROM.read(address+2));

 //Serial.println(EEPROM.read(address+4));
lcd.setCursor(0,1);
lcd.print("MNC-");
lcd.setCursor(5,1);
lcd.print(EEPROM.read(address+4));
  // Serial.println(EEPROM.read(address+8));
lcd.setCursor(8,1);
lcd.print("SKP-");
lcd.setCursor(13,1);
lcd.print(EEPROM.read(address+8));
result_exit=user_input();
if(result_exit==(66-48))
   First_Menu();
   else
   First_Menu();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

                                                                        // CLEARING Menu's //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

                                                                        //Clear Results //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void clear_results()
{

  for(int i=0;i<=10;i++)
  {
    EEPROM.update(i,0);
    
  }
 
  lcd.print("Results Cleared.");
           delay(1000);
    First_Menu();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

                                                                        // Clear Voters //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void clear_voters()
{
  int i;
for(i=10;i<40;i++)
{
  EEPROM.update(i,0);
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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

                                                           // Get fingerprint Enrollment //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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
 lcd.clear();
lcd.print("Remove Finger");
delay(1000); 
 check_already_enrolled(p);

//Serial.println("Remove finger");

  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  
    lcd.setCursor(0,1);
    lcd.print("ID : ");
      lcd.setCursor(6,1);
    lcd.print(id);                                  //to be edited

    delay(2200);
   
  //Serial.print("ID "); Serial.println(id);
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
   // check_already_enrolled(p);
    
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
   // Serial.println("Fingerprints did not match");
     // lcd.setCursor(0,1);
     lcd.clear();
    lcd.print("Prints not");
    lcd.setCursor(0,1);
    lcd.print("Matched.");
    delay(2000);
   //getFingerprintEnroll();   
   Second_menu_1();
   
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }   


  //Serial.print("ID "); Serial.println(id_no);
  p = finger.storeModel(id);
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
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

                                                                //Already Enrolled Checking//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void check_already_enrolled(int double_entry_checking)
{
  
   int check = double_entry_checking;

  // OK converted!
  
   //Searching the database
   
  check = finger.fingerFastSearch();
  if (check == FINGERPRINT_OK) {

   lcd.clear();
   lcd.print("Found match.");
   delay(1000);
       Serial.println("Found a print match!");
    lcd.clear();
    lcd.print("Voter Already");
    lcd.setCursor(0,1);
    lcd.print("Enrolled");
    lcd.setCursor(13,0);
 
  Second_menu_1();
    
  } else if (check == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
   // return p;
  } else if (check == FINGERPRINT_NOTFOUND) {
    Serial.println("New Voter");
    lcd.clear();
    lcd.print("New Voter");
   // delay(1300);
   
   // return p;
  } else {
    Serial.println("Unknown error");
    //return p;
  }   

  
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

                                                              //Get Fingerprint ID//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t getFingerprintID() {

 /*  if(user_input()==(66-49))
   {
    First_Menu();
   }*/

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
    display_voter_authentication();
  //  start_voting();
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



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

                                                                        //Start VOTING//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void display_voter_authentication()
{
  // spacing();
  // Serial.print("Found ID #"); Serial.print(finger.fingerID); 
  // Serial.print(" with confidence of "); Serial.println(finger.confidence); 
  lcd.clear();
 lcd.print("ID :");
 lcd.setCursor(5,0);
 lcd.print(finger.fingerID);
  lcd.setCursor(0,1);
  lcd.print("CONFIDENCE:");
  lcd.setCursor(13,1);
  lcd.print(finger.confidence);
     delay(2000);

  
  if(((finger.fingerID)+16)==EEPROM.length())
   {
    Serial.println("ID number out of bounds");
    
   }
   
  if((EEPROM.read((finger.fingerID)+16))==finger.fingerID)
   {
    //Serial.println("Sorry You have already voted");
    lcd.clear();
    lcd.print("ALREADY VOTED !");
       delay(1000);
    //First_Menu();
   // getFingerprintID();
  Continue();
   }
   else{
   EEPROM.write(((finger.fingerID)+16),finger.fingerID);
   Serial.println(finger.fingerID);
   spacing();
 /* Serial.println("Select your party");
  Serial.println("1.BJP");
  Serial.println("2.Shivsena");
  Serial.println("3.Congress");Serial.println("4.MNS");*/
   }

   start_voting();
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

                                                                        //Continue VOTING??//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void Continue()
{
  int continue_input=0;
  lcd.clear();
  lcd.print("1.Voting");
  lcd.setCursor(0,1);
  lcd.print("2.Exit");
  continue_input=user_input();
  if(continue_input==1)
  {
    VOTING();
  }
  else if(continue_input==2)
  {
    First_Menu();
  }
  else
  {
    lcd.clear();
    lcd.print("INVALID INPUT");
    Continue();
  }
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

                                                                  //Register Votes//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void start_voting()
{
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
       start_voting();
  }
  else{
  switch(vote)
  {
    case 1:    lcd.clear();
    lcd.print("RESPONSE");
    lcd.setCursor(0,1);
    lcd.print("RECORDED");
    delay(1000);
    break;
    case 2:    lcd.clear();
    lcd.print("RESPONSE");
  lcd.setCursor(0,1);
    lcd.print("RECORDED");
    delay(1000);
    break;
    case 3:    lcd.clear();
        lcd.print("RESPONSE");
  lcd.setCursor(0,1);
    lcd.print("RECORDED");
    delay(1000);
    break;
    
    case 4:    lcd.clear();
      lcd.print("RESPONSE");
     lcd.setCursor(0,1);
    lcd.print("RECORDED");
    delay(1000);
    break;
    default:
      lcd.clear();
    lcd.print("INVALID INPUT");
    delay(1000);
    start_voting();
    break;
  }
 
  }
    //Serial.println("Your response has been recorded");

    switch(vote)
    {
      case 1: BJP++;
      EEPROM.update(address,BJP);
              break;
      case 2: Shivsena++;
      EEPROM.update(address+2,Shivsena);
              break;
      case 3: Congress++;
      EEPROM.update(address+4,Congress);
              break;
      case 4: MNS++;
      EEPROM.update(address+8,MNS);
              break;                      
    }
  

Continue();
} 


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

                                                            // Second Menu of VOTING menu //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


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













