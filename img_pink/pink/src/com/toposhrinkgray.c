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
/*! \file toposhrinkgray.c

\brief topologically controled grayscale shrinking (one step)

<B>Usage:</B> toposhrinkgray in.pgm connex t+min t+max t--min t--max {0|a|d} [inhibit] out.pgm

<B>Description:</B>
Topologically controled grayscale shrinking (one step).

The parameter <0|a|d> indicates the action performed on the selected point p:
\li 0: it is lowered down to 0;
\li a: it is lowered down to alpha-(p,F) where F denotes the original image;
\li d: it is lowered down to delta-(p,F) where F denotes the original image.

The parameter \b connex indicates the connectivity of the binary object.
Possible choices are 4, 8 in 2D and 6, 18, 26 in 3D.

If the parameter \b inhibit is given and is a binary image name,
then the points of this image (set Y) will be left unchanged. 

\verbatim
  For all points p, not in Y, and such that 
      t+min <= T+(p) <= t+max and t--min <= T--(p) <= t--max 
    lower p according to the chosen strategy
\endverbatim

<B>Types supported:</B> byte 2d, byte 3d

<B>Category:</B> topogray
\ingroup  topogray

References:

[BEC97] G. Bertrand, J. C. Everat and M. Couprie: <A HREF="http://www.esiee.fr/~info/a2si/Ps/elimag97.ps.gz">"Image segmentation through operators based upon topology"</A>, <I> Journal of Electronic Imaging</I>, Vol.&nbsp;6, No.&nbsp;4, pp.&nbsp;395-405, 1997.

[CBB01] M. Couprie, F.N. Bezerra, Gilles Bertrand: <A HREF="http://www.esiee.fr/~info/a2si/Ps/elimag01.ps.gz">"Topological operators for
grayscale image processing"</A>, <I> Journal of Electronic Imaging</I>, Vol.&nbsp;10, No.&nbsp;4, pp.&nbsp;1003-1015, 2001.

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
#include <mcutil.h>



int32_t ltoposhrinkgray(struct xvimage *image,
                        struct xvimage *imageinhibit,
                        int32_t connex, 
                        int32_t tpmin, 
                        int32_t tpmax, 
                        int32_t tmmmin, 
                        int32_t tmmmax,
                        uint8_t(*ThinningMethod)(uint8_t*, index_t, index_t, index_t));
                       
int32_t ltoposhrinkgray3d(struct xvimage *image,
                          struct xvimage * imageinhibit,
                          int32_t connexmin,
                          int32_t tpmin, 
                          int32_t tpmax, 
                          int32_t tmmmin, 
                          int32_t tmmmax,
                          uint8_t(*ThinningMethod)(uint8_t*, index_t, index_t, index_t, index_t));
                     


int main(int argc, char** argv)
{
    struct xvimage * image;
    int32_t connex;
    struct xvimage * inhibimage = NULL;
    int32_t tpmin, tpmax, tmmmin, tmmmax;
    uint8_t(*pValAb)(uint8_t*, index_t, index_t, index_t);
    uint8_t(*pValAb3d)(uint8_t*, index_t, index_t, index_t, index_t);

    if ((argc != 9) && (argc != 10))
    {
        fprintf(stderr, "usage: %s in.pgm connex t+min t+max t--min t--max {0|a|d} [inhibit] out.pgm\n", argv[0]);
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
    
    if(strcmp(argv[7], "0") == 0)
    {
        pValAb = NULL;
        pValAb3d = NULL;
    }
    else
    {
        if (strcmp(argv[7], "a") == 0)
        {
            pValAb = alpha8m;
            pValAb3d = mctopo3d_alpha26m;
        }
        else
        {
            if (strcmp(argv[7], "d") == 0)
            {
                if ((tpmin == 1) && (tpmax == 1) && (tmmmin == 1) && (tmmmin == 1))
                {
                    if (connex == 8) pValAb = delta8m; else pValAb = delta4m;
                    if (connex == 26) pValAb3d = mctopo3d_delta26m; else pValAb3d = mctopo3d_delta6m;
                }
                else
                {
                    fprintf(stderr, "%s: method delta => t+min = t+max = t--min = t--max = 1\n", argv[0]);
                    exit(1);
                }
            }
            else
            {
                fprintf(stderr, "%s: bad option for thinning method\n", argv[0]);
                exit(1);
            }
        }
    }

    if (argc == 10) 
    {
        inhibimage = readimage(argv[8]);
        if (inhibimage == NULL)
        {
            fprintf(stderr, "%s: readimage failed\n", argv[0]);
            exit(1);
        }
    }

    if (depth(image) == 1)
    {
         if (! ltoposhrinkgray(image, inhibimage, connex, tpmin, tpmax, tmmmin, tmmmax, pValAb))
         {
             fprintf(stderr, "%s: ltoposhrink failed\n", argv[0]);
             exit(1);
        }
    }
    else
    {
        if (! ltoposhrinkgray3d(image, inhibimage, connex, tpmin, tpmax, tmmmin, tmmmax, pValAb3d))
        {
            fprintf(stderr, "%s: ltoposhrink3d failed\n", argv[0]);
            exit(1);
        }
    }

    writeimage(image, argv[argc-1]);
    freeimage(image);
    if (inhibimage) freeimage(inhibimage);

    return 0;
} /* main */




