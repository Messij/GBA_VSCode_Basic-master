#include "heightmap.h"
#include "fastrandom.h"
#include "color.h"
#include "control.h"

// Extern Globale variables
extern u32* KEYS; // in control.c
extern u32* mode; // in draw.c
extern u16* palette; // in draw.c
extern u16* videoBuffer; // in draw.c

void DrawHeightmap()
{
  // mode 4
  REG_DISPCNT = MODE_4 | BACKGROUND2_ENABLE;

  waitKey(KEY_START);

  // initialise la palette de couleurs
  createPalette();

  // cr�ation du plasma
  PGRID_2D g = createPlasma(8, 200.0, 0.5);
  bilinearInterpolation(g);

  // origine de l'affichage
  int x = 0,
      y = 0;

  // boucle principale
  while (*KEYS & KEY_START)
  {
    // affichage du heightmap
    displayPlasma(g, x, y);
    // v-sync
    waitForVSync();

    // gestion des touches
    getInput(g, x, y);
  }

  // liberation memoire
  freeGrid(g);
  delete g;
}

void getInput(PGRID_2D g, int &x, int &y)
{
  if (g->height > SCREEN_HEIGHT)
  {
    if (!(*KEYS & KEY_DOWN))
    {
      y++;
      if (y > g->height)
        y = 0;
    }
    
    if (!(*KEYS & KEY_UP))
    {
      y--;
      if (y < 0)
        y = g->height - 1;
    }
  }

  if (g->width > SCREEN_WIDTH)
  {
    if (!(*KEYS & KEY_RIGHT))
    {
      x++;
      if (x > g->width)
        x = 0;
    }

    if (!(*KEYS & KEY_LEFT))
    {
      x--;
      if (x < 0)
        x = g->width - 1;
    }
  }
} 

void bilinearInterpolation(PGRID_2D g)
{
  int x, y;
  u8 *newGrid = new u8[g->width * g->height];
  for (x = 0; x < g->width; x++)
    for (y = 0; y < g->height; y++)
      newGrid[y * g->width + x] = (g->grid[y * g->width + x]           +
                                   g->grid[y * g->width + x + 1]       +
                                   g->grid[(y + 1) * g->width + x]     +
                                   g->grid[(y + 1) * g->width + x + 1]  ) >> 2;

  delete[] g->grid;
  g->grid = newGrid;

} // fin bilinearInterpolation()

void displayPlasma(PGRID_2D g, int x, int y)
{
  // on copie par segments de 16 bits or notre tableau contient des
  // segments de 8 bits
  int copyWidth   = (g->width - x > SCREEN_WIDTH  ? SCREEN_WIDTH : g->width - x) >> 1;
  int copyWidth2  = (x > SCREEN_WIDTH ? SCREEN_WIDTH : x) >> 1;
  int copyHeight  = (g->height - y > SCREEN_HEIGHT ? y + SCREEN_HEIGHT : g->height);

  // affichage ligne par ligne en utilisant la copie DMA
  for (int l = y; l < copyHeight; l++)
  {
    if (copyWidth > 0)
    {
      REG_DMA3SAD = (u32) (g->grid + (l * g->width) + x);
      REG_DMA3DAD = (u32) (videoBuffer + ((l - y) * SCREEN_HALF_W));
      REG_DMA3CNT = copyWidth | DMA_16NOW;
    }

    if (x > 1)
    {
      REG_DMA3SAD = (u32) (g->grid + (l * g->width));
      REG_DMA3DAD = (u32) (videoBuffer + ((l - y) * SCREEN_HALF_W) + copyWidth);
      REG_DMA3CNT = copyWidth2 | DMA_16NOW;
    }
  }

  int delta = g->height - y;
  // affichage ligne par ligne en utilisant la copie DMA
  for (int l = 0; l < y; l++)
  {
    if (copyWidth > 0)
    {
      REG_DMA3SAD = (u32) (g->grid + (l * g->width) + x);
      REG_DMA3DAD = (u32) (videoBuffer + ((l + delta) * SCREEN_HALF_W));
      REG_DMA3CNT = copyWidth | DMA_16NOW;
    }

    if (x > 1)
    {
      REG_DMA3SAD = (u32) (g->grid + (l * g->width));
      REG_DMA3DAD = (u32) (videoBuffer + ((l + delta) * SCREEN_HALF_W) + copyWidth);
      REG_DMA3CNT = copyWidth2 | DMA_16NOW;
    }
  }

}

void createPalette()
{
  int i;
  for (i = 0; i < 70; i++)
    palette[i] = RGB(0, 0, i + 128);
  for ( ; i < 200; i++)
    palette[i] = RGB(0, i, 0);
  for ( ; i < 256; i++)
    palette[i] = RGB(i, i, i);
}

int getDeviation(double scale)
{
  int iscale = (int) scale;
  return iscale > 0 ? (genrand() % iscale) - (iscale / 2) : iscale;

}

