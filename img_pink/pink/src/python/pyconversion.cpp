/*
  This software is licensed under
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but
  without any warranty to the extent permitted by applicable law.

  (C) UjoImro, 2011
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/
// The pink python wrapper core file



# include "pink_python.h"
# include "ui_convert.hpp"

# include <boost/preprocessor/slot/counter.hpp>

using namespace pink;
using boost::python::arg;
using boost::python::def;



namespace conversion
{
  

  UI_WRAP_CPP(
    "convert2float",
    pink::convert2float,
    (arg("image")),
    "converts an image to float type"
    // end of the documenation
    );
# include BOOST_PP_UPDATE_COUNTER()

// #ifdef JULIETTE
// // juliette
// UI_WRAP_RESULT(
//   int_image,
//   "short2long",
//   short2long,
//   (arg("image")),
//   "This operator converts the short images to long."
//   );
// # include BOOST_PP_UPDATE_COUNTER()

// UI_WRAP_RESULT(
//   int_image,
//   "long2short",
//   long2short,
//   (arg("image")),
//   "This operator converts the long images to short."
//   );
// # include BOOST_PP_UPDATE_COUNTER()

// #endif // JULIETTE

  UI_WRAP_CPP(
    "convert2float",
    pink::convert2float,
    (arg("image")),
    "converts an image to float type"
    // end of the documenation
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_CPP(
    "convert2long",
    pink::convert2long,
    (arg("image")),
    "converts an image to long type"
    // end of the documenation
    );
# include BOOST_PP_UPDATE_COUNTER()

} /* namespace conversion */

using namespace conversion;


void pyconversion()
{
  CALL_EXPORTED_FUNCTIONS(BOOST_PP_COUNTER);

  def(
    "float2byte",
    pink::float2byte,
    ( arg("source image"), arg("mode")=2 ),
    doc__float2byte__c__
    // end of the documenation
    );
  
  def(
    "long2byte",
    pink::long2byte,
    ( arg("image"),arg("mode"),arg("nb_new_val") ),
    doc__long2byte__c__
    // end of the documenation
    );
  
  
  def(
    "short2byte",
    pink::short2byte,
    ( arg("image"), arg("mode") ),
    "converts a short image to long type"
    );
  
  
  def(
    "long2short",
    pink::long2short,
    ( arg("image"), arg("mode") ),
    "converts a long image to short type"
    );
    
}


// LuM end of file
