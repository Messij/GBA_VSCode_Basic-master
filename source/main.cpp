#include "Intellisense.h"

#include "main.h"
#include "color.h"
#include "control.h"
#include "draw.h"
#include "heightmap.h"
#include "sprite.h"

// MAIN
int main() // Red dot
{
	// TheRedDot();
	// DrawHeightmap();
	DrawXWingSprite();
	
	return 0;
}

// Functions 
void WaitForSeconds(int n)
{
	REG_TM3CNT = TIMER_FREQUENCY_1024 | TIMER_ENABLE; // Initialisation son't work. Why ???
  	REG_TM3D = 0;
	while (n--)
	{
		while (REG_TM3D <= 16386);
		REG_TM3D = 0;
	}
}
void WaitForVSync()
{
  volatile u16* vreg = (volatile u16*) 0x04000004;
  while (  *vreg & (1 << 0));
  while (!(*vreg & (1 << 0)));
}

void WaitTime(int seconds, int milliSeconds)
{
  REG_TM2CNT = TIMER_FREQUENCY_256 | TIMER_ENABLE; // Initialisation son't work. Why ???
  REG_TM3CNT = TIMER_OVERFLOW | TIMER_ENABLE; // Initialisation son't work. Why ???
  REG_TM2D = 0;
  REG_TM3D = 0;

  while (REG_TM3D < seconds);

  REG_TM2D = 0;
  while (REG_TM2D / (65536 / 1000) < milliSeconds); // Dont work !

  REG_TM2CNT = 0;
  REG_TM3CNT = 0;
  REG_TM2D = 0;
  REG_TM3D = 0;
}