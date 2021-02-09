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

// #ifndef PINK_DEVELOPMENT
// #  include <pink_python.h>
// #else /* PINK_DEVELOPMENT */
// #  include <pink_development.h>
// #endif /* PINK_DEVELOPMENT */

#include "ujimage.hpp"
#include "pink_python.h"
#include "ui_genball.hpp"
#include "uiNormalize.hpp"

using namespace boost::python;
using namespace pink;



void pynormalization()
{
  //CALL_EXPORTED_FUNCTIONS(BOOST_PP_COUNTER);
  def(
    "genball",
    pink::genball,
    ( arg("radius"), arg("dimension")=2 ),
    "Generates a discrete Euclidean ball with given radius and dimensions."
    );

  def(
    "normalize",
    &pink::normalize<char_image, 0, 255>,
    ( arg("image"), arg("minval")=0, arg("maxval")=255),
    doc__normalize__c__
    );

  def(
    "normalize",
    &pink::normalize<int_image, 0, 255>,
    ( arg("image"), arg("minval")=0, arg("maxval")=255),
    doc__normalize__c__
    );

  def(
    "normalize",
    &pink::normalize<float_image, 0, 1>,
    ( arg("image"), arg("minval")=0, arg("maxval")=1),
    doc__normalize__c__    
    );
  
}


// LuM end of file
