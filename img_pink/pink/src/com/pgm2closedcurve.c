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
/*! \file pgm2closedcurve.c

\brief extracts a curve from a binary image

<B>Usage:</B> pgm2closedcurve image.pgm connex [x y [z]] out.curve

<B>Description:</B>
Extracts a closed curve from a binary image, that is, a curve with the
same starting and ending point.
The parameter \b connex is the connexity of the curve.
It may be equal to 4 or 8 in 2D, and to 6, 18 or 26 in 3D.
If given, the point <B>(x, y, z)</B> (2D) or <B>(x, y, z)</B> (3D) 
is taken as the starting point of the curve, and must be a curve point. 
The output is the text file \b out.curve, with the following format:<br>
b nbpoints<br>
x1 y1<br>
x2 y2<br>
...<br>
or (3D): 
B nbpoints<br>
x1 y1 z1<br>
x2 y2 z2<br>
...<br>

The points of the curve may also be valued. This is must be indicated by 
a value of 40, 80, 60, 180 or 260 for the parameter \b connex, instead
of 4, 8, 6, 18 or 26 respectively. In this case,
the output is the text file \b out.curve, with the following format:<br>
n nbpoints<br>
x1 y1 v1<br>
x2 y2 v2<br>
...<br>
or (3D): 
N nbpoints<br>
x1 y1 z1 v1<br>
x2 y2 z2 v2<br>
...<br>

<B>Types supported:</B> byte 2D, byte 3D

<B>Category:</B> convert geo
\ingroup  convert geo

\author Michel Couprie
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <assert.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mctopo.h>
#include <mctopo3d.h>

/* =============================================================== */
int32_t uniquevoisin4(  
  uint8_t *img,          /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t N)                       /* taille image */
/* =============================================================== */
/* retourne l'indice du premier voisin objet de p trouvé dans le voisinage */
{
  register uint8_t * ptr = img+p;
  if ((p%rs!=rs-1) && (*(ptr+1))) return p+1;
  if ((p>=rs) && (*(ptr-rs))) return p-rs;
  if ((p%rs!=0) && (*(ptr-1))) return p-1;
  if ((p<N-rs) && (*(ptr+rs))) return p+rs;  
  assert(1); exit(1);
} // uniquevoisin4()

/* =============================================================== */
int32_t uniquevoisin8(  
  uint8_t *img,          /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t N)                       /* taille image */
/* =============================================================== */
/* retourne l'indice du premier voisin objet de p trouvé dans le voisinage */
{
  register uint8_t * ptr = img+p;
  if ((p%rs!=rs-1) && (*(ptr+1))) return p+1;
  if (((p%rs!=rs-1)&&(p>=rs)) && (*(ptr+1-rs))) return p+1-rs;
  if ((p>=rs) && (*(ptr-rs))) return p-rs;
  if (((p>=rs)&&(p%rs!=0)) && (*(ptr-rs-1))) return p-rs-1;
  if ((p%rs!=0) && (*(ptr-1))) return p-1;
  if (((p%rs!=0)&&(p<N-rs)) && (*(ptr-1+rs))) return p-1+rs;
  if ((p<N-rs) && (*(ptr+rs))) return p+rs;
  if (((p<N-rs)&&(p%rs!=rs-1)) && (*(ptr+rs+1))) return p+rs+1;
  assert(1); exit(1);
} // uniquevoisin8()

/* ========================================== */
int32_t uniquevoisin6(
  uint8_t *B,            /* pointeur base image */
  int32_t i,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N)                       /* taille image */
/* ========================================== */
/* retourne l'indice du premier voisin objet de i trouvé dans le voisinage */
{
  if ((i%rs!=rs-1) && B[i+1]) return i+1;
  if (((i%ps)>=rs) && B[i-rs]) return i-rs;
  if ((i%rs!=0) && B[i-1]) return i-1;
  if (((i%ps)<ps-rs) && B[i+rs]) return i+rs;
  if ((i>=ps) && B[i-ps]) return i-ps;
  if ((i<N-ps) && B[i+ps]) return i+ps;
  assert(1); exit(1);
} /* uniquevoisin6() */

/* ========================================== */
int32_t uniquevoisin18(
  uint8_t *B,            /* pointeur base image */
  int32_t i,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N)                       /* taille image */
