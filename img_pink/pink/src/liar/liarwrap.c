/*------------------------------------------------------------------------
 *
 * minimal C wrappers for LIAR
 *
 * Hugues Talbot	 4 Jan 2001
 *      
 *-----------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "liarp.h"
#include "liarwrap.h"

char *imgtypestr_[] = {
    "IM_SINGLE",
    "IM_RGB",
    "IM_SPECTRUM",
    "IM_MULTI",
    "IM_BADIMAGE",
    "IM_UNSPEC",
    "IM_ERROR"
};

char *pixtypestr_[] = {
    "IM_BINARY",
    "IM_INT1",  
    "IM_UINT1", 
    "IM_INT2",  
    "IM_UINT2", 
    "IM_INT4",  
    "IM_UINT4", 
    "IM_INT8",  
    "IM_UINT8", 
    "IM_FLOAT", 
    "IM_DOUBLE",
};


int pixsize_[] = {
    sizeof(char),    /* IM_BINARY */
    sizeof(char),    /* IM_INT1 */  
    sizeof(char),    /* IM_UINT1 */ 
    sizeof(short),   /* IM_INT2 */  
    sizeof(short),   /* IM_UINT2 */ 
    sizeof(int),     /* IM_INT4 */  
    sizeof(int),     /* IM_UINT4 */ 
    8,		     /* IM_INT8 */  
    8,		     /* IM_UINT8 */ 
    sizeof(float),   /* IM_FLOAT */ 
    sizeof(double)   /* IM_DOUBLE */
};

static int debugIsOn = 0;

#ifdef HAVE_TIFF_LIB
IMAGE *imloadtiff(const char *path)
{
    int            start[3], end[3];
    int            pi, sf, spp, bps;
    unsigned short *colourmap[3] = {NULL, NULL, NULL};
    long           ncolours;
    void          *inbuf;
    int            result,i;
    pixtype        pt;
    imgtype        it;
    IMAGE         *outimage;
    
    result = load_tiff(path,
		       0,
		       start,
		       end,
		       &pi,
		       &sf,
		       &spp,
		       &bps,
		       colourmap,
		       &ncolours,
		       &inbuf);

    if (result != 0)
	return NULL;

    outimage = (IMAGE *)calloc(1,sizeof(IMAGE));

    imsetfirstx(outimage,0,start[0]);
    imsetfirsty(outimage,0,start[1]);
    imsetfirstz(outimage,0,start[2]);
    imsetfirstt(outimage,0, 0);

    imsetnx(outimage, 0, end[0]-start[0]+1);
    imsetny(outimage, 0, end[1]-start[1]+1);
    imsetnz(outimage, 0, end[2]-start[2]+1);
    imsetnt(outimage, 0, 1); /* one time slice */

    imsetnumcomp(outimage, spp);
    
    getTiffType(pi,sf,spp,bps,
		&pt, &it);

    imsetpixtype(outimage,0,pt);
    imsetimgtype(outimage, it);

    outimage->buff = inbuf; /* it works in this case. to be avoided */
    
    return outimage;
    
}
#endif // HAVE_TIFF_LIB

int LIARdebug(const char * msg,...)
{
    char strarg[BUFSIZE];
    int  ret = 0;
    va_list args;

    if (debugIsOn) {
        im_va_start(args,msg);

#       ifdef WIN32
	_snprintf(strarg, BUFSIZE, DEBUGPROMPT "%s\n", msg);
#       else /* NOT WIN32 */
	snprintf(strarg, BUFSIZE, DEBUGPROMPT "%s\n", msg);
#       endif /* NOT WIN32 */

        //ret = vfprintf(stderr,strarg,args);
        ret = vprintf(strarg, args);
        va_end(args);
    } 
    
    return ret;
}


void LIAREnableDebug(void)
{
    debugIsOn = 1;
}

void LIARDisableDebug(void)
{
    debugIsOn = 0;
}

int LIARerror(const char *msg, ...)
{
    char strarg[BUFSIZE];
    int  ret = 0;
    va_list args;

    im_va_start(args,msg);

#   ifdef WIN32
    _snprintf(strarg, BUFSIZE, ERRORPROMPT "%s", msg);
#   else /* NOT WIN32 */
    snprintf(strarg, BUFSIZE, ERRORPROMPT "%s", msg);
#   endif /* NOT WIN32 */

    ret = vfprintf(stderr, strarg, args);


    va_end(args);
    
}

int imexception(const char *msg, ...)
{
    char strarg[BUFSIZE];
    int  ret = 0;
    va_list args;

    im_va_start(args,msg);

#   ifdef WIN32
    _snprintf(strarg, BUFSIZE, EXCEPTPROMPT "%s", msg);
#   else /* NOT WIN32 */
    snprintf(strarg, BUFSIZE, EXCEPTPROMPT "%s", msg);
#   endif /* NOT WIN32 */

    ret = vfprintf(stderr, strarg, args);

    va_end(args);

    /* used to call exit().. a bit rough */
    return(1);
}

#ifdef HAVE_TIFF_LIB
#  include "loadtiff-code.h"
#endif // HAVE_TIFF_LIB
