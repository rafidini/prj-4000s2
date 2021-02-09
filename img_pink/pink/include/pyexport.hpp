/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by applicable law.
  
  (C) UjoImro, 2010
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/

/**   \file pyexport.hpp

      \ingroup development

      \brief This file defines the macros, which facilitate the export
      of functions in Python. The emphasis is given to multiple
      inclusion of template functions with all different types. As
      python cannot 'compile' in runtime, you must compile the
      template functions with all the possible image types. The macros
      and templates in this file ease you do that.

      Example:

      \code
 void py_maxflow_export(){
  
  boost::python::def( "cpp_maxflow_float", &maxflow_float,
       args("source and sink", "boundaries", "iteration", "tau", "the number of threads"),
       "maximal flows, returns the image (potential field) \n"
       " \n"
 // end of the documenation
    );
  
  boost::python::def( "cpp_maxflow_cami_float", &maxflow_cami,
       args("source and sink", "boundaries", "iteration", "tau", "the number of threads"),
       "maximal flows, returns the flow (float array of size pixels * dimension ) \n"
       " \n"
 // end of the documenation
    );
  \endcode

*/

// boost python wrapper
// This macro exports a template function to all possible image types
#ifndef PYEXPORT_HPP
#define PYEXPORT_HPP

// Defines the longest pink function to wrap. Can be increased
#define MAX_PARAMETERS 15

#include <boost/preprocessor.hpp>
#include "ui_wrap_function.hpp"

 /**
 \brief Generates a Python wrapper function for a C++ function using
 all image types.

 This macro generates a function named "FNAME_export()" which you can
 call from 'pypink.cpp'.  You can use this function to export a
 template function. It takes an image type as template parameter and
 it substitutes all the image types in it.

 \param FNAME The function name in Python (a string)
 \param FN The C++ template function to wrap
 \param ARGS The description of the arguments (see \ref python_export)
 \param DOC The documentation string. This string appears in Python if
 the help function is called. 
 */
#define UI_EXPORT_FUNCTION(FNAME, FN, ARGS, DOC)                        \
                                                                        \
  void FNAME##_export()                                                 \
  {									\
    boost::python::def( BOOST_PP_STRINGIZE(FNAME), &BOOST_PP_EXPAND(FN)<pink::char_image>, \
         ARGS,                                                          \
         DOC                                                            \
      );                                                                \
                                                                        \
    boost::python::def( BOOST_PP_STRINGIZE(FNAME), &BOOST_PP_EXPAND(FN)<pink::short_image>,  \
         ARGS,                                                          \
         DOC                                                            \
      );                                                                \
                                                                        \
    boost::python::def( BOOST_PP_STRINGIZE(FNAME), &BOOST_PP_EXPAND(FN)<pink::int_image>,    \
         ARGS,                                                          \
         DOC                                                            \
      );                                                                \
                                                                        \
    boost::python::def( BOOST_PP_STRINGIZE(FNAME), &BOOST_PP_EXPAND(FN)<pink::float_image>,  \
         ARGS,                                                          \
         DOC                                                            \
      );                                                                \
                                                                        \
    boost::python::def( BOOST_PP_STRINGIZE(FNAME), &BOOST_PP_EXPAND(FN)<pink::double_image>, \
         ARGS,                                                          \
         DOC                                                            \
      );                                                                \
                                                                        \
    boost::python::def( BOOST_PP_STRINGIZE(FNAME), &BOOST_PP_EXPAND(FN)<pink::fcomplex_image>, \
         ARGS,                                                          \
         DOC                                                            \
      );                                                                \
                                                                        \
    boost::python::def( BOOST_PP_STRINGIZE(FNAME), &BOOST_PP_EXPAND(FN)<pink::dcomplex_image>, \
         ARGS,                                                          \
         DOC                                                            \
      );                                                                \
                                                                        \
  }									

 /**
 \brief Generates a Python wrapper function for a C++ function using
 all image types.

 This macro generates a block with function calls for all image functions.
You can call it from BOOST_PYTHON_MODULE.

 \param FNAME The function name in Python (a string)
 \param FN The C++ template function to wrap
 \param ARGS The description of the arguments (see \ref python_export)
 \param DOC The documentation string. This string appears in Python if
 the help function is called. 
 */
#define UI_DEFINE_FUNCTION( FNAME, FN, ARGS, DOC )                      \
  {                                                                     \
    boost::python::def( FNAME, BOOST_PP_EXPAND(FN)<pink::char_image>,                        \
         ARGS,                                                          \
         DOC                                                            \
      );                                                                \
                                                                        \
    boost::python::def( FNAME, BOOST_PP_EXPAND(FN)<pink::short_image>,                       \
         ARGS,                                                          \
         DOC                                                            \
      );                                                                \
                                                                        \
    boost::python::def( FNAME, BOOST_PP_EXPAND(FN)<pink::int_image>,                         \
         ARGS,                                                          \
         DOC                                                            \
      );                                                                \
                                                                        \
    boost::python::def( FNAME, BOOST_PP_EXPAND(FN)<pink::float_image>,                       \
         ARGS,                                                          \
         DOC                                                            \
      );                                                                \
                                                                        \
    boost::python::def( FNAME, BOOST_PP_EXPAND(FN)<pink::double_image>,                      \
         ARGS,                                                          \
         DOC                                                            \
      );                                                                \
                                                                        \
    boost::python::def( FNAME, BOOST_PP_EXPAND(FN)<pink::fcomplex_image>,                    \
         ARGS,                                                          \
         DOC                                                            \
      );                                                                \
                                                                        \
    boost::python::def( FNAME, BOOST_PP_EXPAND(FN)<pink::dcomplex_image>,                    \
         ARGS,                                                          \
         DOC                                                            \
      );                                                                \
                                                                        \
                                                                        \
  }


 /**
 \brief Generates a Python wrapper function for a C++ function using
 all image types.

 You can use this function to export a template function from within
 'pypink.cpp'. It takes an image type as template parameter and it
 substitutes all the image types in it. 

 \note This macro calls the boost python's 'def' function with all the
 image types.

 \param FNAME The function name in Python (a string)
 \param FN The C++ template function to wrap
 \param ARGS The description of the arguments (see \ref python_export)
 \param DOC The documentation string. This string appears in Python if
 the help function is called. 
 */
