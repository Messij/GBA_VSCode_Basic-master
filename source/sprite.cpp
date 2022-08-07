#include "sprite.h"
#include "control.h"
#include "xwing.h"

// Global Variable
OAMEntry SpritesTable[MAX_SPRITES]; // Sprite Table
u16* OAM = (u16*) 0x7000000; // OAM Memory
u16* OAMData = (u16*) 0x6010000; // Sprite Datas
u16* spritesPalette = (u16*) 0x5000200; // Sprite Color Palette
pSprite xwing; // XWing Sprite
pRotData rotationData = (pRotData) SpritesTable;

// External Global Variable
extern const u16 xwingPalette[]; 
extern const u16 xwingData[]; 
extern u32* KEYS;
extern s32 COS[];
extern s32 SIN[];

void DrawSpritesTable()
{
	REG_DMA3SAD = (u32) SpritesTable; // creation d'un pointeur sur la memoire OAM
	REG_DMA3DAD = (u32) OAM;
	REG_DMA3CNT = (u32) MAX_SPRITES * 4 | DMA_16NOW; // La copie DMA demande une copie de 4 * 128 * 2 octets
}

void InitSprites()
{
  for (int i = 0; i < MAX_SPRITES; i++)
  {
    SpritesTable[i].attribute0 = 160;            // y = 160
    SpritesTable[i].attribute1 = 240;            // x = 240
    SpritesTable[i].attribute2 = 0;
  }
}

void LoadXWingSprite()
{
  u16* spritesPalette = (u16*) 0x5000200;
  for (int i = 0; i < 256; i++)
    spritesPalette[i] = xwingPalette[i];

	xwing = CreateSprite(0, 0);
  xwing->x = 100;
  xwing->y = 60;

	REG_DMA3SAD = (u32) xwingData;
  REG_DMA3DAD = (u32) (&OAMData[512 * 16]); // l'indice de la mémoire OAMData utilisé n'est pas zéro, mais 512*16. L'explication tient dans le fait que notre programme emploie le mode 4 avec une image en arrière plan. De ce fait, ces données se trouvent déjà exploitées. Par la suite, nous copions 256 * 8 segments de 16 bits dans la mémoire OAM des objets. En effet, nous utilisons un sprite de 64x64 pixels. Celui-ci se représente en mémoire sous la forme de blocs (les tiles) de 8x8 pixels. Un sprite correspond donc à 8x8 blocs de 8x8 pixels. Ainsi, une ligne de tiles occupe 8x8x8 = 512 octets. Or nous avons 8 lignes de tiles, donc 512 * 8 octets à copier.
  REG_DMA3CNT = 256 * 8 | DMA_16NOW; // Puisque la copie DMA s'effectue par segments de 16 bits, nous devons copier 256 * 8 segments de 16 bits.

  xwing->oam->attribute0 = COLOR_256 | ROTATION_FLAG | SQUARE | xwing->y;
  xwing->oam->attribute1 = SIZE_64 | ROTDATA(xwing->rotationID) | xwing->x;
  xwing->oam->attribute2 = xwing->spriteID + 512; // Sprite first tile adresse
}

void DrawXWingSprite()
{
	REG_DISPCNT = MODE_4 | BACKGROUND2_ENABLE | SPRITE_ENABLE | SPRITE_DATA_AS_STACK; // Set Mode
	InitSprites();

	LoadXWingSprite();

	while (!START)
	{
	    WaitForVSync();
	    DrawSpritesTable();
		  GetSpriteInput();
		  MoveSprite(xwing);
      TransformSprite(xwing);
	}

  delete xwing;
}

pSprite CreateSprite(u16 spriteID, u16 rotationID)
{
	pSprite sprite = new Sprite;
    sprite->spriteID = spriteID;
    sprite->rotationID = rotationID;
    sprite->x = 240;
    sprite->y = 160;
    sprite->angle = 0;
    sprite->xscale = (1 << 8);
    sprite->yscale = (1 << 8);
    sprite->oam = &SpritesTable[spriteID];
  
    return sprite;
}

void GetSpriteInput()
{
  if (KEY_PRESSED)
  {
    if ( DOWN )
      if (xwing->y < SCREEN_HEIGHT - xwing_H_H)
      xwing->y++;

    if ( RIGHT )
      if (xwing->x < SCREEN_WIDTH - xwing_H_W)
      xwing->x++;

    if ( UP )
      if (xwing->y > 0)
        xwing->y--;

    if ( LEFT )
      if (xwing->x > 0)
        xwing->x--;
    
    if ( A )
      if (xwing->xscale < 512)
        xwing->yscale = ++xwing->xscale;

    if ( B )
      if (xwing->xscale > 155)
        xwing->yscale = --xwing->xscale;

    if ( R )
      if (xwing->angle < 360)
        xwing->angle++;
      else
        xwing->angle = 0;

    if ( L )
    {
      if (xwing->angle > 0)
        xwing->angle--;
      else
        xwing->angle = 359;
    }
  }
}

void MoveSprite(pSprite sprite)
{
    sprite->oam->attribute0 &= 0xFF00;
    sprite->oam->attribute0 |= sprite->y;
    sprite->oam->attribute1 &= 0xFE00;
    sprite->oam->attribute1 |= sprite->x;
}

void RotateAndScaleSprite(pSprite sprite)
{
  rotationData[sprite->rotationID].corner1 = (u16) (((sprite->xscale) *  COS[sprite->angle]) >> 8);
  rotationData[sprite->rotationID].corner2 = (u16) (((sprite->yscale) *  SIN[sprite->angle]) >> 8);
  rotationData[sprite->rotationID].corner3 = (u16) (((sprite->xscale) * -SIN[sprite->angle]) >> 8);
  rotationData[sprite->rotationID].corner4 = (u16) (((sprite->yscale) *  COS[sprite->angle]) >> 8);
}

void TransformSprite(pSprite sprite)
{
  MoveSprite(sprite);
  RotateAndScaleSprite(sprite);
}









