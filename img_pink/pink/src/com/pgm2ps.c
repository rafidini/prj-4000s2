/*
Copyright ESIEE (2009) 

m.couprie@esiee.fr

This software is an image processing library whose purpose is to be
used primarily for research and teaching.

This software is governed by the CeCILL  license under French law and
abiding by the rules of distribution of free software. You can  use, 
modify and/ or redistribute the software under the terms of the CeCILL
license as circulated by CEA, CNRS and INRIA at the following URL
"http://www.cecill.info". 

As a counterpart to the access to the source code and  rights to copy,
modify and redistribute granted by the license, users are provided only
with a limited warranty  and the software's author,  the holder of the
economic rights,  and the successive licensors  have only  limited
liability. 

In this respect, the user's attention is drawn to the risks associated
with loading,  using,  modifying and/or developing or reproducing the
software by the user in light of its specific status of free software,
that may mean  that it is complicated to manipulate,  and  that  also
therefore means  that it is reserved for developers  and  experienced
professionals having in-depth computer knowledge. Users are therefore
encouraged to load and test the software's suitability as regards their
requirements in conditions enabling the security of their systems and/or 
data to be ensured and,  more generally, to use and operate it in the 
same conditions as regards security. 

The fact that you are presently reading this means that you have had
knowledge of the CeCILL license and that you accept its terms.
*/
/*! \file pgm2ps.c

\brief converts from pgm to ps for illustrating small image configurations

<B>Usage:</B> pgm2ps in.pgm mode {label.pgm|null} {marker.pgm|null} coord maxval out.ps

<B>Description:</B>
Produces a Postscript file from a binary or grayscale image.
If <B>mode</B> = 
\li     b: binary image
\li     c: grayscale image (levels not printed)
\li     n: grayscale image (levels printed as numbers on a colored background)
\li     m: grayscale image (levels printed as numbers on a colored background)
\li     d: grayscale image (idem n - levels are square-rooted)
\li     i: grayscale image (levels printed as numbers on an inverted colored background)
\li     p: grayscale image (levels printed as numbers)
\li     a: grayscale image (levels printed as letters: a=1, b=2...)
\li     g: grayscale image (levels showed as colored items)
\li     v: vector image
\li     B: binary khalimsky grid
\li     N: grayscale khalimsky grid (levels printed as numbers)
\li     G: grayscale khalimsky grid (levels showed as colored items)
\li     C: binary khalimsky grid (dual of B)
\li     M: grayscale khalimsky grid (dual of N)
\li     H: grayscale khalimsky grid (dual of G)
\li     T: binary triangular grid (simplicial complex)

If an image \b label.pgm is present and the mode is n, the level number will be replaced by a letter for each pixel not null in the label image.

If an image \b label.pgm is present and the mode is m, the printed number will
taken in the image \b label.pgm while the color will be taken from \b in.pgm .

If an image \b marker.pgm is present, a circle will be drawn on each point not null in the marker image.

The parameter \b coord is a int32_t (0 or 1) which commands the inclusion of axis coordinates in the
figure. 

The parameter \b maxval is an integer which indicates the maximum grayscale value of the figure
(not necessarily the maximum value of the input image).

<B>Types supported:</B> byte 2d, int32_t 2d

<B>Category:</B> convert
\ingroup  convert

\author Michel Couprie
*/
/* 
  Michel Couprie - avril 2000
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcutil.h>
#include <mckhalimsky2d.h>

#define BOLDCIRCLES
#define KHALIMSKY1

#ifdef KHALIMSKY1
/* parametres pour les elements de Khalimsky: */
#define R1 14  /* rayon des cercles, demi-largeur des rectangles */
#define R2 28  /* demi cote des carres */
#define GS 54  /* pas de la grille */
#endif

#ifdef KHALIMSKY1bis
/* parametres pour les elements de Khalimsky: */
#define R1 18  /* rayon des cercles, demi-largeur des rectangles */
#define R2 24  /* demi cote des carres */
#define GS 54  /* pas de la grille */
#endif

#ifdef KHALIMSKY2
/* parametres pour les elements de Khalimsky: */
#define R0 6  /* rayon des cercles */
#define R1 3  /* demi-largeur des rectangles */
#define R2 24  /* demi cote des carres */
#define GS 24  /* pas de la grille */
#endif

#ifdef KHALIMSKY2bis
/* parametres pour les elements de Khalimsky: */
#define R0 3  /* rayon des cercles */
#define R1 1  /* demi-largeur des rectangles */
#define R2 24  /* demi cote des carres */
#define GS 24  /* pas de la grille */
#endif

#define TRG_R    6 /* rayon des cercles */
#define TRG_W   60 /* cote des triangles */
#define TRG_L   12 /* largeur séparation */
#define TRG_GS 108 /* pas de la grille */
#define TRG_M  108 /* marge */
#define TRG_T    5 /* epaisseur du trait */

//#define FONTHEIGHT 24
#define FONTHEIGHT 18
//#define FONTSCALE 32
#define FONTSCALE 16
#define GLOBALSCALE 0.25
#define P2P_EPSILON 2

#define GRAYMARK 0.7 /* niveau de gris pour le marquage en Khalimsky */

/*========================================*/
void move (FILE *fd, double x, double y) 
/*========================================*/
{
  int32_t xx, yy;
  xx = (int32_t)x;
  yy = (int32_t)y;
  fprintf(fd,"%d %d moveto ", xx, yy);
} /* move() */

/*========================================*/
void draw (FILE *fd, double x, double y)
/*========================================*/
{
  int32_t xx, yy;
  xx = (int32_t)x;
  yy = (int32_t)y;
  fprintf(fd,"%d %d lineto ", xx, yy);
} /* draw() */

/*========================================*/
void drawline (FILE *fd, double xd, double yd, double xf, double yf, int32_t width)
/*========================================*/
{
fprintf(fd,"newpath %d slw ", width);
  move(fd, xd, yd);
  draw(fd, xf, yf);
  fprintf(fd,"stroke\n");
} /* drawline() */

