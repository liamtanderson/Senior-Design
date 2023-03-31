  #include <ezButton.h>
  // Initalize the buttons
  ezButton button1(3);
  ezButton button2(4);
  ezButton button3(5);
  int button1State = 0;
  int button2State = 0;
  int button3State = 0;

void setup() {
  //Initialize the serial communication
  Serial.begin(9600);
  button1.setDebounceTime(30); // set debounce time to 30 milliseconds
  button2.setDebounceTime(30); 
  button3.setDebounceTime(30); 

}

void loop() {
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

  delay(30);
}
