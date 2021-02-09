/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by applicable law.
  
  (C) UjoImro, 2011
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/

// this header defines a function wrapper make_result
/** \file ui_make_result.hpp
  \ingroup development

  \brief This is a helper function for exporting Pink C functions in
  Python. Note that this template file is rarely used directly and is
  called by the UI_WRAP_RESULT macro. 

  \section note Note 
  For debugging macro definitions you can use the

  \verbatim
  g++ -E -P file.cpp -o file_pp.cpp
  \endverbatim

  commands, which outputs the source code after preprocessing. Also 

  \verbatim
  astyle -A3 -z2 -k1 -s2 -j file_pp.cpp 
  \endverbatim

  indents the preprocessed code and inserts lot's of newlines, which
  improoves very much the readability of the code. 

  \section export Exporting functions from C

  For exporting functions from Pink you should make them follow the
  \ref dev_conventions.

  You may use printf and you may call exit(1) if there's an error.
  type_k can be any type that python recognizes (int, float, ...) and xvimage*
  The return value is 1 on success and 0 otherwise. To export this function
  you should include a function call in pypink.cpp of the form

  \code
  def( 
    "function's name in python",
    &make_result<result_type, image_type, T1, T2, ..., Tn, &pink_c_function>,
    (arg("argument 1 name"), arg("argument 2 name"), ..., arg(argument n name) )
    doc__my_c_function__c__
  )
  \endcode

  make_result is a template. In the first parameter you specify the
  result image type. Second, you specify the input image type. Thirds
  you specify the types of the parameters and last you put the pointer
  to your Pink function. 

  Example:

  \code
  def( "ptisolated",
       &make_result<int_image, char_image, int, &lptisolated>,
       ( arg("image"), arg("connexity") ),
       doc__ptisolated__c__
     );
  \endcode

  \section advanced Advanced

  This file contains C++ macro language and template metaprogramming,
  which is difficult to understand. The file is first treated with the
  preprocessor. The generated code looks like this:

  \code
  template < class result_type,
             class image_type,
             int (*mcfunction) (
               typename convert_if<image_type>::type,
               typename convert_if<result_type>::type
               )
             >
  result_type make_result( image_type image )
  {
    result_type result(image.get_size());
    if (!mcfunction(image, result)) { {
        std::stringstream fullmessage;
        fullmessage << "in ui_make_result.hpp: " << "mcfunction failed";
        call_error(fullmessage.str());
      };
    }
    return result;    
  }

  template < class result_type,
             class image_type,
             class T0 ,
             int (*mcfunction) (
               typename convert_if<image_type>::type,
               typename convert_if<T0>::type ,
               typename convert_if<result_type>::type )
             >
  result_type make_result( image_type image , T0 t0 )
  {
    result_type result(image.get_size());
    if (!mcfunction(image, t0 , result)) { {
        std::stringstream fullmessage;
        fullmessage << "in ui_make_result.hpp: " << "mcfunction failed";
        call_error(fullmessage.str());
      };
    }
    return result;
  }

  template <
    class result_type,
    class image_type,
    class T0 ,
    class T1 ,
    int (*mcfunction) (
      typename convert_if<image_type>::type,
      typename convert_if<T0>::type ,
      typename convert_if<T1>::type ,
      typename convert_if<result_type>::type
      )
    >
  result_type make_result( image_type image , T0 t0 , T1 t1 )
  {
    result_type result(image.get_size());
    if (!mcfunction(image, t0 , t1 , result)) { {
        std::stringstream fullmessage;
        fullmessage << "in ui_make_result.hpp: " << "mcfunction failed";
        call_error(fullmessage.str());
      };
    }
    return result;
  }
  \endcode

  The macro generates the make_function template for each number of
  parameters. The template takes the specified parameters and wraps
  the function in Python. The most important part of the macro is the
  conversion of the 'xvimage*' pointer into the appropriate image
  class (like 'char_image'). 
*/


#ifndef UI_MAKE_RESULT_HPP
#define UI_MAKE_RESULT_HPP

#include <boost/preprocessor.hpp>
#include "ui_convert_if.hpp"
#include "ui_pink_types.hpp"

#ifndef MAX_PARAMETERS
#  define MAX_PARAMETERS 15
#  define CLEAN_UP_MAX_PARAMETERS
#endif /* MAX_PARAMETERS */

#define                                         \
  PARAM(z, n, text)                             \
  T##n t##n

#define                                         \
  CONVERT_IF(z, n, text)                        \
  typename convert_if<T##n>::type

//BOOST_PP_ENUM_PARAMS( MAX_PARAMETERS, class param_type)


#define                                                                 \
  MAKE_RESULT(z, n, text)                                               \
  template < class result_type, class image_type,                       \
  BOOST_PP_ENUM_PARAMS(n, class T ) BOOST_PP_COMMA_IF(n)                \
  int (*mcfunction) (                                                   \
    typename convert_if<image_type>::type,                              \
    BOOST_PP_ENUM(n, CONVERT_IF, ~) BOOST_PP_COMMA_IF(n)                \
    typename convert_if<result_type>::type                              \
    )                                                                   \
  >                                                                     \
  result_type make_result( image_type image BOOST_PP_COMMA_IF(n)        \
                           BOOST_PP_ENUM(n, PARAM, ~)                   \
    )                                                                   \
  {                                                                     \
  result_type result(image.get_size());                                 \
                                                                        \
  if (!mcfunction(image, BOOST_PP_ENUM_PARAMS(n, t) BOOST_PP_COMMA_IF(n) result)) \
  {                                                                     \
  pink_error("mcfunction failed");                                      \
  }                                                                     \
                                                                        \
  return result;                                                        \
  }

namespace pink
{
  namespace python
  {
    BOOST_PP_REPEAT(MAX_PARAMETERS, MAKE_RESULT, ~)
  } /* namespace python */
} /* namespace pink */


// cleaning up after us
#undef CONVERT_IF
#undef PARAM
#undef MAKE_FUNCTION
#undef error
#ifdef CLEAN_UP_MAX_PARAMETERS
#  undef MAX_PARAMETERS
#endif /* CLEAN_UP_MAX_PARAMETERS */








#endif /* UI_MAKE_RESULT_HPP */
// LuM end of file
