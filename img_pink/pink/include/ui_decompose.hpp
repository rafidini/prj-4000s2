/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by applicable law.
  
  (C) UjoImro, 2011
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/

#ifndef UI_DECOMPOSE_HPP
#define UI_DECOMPOSE_HPP

#include <boost/preprocessor.hpp>

// This header defines a polymorphic struct decompose
// this structure takes as a template parameter a function Fn
// and defines three types of parameters:
// decompose<Fn>::arity        - the number of arguments of the function
// decompose<Fn>::result_type  - the type of the result of the function
// decompose<Fn>::T0
// ...
// decompose<Fn>::TN           - the types of the function's parameters

#ifndef MAX_PARAMETERS
#  define MAX_PARAMETERS 15
#  define CLEAN_UP_MAX_PARAMETERS
#endif /* MAX_PARAMETERS */

template <class Fn> struct decompose;

#define PARAMETERS(z, n, text)                  \
  typedef PT##n T##n;                           
  

#define DECOMPOSE(z, n, text)                   \
  template< class R BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_PARAMS(n, class PT) > \
  struct decompose <R(*)(BOOST_PP_ENUM_PARAMS(n, PT))>          \
  {                                                             \
    static const unsigned arity = n;                            \
    typedef R result_type;                                      \
    BOOST_PP_REPEAT(n, PARAMETERS, ~);                          \
  };




BOOST_PP_REPEAT(MAX_PARAMETERS, DECOMPOSE, ~);


// cleaning up after us
#undef PARAMETERS
#undef DECOMPOSE
#ifdef CLEAN_UP_MAX_PARAMETERS
#  undef MAX_PARAMETERS
#endif /* CLEAN_UP_MAX_PARAMETERS */


// the generated code is:

// template <class Fn> struct decomposer;
// template< class R, > struct decompose <R(*)()> {
//   static const unsigned arity = 0;
//   typedef R result_type; ;
// };
// template< class R, class PT0 > struct decompose <R(*)( PT0)> {
//   static const unsigned arity = 1;
//   typedef R result_type;
//   typedef PT0 T0;;
// };
// template< class R, class PT0 , class PT1 > struct decompose <R(*)( PT0 , PT1)> {
//   static const unsigned arity = 2;
//   typedef R result_type;
//   typedef PT0 T0;
//   typedef PT1 T1;;
// };
// template< class R, class PT0 , class PT1 , class PT2 > struct decompose <R(*)( PT0 , PT1 , PT2)> {
//   static const unsigned arity = 3;
//   typedef R result_type;
//   typedef PT0 T0;
//   typedef PT1 T1;
//   typedef PT2 T2;;
// };
// template< class R, class PT0 , class PT1 , class PT2 , class PT3 > struct decompose <R(*)( PT0 , PT1 , PT2 , PT3)> {
//   static const unsigned arity = 4;
//   typedef R result_type;
//   typedef PT0 T0;
//   typedef PT1 T1;
//   typedef PT2 T2;
//   typedef PT3 T3;;
// };;







































#endif /* UI_DECOMPOSE_HPP */
// LuM end of file