#define UI_WRAP_CPP( FNAME, FN, ARGS, DOC )                             \
  void BOOST_PP_CAT(export_function, BOOST_PP_COUNTER)()                \
  {		     							\
    boost::python::def( FNAME, BOOST_PP_EXPAND(FN)<pink::char_image>,                        \
         ARGS,								\
         DOC                                                            \
      );                                                                \
                                                                        \
    boost::python::def( FNAME, BOOST_PP_EXPAND(FN)<pink::short_image>,                       \
         ARGS,								\
         DOC                                                            \
      );                                                                \
                                                                        \
    boost::python::def( FNAME, BOOST_PP_EXPAND(FN)<pink::int_image>,                         \
         ARGS,								\
         DOC                                                            \
      );                                                                \
                                                                        \
    boost::python::def( FNAME, BOOST_PP_EXPAND(FN)<pink::float_image>,                       \
         ARGS,								\
         DOC                                                            \
      );                                                                \
                                                                        \
    boost::python::def( FNAME, BOOST_PP_EXPAND(FN)<pink::double_image>,                      \
         ARGS,								\
         DOC                                                            \
      );                                                                \
                                                                        \
                                                                        \
  }									 


 /**
 \brief Generates a Python wrapper function for a C++ function using
 all image types.

 This macro generates a function named "FNAME_export()" which you can
 call from 'pypink.cpp'.  You can use this function to export a
 template function. It takes an image type as template parameter and
 it substitutes all the image types in it.

 \param FNAME The function name in Python (a string)
 \param FN The C++ template function to wrap
 \param ARGS The description of the arguments (see \ref python_export)
 \param DOC The documentation string. This string appears in Python if
 the help function is called. 
 */
#define UI_EXPORT_ONE_FUNCTION( FNAME, FN, ARGS, DOC )                  \
                                                                        \
  void FNAME##_export()                                                 \
  {									\
    boost::python::def( BOOST_PP_STRINGIZE(FNAME), &BOOST_PP_EXPAND(FN),               \
         ARGS,                                                          \
         DOC                                                            \
      );								\
  }									

#define UI_DEFINE_ONE_FUNCTION( FNAME, FN, ARGS, DOC )                  \
                                                                        \
  {									\
    boost::python::def( BOOST_PP_STRINGIZE(FNAME), &BOOST_PP_EXPAND(FN),               \
	 ARGS,								\
	 DOC                                                            \
      );								\
    									\
  }									


  
  

// // } /* py_maxflow_export */

//   template < class image_type >
//   class convert_if
//   {
//   public:
//     typedef typename boost::mpl::if_< boost::is_base_of<pink::pink_image, image_type>,
//                                       xvimage*,
//                                       image_type >::type type;
//   }; /* convert_if */

// // **********************************************************
// // **********************************************************
// // **********************************************************
// // ***** THIS IS REPLACED WITH THE PREPROCESSING MACRO ******
// // **********************************************************
// // **********************************************************
// // **********************************************************




// // this macro is for classical pink functions
// // like
// // int mcfunction( image, param1, ... )
// BOOST_PP_REPEAT(MAX_PARAMETERS, MAKE_FUNCTION, ~)


// // this function is for pink functions which supply the result in a different
// // image like
// // int mcfunction( image, param1, ..., paramn, result )
// BOOST_PP_REPEAT(MAX_PARAMETERS, MAKE_RESULT, ~)



template <class image_type>
xvimage * can_be_null( image_type & image )
{
  if (image.isnull())
  {
    return NULL;        
  }
  else /* NOT image.isnull() */
  {
    return image.get_output();        
  }      
} /* can_be_null */


/**
   \brief Generates and 'void export_function15();' declaration. It is used by the UI_WRAP_FUNCTION in 'pypink.cpp'
   Generates and 'export_function15();' declaration. It is used by the UI_WRAP_FUNCTION in 'pypink.cpp'
*/
#define EXPORT_FUNCTION(z,n,text)               \
  export_function##n();                         


/**
   \brief Call the exported functions
   \code
   export_function0();
   export_function1();
   export_function2();
   export_function3();
   export_function4();
   ...
   export_function##n();    
   \endcode

   The exported functions are generated by the WRAP macros.
*/
#define CALL_EXPORTED_FUNCTIONS(n)              \
  BOOST_PP_REPEAT(n, EXPORT_FUNCTION, ~)









#endif /* PYEXPORT_HPP */
// LuM end of file