//////////////////////////////////////////////////////////////////////
// Cr�e un terrain al�atoire (un "plasma") en employant l'algorithme
// de rendu dit "du diamant". La taille du terrain est 2^iterations.
//
// param : int iteration,    nombre d'it�rations
// param : double scale,     param�tre d'�l�vation
// param : double ratio,     facteur de modification de l'�l�vation
//
// renv. : PGRID_2D g,       une grille
//
//////////////////////////////////////////////////////////////////////
PGRID_2D createPlasma(int iteration, double scale, double ratio)
{
  // calcul des caract�ristiques du plasma
  int    mapSize  = 1 << iteration;   //(int) pow(2, iteration);
  double mapRatio = pow(2, -ratio);

  // cr�ation de la grille
  PGRID_2D g = initGrid(mapSize, mapSize);

  // initialisation al�atoire
  setGridValue(g, 0,           0,           (u8) genrand() % 255);
  setGridValue(g, 0,           mapSize - 1, (u8) genrand() % 255);
  setGridValue(g, mapSize - 1, 0,           (u8) genrand() % 255);
  setGridValue(g, mapSize - 1, mapSize - 1, (u8) genrand() % 255);

  // variables diverses
  int stride = mapSize,
      s      = mapSize,
      x      = 0,
      y      = 0;

  // boucle
  while (stride > 1)
  {
    // premi�re passe : les carr�s
    for (y = 0; y < mapSize; y += stride)
    {
      for (x = 0; x < mapSize; x += stride)
      {
        setGridValue(g, x + ((s + 1) >> 1), y + ((s + 1) >> 1),
                    ((getGridValue(g, x, y)     + getGridValue(g, x + s, y) +
                      getGridValue(g, x, y + s) + getGridValue(g, x + s, y + s)) >> 2) +
                    getDeviation(scale));
      }
    }

    s >>= 1;
    // seconde passe : les diamants
    for (y = 0; y < mapSize; y += stride)
    {
      for (x = 0; x < mapSize; x += stride)
      {
        // droite du diamant
        setGridValue(g, x + s, y, ((getGridValue(g, x + s, y - s) + getGridValue(g, x + s, y + s) +
                                    getGridValue(g, x, y)         + getGridValue(g, x + 2  * s, y)) >> 2) +
                                    getDeviation(scale));
        // bas du diamant
        setGridValue(g, x, y + s, ((getGridValue(g, x, y)         + getGridValue(g, x, y + 2* s) +
                                    getGridValue(g, x - s, y + s) + getGridValue(g, x + s, y + s)) >> 2) +
                                    getDeviation(scale));
      }
    }

    scale *= mapRatio;
    stride = s;
  }

  return g;

} // fin createPlasma()

void freeGrid(PGRID_2D g)
{
  delete[] g->grid;
}

PGRID_2D initGrid(int width, int height)
{
  PGRID_2D g = new GRID_2D; // un pointeur sur GRID_2D
  g->width  = width;
  g->height = height;
  g->grid   = new u8[width * height];

  return g;
}

u8 getGridValue(PGRID_2D g, int x, int y)
{
  if (x < 0)
    x += g->width;
  else if (x >= g->width)
    x -= g->width;
  if (y < 0)
    y += g->height;
  else if (y >= g->height)
    y -= g->height;

  return g->grid[y * g->width + x];

}

//////////////////////////////////////////////////////////////////////
// Modifie la valeur d'une donn�e de la grille. La donn�e sera
// repositionn�e dans la grille si ses coordonn�es d�passent la taille
// de la grille. La valeur doit �tre comprise en 0 et 256.
//
// param : PGRID_2D g,       une grille
// param : int x,            coordonn�e x
// param : int y,            coordonn�e y
// param : u8 value,         la valeur
//
//////////////////////////////////////////////////////////////////////
void setGridValue(PGRID_2D g, int x, int y, int value)
{
  if (x < 0)
    x += g->width;
  else if (x >= g->width)
    x -= g->width;
  if (y < 0)
    y += g->height;
  else if (y >= g->height)
    y -= g->height;
  if (value < 0)
    value = 0;
  else if (value > 255)
    value = 255;

  g->grid[y * g->width + x] = (u8) value;

}

void flipBuffers()
{
  if (REG_DISPCNT & BACKBUFFER)
  {
    REG_DISPCNT &= ~BACKBUFFER;
    videoBuffer = videoBackBuffer;
  } else {
    REG_DISPCNT |= BACKBUFFER;
    videoBuffer = videoFrontBuffer;
  }

}

void plotPixel(int x, int y, u16 color)
{
  if (x < 0 || x > SCREEN_WIDTH || y < 0 || y > SCREEN_HEIGHT)
    return;
  videoBuffer[(y * SCREEN_WIDTH) + x] = color;

}

void waitForVSync()
{
  volatile u16* vreg = (volatile u16*) 0x04000004;
  while (  *vreg & (1 << 0));
  while (!(*vreg & (1 << 0)));
}

void waitKey(int key)
{
  REG_TM2CNT = TIMER_FREQUENCY_256 | TIMER_ENABLE;
  REG_TM3CNT = TIMER_OVERFLOW | TIMER_ENABLE;

  while(1)
  {
    if (!(*KEYS & key))
    {
      break;
    }
  }

  sgenrand(REG_TM2D);

  REG_TM2CNT = 0;
  REG_TM3CNT = 0;

  REG_TM2D = 0;
  REG_TM3D = 0;

}
