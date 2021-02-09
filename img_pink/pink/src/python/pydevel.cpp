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

#ifndef PINK_DEVELOPMENT
#  include "pink_python.h"
#else /* PINK_DEVELOPMENT */
#  include "pink_development.h"
#endif /* PINK_DEVELOPMENT */

using boost::python::arg;
using boost::python::def;

#ifdef PINK_DEVELOPMENT
# include "uiCircle.hpp"
# include "uiGradient.hpp"
# include "ui_measure.hpp"
# include "ui_polythread.hpp"
# include "ui_fit_circle.hpp"
#endif /* PINK_DEVELOPMENT */

void pydevel()
{
  //CALL_EXPORTED_FUNCTIONS(BOOST_PP_COUNTER);

# ifdef PINK_DEVELOPMENT

  def( "circle_tangent", &pink::gsl::circle_tangent,
       (boost::python::arg("x coordinates"), boost::python::arg("y coordinates"), boost::python::arg("point of derivation")),
       "This function estimates the derivativ of the function given by points. It "
       " models a circle and returns its tangent at the given point."
    );

  def( "fit_circle", &pink::py_fit_circle,
       (boost::python::arg("x coordinates"), boost::python::arg("y coordinates"),boost::python::arg("filename")=""),
       "This function fits a circle to the given points. It returns a vector for the following formula:\n"
       "  a*x^2 + a*y^2 + b*x + c*y + d == 0\n"
       "If the argument filename is given, than the function will generate an example Mathematica\n"
       "code with the circle and the points."
       "The algorithm is a modified version of that in:\n"
       "Andrew Fitzgibbon, Maurizio Pilu, Robert B. Fisher, 'Direct Least Square Fitting of Ellipses', IEEE Transactions on Pattern Analysis and Machine Intelligence, vol. 21, no. 5, pp. 476-480, May 1999, doi:10.1109/34.765658\n"
    );

  def( "circle_equation_to_coordinates", &pink::py_circle_equation_to_coordinates,
       (boost::python::arg("equation")),
       "This function converts the vector of equation coordinates to a vector of geometric coordinates:\n"
       "the input vector is [a,b,c,d], where\n"
       "  a*x^2 + a*y^2 + b*x + c*y + d == 0\n"
       "the output vector is [center_x, center_y, r], where\n"
       "  (x-center_x)^2 + (y-center_y)^2 == r^2 "
    );

  def(
    "gradient_abs",
    pink::uiGradientAbs,
    (arg("image")),
    doc__gradient_abs__cpp__
    );

  def(
    "measure",
    pink::lmeasure<pink::float_image>,
    (arg("image")),
    doc__measure__cpp__
    );

# endif /* PINK_DEVELOPMENT */

  
}


// LuM end of file
