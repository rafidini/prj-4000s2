/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by applicable law.
  
  (C) UjoImro, 2011
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/

/** \file ui_wrap_function.hpp
  \ingroup development

  \brief This is a helper function for exporting Pink C functions in
  Python. This file defines 'UI_WRAP_FUNCTION'.

  \section note Note 
  For debugging macro definitions uncomment the debug part at the end
  of and the beginning of the file. Then you can use the

  \verbatim
  g++ -E -P file.cpp -o file_pp.cpp
  \endverbatim

  commands, which outputs the source code after preprocessing. Also 

  \verbatim
  astyle -A3 -z2 -k1 -s2 -j file_pp.cpp 
  \endverbatim

  Indents the preprocessed code and inserts lots of newlines, which
  improoves very much the readability of the code. 

  \section export Exporting functions from C

  For exporting functions from Pink you should make them follow the
  \ref dev_conventions.

  You may use printf and you may call exit(1) if there's an error.
  type_k can be any type that python recognizes (int, float, ...) and xvimage*
  The return value is 1 on success and 0 otherwise. To export this function
  you should include a function call in pypink.cpp of the form

  \code
  UI_WRAP_FUNCION(
  "function's name in Python",
  pink_c_function, 
  (arg("argument 1 name"), arg("argument 2 name"), ..., arg(argument n name)),
  doc__my_c_function__c__
  )
  #include BOOST_PP_UPDATE_COUNTER()
  \endcode

  Example:
  \code
  UI_WRAP_FUNCTION(
  "ptisolated",
  lptisolated,
  ( arg("image"), arg("connexity") ),
  doc__ptisolated__c__
  );
  #include BOOST_PP_UPDATE_COUNTER()
  \endcode

  \section advanced Advanced

  This file contains C++ macro language and template metaprogramming,
  which is difficult to understand. The file is first treated with the
  preprocessor. The generated code looks like this:

  \code
  // the wrapper uses boost enable_if class for conditional compilation
  // this is the base class declaration
  template <class T, class enabled = void>
  struct instance0 { };
  
  // for every number of arguments there is a special wrapper 
  // the above instantiation only occurs if the number of arguments
  // (including the 'image' object) is 1
  template <class FA>
  struct instance0< FA, typename enable_if< is_same<FA, int_<1> > >::type >
  {
    template <class Fn, class Args>
    void define(const char* fname, Fn fn, Args args, const char* doc)
    {
      // decomposition of the function for parameters
      typedef struct decompose<Fn> FT;

      // for each image type in Pink the appropriate 'make_function'
      // macro is called
      def(fname, make_function< char_image, (pink::python::ui_cheat<typename FT::result_type, xvimage*, greet>)>, args, doc);
      def(fname, make_function< short_image, (pink::python::ui_cheat<typename FT::result_type, xvimage*, greet>)>, args, doc);
      def(fname, make_function< int_image, (pink::python::ui_cheat<typename FT::result_type, xvimage*, greet>)>, args, doc);
      def(fname, make_function< float_image, (pink::python::ui_cheat<typename FT::result_type, xvimage*, greet>)>, args, doc);
      def(fname, make_function< double_image, (pink::python::ui_cheat<typename FT::result_type, xvimage*, greet>)>, args, doc);
      def(fname, make_function< fcomplex_image, (pink::python::ui_cheat<typename FT::result_type, xvimage*, greet>)>, args, doc);
      def(fname, make_function< dcomplex_image, (pink::python::ui_cheat<typename FT::result_type, xvimage*, greet>)>, args, doc);
    }
  };
  
  template <class FA> struct instance0< FA, typename enable_if< is_same<FA, int_<2> > >::type >
  {
    template <class Fn, class Args>
    void define(const char* fname, Fn fn, Args args, const char* doc)
    {
      typedef struct decompose<Fn> FT;
      def(fname, make_function< char_image, typename xv2pink< typename FT::T1 >::type, greet>, args, doc);
      def(fname, make_function< short_image, typename xv2pink< typename FT::T1 >::type, greet>, args, doc);
      def(fname, make_function< int_image, typename xv2pink< typename FT::T1 >::type, greet>, args, doc);
      def(fname, make_function< float_image, typename xv2pink< typename FT::T1 >::type, greet>, args, doc);
      def(fname, make_function< double_image, typename xv2pink< typename FT::T1 >::type, greet>, args, doc);
      def(fname, make_function< fcomplex_image, typename xv2pink< typename FT::T1 >::type, greet>, args, doc);
      def(fname, make_function< dcomplex_image, typename xv2pink< typename FT::T1 >::type, greet>, args, doc);
    }
  };
  
  template <class FA> struct instance0< FA, typename enable_if< is_same<FA, int_<3> > >::type >
  {
    template <class Fn, class Args>
    void define(const char* fname, Fn fn, Args args, const char* doc)
    {
      typedef struct decompose<Fn> FT;
      def(fname, make_function< char_image, typename xv2pink< typename FT::T1 >::type, typename xv2pink< typename FT::T2 >::type, greet>, args, doc);
      def(fname, make_function< short_image, typename xv2pink< typename FT::T1 >::type, typename xv2pink< typename FT::T2 >::type, greet>, args, doc);
      def(fname, make_function< int_image, typename xv2pink< typename FT::T1 >::type, typename xv2pink< typename FT::T2 >::type, greet>, args, doc);
      def(fname, make_function< float_image, typename xv2pink< typename FT::T1 >::type, typename xv2pink< typename FT::T2 >::type, greet>, args, doc);
      def(fname, make_function< double_image, typename xv2pink< typename FT::T1 >::type, typename xv2pink< typename FT::T2 >::type, greet>, args, doc);
      def(fname, make_function< fcomplex_image, typename xv2pink< typename FT::T1 >::type, typename xv2pink< typename FT::T2 >::type, greet>, args, doc);
      def(fname, make_function< dcomplex_image, typename xv2pink< typename FT::T1 >::type, typename xv2pink< typename FT::T2 >::type, greet>, args, doc);
    }
  };
  
  // this function does the first decomposition and the 
  // appropriate wrapper class instantiation
  template <class Fn, class Args>
  void call0(const char* fname, Fn fn, Args args, const char* doc)
  {
    const int FA = decompose<Fn>::arity;
    instance0< int_<FA> > wrapped_function;
    wrapped_function.define(fname, fn, args, doc);
  }
  
  // this is a helper function so we wouldn't have to 
  // call call0 again in 'BOOST_PYTHON_MODULE'
  void export_function0()
  {
    call0("greet", greet, (arg("image"), (arg("int"))), "WRITE ME!!");
  };
  
  \endcode
  
  The macro generates the wrapper 'instance' for each number of
  parameters. The template takes the specified parameters and wraps
  the function in Python. The most important part of the macro is the
  conversion of the 'xvimage*' pointer into the appropriate image
  class (like 'char_image'). 
  */
  

