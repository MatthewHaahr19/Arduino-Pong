#include "LedControl.h"
#include "binary.h"

#define Lbutton 8
#define Rbutton 9
#define Lstick A1
#define Rstick A0

int x = 0;
int y = 0;
int xnew = 0;
int ynew = 0;
int xvel = 1;
int yvel = 1;
int delaytime = 20;
int LSval = 0;
int RSval = 0;

/*
 DIN connects to pin 7
 CLK connects to pin 5
 CS connects to pin 6 
*/
LedControl lc=LedControl(7,5,6,4);

void setup() {
  for(int i=0;i<4; i++){
    lc.shutdown(i,false);
  
    lc.setIntensity(i,12);// Set brightness 
    
    lc.clearDisplay(i);  // Clear the display
  }  
  Serial.begin(9600);
  pinMode(Lstick, INPUT); //Sets up Left Joystick
  pinMode(Lbutton, INPUT); //Sets up Left Button
  digitalWrite(Lbutton, HIGH); //Sets up Left Button
  pinMode(Rstick, INPUT); //Sets up Right Joystick
  pinMode(Rbutton, INPUT); //Sets up Right Button
  digitalWrite(Rbutton, HIGH); //Sets up Right Button
  for (int i=0; i<8; i++){ //draws the lines at the edges of the display
    drawDot(0,i,1);
    drawDot(31,i,1);
  }
  
}

void drawDot(int x,int y, int state){
    int panel = x / 8; // sets the panel that the ball appears on
    int ax = 7 - x%8; // sets the adjusted x value of the ball due to the panels changing the coordinates
       
    lc.setLed(panel,y,ax,state); // sets the LED the coordinate y, ax on the specified panel to the state
}

void loop(){
  int newLSval = analogRead(Lstick); //sets the read value of the Left joystick to a new variable
  int newRSval = analogRead(Rstick); //sets the read value of the Right joystick to a new variable
  newLSval = map(newLSval, 0, 1000, 1, 6); //maps the read value to a usable value 
  newRSval = map(newRSval, 0, 1010, 1, 6); //maps the read value to a usable value 

  int LBval = digitalRead(Lbutton); //sets the read value of the Left button to a new variable
  int RBval = digitalRead(Rbutton); //sets the read value of the Right button to a new variable
  LBval = map(LBval,0,1,1,0); //maps the read value to a usable value 
  RBval = map(RBval,0,1,1,0); //maps the read value to a usable value 

  drawDot(2,LSval,0); //turns off the old position of the middle pixel of the Left paddle
  drawDot(2,(LSval-1),0); //turns off the old position of the bottom pixel of the Left paddle
  drawDot(2,(LSval+1),0); //turns off the old position of the Top pixel of the Left paddle
  drawDot(29,(RSval),0); //turns off the old position of the middle pixel of the Right paddle
  drawDot(29,(RSval-1),0); //turns off the old position of the bottom pixel of the Right paddle
  drawDot(29,(RSval+1),0); //turns off the old position of the top pixel of the Right paddle

  drawDot(2,newLSval,1); //turns on the old position of the middle pixel of the Left paddle
  drawDot(2,(newLSval-1),1); //turns on the old position of the bottom pixel of the Left paddle
  drawDot(2,(newLSval+1),1); //turns on the old position of the top pixel of the Left paddle
  drawDot(29,(newRSval),1); //turns on the old position of the middle pixel of the Right paddle
  drawDot(29,(newRSval-1),1); //turns on the old position of the bottom pixel of the Right paddle
  drawDot(29,(newRSval+1),1); //turns on the old position of the top pixel of the Right paddle

  LSval = newLSval; //updates Left paddle position
  RSval = newRSval; //updates Right paddle position
  
  

}
