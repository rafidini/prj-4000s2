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

#include "lfft.h"
#include "larith.h"
#include "pink_python.h"
#include "ui_any_to_complex.hpp"

#include <boost/preprocessor/slot/counter.hpp>

// ***********************************************************************
// ***********************************************************************
// ***********************************************************************
// ************** FUNCTIONS DEALING WITH COMPLEX IMAGES ******************
// ***********************************************************************
// ***********************************************************************
// ***********************************************************************

using namespace pink;
using namespace boost::python;

namespace complex
{
  
  UI_WRAP_CPP(
    "any2complex",
    pink::lany2complex,
    (arg("real part"), arg("imaginary part")),
    doc__ui_any_to_complex__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
    "fft",
    lfft,
    (arg("image"), arg("direction")),
    doc__fft__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_RESULT(
    float_image,
    "complex_modulus",
    lmodulus,
    (arg("image")),
    doc__complex_modulus__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_RESULT(
    float_image,
    "modulus",
    lmodulus,
    (arg("image")),
    "WRITE ME!!!"
    );
#include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_RESULT(
    float_image,
    "real",
    lreal,
    (arg("image")),
    "WRITE ME!!!"
    );
#include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_RESULT(
    float_image,
    "imaginary",
    limaginary,
    (arg("image")),
    "WRITE ME!!!"
    );
#include BOOST_PP_UPDATE_COUNTER()


} /* namespace complex */

using namespace complex;

void pycomplex()
{
  CALL_EXPORTED_FUNCTIONS(BOOST_PP_COUNTER);
}


