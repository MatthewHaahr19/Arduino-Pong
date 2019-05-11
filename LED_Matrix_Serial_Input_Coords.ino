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
    int x = xval; //sets the x position that the dot appears on
    int y = yval; //sets the y position that the dot appears on
    int panel = x / 8; // sets the panel that the dot appears on
    int ax = 7 - x%8; // sets the adjusted x value of the dot due to the panels changing the coordinates
       
    lc.setLed(panel,y,ax,state); // sets the LED the coordinate y, ax on the specified panel to the state
    Serial.print("X Value: ");
    Serial.print(x); //prints overall x position
    Serial.print("\t Y Value: ");
    Serial.print(y); //prints overall y position
    Serial.print("\t Adjusted X Value: ");
    Serial.print(ax); //prints adjust x position
    Serial.print("\t Panel Number: ");
    Serial.println(panel); //prints panel number
}


void loop(){
  if (Serial.available() > 0) {
    int incomingByte = Serial.read(); //reads the incoming serial data
    if (('0' <= incomingByte) && (incomingByte <= '9')){ //makes sure that the incoming data is a digit
      int digit = (incomingByte) - '0';  //sets the digit to read the first digit
      dig = dig +1; //advances the digit by one
      Serial.print("Dig: "); 
      Serial.print(dig); //prints which digit it is
      Serial.print("\t Read from Serial: ");
      Serial.println(incomingByte); //prints what the arduino recieves from the serial
      if(dig == 1){ //checks if it is on the first digit (10s position for the x coordinate)
        xval += digit * 10; //sets the read value equal to the 10s position of the x value
      }
      
      if(dig == 2){ //checks if it is on the second digit (1s position for the x coordinate)
        xval += digit * 1; //sets the read value equal to the 1s position of the x value
      }
      if(dig == 3){ //checks if it is on the third digit (10s position for the y coordinate)
        yval += digit * 10; //sets the read value equal to the 10s position of the y value
      }
      
      if(dig >= 4){ //checks if it is on the fourth digit (1s position for the y coordinate)
        yval += digit * 1; //sets the read value equal to the 1s position of the y value
        dig = 0; //resets digit count to 0
        drawDot(xvalold,yvalold, 0); //turns off LED position
        drawDot(xval,yval, 1); //turns on new LED position
        xvalold=xval; //sets the old x value to equal the new
        yvalold=yval; //sets the old y value to equal the new
        xval=0; //resets x value
        yval=0; //resets y value
      }
      
    } 
  }
}
