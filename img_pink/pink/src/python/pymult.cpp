/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by applicable law.
  
  (C) UjoImro, 2011
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/

#include <boost/preprocessor.hpp>
#include "pink_python.h"



using namespace boost::python;
using namespace pink;

namespace pink {
  namespace python {

    template <class result_type, class multiplier_type>
    result_type
    py_mult(
      const result_type & image,
      const multiplier_type & mult
      )
    {
      if (image.get_size()!=mult.get_size())
      {
        pink_error("The image sizes must be equal for pixelwise multiplication.");        
      }
      
      result_type result;
      result = image.clone();
      
      FOR(q, result.get_size().prod())
      {
        result[q]*=mult[q];        
      } /* FOR */
      
      return result;      
    } /* py_fft */

  } /* namespace python */
} /* namespace pink */


// we combine the multiplication operand types in every possible way

#define NO_COMPLEX_IMAGE_TYPES (char_image, (short_image, (int_image, (float_image, (double_image, BOOST_PP_NIL)))))
#define ALL_IMAGE_TYPES (char_image, (short_image, (int_image, (float_image, (double_image, (fcomplex_image, (dcomplex_image, BOOST_PP_NIL)))))))

#define GENERATE_MULTIPLIER(r, product)                                 \
  def(                                                                  \
    "mult",                                                             \
    pink::python::py_mult< BOOST_PP_LIST_FIRST(product), BOOST_PP_LIST_REST(product) >, \
    ( arg("image (also determines the result type)"), arg("multiplier")), \
    doc__mult__c__                                                      \
    );

void mult_export()
{
  BOOST_PP_LIST_FOR_EACH_PRODUCT(GENERATE_MULTIPLIER, 2, (ALL_IMAGE_TYPES, NO_COMPLEX_IMAGE_TYPES) );
  GENERATE_MULTIPLIER(_, (fcomplex_image, fcomplex_image) );
  GENERATE_MULTIPLIER(_, (dcomplex_image, dcomplex_image) );
} /* export_mult */




// cleaning up after us
#undef GENERATE_MULTIPLIER
#undef GENERATE_RESULT_TYPE
#undef NO_COMPLEX_IMAGE_TYPES
#undef ALL_IMAGE_TYPES


























// LuM end of file
