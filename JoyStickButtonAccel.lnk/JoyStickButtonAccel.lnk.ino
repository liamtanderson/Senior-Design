#include <Time.h> 
#include <Wire.h>
//Todo, cleanup accel code into import statement
//Jitter and Latency Values
unsigned long clocktime;
int lasttime = 0;
int numMeasurements = 0;
int jitter = 0;
int totalJitter = 0;
int latency = 0;
int lastLatency = 0;
int delayTime = 1000;

//WhatShouldRun
int JoystickRun = 1;
int GyroRun = 1;
int AccelRun = 1;
int ButtonRun = 1;

//Joystick Values
int joyX = A0;
int joyY = A1;
//int mapX = 0;
//int mapY = 0;

//Button Values
const int button1 = 3;
const int button2 = 4;
const int button3 = 5;

int button1State = 0; //Current State of Button
int lastButton1State = 0; //Previous Button State
int button2State = 0; //Current State of Button
int lastButton2State = 0; //Previous Button State
int button3State = 0; //Current State of Button
int lastButton3State = 0; //Previous Button State

//Accelerometer Values
long accelX, accelY, accelZ;
float gForceX, gForceY, gForceZ;
long gyroX, gyroY, gyroZ;
float rotX, rotY, rotZ;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

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
  }
}

void loop() {
  //Run Joystick Code
  if(JoystickRun == 1){
    int xValue = analogRead(joyX);
    int yValue = analogRead(joyY);
    /*
    mapX = map(xValue, 0, 1023, -512, 512);
    mapY = map(yValue, 0, 1023, -512, 512);
    */
    Serial.print("JoyX: ");
    Serial.print(xValue);
    Serial.print(" | JoyY: ");
    Serial.println(yValue);
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
  
  // Calculate Latency
  delay(delayTime);
  clocktime=millis();
  lastLatency = latency;
  latency = clocktime - lasttime - delayTime;
  lasttime = clocktime;
  
  // Calculate Jitter
  numMeasurements++;
  totalJitter += abs(latency - lastLatency);
  jitter = totalJitter/numMeasurements;
  Serial.print("Latency: ");
  Serial.print(latency);
  Serial.print(" | Average Jitter: ");
  Serial.println(jitter);
}

//Button Code
void runButtonCode(){
  button1State = digitalRead(button1);
  button2State = digitalRead(button2);
  button3State = digitalRead(button3);

  //See if button changed
  if(button1State != lastButton1State) {
    //If the state has changed, do nothing
    if(button1State == 1){
      
    } 
    else{
      //If current state is low, send value
      Serial.println(1);
    }
  }

  //See if button changed
  if(button2State != lastButton2State) {
    //If the state has changed, do nothing
    if(button2State == 1){
      
    } 
    else{
      //If current state is low, send value
      Serial.println(2);
    }
  }

  //See if button3 changed
  if(button3State != lastButton3State) {
    //If the state has changed, do nothing
    if(button3State == 1){
      
    } 
    else{
      //If current state is low, send value
      Serial.println(3);
    }
  }

  //Update last state
  lastButton1State = button1State;
  lastButton2State = button2State;
  lastButton3State = button3State;
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
}