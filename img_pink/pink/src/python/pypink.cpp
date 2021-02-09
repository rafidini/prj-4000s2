/*
  This software is licensed under
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but
  without any warranty to the extent permitted by applicable law.

  (C) UjoImro, 2009-2010
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/
// The pink python wrapper core file

#ifndef PINK_DEVELOPMENT
#  include "pink_python.h"
#else /* PINK_DEVELOPMENT */
#  include "pink_development.h"
#endif /* PINK_DEVELOPMENT */

#include "lsym.h"
#include "lskew.h"
#include "lhello.h"
#include "lsquel.h"
#include "loffset.h"
#include "lstretch.h"
#include "lseltopo.h"
#include "lsquelbin.h"
#include "ltopotypes.h"
#include "lfiltrestopo.h"

using namespace boost::python;
using namespace pink;


/*! \file pypink.c
  For exporting functions from pink you should make them into the following
  format:

  * In C
  int pink_c_function( xvimage * image, type_1 var_1, type_2 var_2, ..., type_n var_n)

  You may use printf and you may call exit(1) if there's an error.
  type_k can be any type that python recognizes (int, float, ...) and xvimage*
  The return value is 1 on success and 0 otherwise. To export this function
  you should include a function call in pypink.cpp of the form

  def( "function's name in python",
  &make_function<char_image, type_1, type_2, ..., type_n, &pink_c_function>,
  (arg("argument 1 name"), arg("argument 2 name"), ..., arg(argument n name) )
  "documentation of my function"
  )

  make_function is a template. In the first parameter you specify the image type. If
  you want all image types to be exported you put 'base_image' as type. Second, you
  specify the types of the parameters and last you put the pointer to your pink function.
  Third you may specify the names of the parameters to appear in python's help. Finally
  you can put a string with the documentation which will appear in the python's help.

  Example:
  def( "ptisolated",
  &make_function<char_image, int, &lptisolated>,
  ( arg("image"), arg("connexity") ),
  doc__ptisolated__c__
  );


  * in C++

  You should write your function in format

  char_image cpp_function( const char_image & image, type_1 var_1, type_2 var_2, ..., type_n var_n )
  here you can put any image type: char_image, int_image, float_image...

  or

  template <class image_type>
  image_type cpp_function( const image_type & image, type_1 var_1, type_2 var_2, ..., type_n var_n )

  You can call 'cout <<' as well as you can use exceptions. By convention you should never change the
  input image. Allways make a copy of it before processing. Remember that char_image is a SHALLOW TYPE
  char_image image2 (image1) will NOT make a copy. On the other hand you can use function(char_image I)
  as a parameter-type without the copy penalty.
  To export this function you should call

  UI_DEFINE_ONE_FUNCTION(
  function_name_in_python,
  cpp_function,
  ( arg("parameter 1 name"), ..., arg("parameter n name") ),
  "documentation of cpp_function"
  )

  or, if you have a template function:

  UI_DEFINE_FUNCTION(
  function_name_in_python,
  cpp_function,
  ( arg("parameter 1 name"), ..., arg("parameter n name") ),
  "documentation of cpp_function"
  )

  Examples:
  UI_DEFINE_ONE_FUNCTION(
  float2byte,
  pink::float2byte,
  ( arg("source image"), arg("mode")=2 ),
  doc__float2byte__c__
  // end of the documenation
  );

  UI_DEFINE_FUNCTION(
  convert2float,
  pink::convert2float,
  (arg("image")),
  "converts an image to float type"
  // end of the documenation
  );


*/
// declarations
// void deep_xvimage_object_export();void shallow_xvimage_object_export();
// void xvimage_object_export();
// void writeimage_export();
void vint_object_export(); void vector_int_object_export();void wshedtopo_export();
/*void erosball_export()*/;void medianfilter_export();

// skeleton function
void skeleton2_export();void zoom_export();

void dilation_export(); void erosion_export(); void geodilat_export();
void geoeros_export(); void opening_export(); void drawcurve2D_export();
void uiSqhool_object_export(); void gradient_export();
// Fast morpho operators
void fdilaterect_export();
void fdilatepoly_export();
void feroderect_export();
void ferodepoly_export();
void fcloserect_export();
void fclosepoly_export();
void fopenrect_export();
void fopenpoly_export();
// opening by unions of lines
void openbun_export();
void closebin_export();

void read_raw_image_export(); void seuil_export(); void plane3d_export(); void draw_plane_export();
void project_plane_export(); void border_export(); void identifyline_export(); void surimp_export();
void generate_rgb_image_export(); void closing_export(); /*void closeball_export();*/ void minmax_export();
/*void dilatball_export();*/ void asfbin_export();
void skelcurv_export(); void distc_export();
void readimage_export(); void skelsurf_export(); void toposhrink_export(); void htkern_export();
/*void openball_export();*/ void gradmorph_export(); void mcube_export(); void minima_export();
void complex_export(); void fft_export(); void mult_export(); void pymaxflow();

// juliette
int short2long(xvimage *image, xvimage *result);
int long2short(xvimage *image, xvimage *result);
int imferode_rect( struct xvimage *input, int SEnx, int SEny, struct xvimage *output);
int imfdilat_rect( struct xvimage *input, int SEnx, int SEny, struct xvimage *output);
int imfopen_rect( struct xvimage *input, int SEnx, int SEny, struct xvimage *output);
int imfclose_rect( struct xvimage *input, int SEnx, int SEny, struct xvimage *output);

// skeleton
void pyskeleton();

// graphs
void pygraph();

