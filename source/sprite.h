#ifndef SPRITE_H
#define SPRITE_H

#include <math.h>

#include "main.h"

#define MAX_SPRITES 128

// Attribute 0
#define ROTATION_FLAG         0x100
#define SIZE_DOUBLE           0x200
#define MODE_NORMAL           0x0
#define MODE_ALPHA            0x400
#define MODE_WINDOWED         0x800
#define MOSAIC                0x1000
#define COLOR_16              0x0000
#define COLOR_256             0x2000
#define SQUARE                0x0
#define WIDE                  0x4000
#define TALL                  0x8000

// Attribute 1
#define ROTDATA(n)            ((n) << 9)
#define HORIZONTAL_FLIP       0x1000
#define VERTICAL_FLIP         0x2000
#define SIZE_8                0x0
#define SIZE_16               0x4000
#define SIZE_32               0x8000
#define SIZE_64               0xC000

// Attribut 2
#define PRIORITY(n)           ((n) << 10)
#define PALETTE(n)            ((n) << 12)

// Attribute 3
#define PI                    3.14159
#define RADIAN(n)             (((float) n)/ (float) 180 * PI)

// OAM structure (Object Attribute Manager) Sprite Table
typedef struct t_OAMEntry
{
  u16 attribute0; // y coordinate
  u16 attribute1; // x coordinate
  u16 attribute2; //  
  u16 attribute3; //

} OAMEntry, *pOAMEntry;

// Sprite structure
typedef struct t_Sprite
{
  u16 spriteID;           // identificateur du sprite inndice du sprite dans la SpriteTable
  u16 rotationID;         // indicateur de rotation/mise a l'echelle (entre 0 et 31)
  pOAMEntry oam;          // donnees OAM

  int x,                  // coordonnee x
      y,                  // coordonnee y
      angle;              // angle de rotation
  s32 xscale,             // mise a l'echelle x
      yscale;             // mise a l'echelle x

} Sprite, *pSprite;

// Rotation Structure
typedef struct t_RotData
{
  // OAMBuffer[0]
  u16 filler1[3];
  u16 corner1;
  // OAMBuffer[1]
  u16 filler2[3];
  u16 corner2;
  // OAMBuffer[2]
  u16 filler3[3];
  u16 corner3;
  // OAMBuffer[3]
  u16 filler4[3];
  u16 corner4;

} RotData, *pRotData;

void DrawSpritesTable(); // OAM Buffer
void InitSprites(); // Initialise Sprite out of screen
void LoadXWingSprite();
pSprite CreateSprite(u16 spriteID, u16 rotationID);
void DrawSprite();
void GetSpriteInput();
void MoveSprite(pSprite sprite);

#endif