/* ========================================== */
/* retourne l'indice du premier voisin objet de i trouvé dans le voisinage */
{
  if (((i<N-ps)&&(i%rs!=rs-1)) && B[ps+i+1]) return ps+i+1;
  if (((i<N-ps)&&(i%ps>=rs)) && B[ps+i-rs]) return ps+i-rs;
  if (((i<N-ps)&&(i%rs!=0)) && B[ps+i-1]) return ps+i-1;
  if (((i<N-ps)&&(i%ps<ps-rs)) && B[ps+i+rs]) return ps+i+rs;
  if (((i<N-ps)) && B[ps+i]) return ps+i;
  if (((i%rs!=rs-1)) && B[i+1]) return i+1;
  if (((i%rs!=rs-1)&&(i%ps>=rs)) && B[i+1-rs]) return i+1-rs;
  if (((i%ps>=rs)) && B[i-rs]) return i-rs;
  if (((i%ps>=rs)&&(i%rs!=0)) && B[i-rs-1]) return i-rs-1;
  if (((i%rs!=0)) && B[i-1]) return i-1;
  if (((i%rs!=0)&&(i%ps<ps-rs)) && B[i-1+rs]) return i-1+rs;
  if (((i%ps<ps-rs)) && B[i+rs]) return i+rs;
  if (((i%ps<ps-rs)&&(i%rs!=rs-1)) && B[i+rs+1]) return i+rs+1;
  if (((i>=ps)&&(i%rs!=rs-1)) && B[-ps+i+1]) return -ps+i+1;
  if (((i>=ps)&&(i%ps>=rs)) && B[-ps+i-rs]) return -ps+i-rs;
  if (((i>=ps)&&(i%rs!=0)) && B[-ps+i-1]) return -ps+i-1;
  if (((i>=ps)&&(i%ps<ps-rs)) && B[-ps+i+rs]) return -ps+i+rs;
  if (((i>=ps)) && B[-ps+i]) return -ps+i;
  assert(1); exit(1);
} /* uniquevoisin18() */

/* ========================================== */
int32_t uniquevoisin26(
  uint8_t *B,            /* pointeur base image */
  int32_t i,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N)                       /* taille image */
