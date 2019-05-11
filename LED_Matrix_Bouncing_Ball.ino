#include "LedControl.h"
#include "binary.h"
int x = 0;
int y = 0;
int xnew = 0;
int ynew = 0;
int xvel = 1;
int yvel = 1;
int delaytime = 20;

/*
 DIN connects to pin 7
 CLK connects to pin 5
 CS connects to pin 6 
*/
LedControl lc=LedControl(7,5,6,4);

void setup() {
  for(int i=0;i<4; i++){
    lc.shutdown(i,false);
  
    lc.setIntensity(i,8);// Set brightness 
    
    lc.clearDisplay(i);  // Clear the display
  }  
  Serial.begin(9600);
}

void drawDot(int x,int y, int state){
    int panel = x / 8; // sets the panel that the ball appears on
    int ax = 7 - x%8; // sets the adjusted x value of the ball due to the panels changing the coordinates
       
    lc.setLed(panel,y,ax,state); // sets the LED the coordinate y, ax on the specified panel to the state
    /*
    Serial.print("X Value: ");
    Serial.print(x);
    Serial.print("\t Y Value: ");
    Serial.print(y);
    Serial.print("\t Adjusted X Value: ");
    Serial.print(ax);
    Serial.print("\t Panel Number: ");
    Serial.println(panel);
    */
}


void loop(){
  xnew +=xvel; //updates new x-value
  ynew +=yvel; //updates new y-value
  drawDot(x,y,0); //turns off the old LED
  drawDot(xnew,ynew,1); //turns on the new LED
  delay(delaytime); 
  
  if((ynew >= 7) || (ynew <= 0)){
    yvel=-yvel; //flips y velocity due to bounce
  }

  if((xnew >= 31) || (xnew <= 0)){
    xvel=-xvel;//flips x velocity due to bounce
  }
  
  x=xnew; //updates x-value
  y=ynew; //updates y-value

}
