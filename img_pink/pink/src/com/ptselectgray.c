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
/*! \file ptselectgray.c

\brief detects points in a 2D or 3D grayscale image corresponding to a given criterion 

<B>Usage:</B> ptselectgray in.pgm connex mint+ maxt+ mint-- maxt-- out.pgm

<B>Description:</B>

Select all points p such that the condition:<BR>
mint+ <= T+(p) <= maxt+ and mint-- <= T--(p) <= maxt--<BR>
is satisfied.<BR>

The parameter \b connex gives the connectivity used for the minima;
possible choices are 4, 8 in 2D and 6, 26 in 3D.

<B>Types supported:</B> byte 2d, byte 3d 

<B>Category:</B> topogray
\ingroup  topogray

\author Nicolas Combaret 2006
*/


#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcfah.h>
#include <mctopo.h>
#include <mctopo3d.h>

#define EN_FAH       0
#define CONTRAINTE   2

#include <mcindic.h>
#include <mcutil.h>



int32_t lptselectgray(struct xvimage *image,
                      int32_t connex, 
                      int32_t tpmin, 
                      int32_t tpmax, 
                      int32_t tmmmin, 
                      int32_t tmmmax);
                       
int32_t lptselectgray3d(struct xvimage *image,
                        int32_t connex,
                        int32_t tpmin, 
                        int32_t tpmax, 
                        int32_t tmmmin, 
                        int32_t tmmmax);


int main(int argc, char** argv)
{
    struct xvimage * image;
    int32_t connex;
    int32_t tpmin, tpmax, tmmmin, tmmmax;

    if (argc != 8)
    {
        fprintf(stderr, "usage: %s in.pgm connex mint+ maxt+ mint-- maxt-- out.pgm\n", argv[0]);
        exit(1);
    }

    image = readimage(argv[1]);
    if (image == NULL)
    {
        fprintf(stderr, "%s: readimage failed\n", argv[0]);
        exit(1);
    }

    connex = atoi(argv[2]);
    tpmin = atoi(argv[3]);
    tpmax = atoi(argv[4]);
    tmmmin = atoi(argv[5]);
    tmmmax = atoi(argv[6]);
    
    if (depth(image) == 1)
    {
         if (! lptselectgray(image, connex, tpmin, tpmax, tmmmin, tmmmax))
         {
             fprintf(stderr, "%s: lptselectgray failed\n", argv[0]);
             exit(1);
        }
    }
    else
    {
        if (! lptselectgray3d(image, connex, tpmin, tpmax, tmmmin, tmmmax))
        {
            fprintf(stderr, "%s: lptselectgray3d failed\n", argv[0]);
            exit(1);
        }
    }

    writeimage(image, argv[argc-1]);
    freeimage(image);

    return 0;
} /* main */





/* ==================================== */
int32_t largepdestr4(uint8_t *img,          /* pointeur base image */
                     int32_t p,                       /* index du point */
                     int32_t rs,                      /* taille rangee */
                     int32_t N,                       /* taille de l'image */
                     int32_t t8pmin,
                     int32_t t8pmax,
                     int32_t t4mmmin,
                     int32_t t4mmmax)
/* ==================================== */
{   
    int32_t t4mm,t8p,b,n;
    
    if ((p%rs==rs-1) || (p<rs) || (p%rs==0) || (p>=N-rs)) /* point de bord */
        return 0;

    nbtopo(img, p, rs, N, &t4mm, &b, &t8p, &n);
    if ((t4mmmin <= t4mm) && (t4mm <= t4mmmax) && (t8pmin <= t8p) && (t8p <= t8pmax))
        return 1;
    return 0;
} /* largepdestr4() */





/* ==================================== */
int32_t largepdestr8(uint8_t *img,          /* pointeur base image */
                     int32_t p,                       /* index du point */
                     int32_t rs,                      /* taille rangee */
                     int32_t N,                       /* taille image */
                     int32_t t4pmin,
                     int32_t t4pmax,
                     int32_t t8mmmin,
                     int32_t t8mmmax)
/* ==================================== */
{   
    int32_t t8mm,t4p,b,n;
    
    if ((p%rs==rs-1)||(p<rs)||(p%rs==0)||(p>=N-rs)) /* point de bord */
        return 0;

    nbtopo2(img, p, rs, N, &t8mm, &b, &t4p, &n);
    if ((t8mmmin <= t8mm) && (t8mm <= t8mmmax) && (t4pmin <= t4p) && (t4p <= t4pmax))
        return 1;
    return 0;
} /* largepdestr8() */





/* ==================================== */
int32_t lptselectgray(struct xvimage *image, 
                      int32_t connex, 
                      int32_t tpmin,
                      int32_t tpmax,
                      int32_t tmmmin, 
                      int32_t tmmmax)
        /* ==================================== */