/*========================================*/
void drawsquare (FILE *fd, double x, double y, double r, int32_t width, int32_t fill)
/*========================================*/
/* dessine un carré de coin inférieur gauche (x,y), de cote r */ 
{
  fprintf(fd,"newpath %d slw ", width);
  move(fd, x, y);
  draw(fd, x+r, y);
  draw(fd, x+r, y+r);
  draw(fd, x, y+r);
  draw(fd, x, y);
  if (fill) fprintf(fd,"fill stroke\n"); else fprintf(fd,"stroke\n");
} /* drawsquare() */

/*========================================*/
void drawcenteredsquare (FILE *fd, double x, double y, double r, int32_t fill)
/*========================================*/
/* dessine un carre centre en (x,y), de demi-cote r */ 
{
  double R = r + r, X = x-r, Y = y-r;
  fprintf(fd,"newpath 1 slw ");
  move(fd, X, Y);
  draw(fd, X+R, Y);
  draw(fd, X+R, Y+R);
  draw(fd, X, Y+R);
  draw(fd, X, Y);
  if (fill) fprintf(fd,"fill stroke\n"); else fprintf(fd,"stroke\n");
} /* drawcenteredsquare() */

/*========================================*/
void drawtriangle (FILE *fd, double x1, double y1, double x2, double y2, double x3, double y3, int32_t fill)
/*========================================*/
/* dessine un triangle */ 
{
  fprintf(fd,"newpath 1 slw ");
  move(fd, x1, y1);
  draw(fd, x2, y2);
  draw(fd, x3, y3);
  draw(fd, x1, y1);
  if (fill) fprintf(fd,"fill stroke\n"); else fprintf(fd,"stroke\n");
} /* drawtriangle() */
 
/*========================================*/
void fillsquare (FILE *fd, double x, double y, double r, double dcol)
/*========================================*/
/* dessine un carre de coin sup. gauche (x,y), de cote r, rempli par la couleur dcol */ 
{
  fprintf(fd,"newpath 1 slw ");
  move(fd, x, y);
  draw(fd, x+r, y);
  draw(fd, x+r, y+r);
  draw(fd, x, y+r);
  draw(fd, x, y);
  fprintf(fd,"%g setgray\n", dcol);
  fprintf(fd,"fill stroke\n");
  fprintf(fd,"newpath 1 slw ");
  move(fd, x, y);
  draw(fd, x+r, y);
  draw(fd, x+r, y+r);
  draw(fd, x, y+r);
  draw(fd, x, y);
  if (dcol > 0.5)
    fprintf(fd,"0 setgray\nstroke\n");
  else
    fprintf(fd,"1 setgray\nstroke\n");
} /* fillsquare() */
 
/*========================================*/
void colsquare (FILE *fd, double x, double y, double r, double col, int32_t width)
/*========================================*/
/* dessine un carre centre en (x,y), de demi-cote r */ 
{
  fprintf(fd,"newpath 1 slw ");
  move(fd, x-r, y-r);
  draw(fd, x+r, y-r);
  draw(fd, x+r, y+r);
  draw(fd, x-r, y+r);
  draw(fd, x-r, y-r);
  fprintf(fd,"%g setgray\n", col);
  fprintf(fd,"fill stroke\n");
  fprintf(fd,"newpath %d slw ", width);
  move(fd, x-r, y-r);
  draw(fd, x+r, y-r);
  draw(fd, x+r, y+r);
  draw(fd, x-r, y+r);
  draw(fd, x-r, y-r);
  fprintf(fd,"col0 stroke\n");
} /* colsquare() */

/*========================================*/
void drawvrect (FILE *fd, double x, double y, double r, double R, int32_t fill)
/*========================================*/
/* dessine un rectangle vertical centre en (x,y), de demi-largeur r, de demi-longueur R */ 
{
  fprintf(fd,"newpath 1 slw ");
  move(fd, x-r, y-R);
  draw(fd, x+r, y-R);
  draw(fd, x+r, y+R);
  draw(fd, x-r, y+R);
  draw(fd, x-r, y-R);
  if (fill) fprintf(fd,"fill stroke\n"); else fprintf(fd,"stroke\n");
} /* drawvrect() */

/*========================================*/
void colvrect (FILE *fd, double x, double y, double r, double R, double col, int32_t width)
/*========================================*/
/* dessine un rectangle vertical centre en (x,y), de demi-largeur r, de demi-longueur R */ 
{
  fprintf(fd,"newpath 1 slw ");
  move(fd, x-r, y-R);
  draw(fd, x+r, y-R);
  draw(fd, x+r, y+R);
  draw(fd, x-r, y+R);
  draw(fd, x-r, y-R);
  fprintf(fd,"%g setgray\n", col);
  fprintf(fd,"fill stroke\n");
  fprintf(fd,"newpath %d slw ", width);
  move(fd, x-r, y-R);
  draw(fd, x+r, y-R);
  draw(fd, x+r, y+R);
  draw(fd, x-r, y+R);
  draw(fd, x-r, y-R);
  fprintf(fd,"col0 stroke\n");
} /* colvrect() */

/*========================================*/
void drawhrect (FILE *fd, double x, double y, double r, double R, int32_t fill)
/*========================================*/
/* dessine un rectangle horizontal centre en (x,y), de demi-largeur r, de demi-longueur R */ 
{
  fprintf(fd,"newpath 1 slw ");
  move(fd, x-R, y-r);
  draw(fd, x+R, y-r);
  draw(fd, x+R, y+r);
  draw(fd, x-R, y+r);
  draw(fd, x-R, y-r);
  if (fill) fprintf(fd,"fill stroke\n"); else fprintf(fd,"stroke\n");
} /* drawhrect() */

