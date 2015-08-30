/*
  Muestra la implementación de un juego similar al PONG utilizando
  un display oled de 128x64, 2 botones y 2 potenciómetros.

  TODO:
  * Todas las coordenadas están en duro en el código al igual que la velocidad
  * Fata agregar despliegue de puntaje y fin de juego
  * Falta agregarcambio en la dirección de la pelota según punto de golpe del pad
  * Muchas otras mejoras
  
  Este ejemplo de código es de dominio público
  This example code is in the public domain.
 */
#include <U8glib.h> //Graphic library for GLCD
#include <stdlib.h> //Standard IO from C, used for itoa
 
//U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE|U8G_I2C_OPT_DEV_0);  // I2C / TWI 
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_DEV_0|U8G_I2C_OPT_NO_ACK|U8G_I2C_OPT_FAST); // Fast I2C / TWI 
//U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NO_ACK);  // Display which does not send AC

#define LEFTPADPIN      A0    // potenciómetro izquierdo
#define RIGHTPADPIN     A6    // potenciómetro derecho
#define LEFTBUTTONPIN   11    // botón izquierdo
#define RIGHTBUTTONPIN  10    // botón derecho

short leftPadY;               // posición en Y del pad izquierdo
short rightPadY;              // posición en Y del pad derecho
short ballX;                  // posición en X de la pelota
short ballY;                  // posición en Y de la pelota
short ballXMov;               // sentido y velocidad de desplazamiento en X de la pelota
short ballYMov;               // sentido y velocidad de desplazamiento en Y de la pelota

/*
 * Pantalla de inicio
 */
void splashScreen() 
{
  u8g.firstPage();
  do {
    u8g.setFont(u8g_font_unifont);
    u8g.setPrintPos(12, 15); 
    u8g.print("Para Comenzar");
    u8g.setPrintPos(20, 30); 
    u8g.print("Presione un");
    u8g.setPrintPos(44, 45); 
    u8g.print("Boton");
  }
  while(u8g.nextPage());

  // espera por un botón
  while(!digitalRead(LEFTBUTTONPIN) && !digitalRead(RIGHTBUTTONPIN) ) 
  {
  }

  // espera a que lo suelte
  while(digitalRead(LEFTBUTTONPIN) || digitalRead(RIGHTBUTTONPIN) ) 
  {
  }
}

/*
 * Refresca la pantalla
 */
void refreshScreen()
{

  u8g.firstPage();  
  do {
    // el borde
    u8g.drawFrame(0, 0, 128, 64);

    // la línea central vertical
    for (int i=0; i<64; i+=8){
      u8g.drawLine(64, i, 64, i+4);
    }

    // los pads
    u8g.drawBox(6, 1 + leftPadY, 3, 10);
    u8g.drawBox(119, 1 + rightPadY, 3, 10);

    // la pelota
    u8g.drawBox( ballX, ballY, 4, 4);
  } while( u8g.nextPage() );
}

/*
 * Inicializa el punto a jugar
 * fromLeft indica si saca el player de la izquierda o el de la derecha
 */
void initPoint(bool fromLeft)
{
  randomSeed(analogRead(0));
  if(fromLeft)
  {
    ballX = 1;
    ballXMov = 2;
  }
  else
  {
    ballX = 127;
    ballXMov = -2;
  }
  ballY = 3 + random(10)*4;
  ballYMov = 2;
}

/*
 * Inicializa el juego
 */
void setup(void) 
{
  pinMode(LEFTBUTTONPIN, INPUT);
  pinMode(RIGHTBUTTONPIN, INPUT);
  
  //u8g.setColorIndex(1);
  //u8g.setRot180();

  splashScreen();
  initPoint(true);
  refreshScreen();
}

void loop(void) {
  // leemos los botones para resetear el juego
  if( digitalRead(LEFTBUTTONPIN) || digitalRead(RIGHTBUTTONPIN))
  {
    delay(1000);
    splashScreen();
    initPoint(true);
    refreshScreen();
    return;
  }

  // leemos lo spotenciómetros
  int lp = 1023 - analogRead(LEFTPADPIN);
  int rp = analogRead(RIGHTPADPIN);

  // obtenemos la posición de los pads
  leftPadY = map(lp, 0, 1023, 0, 64 - 2 - 10);
  rightPadY = map(rp, 0, 1023, 0, 64 - 2 - 10);

  // la pelota se mueve hacia la izquierda
  if(ballXMov<0)
  {
    // la pelota se perdió en el lado izquierdo
    if(ballX<=1)
    {
      delay(2000);
      initPoint(false);
      refreshScreen();
      return;    
    }
    // la pelota golpea el pad izquierdo
    else if(ballX==9 && ballY + 3 >=leftPadY && ballY<=leftPadY + 9)
    {
      ballXMov = -ballXMov;
    }
  }
  // la pelota se mueve hacia la derecha
  else
  {
    // la pelota se perdió en el lado derecho
    if(ballX>=123)
    {
      delay(2000);
      initPoint(true);
      refreshScreen();
      return;
    }
    // la pelota golpea el pad derecho
    else if(ballX==115 && ballY + 3 >=rightPadY && ballY<=rightPadY + 9)
    {
      ballXMov = -ballXMov;
    }
  }
  ballX += ballXMov;
  
  // ubicación de la pelota en Y
  if(ballY<=1 || ballY>=59) ballYMov = -ballYMov;
  ballY += ballYMov;

  refreshScreen();
}



