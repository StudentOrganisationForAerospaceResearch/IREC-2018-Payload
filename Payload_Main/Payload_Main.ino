#include <SD.h> //For the SD card
#include <SPI.h> //To read the radiation Arduinos

File myFile; //Initialize file to log data to

//Select the SD card data pin, led pin, and buzzer pin
const int chipSelect = BUILTIN_SDCARD;
int ledPin=13;
int buzzPin = 35;

int timeDelay=100; //Data logging rate in ms (50 = 20Hz)
int led = 1; //Toggling variable to store LED status

//Initialize variables to store sensor readings
byte Rad1;
byte Rad2;
int PhotoV1;
int PhotoV2;
int PhotoV3;
int PhotoV4;
int ThermV1;
int ThermV2;
int ThermV3;
int ThermV4;
int ThermV5;
int ThermV6;
int TempV;
int PresV;
int HumidityV;

unsigned long t=millis(); //Variable to keep track of time

void setup()  {
  //Set LED and buzzer pins to output mode
  pinMode(ledPin, OUTPUT);
  pinMode(buzzPin, OUTPUT);
  
  //Open radiation serial ports, wait for port to open
  Serial2.begin(9600);
  Serial3.begin(9600);
  delay(1000);

  //Initialize the SD card. Further, if initialization fails
  //sound one long (1 sec) angry beep through the buzzer
  if (!SD.begin(chipSelect)) {
    digitalWrite(buzzPin, HIGH);
    delay(1000);
    digitalWrite(buzzPin, LOW);
    return;
  }

  //Open 'test.txt' on the SD card in append mode
  myFile = SD.open("test.txt", FILE_WRITE);

  //If file initialization was successful, 10 quick happy 
  //beeps, and print 'START OF TEST' to file
  if (myFile) {
    for (int i = 0; i < 10; i++) {
      digitalWrite(buzzPin, HIGH);
      delay(50);
      digitalWrite(buzzPin, LOW);
      delay(50);
    }
    myFile.println("START OF TEST");
    myFile.close();

  //If file initialization was unsuccessful, one long 
  //angry beep
  } else {
    digitalWrite(buzzPin, HIGH);
    delay(1000);
    digitalWrite(buzzPin, LOW);
  }
}

void loop() {
  
  t=millis(); //Update t to current time in milliseconds

  led = !led; //Toggle LED variable (1->0 or 0->1)

  //Read voltages from all sensors, store them in the range 0-1023
  HumidityV = analogRead(A14); //0-3.3V corresponds to 0-75% hum
  TempV = analogRead(A15); 
  PresV = analogRead(A17);
  PhotoV1 = analogRead(A9);
  PhotoV2 = analogRead(A8);
  PhotoV3 = analogRead(A7);
  PhotoV4 = analogRead(A6);
  ThermV1 = analogRead(A5);
  ThermV2 = analogRead(A4);
  ThermV3 = analogRead(A3);
  ThermV4 = analogRead(A2);
  ThermV5 = analogRead(A1);
  ThermV6 = analogRead(A0);

  //The radiation Arduinos spit out serial data in integers, 
  //corresponding to the radiation events triggered since the
  //last transmit. -1 corresponds to no data, 255 corresponds
  //to MORE THAN 253 radiation events since last transmit
  Rad1=Serial3.read(); 
  Rad2=Serial2.read();

  //Toggle the onboard LED (just for debugging)
  digitalWrite(ledPin, led);

  //Open the file on the SD card and print all variables in
  //.csv format. Format is [time,hum,temp,...,rad1,rad2]
  myFile = SD.open("test.txt", FILE_WRITE);
  myFile.print(t / 1000.); myFile.print(",");
  myFile.print(HumidityV); myFile.print(",");
  myFile.print(TempV); myFile.print(",");
  myFile.print(PresV); myFile.print(",");
  myFile.print(PhotoV1); myFile.print(",");
  myFile.print(PhotoV2); myFile.print(",");
  myFile.print(PhotoV3); myFile.print(",");
  myFile.print(PhotoV4); myFile.print(",");
  myFile.print(ThermV1); myFile.print(",");
  myFile.print(ThermV2); myFile.print(",");
  myFile.print(ThermV3); myFile.print(",");
  myFile.print(ThermV4); myFile.print(",");
  myFile.print(ThermV5); myFile.print(",");
  myFile.print(ThermV6); myFile.print(",");
  myFile.print(Rad1); myFile.print(",");
  myFile.println(Rad2);
  myFile.close(); //close file after write (more robust)

  //Quick loop to set logging rate accurately. Loops until
  //current loop time is higher than logging variable
  while(millis()-t<timeDelay){};
}
