/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by applicable law.
  
  (C) UjoImro, 2009-2010
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/

#include "lzoom.h"
#include "pink_python.h"


using namespace boost::python;
using namespace pink;


// note this function is too rigid (you can't choose the zoom algorithm)
// and should definitely be replaced later

namespace pink {
  namespace python {

    template <class image_type>
    image_type zoom(
      const image_type & image,
      double fx, double fy, double fz // the zoom factors 		
      )
    {
      image_type copy = image.clone();
  
      xvimage * out;

      lzoom( copy.get_output() ,
             &out,
             fx, fy, fz 
        );
    
      image_type result(out);

      free(out); // NOT freeimage !!!!
 
      return result;
    } /* zoom */

  } /* namespace python */
} /* namespace pink */

UI_EXPORT_FUNCTION(
  zoom,
  pink::python::zoom,
  ( arg("src"),
    arg("zoom factor X"),
    arg("zoom factor Y"),
    arg("zoom factor Z" ) ),
  "Zoom (shrink or engarle) an image. "
  "The three arguments are the zoom factors. "
  "More advanced parametrization through the 'pink.zoom' function."
  "Note: this function is too rigid (you can't choose the zoom algorithm) "
  " and should definitely be replaced later"
  );



















































// LuM end of file
