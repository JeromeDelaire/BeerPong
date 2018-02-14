
#include <Wire.h>

int counter = 0 ;
/* Pins */
int red = 9, blue = 3, green = 5 ;

/* PWM values */
int pwm_red = 255, pwm_green = 255, pwm_blue = 255 ;

void setup() {
  pinMode(red, OUTPUT);
  pinMode(blue, OUTPUT);
  pinMode(green, OUTPUT);
  Wire.begin(0);
  Wire.onReceive(receiveEvent);
  Serial.begin(9600); 
}

void loop() {
  analogWrite(blue, pwm_blue);
  analogWrite(red, pwm_red);
  analogWrite(green, pwm_green);
}

void receiveEvent(int howMany) {
  while (1 < Wire.available()) { // loop through all but the last
    char c = Wire.read(); // receive byte as a character
  }
  int x = Wire.read();    // receive byte as an integer
      
  if(counter==0){
    counter=1 ;
    pwm_red = 255-x;
  }

  else if(counter==1){
    counter=2 ;
    pwm_green=255-x ;
  }

  else if(counter==2){
    counter=0;
    pwm_blue=255-x ;
    Serial.println((String) "r : " + pwm_red);
    Serial.println((String) "g : " + pwm_green);
    Serial.println((String) "b :" + pwm_blue);
  }
}
