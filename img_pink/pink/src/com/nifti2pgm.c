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
/*! \file nifti2pgm.c

\brief converts from nifti format into pgm format

<B>Usage:</B> nifti2pgm in.nii out.pgm

<B>Description:</B> Converts from nifti format into pgm format. 

Parameters:
\li \b in.nii : source file in nifti format

<B>Types supported:</B> byte 2D-3D, int16_t 2D-3D, int32_t 2D-3D, float 2D-3D

\warning NOT ALL NIFTI FILES ARE SUPPORTED

<B>Category:</B> convert
\ingroup convert

\author Michel Couprie
*/

/* Michel Couprie - janvier 2000 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <assert.h>
#include <mcimage.h>
#include <mccodimage.h>
#include <mcutil.h>

/*! \struct nifti_1_header
    \brief Data structure defining the fields in the nifti1 header.
           This binary header should be found at the beginning of a valid
           NIFTI-1 header file.
 */
                        /*************************/  /************************/
struct nifti_1_header { /* NIFTI-1 usage         */  /* ANALYZE 7.5 field(s) */
                        /*************************/  /************************/

                                           /*--- was header_key substruct ---*/
 int   sizeof_hdr;    /*!< MUST be 348           */  /* int sizeof_hdr;      */
 char  data_type[10]; /*!< ++UNUSED++            */  /* char data_type[10];  */
 char  db_name[18];   /*!< ++UNUSED++            */  /* char db_name[18];    */
 int   extents;       /*!< ++UNUSED++            */  /* int extents;         */
 short session_error; /*!< ++UNUSED++            */  /* short session_error; */
 char  regular;       /*!< ++UNUSED++            */  /* char regular;        */
 char  dim_info;      /*!< MRI slice ordering.   */  /* char hkey_un0;       */

                                      /*--- was image_dimension substruct ---*/
 short dim[8];        /*!< Data array dimensions.*/  /* short dim[8];        */
 float intent_p1 ;    /*!< 1st intent parameter. */  /* short unused8;       */
                                                     /* short unused9;       */
 float intent_p2 ;    /*!< 2nd intent parameter. */  /* short unused10;      */
                                                     /* short unused11;      */
 float intent_p3 ;    /*!< 3rd intent parameter. */  /* short unused12;      */
                                                     /* short unused13;      */
 short intent_code ;  /*!< NIFTI_INTENT_* code.  */  /* short unused14;      */
 short datatype;      /*!< Defines data type!    */  /* short datatype;      */
 short bitpix;        /*!< Number bits/voxel.    */  /* short bitpix;        */
 short slice_start;   /*!< First slice index.    */  /* short dim_un0;       */
 float pixdim[8];     /*!< Grid spacings.        */  /* float pixdim[8];     */
 float vox_offset;    /*!< Offset into .nii file */  /* float vox_offset;    */
 float scl_slope ;    /*!< Data scaling: slope.  */  /* float funused1;      */
 float scl_inter ;    /*!< Data scaling: offset. */  /* float funused2;      */
 short slice_end;     /*!< Last slice index.     */  /* float funused3;      */
 char  slice_code ;   /*!< Slice timing order.   */
 char  xyzt_units ;   /*!< Units of pixdim[1..4] */
 float cal_max;       /*!< Max display intensity */  /* float cal_max;       */
 float cal_min;       /*!< Min display intensity */  /* float cal_min;       */
 float slice_duration;/*!< Time for 1 slice.     */  /* float compressed;    */
 float toffset;       /*!< Time axis shift.      */  /* float verified;      */
 int   glmax;         /*!< ++UNUSED++            */  /* int glmax;           */
 int   glmin;         /*!< ++UNUSED++            */  /* int glmin;           */

                                         /*--- was data_history substruct ---*/
 char  descrip[80];   /*!< any text you like.    */  /* char descrip[80];    */
 char  aux_file[24];  /*!< auxiliary filename.   */  /* char aux_file[24];   */

 short qform_code ;   /*!< NIFTI_XFORM_* code.   */  /*-- all ANALYZE 7.5 ---*/
 short sform_code ;   /*!< NIFTI_XFORM_* code.   */  /*   fields below here  */
                                                     /*   are replaced       */
 float quatern_b ;    /*!< Quaternion b param.   */
 float quatern_c ;    /*!< Quaternion c param.   */
 float quatern_d ;    /*!< Quaternion d param.   */
 float qoffset_x ;    /*!< Quaternion x shift.   */
 float qoffset_y ;    /*!< Quaternion y shift.   */
 float qoffset_z ;    /*!< Quaternion z shift.   */

 float srow_x[4] ;    /*!< 1st row affine transform.   */
 float srow_y[4] ;    /*!< 2nd row affine transform.   */
 float srow_z[4] ;    /*!< 3rd row affine transform.   */

 char intent_name[16];/*!< 'name' or meaning of data.  */

 char magic[4] ;      /*!< MUST be "ni1\0" or "n+1\0". */

} ;                   /**** 348 bytes total ****/

