/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by applicable law.
  
  (C) UjoImro, 2011
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/

// this header defines a function wrapper make_malloc
/** \file ui_make_malloc.hpp
  \ingroup development

  \brief This is a helper function for exporting Pink C functions in
  Python. Note that this template file is rarely used directly and is
  called by the UI_WRAP_FUNCTION macro. 

  \section note Note 
  For debugging macro definitions you can use the

  \verbatim
  g++ -E -P file.cpp -o file_pp.cpp
  \endverbatim

  commands, which outputs the source code after preprocessing. Also 

  \verbatim
  astyle -A3 -z2 -k1 -s2 -j file_pp.cpp 
  \endverbatim

  Indents the preprocessed code and inserts lot's of newlines, which
  improoves very much the readability of the code. 

  \section export Exporting functions from C

  For exporting functions from Pink you should make them follow the
  \ref dev_conventions.

  You may use printf and you may call exit(1) if there's an error.
  type_k can be any type that python recognizes (int, float, ...) and xvimage*
  The return value is 1 on success and 0 otherwise. To export this function
  you should include a function call in pypink.cpp of the form

  \code
  \endcode

  make_malloc is a template. In the first parameter you specify the
  image type.  Second, you specify the types of the parameters and
  last you put the pointer to your Pink function. 

  Example:

  \code
  \endcode

  \section advanced Advanced

  This file contains C++ macro language and template metaprogramming,
  which is difficult to understand. The file is first treated with the
  preprocessor. The generated code looks like this:

  \code
  \endcode

  The macro generates the make_malloc template for each number of
  parameters. The template takes the specified parameters and wraps
  the function in Python. The most important part of the macro is the
  conversion of the 'xvimage*' pointer into the appropriate image
  class (like 'char_image'). 
*/



#ifndef UI_MAKE_MALLOC_HPP
#define UI_MAKE_MALLOC_HPP


#include <boost/python.hpp>
#include <boost/preprocessor.hpp>
#include "ui_convert_if.hpp"
#include "ui_pink_types.hpp"

using namespace pink::python;


#ifndef MAX_PARAMETERS
#  define MAX_PARAMETERS 15
#  define CLEAN_UP_MAX_PARAMETERS
#endif /* MAX_PARAMETERS */


#define                                       \
  PARAM(z, n, text)                           \
  T##n t##n

#define                                         \
  CONVERT_IF(z, n, text)                        \
  typename convert_if<T##n>::type

//BOOST_PP_ENUM_PARAMS( MAX_PARAMETERS, class param_type)

#define                                                                 \
  MAKE_MALLOC(z, n, text)                                               \
  template < class image_type,                                          \
  BOOST_PP_ENUM_PARAMS(n, class T ) BOOST_PP_COMMA_IF(n)                \
  xvimage * (*mcfunction) (                                             \
    typename convert_if<image_type>::type BOOST_PP_COMMA_IF(n)          \
    BOOST_PP_ENUM(n, CONVERT_IF, ~)                                     \
    )                                                                   \
  >                                                                     \
  boost::python::object make_malloc( image_type image BOOST_PP_COMMA_IF(n) \
                            BOOST_PP_ENUM(n, PARAM, ~)                  \
    )                                                                   \
  {                                                                     \
                                                                        \
  xvimage * presult;                                                    \
  presult = mcfunction(image BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_PARAMS(n, t)); \
                                                                        \
  if (presult == NULL)                                                  \
  {                                                                     \
  pink_error("mcfunction failed");                                      \
  }                                                                     \
                                                                        \
  image_type iresult(presult);                                          \
  boost::python::object result(iresult);                                \
  return result;                                                        \
  };                                                                    
  


namespace pink
{
  namespace python
  {

    // the special case with one parameter for ui_make_funcion

    template < class image_type,
               xvimage * (*mcfunction) (
                 typename convert_if<image_type>::type
                 )
               >
    boost::python::object make_malloc_one( image_type image )
    {
      xvimage * presult;
      presult = mcfunction(image);

      if (presult == NULL)
      {
        pink_error("mcfunction failed!\n");        
      }

      image_type iresult(presult);      
      boost::python::object result(iresult);
      return result;
    };
       
    
    BOOST_PP_REPEAT(MAX_PARAMETERS, MAKE_MALLOC, ~);    

  } /* namespace python */
} /* namespace pink */

    
// cleaning up after us
#undef CONVERT_IF
#undef PARAM
#undef MAKE_MALLOC
#undef error
#ifdef CLEAN_UP_MAX_PARAMETERS
#  undef MAX_PARAMETERS
#endif /* CLEAN_UP_MAX_PARAMETERS */




#endif /* UI_MAKE_MALLOC_HPP */
// LuM end of file
