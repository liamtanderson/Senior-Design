#include <Time.h> 
#include <Wire.h>

//Get this song 
//Todo, cleanup accel code into import statement
//Jitter and Latency Values
unsigned long clocktime;
int lasttime = 0;
int numMeasurements = 0;
int jitter = 0;
int totalJitter = 0;
int latency = 0;
int lastLatency = 0;
int delayTime = 10;

//WhatShouldRun
int JoystickRun = 1;
int GyroRun = 1;
int AccelRun = 0;
int ButtonRun = 1;

//Joystick Values
int joyX = A0;
int joyY = A1;
//int mapX = 0;
//int mapY = 0;

#include <ezButton.h>
// Initalize the buttons
ezButton button1(3);
ezButton button2(4);
ezButton button3(5);
int button1State = 0;
int button2State = 0;
int button3State = 0;

//Accelerometer Values
long accelX, accelY, accelZ;
float gForceX, gForceY, gForceZ;
long gyroX, gyroY, gyroZ;
float rotX, rotY, rotZ;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(57600);

  //Joystick Init
  if(JoystickRun == 1){
    pinMode(joyX, INPUT);
    pinMode(joyY, INPUT);
  }

  //Accel Init
  if(AccelRun == 1 || GyroRun == 1){
    Wire.begin();
    setupMPU();
  }

  // Initalize the buttons
  if(ButtonRun == 1){
    pinMode(button1, INPUT);
    pinMode(button2, INPUT);
    pinMode(button3, INPUT);
    //Initialize the serial communication
    button1.setDebounceTime(30); // set debounce time to 30 milliseconds
    button2.setDebounceTime(30); 
    button3.setDebounceTime(30); 
  }
}

void loop() {
  //Run Joystick Code
  if(JoystickRun == 1){
    processJoyStick();
    /*
    int xValue = analogRead(joyX);
    int yValue = analogRead(joyY);
    mapX = map(xValue, 0, 1023, -512, 512);
    mapY = map(yValue, 0, 1023, -512, 512);
    Serial.print("JoyX: ");
    Serial.print(xValue);
    Serial.print(" | JoyY: ");
    Serial.println(yValue);
    */
  }
  //End Joystick Code

  //Run Accel Code
  if(AccelRun == 1){
    recordAccelRegisters();
  }
  if(GyroRun == 1){
    recordGyroRegisters();
  }
  if(AccelRun == 1 || GyroRun == 1){
    printAccelData();
  }
  //End Accel Code

  //Button Code
  if(ButtonRun == 1){
    runButtonCode();
  }
  
  delay(delayTime);
}

//Button Code
void runButtonCode(){
  // read button states
  button1.loop();
  button2.loop();
  button3.loop();

  button1State = button1.getState();
  button2State = button2.getState();
  button3State = button3.getState();

  if(!(button1State) && !(button2State) && !(button3State)){ // 000
    Serial.println(0);
  }

  if(button1State && !(button2State) && !(button3State)){ // 001
    Serial.println(2);
  }

  if(!(button1State) && button2State && !(button3State)){ // 010
    Serial.println(4);
  }

  if(button1State && button2State && !(button3State)) { // 011
    Serial.println(5);
  }

  if(!(button1State) && !(button2State) && button3State) { // 100
    Serial.println(7);
  }

  if(button1State && !(button2State) && button3State) { // 101
    Serial.println(9);
  }

  if(!(button1State) && button2State && button3State) { // 110
    Serial.println(11);
  }

  if(button1State && button2State && button3State) { // 111
    Serial.println(12);
  }
}
//Accel Code
void setupMPU(){
  Wire.beginTransmission(0b1101000); //This is the I2C address of the MPU (b1101000/b1101001 for AC0 low/high datasheet sec. 9.2)
  Wire.write(0x6B); //Accessing the register 6B - Power Management (Sec. 4.28)
  Wire.write(0b00000000); //Setting SLEEP register to 0. (Required; see Note on p. 9)
  Wire.endTransmission();  
  Wire.beginTransmission(0b1101000); //I2C address of the MPU
  Wire.write(0x1B); //Accessing the register 1B - Gyroscope Configuration (Sec. 4.4) 
  Wire.write(0x00000000); //Setting the gyro to full scale +/- 250deg./s 
  Wire.endTransmission(); 
  Wire.beginTransmission(0b1101000); //I2C address of the MPU
  Wire.write(0x1C); //Accessing the register 1C - Acccelerometer Configuration (Sec. 4.5) 
  Wire.write(0b00000000); //Setting the accel to +/- 2g
  Wire.endTransmission(); 
}

void recordAccelRegisters() {
  Wire.beginTransmission(0b1101000); //I2C address of the MPU
  Wire.write(0x3B); //Starting register for Accel Readings
  Wire.endTransmission();
  Wire.requestFrom(0b1101000,6); //Request Accel Registers (3B - 40)
  while(Wire.available() < 6);
  accelX = Wire.read()<<8|Wire.read(); //Store first two bytes into accelX
  accelY = Wire.read()<<8|Wire.read(); //Store middle two bytes into accelY
  accelZ = Wire.read()<<8|Wire.read(); //Store last two bytes into accelZ
  processAccelData();
}

void processAccelData(){
  gForceX = accelX / 16384.0;
  gForceY = accelY / 16384.0; 
  gForceZ = accelZ / 16384.0;
}

void recordGyroRegisters() {
  Wire.beginTransmission(0b1101000); //I2C address of the MPU
  Wire.write(0x43); //Starting register for Gyro Readings
  Wire.endTransmission();
  Wire.requestFrom(0b1101000,6); //Request Gyro Registers (43 - 48)
  while(Wire.available() < 6);
  gyroX = Wire.read()<<8|Wire.read(); //Store first two bytes into accelX
  gyroY = Wire.read()<<8|Wire.read(); //Store middle two bytes into accelY
  gyroZ = Wire.read()<<8|Wire.read(); //Store last two bytes into accelZ
  processGyroData();
}

void processGyroData() {
  rotX = gyroX / 131.0;
  rotY = gyroY / 131.0; 
  rotZ = gyroZ / 131.0;
}

void printAccelData() {
  if(rotX > 50){
    Serial.println(rotX+ 100);
  }
  /*
  Serial.print("Gyro (deg)");
  Serial.print(" X=");
  Serial.print(rotX);
  Serial.print(" Y=");
  Serial.print(rotY);
  Serial.print(" Z=");
  Serial.print(rotZ);
  Serial.print(" Accel (g)");
  Serial.print(" X=");
  Serial.print(gForceX);
  Serial.print(" Y=");
  Serial.print(gForceY);
  Serial.print(" Z=");
  Serial.println(gForceZ);
  */
}

void processJoyStick(){
  int xValue = analogRead(joyX);
  int yValue = analogRead(joyY);
  if(yValue < 100 && xValue < 100){
    Serial.println(10000);
  }
  else if(xValue < 100){
    Serial.println(20000);
  }
  else if(xValue < 100 && yValue > 900){
    Serial.println(30000);
  }
  else if(yValue > 900){
    Serial.println(40000);
  }
  else if(yValue > 900 && xValue > 900){
    Serial.println(50000);
  }
  else if(xValue > 900){
    Serial.println(60000);
  }
  else if(xValue > 800 && yValue < 200){
    Serial.println(70000);
  }
  else if(yValue < 100){
    Serial.println(80000);
  }
  
}
