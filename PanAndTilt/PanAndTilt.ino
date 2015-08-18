#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// Ajustar a cada servo. Valores entre 150 y 650
#define PAN_MIN   200
#define PAN_MAX   600
#define TILT_MIN  130
#define TILT_MAX  400

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

void setup() {
  pwm.begin();
  pwm.setPWMFreq(60);
}

// P entre 0 y 100 para cubrir movimiento entre PAN_MIN y PAN_MAX
void pan( uint8_t p, uint16_t ms )
{

  if(p>100) p = 100;
  pwm.setPWM( 0, 0, PAN_MIN + ((PAN_MAX - PAN_MIN)*p)/100  );
  delay(ms);
}

// P entre 0 y 100 para cubrir movimiento entre TILT_MIN y TILT_MAX
void tilt( uint8_t p, uint16_t ms )
{

  if(p>100) p = 100;
  p = 100 - p;
  pwm.setPWM( 1, 0, TILT_MIN + ((TILT_MAX - TILT_MIN)*p)/100  );
  delay(ms);
}

void loop()
{
  pan(0, 800);
  tilt(0, 800);
  delay(1000);
  tilt(100, 800);
  pan(100, 800);
  delay(1000);
}
