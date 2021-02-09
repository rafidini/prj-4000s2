/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by applicable law.
  
  (C) UjoImro, 2011
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/

#ifndef UI_WRAP_RESULT_HPP
#define UI_WRAP_RESULT_HPP

// // DEBUG MODE
// #define MAX_PARAMETERS 2

#include <boost/mpl/if.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/equal.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/preprocessor.hpp>
#include <boost/static_assert.hpp>
#include <boost/utility/enable_if.hpp>



#include "ui_decompose.hpp"
#include "ui_convert_if.hpp"
#include "ui_make_result.hpp"


using pink::python::xv2pink;
using pink::python::make_result;


using boost::mpl::if_c;
using boost::mpl::int_;
using boost::enable_if;
//using boost::mpl::is_same;
using boost::is_same;



#ifndef MAX_PARAMETERS
#  define MAX_PARAMETERS 15
#  define CLEAN_UP_MAX_PARAMETERS
#endif /* MAX_PARAMETERS */

#define UI_WRAP_RESULT_CALL_DEF(z, n, text)     \
  def(fname, MAKE_FUNCTION_(z, n, text), doc )

#define UI_WRAP_RESULT_DECOMPOSE_PARAMS(z, n, text)     \
  FT::T##n

#define UI_WRAP_RESULT_XV2PINK(z, n, text)                      \
  typename xv2pink< typename BOOST_PP_CAT(text, n) >::type,

#define UI_WRAP_RESULT_DEF_FOR_TYPE(image_type, n, text)                \
  def(fname, make_result< result_type, image_type,                      \
      BOOST_PP_REPEAT_FROM_TO(1, n, UI_WRAP_RESULT_XV2PINK, FT::T)      \
      text>, args, doc)

#define UI_WRAP_RESULT_MAKE_INSTANCE(z, n, text)                        \
  template <class result_type, class FA>                                \
  struct BOOST_PP_CAT(instance, BOOST_PP_COUNTER)                       \
  < result_type,                                                        \
    FA,                                                                 \
    typename enable_if< is_same<FA, int_<n> > >::type >                 \
  {                                                                     \
    template <class Fn, class Args>                                     \
      void define(const char* fname, Fn fn, Args args, const char* doc) \
    {                                                                   \
      typedef struct decompose<Fn> FT;                                  \
                                                                        \
      UI_WRAP_RESULT_DEF_FOR_TYPE(char_image   , n, text);              \
      UI_WRAP_RESULT_DEF_FOR_TYPE(short_image  , n, text);              \
      UI_WRAP_RESULT_DEF_FOR_TYPE(int_image    , n, text);              \
      UI_WRAP_RESULT_DEF_FOR_TYPE(float_image  , n, text);              \
      UI_WRAP_RESULT_DEF_FOR_TYPE(double_image , n, text);              \
      UI_WRAP_RESULT_DEF_FOR_TYPE(fcomplex_image , n, text);            \
      UI_WRAP_RESULT_DEF_FOR_TYPE(dcomplex_image , n, text);            \
                                                                        \
    }                                                                   \
                                                                        \
  };                                                                    


// // checking for the proper usage
// #ifndef BOOST_PP_COUNTER
// #error: UI_WRAP_RESULT only makes sense with BOOST_PP_COUNTER defined
// #endif /* BOOST_PP_COUNTER */


#define UI_WRAP_RESULT(RTYPE, FNAME, FN, ARGS, DOC)                     \
  template <class result_type, class T, class enabled = void>           \
  struct BOOST_PP_CAT(instance, BOOST_PP_COUNTER)                       \
  {                                                                     \
  };                                                                    \
                                                                        \
  BOOST_PP_REPEAT_FROM_TO(1, MAX_PARAMETERS, UI_WRAP_RESULT_MAKE_INSTANCE, FN) \
                                                                        \
  template <class Fn, class Args>                                       \
  void BOOST_PP_CAT(call,BOOST_PP_COUNTER)(const char* fname, Fn fn, Args args, const char* doc) \
  {                                                                     \
    const int FA = decompose<Fn>::arity;                                \
                                                                        \
    BOOST_PP_CAT(instance,BOOST_PP_COUNTER)< RTYPE, int_< FA - 1 > > wrapped_function; \
                                                                        \
    wrapped_function.define(fname, fn, args, doc);                      \
                                                                        \
  }                                                                     \
                                                                        \
  void BOOST_PP_CAT(export_function, BOOST_PP_COUNTER)()                \
  {                                                                     \
    BOOST_PP_CAT(call, BOOST_PP_COUNTER)(FNAME, FN, ARGS, DOC);  \
  }


// // DEBUG MODE
// // the following preprocessor call would generate the below code

// #define BOOST_PP_COUNTER 0

// UI_WRAP_RESULT(int_image, "greet", greet, (arg("image"), (arg("int"))) ,"WRITE ME!!");

// #undef BOOST_PP_COUNTER
// #define BOOST_PP_COUNTER 1

// UI_WRAP_RESULT(float_image, "new_greet", greeter, (arg("image"), arg("int"), arg("connexity")=0) ,"READ ME!!");


// // the generated code




















#endif /* UI_WRAP_RESULT_HPP */
// LuM end of file
