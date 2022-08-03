#include "Intellisense.h"
//#include "arcane.h"

//////////////////////////////////////////////////////////////////////
// TYPES ET DEFINITIONS
//////////////////////////////////////////////////////////////////////

// types usuels sur GBA
typedef unsigned char    u8;
typedef unsigned short   u16;
typedef unsigned long    u32;

typedef signed char      s8;
typedef signed short     s16;
typedef signed long      s32;

// couleur RGB
#define RGB(r, g, b)     ((r) | (g << 5) | (b << 10)) // chaque couleur codé sur 5 bits -> 31 nuances
#define RED 	RGB(31,0,0)
#define GREEN 	RGB(0,31,0)
#define BLUE	RGB(0,0,31)
#define WHITE 	RGB(31,31,31)
#define BLACK	RGB(0,0,0)

// dimensions de l'ecran
#define SCREEN_WIDTH     240
#define SCREEN_HEIGHT    160
#define SCREEN_HALF_W    SCREEN_WIDTH / 2
#define SCREEN_HALF_H    SCREEN_HEIGHT / 2
#define SCREEN_RIGHT	SCREEN_WIDTH - 1
#define SCREEN_LEFT		0
#define SCREEN_UP		0
#define SCREEN_DOWN		SCREEN_HEIGHT - 1

// Double buffering
#define BACKBUFFER 			0x10
#define videoFrontBuffer 	(u16*) 0x6000000
#define videoBackBuffer 	(u16*) 0x600A000

// mode 3
#define MODE_0           0x0
#define MODE_1           0x1
#define MODE_2           0x2
#define MODE_3           0x3
#define MODE_4           0x4
#define MODE_5           0x5

// Register
#define REG_TM2D         *(volatile u16*) 0x4000108
#define REG_TM2CNT       *(volatile u16*) 0x400010A
#define REG_TM3D         *(volatile u16*) 0x400010C // timer value
#define REG_TM3CNT       *(volatile u16*) 0x400010E // timer controle register

// Timer
#define TIMER_FREQUENCY_SYSTEM 0x0 // System clock frequency, 16.78 Mhz
#define TIMER_FREQUENCY_64 0x1 // Every 64 clock tick , every 3.814 ms
#define TIMER_FREQUENCY_256 0x2 // Every 256 clock tick , every  15.256 ms
#define TIMER_FREQUENCY_1024 0x3 // Every 1024 clock tick , every  61.025 ms
#define TIMER_OVERFLOW 0x4 // flow timer in the next when max value reach
#define TIMER_ENABLE 0x80
#define TIMER_IRQ_ENABLE 0x40

// background
#define BG2              0x400; // background 2

// Screen adresses
u32* mode  = 	(u32*) 0x4000000;
u16* palette = 	(u16*) 0x5000000; // 256 valeurs sur 16 bits
u16* videoBuffer = (u16*) 0x6000000; // screen 240x160 pixel sur 15 bits 32765 couleurs

// touches
volatile u32* KEYS = (volatile u32*) 0x04000130;
#define A            1
#define B            2
#define SELECT       4
#define START        8
#define RIGHT        16
#define LEFT         32
#define UP           64
#define DOWN         128
#define R            256
#define L            512

// Sprite
typedef struct tSprite
{
  u16 attribute0;
  u16 attribute1;
  u16 attribute2;
  u16 attribute3;
} Sprite;

#define MAX_SPRITES 128
Sprite OAMSpriteBuffer[MAX_SPRITES]; // OAM Buffer

// Drawing Functions
void DrawPixel(int x, int y, u16);
void DrawVerticalLine(int x, u16 color);
void DrawHorizontalLine(int y, u16 color);
void DrawDegraded();
void CreatePalette();
void DrawImage(const u16 data[], const u16 palette[]);
void FlipBuffers();
void WaitKey(int key);
void GetInput();
void WaitForVSync();
void WaitForSeconds(int n);
void TheRedDot(); // Draw a cntrolable red dot
void CopySpriteBuffer(); // OAM Buffer

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
		//videoBuffer = videoBackBuffer;
	} 
	else 
	{
		*mode |= BACKBUFFER;
		//videoBuffer = videoFrontBuffer;
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





