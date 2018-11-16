/*
 * 
 */


#include <SPI.h> // Serial libary to send data to main computer


#define LOG_PERIOD 150   // Logging period in miliseconds

int radPin = 2;   //Pin that geiger tube is connected to
int buzzPin = 7;  //Pin that speaker is connected to
volatile unsigned long counts = 0; //Stores number of radiation events per interval
volatile int blip = 0; //Toggling variable to trigger speaker
unsigned long previousMillis = 0;
unsigned long cmp = 0;
String str_counts = "";
char ch_arr[] = "00000";

void tubeImpulse() {
  counts++;
  blip=1;
}

void setup() {
  Serial.begin(9600); //was previously set to 115200, not sure why?
  pinMode(buzzPin, OUTPUT);
  pinMode(radPin, INPUT_PULLUP);
  interrupts(); //uses interrupt to go out of the main loop when radiation is detected
  attachInterrupt(digitalPinToInterrupt(radPin), tubeImpulse, FALLING);
  
  // Beep a few times 
  for (int i = 0; i < 5; i++) {
    digitalWrite(buzzPin, HIGH);
    delay(50);
    digitalWrite(buzzPin, LOW);
    delay(50);
  }

}

void loop() {
  unsigned long currentMillis = millis();
  if(blip==1){
    blip=0;
    digitalWrite(buzzPin, HIGH);
    delay(2);
    digitalWrite(buzzPin, LOW);
  }
  if (currentMillis - previousMillis > LOG_PERIOD) {
    previousMillis = currentMillis;
    
    //Data transmission is limted to the size of 1 byte, which
    //equates to a maximum number of 255, therefore the data is
    //instead converted to a string and sent as a sequence of chars 
    //(which are each 1 byte). Because we are sending 5 characters, 
    //we are limited to sending a maximum number of 99999 (this 
    //can be increased or decreased by changing the character amount 
    //sent)
    
    if (counts > 99999 || counts < 0) //if the count amount is greater than 99999, we set it to the maximum, which is 99999
    {
      counts = 99999;
    }
    str_counts = String(counts); //converts the counts amount from a number to a string
    for (int i = str_counts.length() - 1, j = 4; i >= 0; i--, j--) //puts the characters in the string into an array of characters
    {
      ch_arr[j] = str_counts.charAt(i);
    }
    Serial.write(ch_arr, 5); //sends the character array
    counts = 0; //resets counts to 0
    for (int i = 0; i < 5; i++)//resets the character array to 0
    {
      ch_arr[i] = '0';
    }
  }
}
