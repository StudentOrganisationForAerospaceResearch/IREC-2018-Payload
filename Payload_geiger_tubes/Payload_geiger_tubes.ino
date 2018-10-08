/*
 * 
 */


#include <SPI.h> // Serial libary to send data to main computer


#define LOG_PERIOD 150   // Logging period in miliseconds

int radPin = 2;
int buzzPin = 7;
volatile unsigned long counts = 0;
volatile int blip = 0;
unsigned long previousMillis = 0;
unsigned long cmp = 0;

void tubeImpulse() {
  counts++;
  blip=1;
}

void setup() {
  Serial.begin(115200);
  pinMode(buzzPin, OUTPUT);
  pinMode(radPin, INPUT_PULLUP);
  interrupts();
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
    //digitalWrite(buzzPin, HIGH);
    delay(2);
    //digitalWrite(buzzPin, LOW);
  }
  if (currentMillis - previousMillis > LOG_PERIOD) {
    previousMillis = currentMillis;
    //Serial.write(counts);
    Serial.println(counts);
    counts = 0;
  }

}
