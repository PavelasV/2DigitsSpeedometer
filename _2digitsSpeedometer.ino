#include "SevSeg.h"
#define reed A4//pin connected to read switch 
SevSeg sevseg;//1,2,3,4,5,6,7,8,9,10,11,12,13 pins for 2 * 7segment1digit displays
float radius = 14;// tire radius (in inches) 
int reedVal; 
long timer = 0;// time between one full rotation (in ms) 
float mph = 0.00; 
float circumference; 
boolean backlight; 

int maxReedCounter = 100;//min time (in ms) of one rotation (for debouncing), smaller is less precise
int reedCounter; 


void setup(){ 

  reedCounter = maxReedCounter; 
  circumference = 2*3.14*radius; 

  pinMode(reed, INPUT); 
  cli();
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0; 
  OCR1A = 1999;
  TCCR1B |= (1 << WGM12); 
  TCCR1B |= (1 << CS11); 
  TIMSK1 |= (1 << OCIE1A); 

  sei(); 
  sevseg.Begin(1,2,3,4,5,6,7,8,9,10,11,12,13);

} 

ISR(TIMER1_COMPA_vect) {
  reedVal = digitalRead(reed);
  if (reedVal){
    if (reedCounter == 0){
      mph = (56.8*float(circumference))/float(timer);
      timer = 0; 
      reedCounter = maxReedCounter;
    } 
    else{ 
      if (reedCounter > 0){
        reedCounter -= 1;
      } 
    } 
  } 
  else{
    if (reedCounter > 0){
      reedCounter -= 1;
    } 
  } 
  if (timer > 2000){ 
    mph = 0;
  } 
  else{ 
    timer += 1; 
  } 
} 

void displayKPH(){ 
  int kph = mph*1.60934;
  sevseg.PrintOutput();
  sevseg.NewNum(kph,(byte) 2);
} 
void displayMPH(){ 
  sevseg.PrintOutput();
  sevseg.NewNum(mph,(byte) 2);
} 

void loop(){ 
  displayKPH(); 
//  displayMPH();
  delay(5); 
} 