#ifndef UI_WRAP_FUNCTION_HPP
#define UI_WRAP_FUNCTION_HPP

// // DEBUG MODE
// #define MAX_PARAMETERS 4

#include <boost/mpl/if.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/equal.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/preprocessor.hpp>
#include <boost/static_assert.hpp>
#include <boost/utility/enable_if.hpp>



#include "ui_decompose.hpp"
#include "ui_convert_if.hpp"
#include "ui_make_function.hpp"


using pink::python::xv2pink;
using pink::python::make_function;


using boost::mpl::if_c;
using boost::mpl::int_;
using boost::enable_if;
//using boost::mpl::is_same;
using boost::is_same;



#ifndef MAX_PARAMETERS
#  define MAX_PARAMETERS 15
#  define CLEAN_UP_MAX_PARAMETERS
#endif /* MAX_PARAMETERS */

#define UI_WRAP_FUNCTION_CALL_DEF(z, n, text)                    \
  def(fname, MAKE_FUNCTION_(z, n, text), doc )

#define UI_WRAP_FUNCTION_DECOMPOSE_PARAMS(z, n, text)           \
  FT::T##n

#define UI_WRAP_FUNCTION_XV2PINK(z, n, text)                     \
  typename xv2pink< typename BOOST_PP_CAT(text, n) >::type,

#define UI_WRAP_FUNCTION_DEF_FOR_TYPE(image_type, n, text)                               \
  def(fname, make_function< image_type,                                 \
      BOOST_PP_REPEAT_FROM_TO(1, n, UI_WRAP_FUNCTION_XV2PINK, FT::T)                     \
      text>, args, doc)

