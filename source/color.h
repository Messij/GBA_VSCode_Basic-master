// couleur RGB
#define RGB(r, g, b)     ((r) | (g << 5) | (b << 10)) // chaque couleur codé sur 5 bits -> 31 nuances
#define RED 	  RGB(31,0,0)
#define GREEN 	RGB(0,31,0)
#define BLUE	  RGB(0,0,31)
#define WHITE 	RGB(31,31,31)
#define BLACK	  RGB(0,0,0)