#define VERBOSE

static void print_nifti_header(struct nifti_1_header NH)
{
  int i;
  printf("sizeof_hdr = %d /*!< MUST be 348           */  /* int sizeof_hdr;      */   /*   0 */\n", NH.sizeof_hdr);    
  printf("dim_info = %d /*!< MRI slice ordering.   */  /* char hkey_un0;       */   /*  39 */\n", NH.dim_info);      
  for (i = 0; i < 8; i++)
    printf("dim[%d] = %d /*!< Data array dimensions.*/  /* short dim[8];        */   /*  40 */\n", i, NH.dim[i]);
  printf("intent_p1 = %g /*!< 1st intent parameter. */  /* short unused8;       */   /*  56 */\n", NH.intent_p1);     
  printf("intent_p2 = %g /*!< 2nd intent parameter. */  /* short unused10;      */   /*  60 */\n", NH.intent_p2);     
  printf("intent_p3 = %g /*!< 3rd intent parameter. */  /* short unused12;      */   /*  64 */\n", NH.intent_p3);     
  printf("intent_code = %d /*!< NIFTIINTENT code.     */  /* short unused14;      */   /*  68 */\n", NH.intent_code);   
  printf("datatype = %d /*!< Defines data type!    */  /* short datatype;      */   /*  70 */\n", NH.datatype);      
  printf("bitpix = %d /*!< Number bits/voxel.    */  /* short bitpix;        */   /*  72 */\n", NH.bitpix);        
  printf("slice_start = %d /*!< First slice index.    */  /* short dim_un0;       */   /*  74 */\n", NH.slice_start);   
  for (i = 0; i < 8; i++)
    printf("pixdim[%i] = %g /*!< Grid spacings.        */  /* float pixdim[8];     */   /*  76 */\n", i, NH.pixdim[i]);     
  printf("vox_offset = %g /*!< Offset into .nii file */  /* float vox_offset;    */   /* 108 */\n", NH.vox_offset);    
  printf("scl_slope = %g /*!< Data scaling: slope.  */  /* float funused1;      */   /* 112 */\n", NH.scl_slope);     
  printf("scl_inter = %g /*!< Data scaling: offset. */  /* float funused2;      */   /* 116 */\n", NH.scl_inter);     
  printf("slice_end = %d /*!< Last slice index.     */  /* float funused3;      */   /* 120 */\n", NH.slice_end);     
  printf("slice_code = %d /*!< Slice timing order.   */                               /* 122 */\n", NH.slice_code);    
  printf("xyzt_units = %d /*!< Units of pixdim[1..4] */                               /* 123 */\n", NH.xyzt_units);    
  printf("cal_max = %g /*!< Max display intensity */  /* float cal_max;       */   /* 124 */\n", NH.cal_max);       
  printf("cal_min = %g /*!< Min display intensity */  /* float cal_min;       */   /* 128 */\n", NH.cal_min);       
  printf("slice_duration = %g /*!< Time for 1 slice.     */  /* float compressed;    */   /* 132 */\n", NH.slice_duration);
  printf("toffset = %g /*!< Time axis shift.      */  /* float verified;      */   /* 136 */\n", NH.toffset);       
  printf("descrip[80] = %s /*!< any text you like.    */  /* char descrip[80];    */   /* 148 */\n", (char *)NH.descrip);   
  printf("aux_file[24] = %s /*!< auxiliary filename.   */  /* char aux_file[24];   */   /* 228 */\n", (char *)NH.aux_file);  
  printf("qform_code = %d /*!< NIFTIXFORM code.      */  /*-- all ANALYZE 7.5 ---*/   /* 252 */\n", NH.qform_code);    
  printf("sform_code = %d /*!< NIFTIXFORM code.      */  /*   fields below here  */   /* 254 */\n", NH.sform_code);    
  printf("intent_name[16] = %s /*!< name or meaning of data.  */                         /* 328 */\n", (char *)NH.intent_name);
  printf("magic[4] = %s /*!< MUST be \"ni1\\0\" or \"n+1\\0\" . */                         /* 344 */\n", (char *)NH.magic);
}

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  FILE *fd = NULL;
  index_t rs, cs, ds, N, headersize, i;
  int32_t littleendian, datatype;
  struct xvimage * image;
  struct nifti_1_header NIFTI_HEADER;

  if (argc != 3)
  {
    fprintf(stderr, "usage: %s in.nii out.pgm \n", argv[0]);
    exit(1);
  }

#ifdef UNIXIO
  fd = fopen(argv[1],"r");
#endif
#ifdef DOSIO
  fd = fopen(argv[1],"rb");
#endif

  if (fd == NULL)
  {
    fprintf(stderr, "%s: fopen failed for %s\n", argv[0], argv[1]);
    exit(1);
  }

  fread(&NIFTI_HEADER, sizeof(struct nifti_1_header), 1, fd);

#ifdef VERBOSE
  print_nifti_header(NIFTI_HEADER);
