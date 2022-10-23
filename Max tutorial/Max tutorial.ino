//Constants
const int buttonPin = 3;//Push Button
const int ledPin = 2;//uno board led on pin

//Variables
int buttonState = 0; //Current State of Button
int lastButtonState = 0; //Previous Button State
int outValue = 120;//value to be sent to serial/max

// put your setup code here, to run once:
void setup() {
  // Initalize the button on pin 2
  //TEST penis
  pinMode(buttonPin, INPUT);
  // Initalize the LED
  pinMode(ledPin, OUTPUT);
  //Initialize the serial communication
  Serial.begin(9600);
}

// put your main code here, to run repeatedly:
void loop() {
  //Read Button State
  //TEST penis
  buttonState = digitalRead(buttonPin);
  //Compare with previous to see if changed
  if(buttonState != lastButtonState) {
    //If the state has changed, do nothing
    if(buttonState == 1){
      
    } 
    else{
      //If current state is low, send value
      Serial.println(130);
    }
    //wait 50ms to do anything
    delay(50);
  }

  lastButtonState = buttonState;
}
