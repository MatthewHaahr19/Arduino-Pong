#include "LedControl.h"
#include "binary.h"

#define Lbutton 8 //sets the pins of each input
#define Rbutton 9
#define Lstick A1
#define Rstick A0

#define Apin1 26 //a segment of left 7-seg display
#define Bpin1 28 //b segment of left 7-seg display
#define Cpin1 30 //c segment of left 7-seg display
#define Dpin1 32 //d segment of left 7-seg display
#define Epin1 34 //e segment of left 7-seg display
#define Fpin1 24 //f segment of left 7-seg display
#define Gpin1 22 //g segment of left 7-seg display

#define Apin2 38 //a segment of right 7-seg display
#define Bpin2 36 //b segment of right 7-seg display
#define Cpin2 44 //c segment of right 7-seg display
#define Dpin2 46 //d segment of right 7-seg display
#define Epin2 48 //e segment of right 7-seg display
#define Fpin2 40 //f segment of right 7-seg display
#define Gpin2 42 //g segment of right 7-seg display

int x = 0; //sets intial x coordinte of the ball
int y = 0; //sets intial y coordinte of the ball
int xnew = 0; //sets initial next x coordinate of the ball
int ynew = 0; //sets initial next y coordinate of the ball
int xvel = 0; //sets intial x velocity of the ball
int yvel = 0; //sets intial y velocity of the ball
int delaytime = 20; //sets delay between frames
int Lscore = 0; //starts Left player's score at 0
int Rscore = 0; //starts Right player's score at 0
int serveSide = 0; //starts serve as neither player can serve

byte paddle[7] = {B11100000, B01110000, B00111000, B00011100, B00001110, B00000111, B00000000}; //establishes each frame of the paddle display



byte leftdigits[] = { Apin1, Bpin1, Cpin1, Dpin1, Epin1, Fpin1, Gpin1 }; //left side digit pins
byte rightdigits[] = { Apin2, Bpin2, Cpin2, Dpin2, Epin2, Fpin2, Gpin2 }; //right side digit pins

byte numberFrame[10][7] = { //frame definition array
  { HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, LOW }, // 0
  { LOW, HIGH, HIGH, LOW, LOW, LOW, LOW }, // 1
  { HIGH, HIGH, LOW, HIGH, HIGH, LOW, HIGH }, // 2
  { HIGH, HIGH, HIGH, HIGH, LOW, LOW, HIGH }, // 3
  { LOW, HIGH, HIGH, LOW, LOW, HIGH, HIGH }, // 4
  { HIGH, LOW, HIGH, HIGH, LOW, HIGH, HIGH }, // 5
  { HIGH, LOW, HIGH, HIGH, HIGH, HIGH, HIGH }, // 6
  { HIGH, HIGH, HIGH, LOW, LOW, LOW, LOW }, // 7
  { HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH }, // 8
  { HIGH, HIGH, HIGH, HIGH, LOW, HIGH, HIGH }, // 9
};

/*
 DIN connects to pin 7
 CLK connects to pin 5
 CS connects to pin 6 
*/
LedControl lc=LedControl(7,5,6,4); //initializes LED matrices

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
  
  reset(); //runs reset function

  for (int i=0; i<8; i++){ //draws the lines at the edges of the display
    drawDot(0,i,1);
    drawDot(31,i,1);
  }
  
  score(); //runs score function
  serveSide = -1; //allows the left side player to serve first
}


void writescore(int side, int score) {
  byte *digits;
  if (side == 0) { // left
    digits = leftdigits; //uses the left side pins if the left is called
  } 
  else {
    digits = rightdigits; //uses the right side pins if the right is called
  }

  byte *number = numberFrame[score]; 

  for (int i = 0; i < 7; i++){
    digitalWrite(digits[i], number[i]); //writes each digit pin according to the value assigned to it in the frame definition array
  }
}

void score() {
  if ((Lscore == 10) || (Rscore == 10)){ //if either player has a score of 10 it resets the game
    Lscore = 0; //resets the left score to 0
    Rscore = 0; //resets the right score to 0
    reset();
  }
  
  writescore(0,Lscore); //calls the score on the left side
  writescore(1,Rscore); //calls the score on the right side
}

void drawDot(int x,int y, int state){
    int panel = x / 8; // sets the panel that the ball appears on
    int ax = 7 - x%8; // sets the adjusted x value of the ball due to the panels changing the coordinates
       
    lc.setLed(panel,y,ax,state); // sets the LED the coordinate y, ax on the specified panel to the state
}

void reset(){
  
  int posrand = random(1,4); //sets the starting postion to a random point of the four center LEDs
  if (posrand ==1){
    xnew=15;
    ynew=3;
  }
  if (posrand ==2){
    xnew=16;
    ynew=3;
  }
  if (posrand ==3){
    xnew=15;
    ynew=4;
  }
  if (posrand ==4){
    xnew=16;
    ynew=4;
  }
  
  drawDot(x,y,0);//turns off the old LED
  x=xnew; //sets the old x value to the new x value
  y=ynew; //sets the old y value to the new y value
  xvel = 0; //resets x velocity to 0
  yvel = 0; //resets y velocity to 0
} 