/* ==================================== */
int32_t largepdestr4(uint8_t *img,          /* pointeur base image */
                     index_t p,                       /* index du point */
                     index_t rs,                      /* taille rangee */
                     index_t N,                       /* taille de l'image */
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
                     index_t p,                       /* index du point */
                     index_t rs,                      /* taille rangee */
                     index_t N,                       /* taille image */
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
int32_t ltoposhrinkgray(struct xvimage *image, 
                        struct xvimage *imageinhibit,
                        int32_t connex, 
                        int32_t tpmin, 
                        int32_t tpmax, 
                        int32_t tmmmin, 
                        int32_t tmmmax,
                        uint8_t(*ThinningMethod)(uint8_t*, index_t, index_t, index_t))
        /* ==================================== */
#undef F_NAME
#define F_NAME "ltoposhrinkgray"
{ 
    index_t x;                       /* index muet de pixel */
    index_t rs = rowsize(image);     /* taille ligne */
    index_t cs = colsize(image);     /* taille colonne */
    index_t N = rs * cs;             /* taille image */
    uint8_t *F = UCHARDATA(image);      /* l'image de depart */
    struct xvimage *travail = copyimage(image);
    uint8_t *G = UCHARDATA(travail);/* l'image de travail */
    uint8_t *I = NULL;

    if (depth(image) != 1) 
    {
        fprintf(stderr, "%s: cette version ne traite pas les images volumiques\n", F_NAME);
        return(0);
    }

    if (imageinhibit != NULL)
    {
        if ((rowsize(imageinhibit) != rs) || (colsize(imageinhibit) != cs) || (depth(imageinhibit) != 1))
        {
            fprintf(stderr, "%s() : bad size for imageinhibit\n", F_NAME);
            return(0);
        }
        if (datatype(imageinhibit) != VFF_TYP_1_BYTE) 
        {
            fprintf(stderr, "%s() : bad type for imageinhibit\n", F_NAME);
            return(0);
        }
        I = UCHARDATA(imageinhibit);
    }    

    /* ================================================ */
    /*                  DEBUT ALGO                      */
    /* ================================================ */

    if (connex == 4)
    {
        for (x = 0; x < N; x++)
        {
            if ((!I || !I[x]) && largepdestr4(F,x,rs,N, tpmin, tpmax, tmmmin, tmmmax)) 
            {
	      if (ThinningMethod == NULL)
		F[x] = 0;
	      else
		F[x] = ThinningMethod(G,x,rs,N);
            }
        }
    }
    else
    {
        for (x = 0; x < N; x++)
        {
            if ((!I || !I[x]) && largepdestr8(F,x,rs,N, tpmin, tpmax, tmmmin, tmmmax)) 
            {
	      if (ThinningMethod == NULL)
		F[x] = 0;
	      else
		F[x] = ThinningMethod(G,x,rs,N);
            }
        }
    }

    freeimage(travail);
    return(1);
} /* ltoposhrinkgray() */

/* ==================================== */
int32_t largepdestr6(uint8_t *img,          /* pointeur base image */
                     index_t p,                       /* index du point */
                     index_t rs,                      /* taille rangee */
                     index_t ps,                      /* taille plan */
                     index_t N,                       /* taille de l'image */
                     int32_t t26pmin,
                     int32_t t26pmax,
                     int32_t t6mmmin,
                     int32_t t6mmmax)
/* ==================================== */
{
//     if ((p < ps) || (p >= N-ps) ||         /* premier ou dernier plan */
//          (p%ps < rs) || (p%ps >= ps-rs) ||  /* premiere ou derniere colonne */
//          (p%rs == 0) || (p%rs == rs-1))     /* premiere ou derniere ligne */
//         return 0;
// 
//     preparecubesh(img, p, img[p], rs, ps, N);
    return ((t26pmin <= mctopo3d_t26p(img, p, rs, ps, N)) && (mctopo3d_t26p(img, p, rs, ps, N) <= t26pmax) &&
            (t6mmmin <= mctopo3d_t6mm(img, p, rs, ps, N)) && (mctopo3d_t6mm(img, p, rs, ps, N) <= t6mmmax));
} /* largepdestr6() */

/* ==================================== */
int32_t largepdestr26(uint8_t *img,          /* pointeur base image */
                        index_t p,                       /* index du point */
                        index_t rs,                      /* taille rangee */
                        index_t ps,                      /* taille plan */
                        index_t N,                       /* taille image */
                        int32_t t6pmin,
                        int32_t t6pmax,
                        int32_t t26mmmin,
                        int32_t t26mmmax)
/* ==================================== */
{
//     if ((p < ps) || (p >= N-ps) ||         /* premier ou dernier plan */
//          (p%ps < rs) || (p%ps >= ps-rs) ||  /* premiere ou derniere colonne */
//          (p%rs == 0) || (p%rs == rs-1))     /* premiere ou derniere ligne */
//         return 0;
//     preparecubesh(img, p, img[p], rs, ps, N);
    return ((t6pmin <= mctopo3d_t6p(img, p, rs, ps, N)) && (mctopo3d_t6p(img, p, rs, ps, N) <= t6pmax) &&
            (t26mmmin <= mctopo3d_t26mm(img, p, rs, ps, N)) && (mctopo3d_t26mm(img, p, rs, ps, N) <= t26mmmax));
} /* largepdestr26() */

/* ==================================== */
int32_t ltoposhrinkgray3d(struct xvimage *image,
                          struct xvimage *imageinhibit,
                          int32_t connexmin,
                          int32_t tpmin, 
                          int32_t tpmax, 
                          int32_t tmmmin, 
                          int32_t tmmmax,
                          uint8_t(*ThinningMethod)(uint8_t*, index_t, index_t, index_t, index_t))
        /* ==================================== */
#undef F_NAME
#define F_NAME "ltoposhrinkgray3d"
{ 
    index_t x;                       /* index muet de pixel */
    index_t rs = rowsize(image);     /* taille ligne */
    index_t cs = colsize(image);     /* taille colonne */
    index_t ps = rs * cs;            /* taille plan */
    index_t ds = depth(image);       /* nombre plans */
    index_t N = ds * ps;             /* taille image */
    uint8_t *F = UCHARDATA(image);  /* l'image de depart */
    struct xvimage *travail = copyimage(image);
    uint8_t *G = UCHARDATA(travail);/* l'image de travail */
    uint8_t *I = NULL;

    mctopo3d_init_topo3d();
    
    if (imageinhibit != NULL)
    {
        if ((rowsize(imageinhibit) != rs) || (colsize(imageinhibit) != cs) || (depth(imageinhibit) != 1))
        {
            fprintf(stderr, "%s() : bad size for imageinhibit\n", F_NAME);
            return(0);
        }
        if (datatype(imageinhibit) != VFF_TYP_1_BYTE) 
        {
            fprintf(stderr, "%s() : bad type for imageinhibit\n", F_NAME);
            return(0);
        }
        I = UCHARDATA(imageinhibit);
    }

    /* ================================================ */
    /*                  DEBUT ALGO                      */
    /* ================================================ */

    if (connexmin == 6)
    {
        for (x = 0; x < N; x++)
        {
            if ((!I || !I[x]) && largepdestr6(F, x, rs, ps, N, tpmin, tpmax, tmmmin, tmmmax))
            {
	      if (ThinningMethod == NULL)
		F[x] = 0;
	      else
		F[x] = ThinningMethod(G,x,rs,ps,N);
            }
        }
    }
    else
    {
        for (x = 0; x < N; x++)
        {
            if ((!I || !I[x]) && largepdestr26(F, x, rs, ps, N, tpmin, tpmax, tmmmin, tmmmax))
            {
	      if (ThinningMethod == NULL)
		F[x] = 0;
	      else
		F[x] = ThinningMethod(G,x,rs,ps,N);
            }
        }
    }

    mctopo3d_termine_topo3d();
    freeimage(travail);
    return(1);
} /* ltoposhrinkgray3d() */
