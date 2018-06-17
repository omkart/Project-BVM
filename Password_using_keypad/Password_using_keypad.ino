#include <Keypad.h>
#include <Wire.h> 


#define Password_Lenght 7 // Give enough room for six chars + NULL char


char Data[Password_Lenght]; // 6 is the number of chars it can hold + the null char = 7
char Master[Password_Lenght] = "123456"; 
byte data_count = 0, master_count = 0;
bool Pass_is_good;
char customKey;

const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
 {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {24,26,28,30}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {32,34, 36, 38};

Keypad customKeypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS); //initialize an instance of class NewKeypad 

void setup()
{
   Serial.begin(9600);
}

void loop()
{
 // lcd.setCursor(0,0);
//  lcd.print("Enter Password");
  Serial.println("Enter Password");
  customKey = customKeypad.getKey();
  if (customKey) // makes sure a key is actually pressed, equal to (customKey != NO_KEY)
  {
    Data[data_count] = customKey; // store char into data array
  
    data_count++; // increment data array by 1 to store new char, also keep track of the number of chars entered
  }

  if(data_count == Password_Lenght-1) // if the array index is equal to the number of expected chars, compare data to master
  {
    //lcd.clear();
   // lcd.setCursor(0, 0);
    Serial.println("Password is ");

    if(!strcmp(Data, Master)) // equal to (strcmp(Data, Master) == 0)
      Serial.println("Good");
    else
      Serial.println("Bad");

    delay(1000);// added 1 second delay to make sure the password is completely shown on screen before it gets cleared.
    
  }
}


