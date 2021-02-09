/* \file gencurv.c

\brief random generation of artificial curvilinear objects

<B>Usage:</B> gencurv rs cs ds nbranches npoints [seed] out

random generation of artificial curvilinear objects

Parameters: 
\li \b rs, \b cs, \b ds Image dimensions
\li \b nbranches Number of crossing branches
\li \b npoints Number of spline control points per branch
\li \b seed (optional) Seed for the random number generator - otherwise the clock is used
\li \b out output file name

<B>Types supported:</B> byte 3D

<B>Category:</B> draw geo experimental
\ingroup  draw geo experimental

\author Michel Couprie
*/

#include <stdio.h>
#include <unistd.h> 
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <time.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcsplines.h>
#include <mcprobas.h>
#include <mcchrono.h>
#include <mcgeo.h>
#include <ldraw.h>
#include <mcutil.h>
#include <larith.h>

#define VERBOSE

/* ==================================== */
static void lgencurv(
  struct xvimage* img
)
/* ==================================== */
{
#undef F_NAME
#define F_NAME "lgencurv"

  int32_t i, n = 10000;
  int32_t x, y, z;
  double t;
  int32_t rs, cs, ds, ps;
  uint8_t *I = UCHARDATA(img);
  rs=rowsize(img);
  cs=colsize(img);
  ds=depth(img);
  ps = rs*cs;

  razimage(img);

  for (i = 0; i < n; i++)
  {
    t = (i * 2 * M_PI) / n;
    x = (int32_t)floor((rs/2) * (1.0 + 0.96*cos(t)));
    y = (int32_t)floor((cs/2) * (1.0 + 0.96*sin(t)));
    z = (int32_t)floor((ds/2) * (1.0 + 0.96*cos(2*t)));
    //    printf("%g %d %d %d\n", t, x, y, z);
    I[z*ps + y*rs + x] = NDG_MAX;
  }

} // lgencurv()

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * img = NULL;
  int32_t rs, cs, ds;

  if (argc != 5)
  {
    fprintf(stderr, "usage: %s rs cs ds out\n", argv[0]);
    exit(1);
  }

  rs = atoi(argv[1]);
  cs = atoi(argv[2]);
  ds = atoi(argv[3]);

  img = allocimage(NULL, rs, cs, ds, VFF_TYP_1_BYTE); assert(img != NULL);

  lgencurv(img);

  writeimage(img, argv[argc-1]);  
  freeimage(img);
  return 0;
} /* main */
