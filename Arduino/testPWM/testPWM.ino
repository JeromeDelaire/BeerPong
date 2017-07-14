
/* Pins */
int red = 9, blue = 3, green = 5 ;

/* PWM values */
int pwm_red = 255, pwm_green = 255, pwm_blue = 255 ;

void setup() {
  pinMode(red, OUTPUT);
  pinMode(blue, OUTPUT);
  pinMode(green, OUTPUT);
}

void loop() {
  analogWrite(blue, pwm_blue);
  analogWrite(red, pwm_red);
  analogWrite(green, pwm_green);

  if(pwm_blue!=0){
   pwm_blue-- ; 
  }
  
  else{
    if(pwm_red!=0){
     pwm_red--; 
    }
    
    else{
      if(pwm_green!=0){
       pwm_green--; 
      }
      
      else{
        pwm_blue = 255 ;
        pwm_red=255;
        pwm_green=255;
      }
    }
  }

  delay(10);
}
