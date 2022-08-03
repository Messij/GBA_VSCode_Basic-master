#include "Intellisense.h"

#include "main.h"
#include "color.h"
#include "control.h"
#include "draw.h"
#include "heightmap.h"

// MAIN
int main() // Red dot
{
	WaitForVSync();
	// CopySpriteBuffer();
	// DrawHeightmapa();

	TheRedDot();
	
	return 0;
}

// Functions 
void WaitForSeconds(int n)
{
	REG_TM3CNT = TIMER_FREQUENCY_1024 | TIMER_ENABLE;
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