#define UI_WRAP_FUNCTION_MAKE_INSTANCE(z, n, text)                                       \
  template <class FA>                                                   \
  struct BOOST_PP_CAT(instance, BOOST_PP_COUNTER)<                      \
    FA, \
    typename enable_if< is_same<FA, int_<n> > >::type > \
  {                                                                     \
    template <class Fn, class Args>                                     \
      void define(const char* fname, Fn fn, Args args, const char* doc) \
    {                                                                   \
      typedef struct decompose<Fn> FT;                                  \
                                                                        \
      UI_WRAP_FUNCTION_DEF_FOR_TYPE(char_image   , n, text);            \
      UI_WRAP_FUNCTION_DEF_FOR_TYPE(short_image  , n, text);            \
      UI_WRAP_FUNCTION_DEF_FOR_TYPE(int_image    , n, text);            \
      UI_WRAP_FUNCTION_DEF_FOR_TYPE(float_image  , n, text);            \
      UI_WRAP_FUNCTION_DEF_FOR_TYPE(double_image , n, text);            \
      UI_WRAP_FUNCTION_DEF_FOR_TYPE(fcomplex_image , n, text);          \
      UI_WRAP_FUNCTION_DEF_FOR_TYPE(dcomplex_image , n, text);          \
                                                                        \
    }                                                                   \
                                                                        \
  };                                                                    

// the function with one parameter is handled specially
#define UI_MAKE_FIRST_INSTANCE(FN)                                      \
  template <class FA>                                                   \
  struct BOOST_PP_CAT(instance,BOOST_PP_COUNTER)< FA, typename enable_if< is_same<FA, int_<1> > >::type > \
  {                                                                     \
    template <class Fn, class Args>                                     \
      void define(const char* fname, Fn fn, Args args, const char* doc) \
    {                                                                   \
      typedef struct decompose<Fn> FT;                                  \
                                                                        \
      def(fname, make_function_one< char_image, typename FT::result_type, FN >, args, doc); \
      def(fname, make_function_one< short_image, typename FT::result_type, FN >, args, doc); \
      def(fname, make_function_one< int_image, typename FT::result_type, FN >, args, doc); \
      def(fname, make_function_one< float_image, typename FT::result_type, FN >, args, doc); \
      def(fname, make_function_one< double_image, typename FT::result_type, FN >, args, doc); \
      def(fname, make_function_one< fcomplex_image, typename FT::result_type, FN >, args, doc); \
      def(fname, make_function_one< dcomplex_image, typename FT::result_type, FN >, args, doc); \
    }                                                                   \
  };



// // checking for the proper usage
// #ifndef BOOST_PP_COUNTER
// #error: UI_WRAP_FUNCTION only makes sense with BOOST_PP_COUNTER defined
// #endif /* BOOST_PP_COUNTER */

#define UI_WRAP_FUNCTION(FNAME, FN, ARGS, DOC)                          \
  template <class T, class enabled = void>                              \
  struct BOOST_PP_CAT(instance, BOOST_PP_COUNTER)                       \
  {                                                                     \
  };                                                                    \
                                                                        \
  UI_MAKE_FIRST_INSTANCE(FN);                                           \
                                                                        \
  BOOST_PP_REPEAT_FROM_TO(2, MAX_PARAMETERS, UI_WRAP_FUNCTION_MAKE_INSTANCE, FN) \
                                                                        \
  template <class Fn, class Args>                                       \
  void BOOST_PP_CAT(call,BOOST_PP_COUNTER)(const char* fname, Fn fn, Args args, const char* doc) \
  {                                                                     \
    const int FA = decompose<Fn>::arity;                                \
                                                                        \
    BOOST_PP_CAT(instance,BOOST_PP_COUNTER)< int_<FA> > wrapped_function; \
                                                                        \
    wrapped_function.define(fname, fn, args, doc);                      \
                                                                        \
  }                                                                     \
                                                                        \
  void BOOST_PP_CAT(export_function, BOOST_PP_COUNTER)()                \
  {                                                                     \
    BOOST_PP_CAT(call, BOOST_PP_COUNTER)(FNAME, FN, ARGS, DOC);         \
  }


// // DEBUG MODE

// #define BOOST_PP_COUNTER 0

// UI_WRAP_FUNCTION("greet", greet, (arg("image"), (arg("int"))) ,"WRITE ME!!");

// #undef BOOST_PP_COUNTER
// #define BOOST_PP_COUNTER 1

// UI_WRAP_FUNCTION("another function", another_function, (arg("image"), arg("int"), arg("connexity")=0) ,"READ ME!!");




















#endif /* UI_WRAP_FUNCTION_HPP */
// LuM end of file
