/*
 Created by Rui Santos
 
 All the resources for this project:
 https://randomnerdtutorials.com/
*/

#include "LedControl.h"
#include "binary.h"
int dig = 0;
int xval = 0;
int yval = 0;
int xvalold = 0;
int yvalold = 0;

/*
 DIN connects to pin 7
 CLK connects to pin 5
 CS connects to pin 6 
*/
LedControl lc=LedControl(7,5,6,4);

void setup() {
  for(int i=0;i<4; i++){
    lc.shutdown(i,false);
  // Set brightness to a medium value
  lc.setIntensity(i,15);
  // Clear the display
  lc.clearDisplay(i);  
  }  
  Serial.begin(9600);
}

void drawDot(int xval,int yval, int state){
    int x = xval;
    int y = yval;
    int panel = x / 8;
    int ax = 7 - x%8;
       
    lc.setLed(panel,y,ax,state);
    Serial.print("X Value: ");
    Serial.print(x);
    Serial.print("\t Y Value: ");
    Serial.print(y);
    Serial.print("\t Adjusted X Value: ");
    Serial.print(ax);
    Serial.print("\t Panel Number: ");
    Serial.println(panel);
}


void loop(){
  if (Serial.available() > 0) {
    int incomingByte = Serial.read();
    if (('0' <= incomingByte) && (incomingByte <= '9')){
      int digit = (incomingByte) - '0';
      dig = dig +1; 
      Serial.print("Dig: ");
      Serial.print(dig);
      Serial.print("\t Read from Serial: ");
      Serial.println(incomingByte);
      if(dig == 1){
        xval += digit * 10;
      }
      
      if(dig == 2){
        xval += digit * 1;
      }
      if(dig == 3){
        yval += digit * 10;
      }
      
      if(dig >= 4){
        yval += digit * 1;
        dig = 0;
        drawDot(xvalold,yvalold, 0);
        drawDot(xval,yval, 1);
        xvalold=xval;
        yvalold=yval;
        xval=0;
        yval=0;
      }
      
    } 
  }
}