void serve(int dir){
  if (dir==1 && serveSide==-1){ //checks to see if the Left player clicks the button and they are allowed to serve
    int Lrand = random(1,3); //randomly picks one of the three directions the ball can travel if the left player has served it
    if (Lrand == 1){
      xvel = 1;
      yvel = 1;
      
      serveSide=0; //resets the serve side so neither can accidentally serve the ball
    }
    if (Lrand == 2){
      xvel = 1;
      yvel = 0;
      
      serveSide=0; //resets the serve side so neither can accidentally serve the ball
    }
    if (Lrand == 3){
      xvel = 1;
      yvel = -1;
       
      serveSide=0; //resets the serve side so neither can accidentally serve the ball
    }
    
  }
  if (dir==0 && serveSide==1 ){ //checks to see if the Right player clicks the button and they are allowed to serve
    int Rrand = random(1,3); //randomly picks one of the three directions the ball can travel if the right player has served it
    if (Rrand == 1){
      xvel = -1;
      yvel = 1;
      
      serveSide=0; //resets the serve side so neither can accidentally serve the ball
    }
    if (Rrand == 2){
      xvel = -1;
      yvel = 0;
      
      serveSide=0; //resets the serve side so neither can accidentally serve the ball
    }
    if (Rrand == 3){
      xvel = -1;
      yvel = -1;
      
      serveSide=0; //resets the serve side so neither can accidentally serve the ball
    }
    
  }
}

void loop(){
  int LSval = analogRead(Lstick); //sets the read value of the Left joystick to a new variable
  int RSval = analogRead(Rstick); //sets the read value of the Right joystick to a new variable
  LSval = map(LSval, 15, 990, 1, 6); //maps the read value to a usable value 
  RSval = map(RSval, 15, 990, 1, 6); //maps the read value to a usable value 

  int LBval = digitalRead(Lbutton); //sets the read value of the Left button to a new variable
  int RBval = digitalRead(Rbutton); //sets the read value of the Right button to a new variable
  LBval = map(LBval,0,1,1,0); //maps the read value to a usable value 
  RBval = map(RBval,0,1,1,0); //maps the read value to a usable value
  
  xnew +=xvel; //updates new x-value
  ynew +=yvel; //updates new y-value

  if(xnew == 0){ //checks to see if the ball hase passed the furtherst left line
    Rscore += 1; //adds one to the right player's score
    serveSide=-1; //sets the serve to the left player's turn
    reset(); //runs the reset program
  }

  if(xnew == 31){ //checks to see if the ball hase passed the furtherst right line
    Lscore += 1; //adds one to the left player's score
    serveSide=1; //sets the serve to the right player's turn
    reset(); //runs the reset program
  }

  if((xnew==3)&&(ynew==LSval+1)){ //if the ball is at the same position as the top pixel of the left paddle, it bounces to right and up
    yvel=1;
    xvel=-xvel;
  }
  if((xnew==3)&&(ynew==LSval)){ //if the ball is at the same position as the middle pixel of the left paddle, it bounces straight to the right
    yvel=0;
    xvel=-xvel;
  }
  if((xnew==3)&&(ynew==LSval-1)){ //if the ball is at the same position as the bottom pixel of the left paddle, it bounces to right and down
    yvel=-1;
    xvel=-xvel;
  }

  if((xnew==28)&&(ynew==RSval+1)){ //if the ball is at the same position as the top pixel of the right paddle, it bounces to left and up
    yvel=1;
    xvel=-xvel;
  }
  if((xnew==28)&&(ynew==RSval)){ //if the ball is at the same position as the middle pixel of the right paddle, it bounces straight to the left
    yvel=0;
    xvel=-xvel;
  }
  if((xnew==28)&&(ynew==RSval-1)){ //if the ball is at the same position as the bottom pixel of the right paddle, it bounces to left and down
    yvel=-1;
    xvel=-xvel;
  }
  
  drawDot(x,y,0); //turns off the old LED
  drawDot(xnew,ynew,1); //turns on the new LED
  delay(delaytime); //delays the next frame by the specified time
  
  if((ynew >= 7) || (ynew <= 0)){
    yvel=-yvel; //flips y velocity due to bounce
  }

  if(LBval == 1){ //checks to see if the left player has clicked in the stick
    serve(1); //runs the serve fucntion specifying that the left player has pressed the button
  }
  if(RBval == 1){ //checks to see if the right player has clicked in the stick
    serve(0); //runs the serve fucntion specifying that the right player has pressed the button
  }

  if(xvel == 0){
    int col;
    int panel;
    if(serveSide == -1){ //sets the server to the left paddle
      panel = 0;
      col = 5;
    }
    
    else if(serveSide == 1){ //sets the server to the right paddle
      panel = 3;
      col = 2;
    }
    
    lc.setColumn(panel,col,paddle[6]); //draws the server's paddle off
    delay(delaytime);
  }

  lc.setColumn(0,5,paddle[LSval-1]); //draws the left player's paddle using the corresponding frame
  lc.setColumn(3,2,paddle[RSval-1]); //draws the right player's paddle using the corresponding frame
  
  
  x=xnew; //updates x-value
  y=ynew; //updates y-value
  score(); //runs the score function to keep the display active
   

}
