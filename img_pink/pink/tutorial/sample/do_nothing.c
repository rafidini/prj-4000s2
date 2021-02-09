
/*! \file powerwatFiltering.c
\defgroup aniso Anisotropic diffusion

\brief Implements anisotropic diffusion 

<B>Usage:</B> ./ perona_malik.exe image_name.pgm out.pgm

<B>Description:</B>
<B>Types supported:</B> byte 2d, int32_t 2d

\ingroup aniso

\author Camille Couprie
*/

// C standard
#include <stdio.h>
#include <sys/types.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <assert.h>
#include <stdint.h>

// Pink-specific
#include <mcutil.h>
#include <mccodimage.h>
#include <mcimage.h>

// Include your own there
#include "do_nothing.h"

/*
 * explain here what your program does
 *
 */

static int usage(char *prgmname)
{
    fprintf(stderr,
            "%s: does nothing much\n"
            "    reads an image and writes the same\n", prgmname);
    
    return 0;
}

/* =============================================================== */
int main(int argc, char **argv) 
/* =============================================================== */
{

    int t1=clock();

    if (argc < 2)
    {
        usage(argv[0]); // argv[0] is the name of the executable
        exit(1);
    }
 
    int i;
    struct xvimage * image_r;
 
    int rs, cs, N;  
   
    // reads the image in argv[1]

    image_r = readimage(argv[1]);
 
    rs = rowsize(image_r);
    cs = colsize(image_r);

    // read some 8-bit image data
    uint8_t *img = UCHARDATA(image_r);

    N = rs * cs;
 
    double  * x = (double  *)malloc(N*sizeof(double));

    // read the data in
    
    for(i=0;i<N;i++) 
        x[i]=img[i];

    // pointwise (and pointless) manipulation of input data
    for(i=0;i<N;i++)
        img[i]=255.0 * exp((255.0-x[i])/255);

    writeimage(image_r, argv[2]);

    int t2=clock();
   
    printf("Total time : %.6lf seconds elapsed\n", ((double)t2-t1)/CLOCKS_PER_SEC);
    

    freeimage(image_r);
  
  
    return 0;
} 


