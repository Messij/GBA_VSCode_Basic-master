#ifndef HEIGHTMAP_H
#define HEIGHTMAP_H

#include "main.h"

// d�finit une grille � deux dimensions
// les donn�es sont plac�es dans un tableau � une dimension

typedef struct grid2d
{
  int width;      // largeur
  int height;     // hauteur
  u8  *grid;      // donn�es

} GRID_2D, *PGRID_2D;

//////////////////////////////////////////////////////////////////////
// PROTOTYPES
//////////////////////////////////////////////////////////////////////

void DrawHeightmap();

// fonctions interaction utilisateur
void     getInput(PGRID_2D g, int &x, int &y);

// fonctions pour la g�n�ration
PGRID_2D createPlasma(int, double, double);
int      getDeviation(double); 

// fonctions d'affichage
void     bilinearInterpolation(PGRID_2D);
void     displayPlasma(PGRID_2D, int, int);
void     createPalette();

// fonctions pour la grille
void     freeGrid(PGRID_2D);
PGRID_2D initGrid(int, int);
u8       getGridValue(PGRID_2D, int, int);
void     setGridValue(PGRID_2D, int, int, int);

// fonctions graphiques
void     flipBuffers();
void     plotPixel(int, int, u16);
void     waitForVSync();

// fonctions diverses
void     waitKey(int);

#endif