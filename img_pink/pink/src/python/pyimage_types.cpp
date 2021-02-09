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


void pyimage_types()
{
  //CALL_EXPORTED_FUNCTIONS(BOOST_PP_COUNTER);
  // here are the image objects ( char_image, short_image, int_image, float_image, double_image )
  // there is a template in pyujimage.hpp to help them export without rewritting all the declarations

  // exposing the base image function
  boost::python::class_<pink_image_wrap, boost::noncopyable> ("any_image");

  ujoi_class_export < pink::char_image > (
    "char_image",
    "This is the image class for 1 byte 'char' images"
    );

  ujoi_class_export < pink::short_image > (
    "short_image",
    "This is the image class for 2 byte 'short int' images"
    );


  ujoi_class_export < pink::int_image > ( // LONG INT is 8 byte's on 64 bit system
    "int_image",
    "This is the image class for 4 byte 'long int' images. \n"
    "Note that on 32bit systems it's equivalent with c++ 'int'. \n"
    "However while 'int' can change on 64bit systems, 'long int' \n"
    "should remain 32bit."
    );

  ujoi_class_export < pink::float_image > (
    "float_image",
    "This is the image class for 4 byte 'float' images"
    );

  ujoi_class_export < pink::double_image > (
    "double_image",
    "This is the image class for 8 byte 'double' images"
    );

  ujoi_class_export < pink::fcomplex_image > (
    "fcomplex_image",
    "This is the image class for 4+4 byte 'fcomplex' images"
    );

  ujoi_class_export < pink::dcomplex_image > (
    "dcomplex_image",
    "This is the image class for 8+8 byte 'dcomplex' images"
    );


  
}


// LuM end of file
