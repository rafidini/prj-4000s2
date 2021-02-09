/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by applicable law.
  
  (C) UjoImro, 2010
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/



#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

#include "pink_python.h"

#include "lskeletons.h"



using namespace boost::python;
using namespace pink;

// you should not define N as a macro
#undef N


namespace pink {
  namespace python {

    // ERROR this function changes the parameters
    // If somebody uses it again it'll have to be corrected
    int_image priority_image(
      char_image image,
      int priovalue
      )
    {
      
      int32_t i, N;
      uint8_t *F;
      xvimage * prio;        
      prio = allocimage(NULL, rowsize(image.get_output()), colsize(image.get_output()), depth(image.get_output()), VFF_TYP_4_BYTE);
      if (prio == NULL)
      {   
        pink_error("allocimage failed");
      }
      N = rowsize(image.get_output()) * colsize(image.get_output()) * depth(image.get_output());
      F = UCHARDATA(image.get_output());
      for (i = 0; i < N; i++) // inverse l'image
        if (F[i]) F[i] = 0; else F[i] = NDG_MAX;
  
      if (priovalue == 0)
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
        if (priovalue == 1)
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
          if (priovalue == 2)
          {
            if (! lchamfrein(image.get_output(), prio))
            {
              pink_error("lchamfrein failed");
            }
          }
          else
            if (priovalue == 3)
            {
              if (! lsedt_meijster(image.get_output(), prio))
              {
                pink_error("lsedt_meijster failed");
              }
            }
            else
            {
              if (! ldist(image.get_output(), priovalue, prio))
              {
                pink_error("ldist failed");
              }
            }
      for (i = 0; i < N; i++) // re-inverse l'image
        if (F[i]) F[i] = 0; else F[i] = NDG_MAX;
        
      
      int_image result(prio);
      free(prio); // NOT freeimage!!!
      
      return result;      
    } /* priority_image */
    
    template <class priority_image_type>    
    char_image general_skelcurv(
      const char_image & image,
      priority_image_type prio,
      int connex,
      char_image inhibit
      )
    {
      char_image result;
      result = image.clone();

      if (image.get_size().size()==2)
      {
        if (! lskelcurv(result, prio, can_be_null(inhibit), connex))
        {
          pink_error("lskelcurv failed");
        }        
      }
      else /* NOT size==2 */
      {
        if (image.get_size().size()==3)
        {
          if (! lskelcurv3d(result, prio, can_be_null(inhibit), connex))
          {
            pink_error("lskelcurv3d failed");
          }
        }
        else /* NOT size==3 */
        {
          pink_error("only 2D and 3D images are supported");
        } /* NOT size==3 */
        
      } /* NOT size==2 */

      return result;
    } /* general_skelcurv */
    
    
    template <class priority_image_type>
    char_image skelcurv(
      const char_image & image, 
      priority_image_type & prio,
      int connex,
      char_image inhibit
      )
    {
      char_image result;
      result = image.clone();      

      result=general_skelcurv(image, prio, connex, inhibit);

      return result;      
    } /* template skelcurv */

    char_image skelcurv2(
      const char_image & image, 
      int priovalue,
      int connex,
      char_image inhibit
      )
    {
      char_image result;
      result = image.clone();      

      int_image prio;
      prio = priority_image(result, priovalue);
      
      result = general_skelcurv(image, prio, connex, inhibit);

      return result;      
    } /* NO TEMPLATE skelcurv */


    template <class priority_image_type>
    char_image skelcurv_short(
      const char_image & image, 
      priority_image_type & prio,
      int connex
      )
    {
      return skelcurv(image, prio, connex, char_image());
    } /* template skelcurv */

    char_image skelcurv2_short(
      const char_image & image, 
      int priovalue,
      int connex
      )
    {
      return skelcurv2(image, priovalue, connex, char_image());
    }
    

  } /* namespace python */
} /* namespace pink */


void skelcurv_export()
{

  UI_DEFINE_FUNCTION(
  "skelcurv",
  pink::python::skelcurv,
  ( arg("image"),  arg("priority"), arg("connex"), arg("inhibit") ),
  doc__skelcurv__c__
  );
  
  def("skelcurv",
      &pink::python::skelcurv2,
      (arg("image"), arg("priority"),arg("connex"), arg("inhibit") ),
      doc__skelcurv__c__
    );

  UI_DEFINE_FUNCTION(
  "skelcurv",
  pink::python::skelcurv_short,
  ( arg("image"),  arg("priority"), arg("connex") ),
  doc__skelcurv__c__
  );
  
  def("skelcurv",
      &pink::python::skelcurv2_short,
      (arg("image"), arg("priority"),arg("connex") ),
      doc__skelcurv__c__
    );

  
} /* skelcurv_export */

















































// LuM end of file
