/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by applicable law.
  
  (C) UjoImro, 2009
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/

#include "pink_development.h"

#include "uiGradient.hpp"

using namespace boost::python;
using namespace pink;



void py_gradient_export(){

  def( "cpp_spline_absolut_gradient_float", &uiGradientAbs,
       args("source image"),
       "returns the image's spline approximated gradient's absolute value\n"
       " \n"
// end of the documenation
    );

  

};
