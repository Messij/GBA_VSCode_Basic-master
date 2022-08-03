#include "main.h"
#include "Intellisense.h"

#include "color.h"
#include "control.h"

// MAIN
int main()
{
	WaitForVSync();
	// CopySpriteBuffer();

	TheRedDot();
	
	return 0;
}

// Functions 
void DrawPixel(int x, int y, u16 color)
{
	if (x < 0 || x > SCREEN_WIDTH || y < 0 || y > SCREEN_HEIGHT) return;
  	videoBuffer[(y * SCREEN_WIDTH) + x] = color;
}
void DrawVerticalLine(int x, u16 color)
{
	*mode = MODE_3 | BG2;
	for (int y = 0; y < SCREEN_HEIGHT; y++)
		{
			DrawPixel(x,y,color);
		}
}
void DrawHorizontalLine(int y, u16 color)
{
	*mode = MODE_3 | BG2;
	for (int x = 0; x < SCREEN_WIDTH; x++)
		{
			DrawPixel(x,y,color);
		}
}
void DrawDegraded()
{
	*mode = MODE_3 | BG2;
  
  	u8 r = 0,  // rouge
     	g = 0,  // vert
     	b = 0;  // bleu

	for (int x = 0; x < SCREEN_WIDTH; x++)
	{
		for (int y = 0; y < SCREEN_HEIGHT; y++)
		{
			DrawPixel(x, y, RGB(r, g, b));
		}

		// degrade
		if (r == 32)
		{
		if (g == 32)
		{
			if (b == 32)
			r = g = b = 0;
			else
			b++;
		} else
			g++;
		} else
		r++;
	}
}
void CreatePalette()
{
//   int i;
//   for (i = 0; i < 70; i++)
//     palette[i] = RGB(0, 0, i + 128);
//   for ( ; i < 200; i++)
//     palette[i] = RGB(0, i, 0);
//   for ( ; i < 256; i++)
//     palette[i] = RGB(i, i, i);
}
void DrawImage(const u16 imageData[], const u16 imagePalette[])
{
	// mode 4
  	*mode = MODE_4 | BG2;

	// copie de la palette de couleurs
	for (int i = 0; i < 256; i++)
    	palette[i] = imagePalette[i];
	
	// affichage de l'image
  	for (int y = 0; y < SCREEN_HEIGHT; y++)
    	for (int x = 0; x < SCREEN_HALF_W; x++)
      		videoBuffer[x + y * SCREEN_HALF_W] = imageData[x + y * SCREEN_HALF_W];
}
void FlipBuffers()
{
	if (*mode & BACKBUFFER)
	{
		*mode &= ~BACKBUFFER;
		videoBuffer = videoBackBuffer;
	} 
	else 
	{
		*mode |= BACKBUFFER;
		videoBuffer = videoFrontBuffer;
	}
}
void WaitKey(int key)
{
	while (1)
		if (!(*KEYS & key)) break;
}
void GetInput()
{
	if (!(*KEYS & DOWN))
	{

	}
	if (!(*KEYS & UP))
	{

	}
	if (!(*KEYS & RIGHT))
	{

	}
    if (!(*KEYS & LEFT))
	{

	}
}
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
void TheRedDot()
{
	*mode = MODE_3 | BG2;

	int x = SCREEN_HALF_W;
	int y = SCREEN_HALF_H;
	// Boucle principale
	while (*KEYS & START)
	{
		WaitForVSync();
		if (!(*KEYS & DOWN))
		{
			y = y < SCREEN_DOWN ? y + 1 : SCREEN_DOWN;
		}
		if (!(*KEYS & UP))
		{
			y = y > SCREEN_UP ? y - 1 : SCREEN_UP;

		}
		if (!(*KEYS & RIGHT))
		{
			x = x < SCREEN_RIGHT ? x + 1 : SCREEN_RIGHT;
		}
		if (!(*KEYS & LEFT))
		{
			x = x > SCREEN_LEFT ? x - 1 : SCREEN_LEFT;
		}
		DrawPixel(x,y, RED);
	}
}
void CopySpriteBuffer()
{
	// REG_DMA3SAD = (u32) SpriteBuffer;
	// REG_DMA3DAD = (u32) OAM;
	// REG_DMA3CNT = (u32) MAX_SPRITES * 4 | DMA_16NOW;
}