#endif

  if (NIFTI_HEADER.sizeof_hdr != 348)
  {
    fprintf(stderr, "%s: Invalid NIFTI file", argv[0]);
    exit(1);
  }

  if (NIFTI_HEADER.dim[0] > 3)
  {
    fprintf(stderr, "%s: Dimension > 3, NYI", argv[0]);
    exit(1);
  }

  rs = NIFTI_HEADER.dim[1];
  cs = NIFTI_HEADER.dim[2];
  if (NIFTI_HEADER.dim[0] == 3)
    ds = NIFTI_HEADER.dim[3];
  else
    ds = 1;
  N = rs * cs * ds;
  headersize = NIFTI_HEADER.vox_offset;
  datatype = 1; /* TO BE EXTENDED */
  littleendian = 0;

  if ((datatype != 1) && (datatype != 2) && (datatype != 4) && (datatype != 5))
  {
    fprintf(stderr, "%s: bad value for pix size: %d\n", argv[0], datatype);
    exit(1);
  }

  if (datatype == 1)
  {
    image = allocimage(NULL, rs, cs, ds, VFF_TYP_1_BYTE);
    if (image == NULL)
    {   fprintf(stderr,"%s : allocimage failed\n", argv[0]);
        exit(1);
    }
    fseek(fd, headersize, SEEK_CUR);
    fread(UCHARDATA(image), sizeof(char), N, fd);
  }
  else if (datatype == 2)
  {
    int32_t * I;
    uint16_t tmp;
    uint8_t tmp1;
    image = allocimage(NULL, rs, cs, ds, VFF_TYP_4_BYTE);
    if (image == NULL)
    {   fprintf(stderr,"%s : allocimage failed\n", argv[0]);
        exit(1);
    }
    fseek(fd, headersize, SEEK_CUR);
    I = SLONGDATA(image);
    for (i = 0; i < N; i++) 
    { 
      fread(&tmp, 2 * sizeof(char), 1, fd);
      if (littleendian)
      {
        tmp1 = tmp & 0x00ff;
        tmp = tmp >> 8;
        tmp = tmp | (((uint32_t)tmp1) << 8);      
      }
      I[i] = (int32_t)tmp; 
    }
  }
  else if (datatype == 4)
  {
    image = allocimage(NULL, rs, cs, ds, VFF_TYP_4_BYTE);
    if (image == NULL)
    {   fprintf(stderr,"%s : allocimage failed\n", argv[0]);
        exit(1);
    }
    fseek(fd, headersize, SEEK_CUR);
    if (littleendian)
    {
      int32_t * I;
      uint32_t tmp;
      uint8_t tmp1, tmp2, tmp3;
      I = SLONGDATA(image);
      for (i = 0; i < N; i++) 
      { 
	fread(&tmp, 4 * sizeof(char), 1, fd);
        tmp1 = tmp & 0xff; tmp = tmp >> 8;
        tmp2 = tmp & 0xff; tmp = tmp >> 8;
        tmp3 = tmp & 0xff; tmp = tmp >> 8;
        tmp = tmp | (((uint32_t)tmp3) << 8);
        tmp = tmp | (((uint32_t)tmp2) << 16);
        tmp = tmp | (((uint32_t)tmp1) << 24);      
      }
      I[i] = (int32_t)tmp; 
    }
    else
    {
      fread(SLONGDATA(image), sizeof(float), N, fd);
    }
  }
  else if (datatype == 5)
  {
    image = allocimage(NULL, rs, cs, ds, VFF_TYP_FLOAT);
    if (image == NULL)
    {   fprintf(stderr,"%s : allocimage failed\n", argv[0]);
        exit(1);
    }
    fseek(fd, headersize, SEEK_CUR);
    if (littleendian)
    {
      float *I;
      uint32_t tmp;
      uint8_t tmp1, tmp2, tmp3;
      I = FLOATDATA(image);
      assert(sizeof(uint32_t) == sizeof(float));
      for (i = 0; i < N; i++) 
      { 
	fread(&tmp, sizeof(uint32_t), 1, fd);
        tmp1 = tmp & 0xff; tmp = tmp >> 8;
        tmp2 = tmp & 0xff; tmp = tmp >> 8;
        tmp3 = tmp & 0xff; tmp = tmp >> 8;
        tmp = tmp | (((uint32_t)tmp3) << 8);
        tmp = tmp | (((uint32_t)tmp2) << 16);
        tmp = tmp | (((uint32_t)tmp1) << 24);      
        I[i] = *((float *)(&tmp)); 
      }
    }
    else
    {
      fread(FLOATDATA(image), sizeof(float), N, fd);
    }
  }

  image->xdim = (double)NIFTI_HEADER.pixdim[0];
  image->ydim = (double)NIFTI_HEADER.pixdim[1];
  image->zdim = (double)NIFTI_HEADER.pixdim[2];

  writeimage(image, argv[argc - 1]);
  freeimage(image);
  fclose(fd);

  return 0;
} /* main */


