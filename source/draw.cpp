#include "draw.h"
#include "control.h"
#include "color.h"

extern u32* KEYS;

// Screen adresses
u32* mode  = 	(u32*) 0x4000000;
u16* palette = 	(u16*) 0x5000000; // 256 valeurs sur 16 bits
u16* videoBuffer = (u16*) 0x6000000; // screen 240x160 pixel on 15 bits = 32765 colors

void DrawPixel(int x, int y, u16 color)
{
	if (x < 0 || x > SCREEN_WIDTH || y < 0 || y > SCREEN_HEIGHT) return;
  	videoBuffer[(y * SCREEN_WIDTH) + x] = color;
}

void DrawVerticalLine(int x, u16 color)
{
	for (int y = 0; y < SCREEN_HEIGHT; y++)
		{
			DrawPixel(x,y,color);
		}
}

void DrawHorizontalLine(int y, u16 color)
{
	for (int x = 0; x < SCREEN_WIDTH; x++)
		{
			DrawPixel(x,y,color);
		}
}

void DrawDegraded()
{
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

void TheRedDot()
{
	*mode = MODE_3 | BACKGROUND2_ENABLE;

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
