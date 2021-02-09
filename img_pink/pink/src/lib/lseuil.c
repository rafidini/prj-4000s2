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
/* operateurs de seuillage */
/* Michel Couprie - juillet 1996 */

#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <math.h>
#include <mcutil.h>
#include <mcimage.h>
#include <mccodimage.h>
#include <lhisto.h>
#include <lseuil.h>

#define VERBOSE

/* ==================================== */
int32_t lseuil(
        struct xvimage *f,
        double seuil)
/* ==================================== */
/* tous les pixels < seuil sont mis a 0, les autres a 255 */
{
  index_t x;                       /* index muet de pixel */
  index_t rs = rowsize(f);         /* taille ligne */
  index_t cs = colsize(f);         /* taille colonne */
  index_t ds = depth(f);           /* nb. plans */
  index_t N = rs * cs * ds;        /* taille image */

  if (datatype(f) == VFF_TYP_1_BYTE)
  {
    uint8_t *F = UCHARDATA(f);
    for (x = 0; x < N; x++) if (F[x] < seuil) F[x] = NDG_MIN; else F[x] = NDG_MAX;  
  }
  else if (datatype(f) == VFF_TYP_2_BYTE)
  {
    int16_t *FS = SSHORTDATA(f);
    for (x = 0; x < N; x++) if (FS[x] < seuil) FS[x] = NDG_MIN; else FS[x] = NDG_MAX;  
  }
  else if (datatype(f) == VFF_TYP_4_BYTE)
  {
    int32_t *FL = SLONGDATA(f);
    for (x = 0; x < N; x++) if (FL[x] < seuil) FL[x] = NDG_MIN; else FL[x] = NDG_MAX;  
  }
  else if (datatype(f) == VFF_TYP_FLOAT)
  {
    float *FF = FLOATDATA(f);
    for (x = 0; x < N; x++) if (FF[x] < seuil) FF[x] = 0.0; else FF[x] = 1.0;  
  }
  else if (datatype(f) == VFF_TYP_DOUBLE)
  {
    double *FD = DOUBLEDATA(f);
    for (x = 0; x < N; x++) if (FD[x] < seuil) FD[x] = 0.0; else FD[x] = 1.0;  
  }
  else
  {   fprintf(stderr,"lseuil() : bad datatype : %d\n", datatype(f));
      return 0;
  }
  return 1;
}

/* ==================================== */
int32_t lseuil2(
        struct xvimage *f,
        uint8_t seuilmin,
        uint8_t seuilmax,
        uint8_t valmin,
        uint8_t valmax)
/* ==================================== */
/* tous les pixels < seuilmin sont mis a valmin */
/* tous les pixels >= seuilmax sont mis a valmax */
{
  index_t x;                       /* index muet de pixel */
  index_t rs = rowsize(f);         /* taille ligne */
  index_t cs = colsize(f);         /* taille colonne */
  index_t d = depth(f);            /* nb. plans */
  index_t N = rs * cs * d;         /* taille image */

  if (datatype(f) == VFF_TYP_1_BYTE)
  {
    uint8_t *F = UCHARDATA(f);
    for (x = 0; x < N; x++)
      if ((int32_t)(F[x]) < (int32_t)seuilmin) F[x] = valmin;  
      else if ((int32_t)(F[x]) >= (int32_t)seuilmax) F[x] = valmax;
  }
  else if (datatype(f) == VFF_TYP_4_BYTE)
  {
    int32_t *F = SLONGDATA(f);
    for (x = 0; x < N; x++)
      if ((int32_t)(F[x]) < (int32_t)seuilmin) F[x] = (int32_t)valmin;  
      else if ((int32_t)(F[x]) >= (int32_t)seuilmax) F[x] = (int32_t)valmax;
  }
  else if (datatype(f) == VFF_TYP_FLOAT)
  {
    float *F = FLOATDATA(f);
    for (x = 0; x < N; x++)
      if ((int32_t)(F[x]) < (int32_t)seuilmin) F[x] = (float)valmin;  
      else if ((int32_t)(F[x]) >= (int32_t)seuilmax) F[x] = (float)valmax;
  }
  else
  {   fprintf(stderr,"lseuil() : bad datatype : %d\n", datatype(f));
      return 0;
  }

  return 1;
}

/* ==================================== */
int32_t lseuil3(
        struct xvimage *f,
        double seuil, double seuil2)
/* ==================================== */
/* tous les seuil <= pixels < seuil2 sont mis a 255, les autres a 0 */
{
  index_t x;                       /* index muet de pixel */
  index_t rs = rowsize(f);         /* taille ligne */
  index_t cs = colsize(f);         /* taille colonne */
  index_t ds = depth(f);           /* nb. plans */
  index_t N = rs * cs * ds;        /* taille image */

  if (datatype(f) == VFF_TYP_1_BYTE)
  {
    uint8_t *F = UCHARDATA(f);
    for (x = 0; x < N; x++) 
      if ((F[x] < seuil2) && (F[x]>=seuil))
	F[x] = NDG_MAX; 
      else F[x] = NDG_MIN;  
  }
  else if (datatype(f) == VFF_TYP_4_BYTE)
  {
    int32_t *FL = SLONGDATA(f);
    for (x = 0; x < N; x++) 
      if ((FL[x] < seuil2) && (FL[x]>=seuil))
	FL[x] = NDG_MAX; 
      else FL[x] = NDG_MIN;  
  }
  else if (datatype(f) == VFF_TYP_FLOAT)
  {
    float *FF = FLOATDATA(f);
    for (x = 0; x < N; x++) 
      if ((FF[x] < seuil2) && (FF[x]>=seuil))
	FF[x] = 1.0; 
      else FF[x] = 0.0;  
  }
  else
  {   fprintf(stderr,"lseuil() : bad datatype : %d\n", datatype(f));
      return 0;
  }
  return 1;
}