/*========================================*/
void colhrect (FILE *fd, double x, double y, double r, double R, double col, int32_t width)
/*========================================*/
/* dessine un rectangle horizontal centre en (x,y), de demi-largeur r, de demi-longueur R */ 
{
  fprintf(fd,"newpath 1 slw ");
  move(fd, x-R, y-r);
  draw(fd, x+R, y-r);
  draw(fd, x+R, y+r);
  draw(fd, x-R, y+r);
  draw(fd, x-R, y-r);
  fprintf(fd,"%g setgray\n", col);
  fprintf(fd,"fill stroke\n");
  fprintf(fd,"newpath %d slw ", width);
  move(fd, x-R, y-r);
  draw(fd, x+R, y-r);
  draw(fd, x+R, y+r);
  draw(fd, x-R, y+r);
  draw(fd, x-R, y-r);
  fprintf(fd,"col0 stroke\n");
} /* colhrect() */

/*========================================*/
void drawcircle (FILE *fd, double x, double y, double r, int32_t fill, int32_t width, double col)
/*========================================*/
{
  int32_t xx, yy, rr;
  xx = (int32_t)x;
  yy = (int32_t)y;
  rr = (int32_t)r;
  fprintf(fd,"newpath %d slw ", width);
  fprintf(fd,"%d %d %d 0 360 arc %g setgray", xx, yy, rr, col); 
  if (fill) fprintf(fd," fill stroke\n"); else fprintf(fd," stroke\n");
} /* drawcircle() */

/*========================================*/
void colcircle (FILE *fd, double x, double y, double r, double col, int32_t width)
/*========================================*/
{
  int32_t xx, yy, rr;
  xx = (int32_t)x;
  yy = (int32_t)y;
  rr = (int32_t)r;
  fprintf(fd,"newpath 1 slw ");
  fprintf(fd,"%d %d %d 0 360 arc ", xx, yy, rr); 
  fprintf(fd,"%g setgray\n", col);
  fprintf(fd,"fill stroke\n");
  fprintf(fd,"newpath %d slw ", width);
  fprintf(fd,"%d %d %d 0 360 arc ", xx, yy, rr); 
  fprintf(fd,"col0 stroke\n");
} /* colcircle() */

/*========================================*/
void drawtext (FILE *fd, double x, double y, char *str, double scale)
/*========================================*/
{
  fprintf(fd,"newpath ");
  fprintf(fd,"/Times-Roman ff %g scf sf\n", scale);
  move(fd, x, y);
  fprintf(fd,"gs (%s) dup stw pop 2 div neg -%d rm col0 sh gr \n", str, FONTHEIGHT/2);
  fprintf(fd,"stroke\n");
} /* drawtext() */

/*========================================*/
void coldrawtext (FILE *fd, double x, double y, char *str, double scale, double dcol)
/*========================================*/
{
  fprintf(fd,"newpath ");
  fprintf(fd,"/Times-Roman ff %g scf sf\n", scale);
  move(fd, x, y);
  fprintf(fd,"gs (%s) dup stw pop 2 div neg -%d rm %g setgray sh gr \n", str, FONTHEIGHT/2, dcol);
  fprintf(fd,"stroke\n");
} /* coldrawtext() */

/*========================================*/
void arrow (FILE *fd, double x1, double y1, double x2, double y2)
/*========================================*/
{
  double x3, y3, a, b, d, t;
  a = x2 - x1;
  b = y2 - y1;             /* (a,b) est un vecteur directeur de l'arc */
  d = sqrt(a * a + b * b); /* longueur de l'arc */
  if (d < P2P_EPSILON) 
  {
    drawcircle(fd, x1, y1, P2P_EPSILON, 1, 1, 0);
    return;
  }
  drawline(fd, x1, y1, x2, y2, 1);
  t = d / 16;
  a /= d; b /= d;          /* norme le vecteur */
  x1 = x2 - 4 * t * a;
  y1 = y2 - 4 * t * b;     /* base de la fleche */
  x3 = x1 + t * -b;        /* (-b,a) est normal a (a,b) */
  y3 = y1 + t * a;
  x1 = x1 - t * -b;
  y1 = y1 - t * a;
  drawtriangle(fd, x1, y1, x2, y2, x3, y3, 1);
} /* arrow() */

