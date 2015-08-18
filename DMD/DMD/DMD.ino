#include <SPI.h>        //SPI.h must be included as DMD is written by SPI (the IDE complains otherwise)
#include <DMD.h>        //
#include <TimerOne.h>   //
#include "SystemFont5x7.h"
#include "Arial_black_16.h"

//Fire up the DMD library as dmd
#define DISPLAYS_ACROSS 1
#define DISPLAYS_DOWN 1
DMD dmd(DISPLAYS_ACROSS, DISPLAYS_DOWN);

/*--------------------------------------------------------------------------------------
  Interrupt handler for Timer1 (TimerOne) driven DMD refresh scanning, this gets
  called at the period set in Timer1.initialize();
--------------------------------------------------------------------------------------*/
void ScanDMD()
{ 
  dmd.scanDisplayBySPI();
}

/*--------------------------------------------------------------------------------------
  setup
  Called by the Arduino architecture before the main loop begins
--------------------------------------------------------------------------------------*/
void setup(void)
{

   //initialize TimerOne's interrupt/CPU usage used to scan and refresh the display
   Timer1.initialize( 5000 );           //period in microseconds to call ScanDMD. Anything longer than 5000 (5ms) and you can see flicker.
   Timer1.attachInterrupt( ScanDMD );   //attach the Timer1 interrupt to ScanDMD which goes to dmd.scanDisplayBySPI()

   //clear/init the DMD pixels held in RAM
   dmd.clearScreen( true );   //true is normal (all pixels off), false is negative (all pixels on)

}

/*--------------------------------------------------------------------------------------
  loop
  Arduino architecture main loop
--------------------------------------------------------------------------------------*/
uint16_t ghost[16]=
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

uint16_t pacman[16]=
{
  0b0000000000000000,
  0b0000001111000000,
  0b0001111111111000,
  0b0011111111001100,
  0b0011111111001100,
  0b0011111111111000,
  0b0111111111110000,
  0b0111111111000000,
  0b0111111111000000,
  0b0111111111111100,
  0b0011111111111100,
  0b0011111111111100,
  0b0011111111111100,
  0b0001111111111000,
  0b0000001111000000,
  0b0000000000000000,
};

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

void loop(void)
{
  for(int x=-16; x<32; x++)
  {
    drawShape( x, 0, pacman);
    delay(50);
  }
  for(int x=-16; x<32; x++)
  {
    drawShape( x, 0, ghost);
    delay(50);
  }
}