// this part is not yet ready for windows
#ifdef UNIXIO
// not sure this is the correct thing to do here HT
// This is the prototype of the automatically generated export function
void Pimview_export(); void Pimviewlogin_export(); void Pimviewputimage_export();
void PLiarEnableDebug_export() ; void PLiarDisableDebug_export(); void Pimview_force_socket_export();
void Pimview_sendcommand_export();
#endif /* UNIXIO */


// functions dealing with complex variables
void pycomplex();

// functions for conversion
void pyconversion();

// development functions
void pydevel();

// normalization and ball generation
void pynormalization();

void pyimage_types();

void pywshed();

void pypoints();

void pyliar();

void pyconnect();

void pytopo();

void pymorpho();

void pyarithmetic();

void pymemtest();

void pysept();

void numpy_export();

// for wrapping every type with one function
// template <class image_type>
// void ujoi_class_export( const char* object_name, const char* object_description );

/// ********************************************************************************************
/// ********************************************************************************************
/// ********************************************************************************************
/// *****                                                                                  *****
/// *****                          ALTERNATIVE 2011 WRAPPING                               *****
/// *****                                                                                  *****
/// ********************************************************************************************
/// ********************************************************************************************
/// ********************************************************************************************


#include <boost/preprocessor/slot/counter.hpp>



// NOTE: 3dlabel input byte 3d output int 3d

// UI_WRAP_FUNCTION(
//   "",
//   l,
//   (arg("image") ),
//   doc____c__
//   );
// #include BOOST_PP_UPDATE_COUNTER()



UI_WRAP_FUNCTION(
  "simplepair",
  lsimplepair,
  (arg("image"), arg("all")),
  doc__simplepair__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "skew",
  lskew,
  (arg("image"), arg("skew")),
  doc__skew__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "squel",
  lsquel,
  (arg("image"), arg("threshold"), arg("ni threshold")),
  doc__squel__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "squelbin",
  lsquelbin,
  (arg("image"), arg("connexity"), arg("ni threshold")),
  doc__squelbin__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "stretch",
  lstretch,
  (arg("image")),
  doc__stretch__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "sym",
  lsym,
  (arg("image"), arg("mode")),
  doc__sym__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "t26pp",
  ltopotypes_t26pp,
  (arg("image")),
  doc__t26pp__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "t4pp",
  lt4pp,
  (arg("image")),
  doc__t4pp__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "t8pp",
  lt8pp,
  (arg("image")),
  doc__t8pp__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "tlf",
  ltlf,
  (arg("image"), arg("connexmin"), arg("radius")),
  doc__tlf__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "translate",
  loffset,
  (arg("image"), arg("ox"), arg("oy"), arg("oz"), arg("mode") =0),
  doc__translate__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

// NOTE: volume's using &vol


// // UI_WRAP_FUNCTION(
// //   "ccv",
// //   lccv,
// //   (arg("image"), arg("elem")),
// //   doc__ccv__c__
// //   );
// // #include BOOST_PP_UPDATE_COUNTER()





BOOST_PYTHON_MODULE(libcpp_pink)
// the final module will be named pink, but that will also contain
// some python functions, like readimage, so it will be created as
// a tree hierarchy
{

# ifdef UJIMAGE_DEBUG
  std::cerr << "Pink was compiled in debug level " << UJIMAGE_DEBUG << ". You will see various debug messages.\n" <<  std::endl;
# endif /* UJIMAGE_DEBUG */

  CALL_EXPORTED_FUNCTIONS(BOOST_PP_COUNTER);

  def("greet", greet, "Gently greets the user.");
  vector_int_object_export();

  // obsolete
  // xvimage_object_export(); // self explaining
  // deep_xvimage_object_export(); // self explaining
  // shallow_xvimage_object_export(); // self explaining

  // created as a method of the object
  //  writeimage_export(); // self explaining

  vint_object_export(); // self explaining

  wshedtopo_export(); /* erosball_export();*/  medianfilter_export();

  // the skeleton function
  skeleton2_export();  zoom_export();

  dilation_export(); erosion_export();  geodilat_export();  geoeros_export();
  opening_export();  drawcurve2D_export();
  // liar operator functions
  fdilaterect_export();
  fdilatepoly_export();
  feroderect_export();
  ferodepoly_export();
  fcloserect_export();
  fclosepoly_export();
  fopenrect_export();
  fopenpoly_export();
  
  openbun_export();
  closebin_export();

//  specialize_export();

  uiSqhool_object_export();
//  gradient_export();

  read_raw_image_export();  seuil_export();  plane3d_export();
//  draw_plane_export();
//  project_plane_export();
  border_export();  identifyline_export();  surimp_export();  generate_rgb_image_export();
  closing_export(); /* closeball_export();*/  minmax_export();  /* dilatball_export();*/
  asfbin_export();  skelcurv_export();
  readimage_export();  distc_export();  skelsurf_export();  toposhrink_export();
  htkern_export();  /*openball_export();*/    gradmorph_export(); mcube_export(); minima_export();
  complex_export(); fft_export();  mult_export();  numpy_export();


  // this part is not yet ready for windows
# ifdef UNIXIO
  // is this enough ? HT
  // this calls the definition in the right place
  Pimview_export();
  Pimviewlogin_export();
  Pimviewputimage_export();
  PLiarEnableDebug_export();
  Pimview_force_socket_export();
  Pimview_sendcommand_export();
# endif /* UNIXIO */

  pysept();
  pytopo();
  pyliar();
  pywshed();
  pydevel();
  pygraph();  
  pymorpho();
  pypoints();
  pymemtest();
  pyconnect();
  pymaxflow();
  pycomplex();
  pyskeleton();
  pyarithmetic();
  pyconversion();
  pyimage_types();
  pynormalization();

} /* BOOST_PYTHON_MODULE */



































// LuM end of file