/* ========================================== */
void psheader(FILE *fd, int32_t w, int32_t h, double scale)
/* ========================================== */
{
  fprintf(fd,"%%!PS-Adobe-2.0 EPSF-2.0\n");
  fprintf(fd,"%%%%Creator: pgm2ps by MC - 2000\n");
  fprintf(fd,"%%%%BoundingBox: 0 0 %d %d\n", (int32_t)(w*GLOBALSCALE), (int32_t)(h*GLOBALSCALE));
  fprintf(fd,"%g %g scale\n", scale, scale);
  fprintf(fd,"/cp {closepath} bind def\n");
  fprintf(fd,"/ef {eofill} bind def\n");
  fprintf(fd,"/gr {grestore} bind def\n");
  fprintf(fd,"/gs {gsave} bind def\n");
  fprintf(fd,"/sa {save} bind def\n");
  fprintf(fd,"/rs {restore} bind def\n");
  fprintf(fd,"/l {lineto} bind def\n");
  fprintf(fd,"/m {moveto} bind def\n");
  fprintf(fd,"/rm {rmoveto} bind def\n");
  fprintf(fd,"/n {newpath} bind def\n");
  fprintf(fd,"/s {stroke} bind def\n");
  fprintf(fd,"/sh {show} bind def\n");
  fprintf(fd,"/slc {setlinecap} bind def\n");
  fprintf(fd,"/slj {setlinejoin} bind def\n");
  fprintf(fd,"/slw {setlinewidth} bind def\n");
  fprintf(fd,"/srgb {setrgbcolor} bind def\n");
  fprintf(fd,"/rot {rotate} bind def\n");
  fprintf(fd,"/sc {scale} bind def\n");
  fprintf(fd,"/sd {setdash} bind def\n");
  fprintf(fd,"/ff {findfont} bind def\n");
  fprintf(fd,"/sf {setfont} bind def\n");
  fprintf(fd,"/scf {scalefont} bind def\n");
  fprintf(fd,"/stw {stringwidth} bind def\n");
  fprintf(fd,"/tr {translate} bind def\n");
  fprintf(fd,"/col0 {0 setgray} bind def\n");
  fprintf(fd,"/col1 {0.333333 setgray} bind def\n");
  fprintf(fd,"/col2 {0.666667 setgray} bind def\n");
  fprintf(fd,"/col3 {1 setgray} bind def\n");
}

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image, *marker, *label;
  FILE *fd = NULL;
  int32_t rs, cs, N, x, y, maxval;
  double X, Y;
  uint8_t *F, *G, *H;
  char type;
  char buf[256];
  int32_t printcoords;

  if (argc != 8) 
  {
    fprintf(stderr, "usage: %s in.pgm {b|c|n|d|i|a|p|g|v|B|N|G|C|M|H|T} {label.pgm|null} {marker.pgm|null} coord maxval out.ps\n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  rs = rowsize(image);
  cs = colsize(image);
  N = rs * cs;
  F = UCHARDATA(image);

  if (strcmp(argv[4], "null"))
  {
    marker = readimage(argv[4]);
    if (marker == NULL)
    {
      fprintf(stderr, "%s: readimage failed\n", argv[4]);
      exit(1);
    }
    if ((rs != rowsize(marker)) || (cs != colsize(marker)))
      {
	fprintf(stderr, "Images not of the same size\n");
	exit(1);
      }
    G = UCHARDATA(marker);
  } else {
    G = NULL;
  }

  if (strcmp(argv[3], "null"))
  {
    label = readimage(argv[3]);
    if (label == NULL)
    {
      fprintf(stderr, "%s: readimage failed\n", argv[3]);
      exit(1);
    }
    if ((rs != rowsize(label)) || (cs != colsize(label)))
      {
	fprintf(stderr, "Images not of the same size\n");
	exit(1);
      }
    H = UCHARDATA(label);
  } else {
    H = NULL;
  }

  printcoords = atoi(argv[5]);
  maxval = atoi(argv[6]);

  type = argv[2][0];
  if ((type != 'b') && (type != 'c') &&  (type != 'n') && (type != 'm') && (type != 'i') && (type != 'p') && (type != 'a') && 
      (type != 'g') && (type != 'B') && (type != 'N') && (type != 'G') && (type != 'd') && (type != 'v') && 
      (type != 'C') && (type != 'M') && (type != 'H') && (type != 'T'))
  {
    fprintf(stderr, "usage: %s in.pgm {b|c|n|m|d|i|a|p|g|v|B|N|G|C|M|H|T} {label.pgm|null} {marker.pgm|null} coord maxval out.ps\n", argv[0]);
    exit(1);
  }

  if ((type != 'v') && (type != 'n') && (type != 'N') && (type != 'c') && (type != 'm') && (type != 'd') && (datatype(image) != VFF_TYP_1_BYTE))
  {
    fprintf(stderr, "%s: bad data type for mode %c (must be byte)\n", argv[0], type);
    exit(1);
  }

  if ((type == 'v') && (datatype(image) != VFF_TYP_FLOAT))
  {
    fprintf(stderr, "%s: bad data type for mode %c (must be float)\n", argv[0], type);
    exit(1);
  }

  if ((type == 'v') && (depth(image) != 2))
  {
    fprintf(stderr, "%s: bad depth for mode %c (must be 2)\n", argv[0], type);
    exit(1);
  }

  if ((type == 'm') && (H == NULL))
  {
    fprintf(stderr, "%s: a label image is required with mode m\n", argv[0]);
    exit(1);
  }

  fd = fopen(argv[argc - 1],"w");
  if (!fd)
  {
    fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[argc - 1]);
    exit(1);
  }

  if (type == 'b') /* binary image */
  {
    if (printcoords)
      psheader(fd, (rs+2) * GS, (cs+2) * GS, GLOBALSCALE);
    else
      psheader(fd, (rs+1) * GS, (cs+1) * GS, GLOBALSCALE);

    for (y = 0; y < cs; y++)
    {
      for (x = 0; x < rs; x++)
      {
        X = (x + 1) * (double)GS;
        Y = (cs - y) * (double)GS;
        drawcircle(fd, X, Y, R1, F[y * rs + x], 1, 0);
      }
      if (printcoords)
      {
	X = (x+1) * (double)GS;
	Y = (cs-y) * (double)GS;
	sprintf(buf, "%d", y+1);
	drawtext(fd, X, Y, buf, FONTSCALE);
      } // if (printcoords)
    } // for y
    if (printcoords)
    {
      for (x = 0; x < rs; x++)
      {
	X = (x+1) * (double)GS;
	Y = (cs+1) * (double)GS;
	sprintf(buf, "%d", x+1);
	drawtext(fd, X, Y, buf, FONTSCALE);
      } // for (x = 0; x < rs; x++)
    } // if (printcoords)
  }
  else if ((type == 'c') || (type == 'n') || (type == 'm') || (type == 'd') || (type == 'i')) 
  { /* grayscale image (levels printed as numbers on color) */
    if (datatype(image) == VFF_TYP_1_BYTE)
    {
      if (printcoords)
	psheader(fd, (rs+1) * GS, (cs+1) * GS, GLOBALSCALE);
      else
	psheader(fd, rs * GS, cs * GS, GLOBALSCALE);
      if (type == 'd') maxval = (int32_t)sqrt(maxval);
      for (y = 0; y < cs; y++)
      {
	for (x = 0; x < rs; x++)
	{
	  X = x * (double)GS;
	  Y = (cs-1-y) * (double)GS;
	  if ((type == 'n') || (type == 'c') || (type == 'm'))
	  {
	    fillsquare(fd, X, Y, GS, (double)(F[y * rs + x])/(double)maxval);
	    if ((G != NULL) && (G[y * rs +x]>0)) 
	      drawcircle(fd, X+(GS/2), Y+(GS/2), GS*0.25, 1, 4, 0);
	  }
	  else if (type == 'd')
	    fillsquare(fd, X, Y, GS, sqrt(F[y * rs + x])/(double)maxval);
	  else
	    fillsquare(fd, X, Y, GS, (double)1.0 - (double)(F[y * rs + x])/(double)maxval);
	  if ((type == 'n') && (H != NULL) && (H[y * rs +x] > 0)) 
	  {
	    sprintf(buf, "%c", (char)(H[y * rs + x]-1+'A'));
	  } else 	if (type == 'm') 
	  {
		if(datatype(label)==VFF_TYP_FLOAT)
			sprintf(buf, "%1.1f", FLOATDATA(label)[y * rs + x]);
		else
	    		sprintf(buf, "%d", H[y * rs + x]);
	  } else 
	  {
	    sprintf(buf, "%d", F[y * rs + x]);
	    //sprintf(buf, "%c.%c", F[y * rs + x]/10+'0', F[y * rs + x]%10+'0');
	  }
	  if (((type == 'd') && (sqrt(F[y * rs + x]) > maxval/2)) || 
	      ((type != 'd') && (F[y * rs + x] > maxval/2)))
	  {
	    if ((type == 'n') || (type == 'm') || (type == 'd'))
	    {
	      coldrawtext(fd, X+(GS/2), Y+(GS/2), buf, FONTSCALE, 0.0);
	      if ((G != NULL) && (G[y * rs +x]>0)) 
	      {
#ifdef BOLDCIRCLES
		drawcircle(fd, X+(GS/2), Y+(GS/2), GS*0.45, 0, 4, 0);
#else
		drawcircle(fd, X+(GS/2), Y+(GS/2), GS*0.45, 0, 1, 0);
#endif
	      }
	    }
	    else if (type != 'c')
	    {
	      coldrawtext(fd, X+(GS/2), Y+(GS/2), buf, FONTSCALE, 1.0);
	      if ((G != NULL) && (G[y * rs +x]>0)) 
	      {
#ifdef BOLDCIRCLES
		drawcircle(fd, X+(GS/2), Y+(GS/2), GS*0.45, 0, 4, 1);
#else
		drawcircle(fd, X+(GS/2), Y+(GS/2), GS*0.45, 0, 1, 1);
#endif
	      }
	    }
	  }
	  else
	  {
	    if ((type == 'n') || (type == 'm') || (type == 'd'))
	    {
	      coldrawtext(fd, X+(GS/2), Y+(GS/2), buf, FONTSCALE, 1.0);
	      if ((G != NULL) && (G[y * rs +x]>0)) 
	      {
#ifdef BOLDCIRCLES
	        drawcircle(fd, X+(GS/2), Y+(GS/2), GS*0.45, 0, 4, 1);
#else
	        drawcircle(fd, X+(GS/2), Y+(GS/2), GS*0.45, 0, 1, 1);
#endif
	      }
	    }
	    else if (type != 'c')
	    {
	      coldrawtext(fd, X+(GS/2), Y+(GS/2), buf, FONTSCALE, 0.0);
	      if ((G != NULL) && (G[y * rs +x]>0)) 
	      {
#ifdef BOLDCIRCLES
		drawcircle(fd, X+(GS/2), Y+(GS/2), GS*0.45, 0, 4, 0);
#else
		drawcircle(fd, X+(GS/2), Y+(GS/2), GS*0.45, 0, 1, 0);
#endif
	      }
	    }
	  }
	} // for (x = 0; x < rs; x++)
	if (printcoords)
	{
	  X = x * (double)GS;
	  Y = (cs-1-y) * (double)GS;
	  sprintf(buf, "%d", y+1);
	  drawtext(fd, X+(GS/2), Y+(GS/2), buf, FONTSCALE);
	} // if (printcoords)
      } // for (y = 0; y < cs; y++)
      if (printcoords)
      {
	for (x = 0; x < rs; x++)
	{
	  X = x * (double)GS;
	  Y = cs * (double)GS;
	  sprintf(buf, "%d", x+1);
	  drawtext(fd, X+(GS/2), Y+(GS/2), buf, FONTSCALE);
	} // for (x = 0; x < rs; x++)
      } // if (printcoords)
    } // if (datatype(image) == VFF_TYP_1_BYTE)
    else if (datatype(image) == VFF_TYP_4_BYTE)
    {
      int32_t *F = SLONGDATA(image);
      if (printcoords)
	psheader(fd, (rs+1) * GS, (cs+1) * GS, GLOBALSCALE);
      else
	psheader(fd, rs * GS, cs * GS, GLOBALSCALE);
      if (type == 'd') maxval = (int32_t)sqrt(maxval);
      for (y = 0; y < cs; y++)
      {
	for (x = 0; x < rs; x++)
	{
	  X = x * (double)GS;
	  Y = (cs-1-y) * (double)GS;
	  if ((type == 'n') || (type == 'm'))
	    fillsquare(fd, X, Y, GS, (double)(F[y * rs + x])/(double)maxval);
	  else if (type == 'd')
	    fillsquare(fd, X, Y, GS, sqrt(F[y * rs + x])/(double)maxval);
	  else
	    fillsquare(fd, X, Y, GS, (double)1.0 - (double)(F[y * rs + x])/(double)maxval);
	  if ((type == 'n') && (H != NULL) && (H[y * rs +x] > 0)) 
	  {
	    sprintf(buf, "%c", (char)(H[y * rs + x]-1+'A'));
	  } else 	if (type == 'm') 
	  {
	    sprintf(buf, "%d", H[y * rs + x]);
	  } else 
	  {
	    sprintf(buf, "%d", F[y * rs + x]);
	  }
	  if (((type == 'd') && (sqrt(F[y * rs + x]) > maxval/2)) || 
	      ((type != 'd') && (F[y * rs + x] > maxval/2)))
	  {
	    if ((type == 'n') || (type == 'm') || (type == 'd'))
	      coldrawtext(fd, X+(GS/2), Y+(GS/2), buf, FONTSCALE, 0.0);
	    else
	      coldrawtext(fd, X+(GS/2), Y+(GS/2), buf, FONTSCALE, 1.0);
	    if ((G != NULL) && (G[y * rs +x]>0)) 
	    {
#ifdef BOLDCIRCLES
	      drawcircle(fd, X+(GS/2), Y+(GS/2), GS*0.45, 0, 4, 0);
#else
	      drawcircle(fd, X+(GS/2), Y+(GS/2), GS*0.45, 0, 1, 0);
#endif
	    }
	  }
	  else
	  {
	    if ((type == 'n') || (type == 'm') || (type == 'd'))
	      coldrawtext(fd, X+(GS/2), Y+(GS/2), buf, FONTSCALE, 1.0);
	    else
	      coldrawtext(fd, X+(GS/2), Y+(GS/2), buf, FONTSCALE, 0.0);
	    if ((G != NULL) && (G[y * rs +x]>0)) 
	    {
#ifdef BOLDCIRCLES
	      drawcircle(fd, X+(GS/2), Y+(GS/2), GS*0.45, 0, 4, 1);
#else
	      drawcircle(fd, X+(GS/2), Y+(GS/2), GS*0.45, 0, 1, 1);
#endif
	    }
	  }
	} // for (x = 0; x < rs; x++)
	if (printcoords)
	{
	  X = x * (double)GS;
	  Y = (cs-1-y) * (double)GS;
	  sprintf(buf, "%d", y+1);
	  drawtext(fd, X+(GS/2), Y+(GS/2), buf, FONTSCALE);
	} // if (printcoords)
      } // for (y = 0; y < cs; y++)
      if (printcoords)
      {
	for (x = 0; x < rs; x++)
	{
	  X = x * (double)GS;
	  Y = cs * (double)GS;
	  sprintf(buf, "%d", x+1);
	  drawtext(fd, X+(GS/2), Y+(GS/2), buf, FONTSCALE);
	} // for (x = 0; x < rs; x++)
      } // if (printcoords)
    } // if (datatype(image) == VFF_TYP_4_BYTE)
    else
    {
      fprintf(stderr, "%s: bad data type: %d\n", argv[0], datatype(image));
      exit(1);
    }
  }
  else if (type == 'p') /* grayscale image (levels printed as numbers) */
  {
    psheader(fd, rs * GS, cs * GS, GLOBALSCALE);
    for (y = 0; y < cs; y++)
      for (x = 0; x < rs; x++)
      {
        X = x * (double)GS;
        Y = (cs-1-y) * (double)GS;
        sprintf(buf, "%d", F[y * rs + x]);
        coldrawtext(fd, X+(GS/2), Y+(GS/2), buf, FONTSCALE, 0.0);
        drawsquare(fd, X, Y, GS, 1, 0);
        if ((G != NULL) && (G[y * rs +x]>0)) 
	{
#ifdef BOLDCIRCLES
	  drawcircle(fd, X+(GS/2), Y+(GS/2), GS*0.45, 0, 4, 0);
#else
	  drawcircle(fd, X+(GS/2), Y+(GS/2), GS*0.45, 0, 1, 0);
#endif
	}
      }
  } 
  else if (type == 'a') /* grayscale image (levels printed as letters) */
  {
    psheader(fd, rs * GS, cs * GS, GLOBALSCALE);
    for (y = 0; y < cs; y++)
      for (x = 0; x < rs; x++)
      {
        X = x * (double)GS;
        Y = (cs-1-y) * (double)GS;
        sprintf(buf, "%c", (char)(F[y * rs + x]-1+'a'));
        coldrawtext(fd, X+(GS/2), Y+(GS/2), buf, FONTSCALE, 0.0);
        drawsquare(fd, X, Y, GS, 1, 0);
        if ((G != NULL) && (G[y * rs +x]>0)) 
	{
#ifdef BOLDCIRCLES
	  drawcircle(fd, X+(GS/2), Y+(GS/2), GS*0.45, 0, 4, 0);
#else
	  drawcircle(fd, X+(GS/2), Y+(GS/2), GS*0.45, 0, 1, 0);
#endif
	}
      }
  } 
  else if (type == 'g') /* grayscale image (levels showed as colors) */
  {
    psheader(fd, rs * GS, cs * GS, GLOBALSCALE);
    for (y = 0; y < cs; y++)
      for (x = 0; x < rs; x++)
      {
        X = x * (double)GS;
        Y = (cs-1-y) * (double)GS;
        fillsquare(fd, X, Y, GS, (double)(F[y * rs + x])/(double)maxval);
      }
  } 
  else if (type == 'B') /* binary khalimsky grid */
  {
    psheader(fd, (rs + 1) * GS, (cs + 1) * GS, GLOBALSCALE);
    for (y = 0; y < cs; y++)
      for (x = 0; x < rs; x++)
      {
        X = (x + 1) * (double)GS;
        Y = (cs - y) * (double)GS;
#ifdef KHALIMSKY1
	if ((G != NULL) && (G[y * rs +x]>0)) 
	{
	  if (CARRE(x,y)) colsquare(fd, X, Y, R2, GRAYMARK, 1);
	  else if (INTERV(x,y)) colvrect(fd, X, Y, R1, R2, GRAYMARK, 1);
	  else if (INTERH(x,y)) colhrect(fd, X, Y, R1, R2, GRAYMARK, 1);
	  else colcircle(fd, X, Y, R1, GRAYMARK, 1);
	} 
	else 
	{
	  if (CARRE(x,y)) drawcenteredsquare(fd, X, Y, R2, F[y * rs + x]);
	  else if (INTERV(x,y)) drawvrect(fd, X, Y, R1, R2, F[y * rs + x]);
	  else if (INTERH(x,y)) drawhrect(fd, X, Y, R1, R2, F[y * rs + x]);
	  else drawcircle(fd, X, Y, R1, F[y * rs + x], 1, 0);
	}
#endif
#ifdef KHALIMSKY2
	if ((G != NULL) && (G[y * rs +x]>0)) 
	{
	  if (CARRE(x,y)) colsquare(fd, X, Y, R2, GRAYMARK, 1);
	} 
	if (F[y * rs + x])
	{
	  if (CARRE(x,y)) drawcenteredsquare(fd, X, Y, R2, 0);
	  else if (INTERV(x,y)) drawvrect(fd, X, Y, R1, R2, F[y * rs + x]);
	  else if (INTERH(x,y)) drawhrect(fd, X, Y, R1, R2, F[y * rs + x]);
	  else drawcircle(fd, X, Y, R0, F[y * rs + x], 1, 0);
	}
#endif
      }
  }
  else if (type == 'T') /* binary triangular grid */
  {
    psheader(fd, 2*TRG_M + ((rs/2)+1) * TRG_GS, 2*TRG_M + ((cs/3)+1) * TRG_GS, GLOBALSCALE);
    for (y = 0; y < cs; y++)
      for (x = 0; x < rs; x++)
      {
	if (TRIANG_SINGL(x,y)) 
	     {
	       X = TRG_M + (x/2)*(double)TRG_GS;
	       Y = TRG_M + (y/3)*(double)TRG_GS;
	       drawcircle(fd, X, Y, TRG_R, F[y * rs + x], 1, 0);
	     }
	else if (TRIANG_EDGE_UP(x,y))
	     { 
	       double X1 = (x/2)*(double)TRG_GS, X2 = X1;
	       double Y1 = (y/3)*(double)TRG_GS + (double)TRG_L;
	       double Y2 = ((y/3)+1)*(double)TRG_GS - (double)TRG_L;
	       X1 += TRG_M; X2 += TRG_M; Y1 += TRG_M; Y2 += TRG_M; 
	       if (F[y * rs + x]) drawline(fd, X1, Y1, X2, Y2, TRG_T); else drawline(fd, X1, Y1, X2, Y2, 1); 
	     }
	else if (TRIANG_EDGE_DN(x,y)) 
	     { 
	       double X1 = ((x/2)+1)*(double)TRG_GS - (double)TRG_L;
	       double X2 = (x/2)*(double)TRG_GS + (double)TRG_L;
	       double Y1 = (y/3)*(double)TRG_GS + (double)TRG_L;
	       double Y2 = ((y/3)+1)*(double)TRG_GS - (double)TRG_L;
	       X1 += TRG_M; X2 += TRG_M; Y1 += TRG_M; Y2 += TRG_M; 
	       if (F[y * rs + x]) drawline(fd, X1, Y1, X2, Y2, TRG_T); else drawline(fd, X1, Y1, X2, Y2, 1); 
	     }
	else if (TRIANG_EDGE_HZ(x,y)) 
	     { 
	       double X1 = (x/2)*(double)TRG_GS + (double)TRG_L;
	       double X2 = ((x/2)+1)*(double)TRG_GS - (double)TRG_L;
	       double Y1 = (y/3)*(double)TRG_GS;
	       double Y2 = Y1;
	       X1 += TRG_M; X2 += TRG_M; Y1 += TRG_M; Y2 += TRG_M; 
	       if (F[y * rs + x]) drawline(fd, X1, Y1, X2, Y2, TRG_T); else drawline(fd, X1, Y1, X2, Y2, 1); 
	     }
	else if (TRIANG_UP(x,y) && (x<(rs-1))) 
	     { 
	       double X1 = (x/2)*(double)TRG_GS + (double)TRG_L;
	       double Y1 = (y/3)*(double)TRG_GS + (double)TRG_L;
	       double X2 = ((x/2)+1)*(double)TRG_GS - (double)(2*TRG_L);
	       double Y2 = Y1;
	       double X3 = X1;
	       double Y3 = ((y/3)+1)*(double)TRG_GS - (double)(2*TRG_L);
	       X1 += TRG_M; Y1 += TRG_M; X2 += TRG_M; Y2 += TRG_M; X3 += TRG_M; Y3 += TRG_M; 
	       drawtriangle(fd, X1, Y1, X2, Y2, X3, Y3, F[y * rs + x]);
	     }
	else if (TRIANG_DN(x,y)) 
	     { 
	       double X1 = ((x/2)+1)*(double)TRG_GS - (double)(TRG_L);
	       double Y1 = (y/3)*(double)TRG_GS + (double)(2*TRG_L);
	       double X2 = (x/2)*(double)TRG_GS + (double)(2*TRG_L);
	       double Y2 = ((y/3)+1)*(double)TRG_GS - (double)(TRG_L);
	       double X3 = X1;
	       double Y3 = Y2;
	       X1 += TRG_M; Y1 += TRG_M; X2 += TRG_M; Y2 += TRG_M; X3 += TRG_M; Y3 += TRG_M; 
	       drawtriangle(fd, X1, Y1, X2, Y2, X3, Y3, F[y * rs + x]);
	     }
	
      }
  }
  else if (type == 'C') /* binary khalimsky grid (dual of B) */
  {
    psheader(fd, (rs + 1) * GS, (cs + 1) * GS, GLOBALSCALE);
    for (y = 0; y < cs; y++)
      for (x = 0; x < rs; x++)
      {
        X = (x + 1) * (double)GS;
        Y = (cs - y) * (double)GS;
        if (CARRE(x,y)) drawcircle(fd, X, Y, R1, F[y * rs + x], 1, 0);
        else if (INTERV(x,y)) drawhrect(fd, X, Y, R1, R2, F[y * rs + x]);
        else if (INTERH(x,y)) drawvrect(fd, X, Y, R1, R2, F[y * rs + x]);
        else drawcenteredsquare(fd, X, Y, R2, F[y * rs + x]);
      }
  }

  else if (type == 'N') /* grayscale khalimsky grid (levels printed as numbers) */
  {
    psheader(fd, (rs + 1) * GS, (cs + 1) * GS, GLOBALSCALE);
    if (datatype(image) == VFF_TYP_1_BYTE)
    {
      for (y = 0; y < cs; y++)
	for (x = 0; x < rs; x++)
	{
	  X = (x + 1) * (double)GS;
	  Y = (cs - y) * (double)GS;
	  if (CARRE(x,y)) drawcenteredsquare(fd, X, Y, R2, 0);
	  else if (INTERV(x,y)) drawvrect(fd, X, Y, R1, R2, 0);
	  else if (INTERH(x,y)) drawhrect(fd, X, Y, R1, R2, 0);
	  else drawcircle(fd, X, Y, R1, 0, 1, 0);
	  sprintf(buf, "%d", F[y * rs + x]);
	  drawtext(fd, X, Y, buf, FONTSCALE);
	}
    }
    else if (datatype(image) == VFF_TYP_DOUBLE)
    {
      double *F = DOUBLEDATA(image);
      for (y = 0; y < cs; y++)
	for (x = 0; x < rs; x++)
	{
	  X = (x + 1) * (double)GS;
	  Y = (cs - y) * (double)GS;
	  if (CARRE(x,y)) drawcenteredsquare(fd, X, Y, R2, 0);
	  else if (INTERV(x,y)) drawvrect(fd, X, Y, R1, R2, 0);
	  else if (INTERH(x,y)) drawhrect(fd, X, Y, R1, R2, 0);
	  else drawcircle(fd, X, Y, R1, 0, 1, 0);
	  sprintf(buf, "%1.1f", F[y * rs + x]);
	  drawtext(fd, X, Y, buf, FONTSCALE);
	}
    }
    else
    {
      fprintf(stderr, "%s: bad data type for mode %c\n", argv[0], type);
      exit(1);
    }
  }
  else if (type == 'M') /* grayscale khalimsky grid (levels printed as numbers, dual of N)) */
  {
    psheader(fd, (rs + 1) * GS, (cs + 1) * GS, GLOBALSCALE);
    for (y = 0; y < cs; y++)
      for (x = 0; x < rs; x++)
      {
        X = (x + 1) * (double)GS;
        Y = (cs - y) * (double)GS;
        if (SINGL(x,y)) drawcenteredsquare(fd, X, Y, R2, 0);
        else if (INTERH(x,y)) drawvrect(fd, X, Y, R1, R2, 0);
        else if (INTERV(x,y)) drawhrect(fd, X, Y, R1, R2, 0);
        else drawcircle(fd, X, Y, R1, 0, 1, 0);
        sprintf(buf, "%d", F[y * rs + x]);
        drawtext(fd, X, Y, buf, FONTSCALE);
      }
  }
  else if (type == 'G') /* grayscale khalimsky grid (levels showed as colored items, only 4 colors) */
  {
    int32_t w;
    psheader(fd, (rs + 1) * GS, (cs + 1) * GS, GLOBALSCALE);
    for (y = 0; y < cs; y++)
      for (x = 0; x < rs; x++)
      {
        X = (x + 1) * (double)GS;
        Y = (cs - y) * (double)GS;
	if (G && G[y*rs + x]) w = 6; else w = 1;
        if (CARRE(x,y)) colsquare(fd, X, Y, R2, (double)F[y * rs + x]/(double)maxval, w);
        else if (INTERV(x,y)) colvrect(fd, X, Y, R1, R2, (double)F[y * rs + x]/(double)maxval, w);
        else if (INTERH(x,y)) colhrect(fd, X, Y, R1, R2, (double)F[y * rs + x]/(double)maxval, w);
        else colcircle(fd, X, Y, R1, (double)F[y * rs + x]/(double)maxval, w);
	/*
        if (INTERV(x,y)) colvrect(fd, X, Y, R1, R2, (double)F[y * rs + x]/(double)maxval, w);
        else if (INTERH(x,y)) colhrect(fd, X, Y, R1, R2, (double)F[y * rs + x]/(double)maxval, w);
	*/
      }
  }
  else if (type == 'H') /* grayscale khalimsky grid (dual of H) */
  {
    int32_t w;
    psheader(fd, (rs + 1) * GS, (cs + 1) * GS, GLOBALSCALE);
    for (y = 0; y < cs; y++)
      for (x = 0; x < rs; x++)
      {
        X = (x + 1) * (double)GS;
        Y = (cs - y) * (double)GS;
	if (G && G[y*rs + x]) w = 6; else w = 1;
        if (CARRE(x,y)) colcircle(fd, X, Y, R1, (double)F[y * rs + x]/(double)maxval, w);
        else if (INTERV(x,y)) colhrect(fd, X, Y, R1, R2, (double)F[y * rs + x]/(double)maxval, w);
        else if (INTERH(x,y)) colvrect(fd, X, Y, R1, R2, (double)F[y * rs + x]/(double)maxval, w);
        else colsquare(fd, X, Y, R2, (double)F[y * rs + x]/(double)maxval, w);
      }
  }
  else if (type == 'v') /* vector image */
  {
    double xr = image->xdim, yr = image->ydim;
    double xb, yb; // base point for the vector
    double xe, ye; // end point for the vector
    double dx, dy; // offsets for the vector
    float *F = FLOATDATA(image);
    psheader(fd, rs * xr, cs * yr, GLOBALSCALE);
    for (y = 0; y < cs; y++)
      for (x = 0; x < rs; x++)
      {
	xb = (x * xr) + (xr / 2); 
	yb = ((cs-1-y) * yr) + (yr / 2); 
	dx = (double)F[y*rs + x] * xr / 2;
	dy = (double)F[N + y*rs + x] * yr / 2;
	xe = xb + dx; ye = yb + dy;
	arrow(fd, xb, yb, xe, ye);
      }    
  }

  fclose(fd);
  freeimage(image);
  return 0;
}
