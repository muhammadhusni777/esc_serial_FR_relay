String readString;


#include <Wire.h> 

#include "ESC.h"
ESC myESC (9, 1000, 2000, 500);         // ESC_Name (PIN, Minimum Value, Maximum Value, Arm Value)


int bldc_speed = 500;
int counter;
int rpm;

unsigned long arm_time;
unsigned long arm_time_prev;

int dir = 1;
int prev_dir = 1;

int dir_pin = 10;
int arm_pin = 11;

bool arming;

void setup() {
  pinMode(dir_pin, OUTPUT);
  pinMode(arm_pin, OUTPUT);
  Serial.begin(9600);
  myESC.arm();
  delay(5000);     
  
 
}





void loop() {
  while (Serial.available()) {
    char c = Serial.read();  
    readString += c; 
    delay(2);  
  }

    if (readString.length() >0) { 
    bldc_speed  = readString.toInt();  
    
 readString=""; 
}
if (bldc_speed < 0){
  dir = -1;
} else {
  dir = 1;
}

//ARMING ULANG
//Serial.println(arming);
if (dir != prev_dir){
  arming = true;
  Serial.println("ARMING START");
} 


if (arming == true){
  arm_time = millis() - arm_time_prev;
  if (arm_time < 500){
    Serial.println("arming part 1");
    pinMode(arm_pin, LOW);
  }
  if(arm_time > 500 and arm_time < 1000){
    Serial.println("arming part 2");
    
    if (dir==-1){
      pinMode(dir_pin, LOW);
    }

    if(dir==1) {
      pinMode(dir_pin, HIGH);
    }
    
  }
  if (arm_time > 1000 and arm_time < 7000){
    pinMode(arm_pin, HIGH);
    Serial.println("arming part 3");
    myESC.speed(abs(1000)); 
    //myESC.arm();
  }
  if (arm_time > 7000){
    arming = false;
    Serial.println("ARMING DONE");
    //arm_time_prev = millis();
  }
  
  
}

if (arming == false){
    arm_time_prev = millis();
    myESC.speed(abs(bldc_speed)); 
  }



prev_dir = dir;
}
