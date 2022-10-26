const int button1 = 3;
const int button2 = 4;
const int button3 = 5;

int button1State = 0; //Current State of Button
int lastButton1State = 0; //Previous Button State
int button2State = 0; //Current State of Button
int lastButton2State = 0; //Previous Button State
int button3State = 0; //Current State of Button
int lastButton3State = 0; //Previous Button State

void setup() {
  // Initalize the buttons
  pinMode(button1, INPUT);
  pinMode(button2, INPUT);
  pinMode(button3, INPUT);
  //Initialize the serial communication
  Serial.begin(9600);
}

void loop() {
  // read button states
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

  //delay 10ms
  delay(10);
}
