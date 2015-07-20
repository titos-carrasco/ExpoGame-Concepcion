
#include <U8glib.h> //Graphic library for GLCD
#include <stdlib.h> //Standard IO from C, used for itoa
 
//U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE|U8G_I2C_OPT_DEV_0);  // I2C / TWI 
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_DEV_0|U8G_I2C_OPT_NO_ACK|U8G_I2C_OPT_FAST); // Fast I2C / TWI 
//U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NO_ACK);  // Display which does not send AC

const unsigned short leftPaddle = A1, leftButton = 7, rightPaddle = A0, rightButton = 8;

void setup(void) {
  Serial.begin(9600);

  pinMode(leftPaddle, INPUT);
  pinMode(leftButton, INPUT);
  pinMode(rightPaddle, INPUT);
  pinMode(rightButton, INPUT);
  
  //u8g.setColorIndex(1);
  //u8g.setRot180();

  while(digitalRead(leftButton)==1 && digitalRead(rightButton)==1 ){
    // Aquí se despliega una pantalla. Se repite lo mismo de manera estática
    // todos los pantallazos se trabajan de la misma manera
    u8g.firstPage();
    do {
      u8g.setFont(u8g_font_unifont);
      u8g.setPrintPos(0, 15); 
      u8g.print("Para Comenzar");
      u8g.setPrintPos(0, 30); 
      u8g.print("Presione un");
      u8g.setPrintPos(0, 45); 
      u8g.print("Boton");
    }
    while(u8g.nextPage());
  }
  delay(1000);
}

void loop(void) {
  int lb = digitalRead(leftButton);
  int lp = analogRead(leftPaddle);
  int rb = digitalRead(rightButton);
  int rp = analogRead(rightPaddle);
  
  u8g.firstPage();  
  do {
      u8g.setFont(u8g_font_unifont);
      u8g.setPrintPos(0, 15); 
      u8g.print("Left : ");
      u8g.setPrintPos(50, 15); 
      u8g.print(lb);
      u8g.setPrintPos(65, 15); 
      u8g.print(lp);
      u8g.setPrintPos(0, 35); 
      u8g.print("Right: ");
      u8g.setPrintPos(50, 35); 
      u8g.print(rb);
      u8g.setPrintPos(65, 35); 
      u8g.print(rp);
  } while( u8g.nextPage() );
  
  //delay(500);
}


