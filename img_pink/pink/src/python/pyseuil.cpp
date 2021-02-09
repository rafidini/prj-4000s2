/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by applicable law.
  
  (C) UjoImro, 2010
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/

// boost python wrapper

#include <pink_python.h>


using namespace boost::python;
using namespace pink;

namespace pink {
  namespace python {

    template <class image_type>
    image_type seuil( 
      const image_type & image,  
      typename image_type::pixel_type threshold,
      typename image_type::pixel_type val_min,
      typename image_type::pixel_type val_max
      )
    {
  
      image_type result(image.get_size());
  
      FOR(q, image.get_size().prod())
      {
        if (image[q]>=threshold)
        {
          result[q]=val_max;
        }
        else /* NOT image[q]>=threshold */
        {
          result[q]=val_min;
        } /* NOT image[q]>=threshold */
      } /* FOR */

      return result;

    } /* seuil */
  } /* namespace python */
} /* namespace pink */


UI_EXPORT_FUNCTION( seuil, 
		    pink::python::seuil, 
		    ( arg("image"), arg("threshold value"), arg("lower value")=0, arg("upper_value")=255),
		    "this function takes an image, and replaces the values under 'threshold' with"
		    " minval, and the values over or equal with the 'threshold' with 'max_val'"
  );










// LuM end of file