/* ========================================== */
/* retourne l'indice du premier voisin objet de i trouvé dans le voisinage */
{
  if (((i<N-ps)&&(i%rs!=rs-1)) && B[ps+i+1]) return ps+i+1;
  if (((i<N-ps)&&(i%rs!=rs-1)&&(i%ps>=rs)) && B[ps+i+1-rs]) return ps+i+1-rs;
  if (((i<N-ps)&&(i%ps>=rs)) && B[ps+i-rs]) return ps+i-rs;
  if (((i<N-ps)&&(i%ps>=rs)&&(i%rs!=0)) && B[ps+i-rs-1]) return ps+i-rs-1;
  if (((i<N-ps)&&(i%rs!=0)) && B[ps+i-1]) return ps+i-1;
  if (((i<N-ps)&&(i%rs!=0)&&(i%ps<ps-rs)) && B[ps+i-1+rs]) return ps+i-1+rs;
  if (((i<N-ps)&&(i%ps<ps-rs)) && B[ps+i+rs]) return ps+i+rs;
  if (((i<N-ps)&&(i%ps<ps-rs)&&(i%rs!=rs-1)) && B[ps+i+rs+1]) return ps+i+rs+1;
  if (((i<N-ps)) && B[ps+i]) return ps+i;
  if (((i%rs!=rs-1)) && B[i+1]) return i+1;
  if (((i%rs!=rs-1)&&(i%ps>=rs)) && B[i+1-rs]) return i+1-rs;
  if (((i%ps>=rs)) && B[i-rs]) return i-rs;
  if (((i%ps>=rs)&&(i%rs!=0)) && B[i-rs-1]) return i-rs-1;
  if (((i%rs!=0)) && B[i-1]) return i-1;
  if (((i%rs!=0)&&(i%ps<ps-rs)) && B[i-1+rs]) return i-1+rs;
  if (((i%ps<ps-rs)) && B[i+rs]) return i+rs;
  if (((i%ps<ps-rs)&&(i%rs!=rs-1)) && B[i+rs+1]) return i+rs+1;
  if (((i>=ps)&&(i%rs!=rs-1)) && B[-ps+i+1]) return -ps+i+1;
  if (((i>=ps)&&(i%rs!=rs-1)&&(i%ps>=rs)) && B[-ps+i+1-rs]) return -ps+i+1-rs;
  if (((i>=ps)&&(i%ps>=rs)) && B[-ps+i-rs]) return -ps+i-rs;
  if (((i>=ps)&&(i%ps>=rs)&&(i%rs!=0)) && B[-ps+i-rs-1]) return -ps+i-rs-1;
  if (((i>=ps)&&(i%rs!=0)) && B[-ps+i-1]) return -ps+i-1;
  if (((i>=ps)&&(i%rs!=0)&&(i%ps<ps-rs)) && B[-ps+i-1+rs]) return -ps+i-1+rs;
  if (((i>=ps)&&(i%ps<ps-rs)) && B[-ps+i+rs]) return -ps+i+rs;
  if (((i>=ps)&&(i%ps<ps-rs)&&(i%rs!=rs-1)) && B[-ps+i+rs+1]) return -ps+i+rs+1;
  if (((i>=ps)) && B[-ps+i]) return -ps+i;
  assert(1); exit(1);
} /* uniquevoisin26() */

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  struct xvimage * sav;
  FILE *fd = NULL;
  int32_t rs, cs, ds, ps, N, x, y, z, p, sp, n, connex, val;
  uint8_t *F;
  uint8_t *S;
  int32_t *P;

  if ((argc != 7) && (argc != 6) && (argc != 4))
  {
    fprintf(stderr, "usage: %s image.pgm connex [x y [z]] out.curve \n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }
  connex = atoi(argv[2]);
  if (connex >= 40)
  {
    val = 1;
    connex = connex / 10;
    sav = copyimage(image);
    S = UCHARDATA(sav);
  }
  else val = 0;

  if (argc > 4)
  { 
    x = atoi(argv[3]);
    y = atoi(argv[4]);
  }
  if (argc > 6)
    z = atoi(argv[5]);

  rs = rowsize(image);
  cs = colsize(image);
  ds = depth(image);
  ps = rs * cs;
  N = ps * ds;
  F = UCHARDATA(image);
  P = (int32_t *)calloc(1,N*sizeof(int32_t));
  if (P == NULL)
  {
    fprintf(stderr, "%s: malloc failed\n", argv[0]);
    exit(1);
  }

  if (argc > 4)
  {   
    if (argc > 6) p = z*ps + y*rs + x; else p = y*rs + x;

    if (!F[p])
    {
      fprintf(stderr, "%s: bad starting point\n", argv[0]);
      exit(1);
    }

    if ((connex == 4) && (nbvois4(F, p, rs, N) != 2))
    {
      fprintf(stderr, "%s: (x,y) not a curve point\n", argv[0]);
      exit(1);
    }
    else if ((connex == 8) && (nbvois8(F, p, rs, N) != 2))
    {
      fprintf(stderr, "%s: (x,y) not a curve point\n", argv[0]);
      exit(1);
    }
    else if ((connex == 6) && (mctopo3d_nbvoiso6(F, p, rs, ps, N) != 2))
    {
      fprintf(stderr, "%s: (x,y,z) not a curve point\n", argv[0]);
      exit(1);
    }
    else if ((connex == 18) && (mctopo3d_nbvoiso18(F, p, rs, ps, N) != 2))
    {
      fprintf(stderr, "%s: (x,y,z) not a curve point\n", argv[0]);
      exit(1);
    }
    else if ((connex == 26) && (mctopo3d_nbvoiso26(F, p, rs, ps, N) != 2))
    {
      fprintf(stderr, "%s: (x,y,z) not a curve point\n", argv[0]);
      exit(1);
    }
  }
  else // no given begin point
  {
    p = -1;
    if (connex == 4)
    { for (x = 0; x < N; x++) if ((F[x]) && (nbvois4(F, x, rs, N) == 2)) { p = x; break; } }
    else if (connex == 8)
    { for (x = 0; x < N; x++) if ((F[x]) && (nbvois8(F, x, rs, N) == 2)) { p = x; break; } }
    else if (connex == 6)
    { for (x = 0; x < N; x++) if ((F[x]) && (mctopo3d_nbvoiso6(F, x, rs, ps, N) != 2)) { p = x; break; } }
    else if (connex == 18)
    { for (x = 0; x < N; x++) if ((F[x]) && (mctopo3d_nbvoiso18(F, x, rs, ps, N) != 2)) { p = x; break; } }
    else if (connex == 26)
    { for (x = 0; x < N; x++) if ((F[x]) && (mctopo3d_nbvoiso26(F, x, rs, ps, N) != 2)) { p = x; break; } }
    if (p == -1)
    {
      fprintf(stderr, "%s: no curve point\n", argv[0]);
      exit(1);
    }
  }

  fd = fopen(argv[argc - 1],"w");
  if (!fd)
  {
    fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[argc - 1]);
    exit(1);
  }

  n = 0;  // counts the number of points 
  sp = p; // save starting poins
  if (connex == 4)
  {
    do
    {
      P[n] = p; n++; F[p] = 0;
      p = uniquevoisin4(F, p, rs, N);
    } while (nbvois4(F, p, rs, N) == 1);
    P[n] = p; n++;
    if (nbvois4(F, p, rs, N) != 0)
      fprintf(stderr, "%s: warning: final point not a curve point\n", argv[0]);
    if (!voisins4(p, sp, rs))
      fprintf(stderr, "%s: warning: not a closed curve\n", argv[0]);
    else
      P[n] = sp; n++;
  }
  else if (connex == 8)
  {
    do
    {
      P[n] = p; n++; F[p] = 0;
      p = uniquevoisin8(F, p, rs, N);
    } while (nbvois8(F, p, rs, N) == 1);
    P[n] = p; n++;
    if (nbvois8(F, p, rs, N) != 0)
      fprintf(stderr, "%s: warning: final point not a curve point\n", argv[0]);
    if (!voisins8(p, sp, rs))
      fprintf(stderr, "%s: warning: not a closed curve\n", argv[0]);
    else
      P[n] = sp; n++;
  }
  else if (connex == 6)
  {
    do
    {
      P[n] = p; n++; F[p] = 0;
      p = uniquevoisin6(F, p, rs, ps, N);
    } while (mctopo3d_nbvoiso6(F, p, rs, ps, N) == 1);
    P[n] = p; n++;
    if (mctopo3d_nbvoiso6(F, p, rs, ps, N) != 0)
      fprintf(stderr, "%s: warning: final point not a curve point\n", argv[0]);
    if (!voisins6(p, sp, rs, ps))
      fprintf(stderr, "%s: warning: not a closed curve\n", argv[0]);
    else
      P[n] = sp; n++;
  }
  else if (connex == 18)
  {
    do
    {
      P[n] = p; n++; F[p] = 0;
      p = uniquevoisin18(F, p, rs, ps, N);
    } while (mctopo3d_nbvoiso18(F, p, rs, ps, N) == 1);
    P[n] = p; n++;
    if (mctopo3d_nbvoiso18(F, p, rs, ps, N) != 0)
      fprintf(stderr, "%s: warning: final point not a curve point\n", argv[0]);
    if (!voisins18(p, sp, rs, ps))
      fprintf(stderr, "%s: warning: not a closed curve\n", argv[0]);
    else
      P[n] = sp; n++;
  }
  else if (connex == 26)
  {
    do
    {
      P[n] = p; n++; F[p] = 0;
      p = uniquevoisin26(F, p, rs, ps, N);
    } while (mctopo3d_nbvoiso26(F, p, rs, ps, N) == 1);
    P[n] = p; n++;
    if (mctopo3d_nbvoiso26(F, p, rs, ps, N) != 0)
      fprintf(stderr, "%s: warning: final point not a curve point\n", argv[0]);
    if (!voisins26(p, sp, rs, ps))
      fprintf(stderr, "%s: warning: not a closed curve\n", argv[0]);
    else
      P[n] = sp; n++;
  }

  if (val)
  {
    if ((connex == 4) || (connex == 8))
    {
      fprintf(fd, "n %d\n", n); 
      for (x = 0; x < n; x++) fprintf(fd, "%d %d %d\n", P[x] % rs, P[x] / rs, S[P[x]]);
    }
    else
    {
      fprintf(fd, "N %d\n", n); 
      for (x = 0; x < n; x++) fprintf(fd, "%d %d %d %d\n", P[x] % rs, (P[x] % ps) / rs, P[x] / ps, S[P[x]]);
    }
  }
  else
  {
    if ((connex == 4) || (connex == 8))
    {
      fprintf(fd, "b %d\n", n); 
      for (x = 0; x < n; x++) fprintf(fd, "%d %d\n", P[x] % rs, P[x] / rs);
    }
    else
    {
      fprintf(fd, "B %d\n", n); 
      for (x = 0; x < n; x++) fprintf(fd, "%d %d %d\n", P[x] % rs, (P[x] % ps) / rs, P[x] / ps);
    }
  }
  fclose(fd);
  if (val) freeimage(sav);
  freeimage(image);
  free(P);
  return 0;
}
