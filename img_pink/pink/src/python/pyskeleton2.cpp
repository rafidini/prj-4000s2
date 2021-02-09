/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by applicable law.
  
  (C) UjoImro, 2009-2010
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>

#include "ldist.h"
#include "lsquelbin.h"
#include "lskeletons.h"
#include "pink_python.h"

// based on skeleton.c

// ERROR N is not supposed to be used as a define macro
#undef N

namespace pink {
  namespace python {

    // the content of this functon is copycat from
    // skeleton.c
    pink::int_image skeleton_distance(
      pink::char_image & image,
      int priocode
      )
    {
      pink::int_image prio(image.get_size());

      int32_t i, N;
      uint8_t *F;
      N = rowsize(image.get_output()) * colsize(image.get_output()) * depth(image.get_output());
      F = UCHARDATA(image.get_output());
      for (i = 0; i < N; i++) // inverse l'image
        if (F[i]) F[i] = 0; else F[i] = NDG_MAX;
  
      if (priocode == 0)
      {
        if (depth(image.get_output()) == 1)
        {
          if (! ldisteuc(image.get_output(), prio))
          {
            pink_error("ldisteuc failed");
          }
        }
        else
        {
          if (! ldisteuc3d(image.get_output(), prio))
          {
            pink_error("ldisteuc3d failed");
          }
        }
      }
      else
        if (priocode == 1)
        {
          if (depth(image.get_output()) == 1)
          {
            if (! ldistquad(image.get_output(), prio))
            {
              pink_error("ldistquad failed");
            }
          }
          else
          {
            if (! ldistquad3d(image.get_output(), prio))
            {
              pink_error("ldistquad3d failed");
            }
          }
        }
        else
          if (priocode == 2)
          {
            if (! lchamfrein(image.get_output(), prio))
            {
              pink_error("lchamfrein failed");
            }
          }
          else
            if (priocode == 3)
            {
              if (! lsedt_meijster(image.get_output(), prio))
              {
                pink_error("lsedt_meijster failed");
              }
            }
            else
            {
              if (! ldist(image.get_output(), priocode, prio))
              {
                pink_error("ldist failed");
              }
            }
      for (i = 0; i < N; i++) // re-inverse l'image
        if (F[i]) F[i] = 0; else F[i] = NDG_MAX;
      
      return prio;
    } /* skeleton_distance */


    
    pink::char_image
    skeleton_im_prioint_connex_inhibimage(
      const pink::char_image & image,
      int priocode,
      int connex,
      /*CAN_BE_NULL*/ pink::char_image inhibimage
      )
    {
      pink::char_image result;
      result = image.clone();      

      pink::int_image prio;
      prio = skeleton_distance(result, priocode);

      if (depth(result.get_output()) == 1) // the image is 2D
      {
        if (! lskelubp2(result.get_output(), prio, connex, can_be_null(inhibimage))) { pink_error("lskelubp2 failed"); }
      }
      else // the image is 3D
      {
        if (! lskelubp3d2(result.get_output(), prio, connex, can_be_null(inhibimage))) { pink_error("lskelubp3d2 failed"); }
      } // the image is 3D

      return result;
    } /* skeleton_im_prioint_connex_inhibimage */



    template <class image_type>
    pink::char_image
    skeleton_im_prioim_connex_inhibimage (
      const pink::char_image & image,
      image_type prioimage,
      int connex,
      pink::char_image inhibimage
      )
    {
      pink::char_image result;
      result = image.clone();
      
      if (depth(result.get_output()) == 1) // the image is 2D
      {
        if (! lskelubp2(result, prioimage, connex, can_be_null(inhibimage))) { pink_error("lskelubp2 failed"); }
      }
      else // the image is 3D
      {
        if (! lskelubp3d2(result, prioimage, connex, can_be_null(inhibimage))) { pink_error("lskelubp3d2 failed"); }
      } // the image is 3D

      return result;
    } /* skeleton_im_prioim_connex_inhibimage */