/*
void loop(void){
  if (start==false){ //Start is false by default and it will be set false whenever a point is marked
    initialisation(); //When a point is marked, we have to initialise the game
  }
  racket_movements(); //This function determines the position of the two rackets depending on the joysticks movements
 
  //Here are described the movements of the ball depending on Posx and Posy
  if(x==true){
    Posx+=ball_speed; //The ball will move to the right
  }
  if(x==false){
    Posx-=ball_speed; //The ball will move to the left
  }
  if(y==true && angle_bounce==true){ //angle_bounce is when the ball bounces on the racket with an angle, not directly
    Posy+=ball_speed; //The ball will move down
  }
  if(y==false && angle_bounce==true){
    Posy-=ball_speed; //The ball will move up
  }
  if(angle_bounce==false){ //If there's no angle, it's a direct shot
    ball_speed=5; //As it's a direct shot, the speed is increased
  }
  else{
    ball_speed=2; //If no direct shot, the ball moves slower
  }
 
  //Horizontal limitations & bounce
  if(Posy<=3){ //If the ball hits the upper wall
    y=true; //As described before, the ball will bounce down
  }
  if(Posy>=60){ //If the ball hits the lower wall
    y=false; //As described before, the ball will bounce up
  }
 
  //Bounces on the rackets
  //Bounce on the left racket
  if (Posx<64){ //This tells arduino to look for bounces only if the ball is in the half of the screen where it can happend
    left_bounce();
  }
  //Bounce on the right racket
  if (Posx>64){ //This tells arduino to look for bounces only if the ball is in the half of the screen where it can happend
    right_bounce();
  }
 
  //Scoring
  if(Posx<=0){ //If the ball went behind the left racket
    scoreL++; //Increments the left score
    start=false; //Initalize the game
  }
  if(Posx>=127){ //If the ball went behind the right racket
    scoreR++; //Increments the right score
    start=false; //Initalize the game
  }
 
 
  u8g.firstPage(); //This is the u8glib function to draw the screen
  do {
    game(); //This corresponds to the game screen
  }
  while(u8g.nextPage());
}
 
void intro(void){
  u8g.drawBitmapP(0,0,16, 64, fond_1); //Draws the beginning screen
  u8g.drawBitmapP(0,0,16, 64, fond_2);
  u8g.drawBitmapP(64,7,2, 16, haut_blanc); //Draws the empty arrows
  u8g.drawBitmapP(104,7,2, 16, haut_blanc);
  if(r==true){ //If the corresponding Joystick has been pushed, fill the arrow
    u8g.drawBitmapP(105,9,1, 8, haut_noir);
  }
  if(l==true){
    u8g.drawBitmapP(65,9,1, 8, haut_noir);
  }
}
 
void game(void) {
  u8g.drawBitmapP(Posx-3,Posy-3,1,8,balle); //Draw the ball
  u8g.drawFrame(0,0,128,64); //Draws a frame
  for (int i=0;i<64;i=i+8){ //Draws a doted line at the middle
    u8g.drawLine(64,i,64,i+4);
  }  
  u8g.drawLine(124,R,124,R+9); //Draws the right racket
  u8g.drawLine(3,L,3,L+9); //Draws the left racket
 
  u8g.setFont(u8g_font_5x7); //Set a default font

  char buffer1[33];
  itoa(scoreR, buffer1, 10);
  char buffer2[33];
  itoa(scoreL, buffer2, 10);
 
  u8g.drawStr(54,8,buffer1); //Draws the score of each player
  u8g.drawStr(66,8,buffer2);
}
 
//FUNCTIONS
 
void initialisation(void){
  Posx=64; //Setting the ball at the middle of the screen
  Posy=32;
  randomSeed(analogRead(A7)); //We use A7 to create a random seed wich is random by the noise on analogic pins
  x1=random(0,100); //We take two random numbers from 0 to 100
  y1=random(0,100);
 
  // All we do here is randomly give the ball a direction, given by a couple of numbers. For instance (Right, Up) or (Left, Up)
  if(x1<50){
    x=false;
  }
  if(x1>50){
    x=true;
  }
  if(y1<50){
    y=false;
  }
  if(y1>50){
    y=true;
  }
  start=!start; // Start was false, we set it to true
}
 
void racket_movements(void){
  a=analogRead(Right); //We take some input from each joystick
  b=analogRead(Left);  
  //Controlling the racket on the right
  if (a > 800 && R>2){
    R-=racket_speed;
  }
  if (a < 200 && R<52){
    R+=racket_speed;
  }
  //Controlling the racket on the left
  if (b > 800 && L>2){
    L-=racket_speed;
  }
  if (b < 200 && L<52){
    L+=racket_speed;
  }
}
 
void left_bounce(void){
  if(Posx>2 && Posx<9 && Posy>=L-2 && Posy<L+3){ //Upper part of the racket
    if(angle_bounce==false){ //If direct hit
      y=false; //The ball goes up
    }
    x=true; //The ball moves to the right
    angle_bounce=true; //If bounce on upper or lower side, stops direct shots
  }
  if(Posx>2 && Posx<9 && Posy>L+6 && Posy<=L+11){ //Lower part of the racket
    if(angle_bounce==false){ //If direct hit
      y=true;  //The ball goes down
    }
    x=true; //The ball moves to the right
    angle_bounce=true; //If bounce on upper or lower side, stops direct shots
  }
  if(Posx>2 && Posx<9 && Posy>=L+3 && Posy<=L+6){ //Middle part of the racket
    x=true; //The ball moves to the right
    angle_bounce=false; //Do a direct shot
  }
}
 
void right_bounce(void){
  if(Posx>117 && Posx<125 && Posy>=R-2 && Posy<R+3){ //Upper part of the racket
    if(angle_bounce==false){
      y=false;
    }
    x=false;
    angle_bounce=true;
  }
  if(Posx>117 && Posx<125 && Posy>R+6 && Posy<=R+11){ //Lower part of the racket
    if(angle_bounce==false){
      y=true;
    }
    x=false;
    angle_bounce=true;
  }
  if(Posx>117 && Posx<125 && Posy>=R+3 && Posy<=R+6){ //Middle part of the racket
    x=false;
    angle_bounce=false;
  }
}*/