#undef F_NAME
#define F_NAME "lptselectgray"
{ 
    int32_t x;                       /* index muet de pixel */
    int32_t rs = rowsize(image);     /* taille ligne */
    int32_t cs = colsize(image);     /* taille colonne */
    int32_t N = rs * cs;             /* taille image */
    uint8_t *F = UCHARDATA(image);      /* l'image de depart */
    uint8_t* R;                    // Masque d'arrivée : les points à 255 sont ceux qui correspondent aux critères, les autres
                                    // restent à 0

    if (depth(image) != 1) 
    {
        fprintf(stderr, "%s: cette version ne traite pas les images volumiques\n", F_NAME);
        return(0);
    }

//    IndicsInit(N);

    R = (uint8_t*)calloc(N, sizeof(uint8_t));
    if (R == NULL)
    {   fprintf(stderr, "%s() : calloc of R failed\n", F_NAME);
        return(0);
    }

    /* ================================================ */
    /*                  DEBUT ALGO                      */
    /* ================================================ */

    /* ========================================================= */
    /* REMPLISSAGE DE R : tous les points correspondant aux critères */
    /* ========================================================= */

    if (connex == 4)
    {
        for (x = 0; x < N; x++)
        {
            if (largepdestr4(F, x, rs, N, tpmin, tpmax, tmmmin, tmmmax)) 
            {
                R[x] = 255;
            }
        }
    }
    else
    {
        for (x = 0; x < N; x++)
        {
            if (largepdestr8(F, x, rs, N, tpmin, tpmax, tmmmin, tmmmax)) 
            {
                R[x] = 255;
            }
        }
    }

    // Recopie du résultat
    for (x = 0; x < N; x++)
    {
        F[x] = R[x];
    }
    
    /* ================================================ */
    /* UN PEU DE MENAGE                                 */
    /* ================================================ */

//    IndicsTermine();
    free(R);
    return(1);
} // lptselectgray()










/* ==================================== */
int32_t largepdestr6(uint8_t *img,          /* pointeur base image */
                     int32_t p,                       /* index du point */
                     int32_t rs,                      /* taille rangee */
                     int32_t ps,                      /* taille plan */
                     int32_t N,                       /* taille de l'image */
                     int32_t t26pmin,
                     int32_t t26pmax,
                     int32_t t6mmmin,
                     int32_t t6mmmax)
/* ==================================== */
{
    return ((t26pmin <= mctopo3d_t26p(img, p, rs, ps, N)) && (mctopo3d_t26p(img, p, rs, ps, N) <= t26pmax) &&
            (t6mmmin <= mctopo3d_t6mm(img, p, rs, ps, N)) && (mctopo3d_t6mm(img, p, rs, ps, N) <= t6mmmax));
} /* largepdestr6() */





/* ==================================== */
int32_t largepdestr26(uint8_t *img,          /* pointeur base image */
                        int32_t p,                       /* index du point */
                        int32_t rs,                      /* taille rangee */
                        int32_t ps,                      /* taille plan */
                        int32_t N,                       /* taille image */
                        int32_t t6pmin,
                        int32_t t6pmax,
                        int32_t t26mmmin,
                        int32_t t26mmmax)
/* ==================================== */
{
    return ((t6pmin <= mctopo3d_t6p(img, p, rs, ps, N)) && (mctopo3d_t6p(img, p, rs, ps, N) <= t6pmax) &&
            (t26mmmin <= mctopo3d_t26mm(img, p, rs, ps, N)) && (mctopo3d_t26mm(img, p, rs, ps, N) <= t26mmmax));
} /* largepdestr26() */





/* ==================================== */
int32_t lptselectgray3d(struct xvimage *image,
                        int32_t connex,
                        int32_t tpmin, 
                        int32_t tpmax, 
                        int32_t tmmmin, 
                        int32_t tmmmax)
        /* ==================================== */
#undef F_NAME
#define F_NAME "ltoposhrinkgray3d"
{ 
    int32_t x;                       /* index muet de pixel */
    int32_t rs = rowsize(image);     /* taille ligne */
    int32_t cs = colsize(image);     /* taille colonne */
    int32_t ps = rs * cs;            /* taille plan */
    int32_t ds = depth(image);       /* nombre plans */
    int32_t N = ds * ps;             /* taille image */
    uint8_t *F = UCHARDATA(image);      /* l'image de depart */
    uint8_t* R;

    mctopo3d_init_topo3d();

    R = (uint8_t*)calloc(N, sizeof(uint8_t));
    if (R == NULL)
    {
        fprintf(stderr, "%s: calloc of R failed\n", F_NAME);
        return(0);
    }
    
    /* ================================================ */
    /*                  DEBUT ALGO                      */
    /* ================================================ */

    /* ========================================================= */
    /* REMPLISSAGE DE R : tous les points correspondant aux critères*/
    /* ========================================================= */
    if (connex == 6)
    {
        for (x = 0; x < N; x++)
        {
            if (largepdestr6(F, x, rs, ps, N, tpmin, tpmax, tmmmin, tmmmax))
            {
                R[x] = 255;
            }
        }
    }
    else
    {
        for (x = 0; x < N; x++)
        {
            if (largepdestr26(F, x, rs, ps, N, tpmin, tpmax, tmmmin, tmmmax))
            {
                R[x] = 255;
            }
        }
    }

    // Recopie du résultat
    for (x = 0; x < N; x++)
    {
        F[x] = R[x];
    }

    /* ================================================ */
    /* UN PEU DE MENAGE                                 */
    /* ================================================ */

    mctopo3d_termine_topo3d();
    free(R);
    return(1);
}
