/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by applicable law.
  
  (C) UjoImro, 2009-2010
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/

#include "lsym.h"
#include "lgeodesic.h"
#include "ldilateros.h"
#include "pink_python.h"
#include "ldilateros3d.h"



using namespace boost::python;
using namespace pink;

namespace pink {
  namespace python {

    template <class image_t>
    image_t opening(
      const image_t    & image, 
      const char_image & elem
      )
    {

      if (!elem.get_size().inside(elem.get_center()))
      {
        pink_error("The center of 'elem' must be specified.");
      }

      if (image.get_size().size()!=elem.get_size().size()){
        pink_error("error: the dimensions of 'image' and 'elem' must be equal");
      }

      image_t result;
      result = image.clone();
      char_image elem_ca;
      elem_ca = elem.clone();

      if (result.get_size().size()==2)
      {
        int rs = elem_ca.get_size()[0];
        int cs = elem_ca.get_size()[1];
        int x = elem_ca.get_center()[0];
        int y = elem_ca.get_center()[1];

        if (! ldilateros_leros( result.get_output(), elem_ca.get_output(), x, y))
        {
          pink_error("function leros failed");
        }              
        
        if (! lsym( elem_ca.get_output(), 'c'))
        {
          pink_error("function lsym failed");
        }
        
        if (! ldilateros_ldilat( result.get_output(), elem_ca.get_output(), rs - 1 - x, cs - 1 - y))
        {
          pink_error("function ldilat failed");
        }
      }
      else /* NOT result.get_size()==2 */ 
      {
        if (result.get_size().size()==3)
        {

          int rs = elem_ca.get_size()[0];
          int cs = elem_ca.get_size()[1];
          int ds = elem_ca.get_size()[2];
          int x = elem_ca.get_center()[0];
          int y = elem_ca.get_center()[1];
          int z = elem_ca.get_center()[2];

          if (! leros3d( result.get_output(), elem_ca.get_output(), x, y, z))
          {
            pink_error("function leros3d failed");
          }
      
          if (! lsym(elem_ca.get_output(), 'c'))
          {
            pink_error("function lsym failed");
          }
      
          if (! ldilat3d( result.get_output(), elem_ca.get_output(), rs - 1 - x, cs - 1 - y, ds - 1 - z))
          {
            pink_error("function leros3d failed");
          }

        } 
        else /* NOT result.get_size()==3 */
        {
          pink_error("pyopening: the image must be 2D or 3D");
        } /* NOT result.get_size()==3 */
    
      } /* NOT result.get_size()==2 */ 
  
  
      return result;    
    } /* opening */


    // char_image openball(
    //   const char_image & image,
    //   int radius,
    //   int mode = 0
    //   )     
    // {

    //   char_image result;
    //   result.copy(image);      
      
    //   if ((mode != 0) && (mode != 2) && (mode != 4) && 
    //       (mode != 8) && (mode != 6) && (mode != 18) && (mode != 26))
    //   {
    //     pink_error("dist = [0|2|4|8|6|18|26] ");
    //   }

    //   if (result.get_size().size()==2) // the result is 2D
    //   {
    //     if (! lerosdisc(result, radius, mode))
    //     {
    //       pink_error("function ldilatdisc failed");
    //     }
    //     if (! ldilatdisc(result, radius, mode))
    //     {
    //       pink_error("function lerosdisc failed");
    //     }
    //   }
    //   else // NOT the result is 2D
    //   {
    //     if (! lerosball(result, radius, mode))
    //     {
    //       pink_error("function ldilatball failed");
    //     }
    //     if (! ldilatball(result, radius, mode))
    //     {
    //       pink_error("function lerosball failed");
    //     }
    //   } // NOT the result is 2D

    //   return result;      
    // } /* openball */
    








  } /* namespace python */
} /* namespace pink */

UI_EXPORT_FUNCTION(
  opening,
  pink::python::opening,
  ( arg("image"), arg("elem") ),
  doc__opening__c__
  // end of the documenation
  );


// UI_EXPORT_ONE_FUNCTION(
//   openball,
//   pink::python::openball,
//   ( arg("image"), arg("radius"), arg("mode")=0 ),
//   doc__openball__c__
//   );






















// LuM end of file
