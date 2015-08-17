// modified by peter kydd
// 17/08/15
// control 2 motors with PWM with SONAR safety stop

#include <NewPing.h>


#define  TRIGGER_PIN  11
#define  ECHO_PIN     10
#define MAX_DISTANCE 200


// SONAR OBJECTS
// NewPing setup of pins and maximum distance.
int DistanceIn;
int DistanceCm;
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

// TIME OBJECTS
unsigned long milliHistory;
unsigned long milliCurrent;

// MOTOR OBJECTS
int E1 = 5;     //M1 Speed Control
int E2 = 6;     //M2 Speed Control
int M1 = 4;     //M1 Direction Control
int M2 = 7;     //M1 Direction Control
int counter=0;
 
void stop(void){
  digitalWrite(E1,0); 
  digitalWrite(M1,LOW);    
  digitalWrite(E2,0);   
  digitalWrite(M2,LOW);    
}   

void advance(char a,char b){
  analogWrite (E1,a);   
  digitalWrite(M1,HIGH);    
  analogWrite (E2,b);    
  digitalWrite(M2,HIGH);
}  

void back_off (char a,char b){
  analogWrite (E1,a);
  digitalWrite(M1,LOW);   
  analogWrite (E2,b);    
  digitalWrite(M2,LOW);
}

void turn_L (char a,char b){
  analogWrite (E1,a);
  digitalWrite(M1,LOW);    
  analogWrite (E2,b);    
  digitalWrite(M2,HIGH);
}

void turn_R (char a,char b){
  analogWrite (E1,a);
  digitalWrite(M1,HIGH);    
  analogWrite (E2,b);    
  digitalWrite(M2,LOW);
}

void current_sense(){
  int val1=digitalRead(2);            
  int val2=digitalRead(3);
  if(val1==HIGH || val2==HIGH){
    counter++;
    if(counter==3){
      counter=0;
      Serial.println("Warning");
    }  
  } 
}

void ping_target(){
  DistanceCm = sonar.ping_cm();
  Serial.print("Ping: ");
  Serial.print(DistanceCm); 
  Serial.println(" cm"); 

  if( DistanceCm < 15){
    stop();
  }
}
 
void setup(void){ 
  
  int i;
  for(i=4;i<=7;i++)
    pinMode(i, OUTPUT);  
  Serial.begin(19200);      //Set Baud Rate
  Serial.println("Run keyboard control");
  digitalWrite(E1,LOW);   
  digitalWrite(E2,LOW); 
  pinMode(2,INPUT);
  pinMode(3,INPUT);

  milliHistory = 0;
} 
 
void loop(void) {
  
  milliCurrent = millis();
  if(milliCurrent > milliHistory+100){
    ping_target();  
    milliHistory = millis();
  }


  if(Serial.available()){
    char val = Serial.read();
    if(val != -1)
    {
      switch(val)
      {
      case 'w'://Move Forward
        advance (255,255);   //move forward in max speed
        break;
      case 's'://Move Backward
        back_off (255,255);   //move back in max speed
        break;
      case 'a'://Turn Left
        turn_L (100,100);        
        break;       
      case 'd'://Turn Right
        turn_R (100,100);
        break;
      case 'z':
        Serial.println("Hello");
        break;
      case 'x':
        stop();
        break;
      }
    }
    else stop();  
  }
 
}
