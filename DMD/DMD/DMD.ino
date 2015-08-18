// DMD está escrito para utilizar SPI
#include <SPI.h>        
#include <DMD.h>        
#include <TimerOne.h>   
#include "SystemFont5x7.h"
#include "Arial_black_16.h"

// Elpanel es manipulado a través de dmd
#define DISPLAYS_ACROSS 1
#define DISPLAYS_DOWN 1
DMD dmd(DISPLAYS_ACROSS, DISPLAYS_DOWN);

// Handler de interrupciones para Timer1 (TimerOne) que maneja el refresco del panel
void ScanDMD()
{ 
  dmd.scanDisplayBySPI();
}

// Llamado antes de ingresar al loop() de main()
void setup(void)
{

   // Timer1 es utilizado para el scan y refresco del panel
   Timer1.initialize( 5000 );           // microsegundos entre llamadas a ScanDMD. Valores sobre 5000us puede producir flicker
   Timer1.attachInterrupt( ScanDMD );   // Asocia la interrupción con ScanDMD

   // Inicializa y limpia el panel
   dmd.clearScreen( true );   // true es normal (pixeles apagados), false indica todos los pixeles encendidos
}

// Los sprites como un arreglo de 16x16 pixeles mono
uint16_t pacmanOpen[16]=
{
  0b0000000000000000,
  0b0000001111000000,
  0b0001111111111000,
  0b0011111111001100,
  0b0011111111001100,
  0b0011111111111000,
  0b0111111111110000,
  0b0111111111000000,
  0b0111111110000000,
  0b0111111111111100,
  0b0011111111111100,
  0b0011111111111100,
  0b0011111111111100,
  0b0001111111111000,
  0b0000001111000000,
  0b0000000000000000,

};

uint16_t pacmanClose[16]=
{
  0b0000000000000000,
  0b0000001111000000,
  0b0001111111111000,
  0b0011111111001100,
  0b0011111111001100,
  0b0011111111111100,
  0b0111111111111110,
  0b0111111111111110,
  0b0111111111111110,
  0b0111111111111110,
  0b0011111111111100,
  0b0011111111111100,
  0b0011111111111100,
  0b0001111111111000,
  0b0000001111000000,
  0b0000000000000000,
};

uint16_t ghost1[16]=
{
  0b0000000000000000,
  0b0000001111000000,
  0b0000111111110000,
  0b0001111111111000,
  0b0011001111001100,
  0b0010000110000100,
  0b0010011110011100,
  0b0110011110011110,
  0b0111001111001110,
  0b0111111111111110,
  0b0111111111111110,
  0b0111111111111110,
  0b0111111111111110,
  0b0110111001110110,
  0b0100011001100010,
  0b0000000000000000
};

// Traza un sprite en el panel
void drawShape( int x, int y, uint16_t shape[])
{
  //dmd.clearScreen( true ); 
  for(int j=0; j<16; j++)
  {
    uint16_t row = shape[j];
    for(int i=0; i<16; i++)
    {
      bool pixel = row & (1<<(16-i));
      dmd.writePixel(x+i, y+j, GRAPHICS_NORMAL, pixel );
    }
  }
}

// el loop de arduino
void loop(void)
{
  // Despliega el pacman
  for(int x=-16; x<32; x+=2)
  {
    drawShape( x, 0, pacmanOpen);
    delay(150);
    drawShape( x+1, 0, pacmanClose);
    delay(100);
  }

  // Despliega el primer fantasma
  for(int x=-16; x<32; x++)
  {
    drawShape( x, 0, ghost1);
    delay(100);
  }
}