    template <class image_type>
    pink::char_image
    skeleton_im_prioim_connex_inhibval (
					const pink::char_image & image,
					image_type prioimage,
					int connex,
					int inhibval=-1
					)
    {
      if (inhibval==-1)
	{
	  pink::char_image result;        
	  result = skeleton_im_prioim_connex_inhibimage(image, prioimage, connex, pink::char_image());
	  return result;        
	} /* inhibval == -1 */

      pink::char_image result;
      result = image.clone();
      
      if (depth(result.get_output()) == 1)  // the image is 2D
	{
	  if (! lskelubp(result.get_output(), prioimage, connex, inhibval))
	    {
	      pink_error("lskelubp failed");
	    }
	}
      else  // the image is 3D
	{
	  if (! lskelubp3d(result.get_output(), prioimage, connex, inhibval))
	    {
	      pink_error("lskelubp3d failed");
	    }
	}  // the image is 3D

      return result;
    } /* skeleton_im_prioim_connex_inhibimage */

    pink::char_image
    skeleton_im_prioint_connex_inhibval(
      const pink::char_image & image,
      int priocode,
      int connex,
      /*CAN_BE_NULL*/ int inhibval = -1
      )
    {
      pink::char_image result;
      result = image.clone();

      pink::int_image prio;
      prio = skeleton_distance(result, priocode);

      result = skeleton_im_prioim_connex_inhibval(image, prio, connex, inhibval);      
      
      return result;
    } /* skeleton_im_prioint_connex_inhibimage */



    pink::char_image skeleton_end_char(
      const pink::char_image & input_image, 
      int connex,
      int seuil			     
      ) 
    {

      pink::char_image res;
      ::xvimage *xvinput;
      res = input_image.clone();
      
      xvinput = res.get_output();
      
      if (depth(xvinput) == 1) {
	if (! lsquelbin(xvinput, connex, seuil)) {
	  pink_error("lsquelbin failed");
	  return res;
	}
	  
      } else {
	uint8_t *endpoint;
	char tablefilename[128];
	int32_t tablesize, ret;
	FILE *fd;

	tablesize = 1<<24;
	endpoint = (uint8_t *)malloc(tablesize);
	if (! endpoint)
	  {
	    pink_error("skelend: malloc failed");
	    return res;
	  }


	sprintf(tablefilename, "%s/src/tables/TabEndPoints.txt", getenv("PINK"));
	fd = fopen (tablefilename, "r");
	if (fd == NULL) 
	  {   
	    pink_error("skelend: error while opening table\n");
	    return res;
	  }
	ret = fread(endpoint, sizeof(char), tablesize, fd);
	if (ret != tablesize)
	  {
	    pink_error("fread failed");
	    return res;
	  }
	fclose(fd);
	if (! lskelend3d(xvinput, connex, endpoint, seuil))
	{
	  pink_error("lskelend3d failed");
	  return res;
	}
      	free(endpoint);

  
	return res;

      }
    }

  } /* namespace python */
} /* namespace pink */




void skeleton2_export()
{
  boost::python::def( "skeleton",
       &pink::python::skeleton_im_prioint_connex_inhibimage,
       ( boost::python::arg("image"), boost::python::arg("prio"), boost::python::arg("connexity"), boost::python::arg("inhibit") ),
       doc__skeleton__c__
    );

  boost::python::def( "skeleton",
       &pink::python::skeleton_im_prioint_connex_inhibval,
       ( boost::python::arg("image"), boost::python::arg("prio"), boost::python::arg("connexity"), boost::python::arg("inhibit")=-1 ),
       doc__skeleton__c__
    );
  
  UI_DEFINE_FUNCTION(
    "skeleton",
    pink::python::skeleton_im_prioim_connex_inhibimage,
    ( boost::python::arg("image"), boost::python::arg("prio"), boost::python::arg("connexity"), boost::python::arg("inhibit") ),
    doc__skeleton__c__
    );

  UI_DEFINE_FUNCTION(
    "skeleton",
    pink::python::skeleton_im_prioim_connex_inhibval,
    ( boost::python::arg("image"), boost::python::arg("prio"), boost::python::arg("connexity"), boost::python::arg("inhibit")=-1 ),
    doc__skeleton__c__
    );

  boost::python::def( "skeleton_end_char", &pink::python::skeleton_end_char,
       boost::python::args("image", "connexity", "threshold"),
       "Description: \n"
       "Homotopic skeletonization by iterative removal of simple,\n"
       "non-end points. Breadth-first strategy.\n"
       "During the first  n iterations (default 0), the end points\n"
       "are removed as well.\n"
       "If  n = -1, the end points are always removed.\n"
       "\n"
       "Types supported: byte 2d, byte 3d"
       );
  
} /* skeleton_export */









































// LuM end of file