/* ==================================== */
int32_t lotsu(
	      struct xvimage *f,
	      struct xvimage *mask
	     )
/* ==================================== */
/* methode d'Otsu - code from http://www.dandiggins.co.uk/arlib-9.html */
{
#undef F_NAME
#define F_NAME "lotsu"

  index_t x;                       /* index muet de pixel */
  index_t rs = rowsize(f);         /* taille ligne */
  index_t cs = colsize(f);         /* taille colonne */
  index_t ds = depth(f);           /* nb. plans */
  index_t N = rs * cs * ds;        /* taille image */
  index_t * histo;

  float  w = 0;                // first order cumulative
  float  u = 0;                // second order cumulative
  float  uT = 0;               // total mean level

  int    I;
  int    k = NDG_MAX;              // maximum histogram index
  int    threshold = 0;        // optimal threshold value
  
  float  histNormalized[NDG_MAX+1]; // normalized histogram values
  
  float  work1, work2;		// working variables
  double work3 = 0.0;


  printf("WARNING %s : obsolete version - use seuilOtsu\n", F_NAME);


  if (datatype(f) == VFF_TYP_1_BYTE)
  {
    uint8_t *F = UCHARDATA(f);

    histo = (index_t *)calloc(1,(NDG_MAX + 1) * sizeof(index_t));
    assert (histo != NULL);
    if (! lhisto(f, mask, histo))
    {
      fprintf(stderr, "%s: function lhisto failed\n", F_NAME);
      exit(1);
    }	

    // Create normalised histogram values
    for (I=1; I<=k; I++) 
      histNormalized[I-1] = histo[I-1]/(float)N;


    // Calculate total mean level
    for (I=1; I<=k; I++) 
      uT+=(I*histNormalized[I-1]);

    // Find optimal threshold value
    for (I=1; I<k; I++) {
      w+=histNormalized[I-1];
      u+=(I*histNormalized[I-1]);
      work1 = (uT * w - u);
      work2 = (work1 * work1) / ( w * (1.0f-w) );
      if (work2>work3) work3=work2;
    }

    // Convert the final value to an integer
    threshold = (int)sqrt(work3);
#ifdef VERBOSE
    printf("Otsu's optimal threshold = %d\n", threshold);
#endif
#ifdef VERBOSE
    printf("%s optimal threshold = %d\n", F_NAME, threshold);
#endif    

    for (x = 0; x < N; x++) 
      if (F[x] >= threshold)
	F[x] = NDG_MAX; 
      else F[x] = NDG_MIN;  
  }
  else
  {   fprintf(stderr,"lseuil() : bad datatype : %d\n", datatype(f));
      return 0;
  }
  return 1;
} // lotsu()

/*
  Seuillage automatique de Ostu. Le seuil choisi est celui qui
  minimise la variance inter-classe ou de manière équivalente celui
  qui maximise la variance intra-classe.
*/

/* ==================================== */
int32_t lseuilOtsu(
        struct xvimage *f)
/* ==================================== */
/* tous les pixels < seuil sont mis a 0, les autres a 255 */
#undef F_NAME
#define F_NAME "lseuilOtsu"
{
  index_t x;                       /* index muet de pixel */
  index_t rs = rowsize(f);         /* taille ligne */
  index_t cs = colsize(f);         /* taille colonne */
  index_t ds = depth(f);           /* nb. plans */
  index_t N = rs * cs * ds;        /* taille image */
  int32_t hist[256];               /* histogramme */
  index_t l;                       /* index muet de niveau de gris */
  int64_t w_o, h_o, w_c, h_c;      /* accumulateurs */
  double vic, vic_star=0.0;
  index_t s_star;                  /* seuil optimal */

  if (datatype(f) == VFF_TYP_1_BYTE)
  {
    uint8_t *F = UCHARDATA(f);
    for (l = 0; l < 256; l++) hist[l] = 0;
    
    for (x = 0; x < N; x++) {
      hist[F[x]]++;
    }

    w_o = 0;
    h_o = 0;
    w_c = 0;
    h_c = 0;

    for (l = 0; l < 256; l++) h_c += l * hist[l];
    
    w_c = N;
    
    //    printf("Valeur moyenne de l'image:%lg\n",  (double)h_c/(double)w_c);

    for (l = 0; l < 256; l++){
      w_o = w_o + hist[l];
      h_o = h_o + hist[l] * l;
      w_c = w_c - hist[l];
      h_c = h_c - hist[l] * l;
      vic = (double)w_o * (double)w_c * 
	(((double)h_o/(double)w_o) - ((double)h_c/(double)w_c)) 
	* (((double)h_o/(double)w_o) - ((double)h_c/(double)w_c));
      if(vic > vic_star){
	vic_star = vic;
	s_star = l;
      }
    }

#ifdef VERBOSE
    printf("%s optimal threshold = %d\n", F_NAME, s_star);
#endif    
    return lseuil(f, s_star+1);  
  }
  else
  {   fprintf(stderr,"lseuil() : bad datatype : %d\nOtsu is only implemented for byte images", datatype(f));
      return 0;
  }
  return 1;
}
