/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by applicable law.
  
  (C) UjoImro, 2009-2010
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/

#include "ldilateros.h"
#include "pink_python.h"
#include "ldilateros3d.h"

using namespace boost::python;
using namespace pink;

namespace pink {
  namespace python {


    template <class image_t>
    image_t erosion(
      const image_t & src, 
      char_image elem
      )
    {

      if (! elem.get_size().inside(elem.get_center()) )
      {
        pink_error("The center of 'elem' must be specified.");
      } /* if center not inside*/

      image_t result;
      result = src.clone();


      if ( src.get_size().size()==2) // the image is 2D
      {        
        if (! ldilateros_leros( result.get_output(), elem.get_output(),
                                elem.get_center()[0], elem.get_center()[1]))
        {
          pink_error("function ldilateros_leros failed");
        } /* (! ldilateros_leros( src, elem_const_away, x, y)) */
      }
      else  // NOT the image is 2D
      {
        if (! leros3d( result.get_output(), elem.get_output(),
                       elem.get_center()[0], elem.get_center()[1], elem.get_center()[2]))          
        {
          pink_error("function leros3d failed");
        } /* (! leros3d( src, elem_const_away, x, y)) */        
      } // NOT the image is 2D
      
      return result;    
    } /* erosion */

  } /* namespace python */
} /* namespace pink */

    
UI_EXPORT_FUNCTION(
  erosion,
  pink::python::erosion,
  (arg("src"), arg("elem")),
  doc__erosion__c__
  // end of the documenation
  );




















































// LuM end of file
