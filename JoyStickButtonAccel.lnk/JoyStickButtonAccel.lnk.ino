#include <Time.h> 
unsigned long clocktime;
int lasttime = 0;
int numMeasurments = 0;
int jitter = 0;
int latency = 0;
int delayTime = 10;
int joyX = A0;
int joyY = A1;
int mapX = 0;
int mapY = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(joyX, INPUT);
  pinMode(joyY, INPUT);
}

void loop() {
  // Run Normal Code
  int xValue = analogRead(joyX);
  int yValue = analogRead(joyY);

  Serial.print("Variable_1:");
  Serial.print(xValue);
  Serial.print(",");
  Serial.print("Variable_2:");
  Serial.println(yValue);
  /*
  mapX = map(xValue, 0, 1023, -512, 512);
  mapY = map(yValue, 0, 1023, -512, 512);
  Serial.print("X: ");
  Serial.print(xValue);
  Serial.print(" | Y: ");
  Serial.print(yValue);
  */
  delay(delayTime);
  
  // Calculate Latency
  /*
  clocktime=millis();
  latency = clocktime - lasttime - delayTime;
  lasttime = clocktime;
  Serial.println(latency);
  */
}
