/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by applicable law.
  
  (C) UjoImro, 2011
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/

/**
 \file   ui_convert_if.hpp
 \ingroup development
 \author UjoImro, 2011

 \brief This function is used for automated type casting. It contains
 template algorithms that cast the 'ujoi<>' classes to 'xvimage*'.
 As it is a casting it is done in constant time, no pixel values are
 copied.
*/


#ifndef UI_CONVERT_IF_HPP
#define UI_CONVERT_IF_HPP

#include <boost/mpl/if.hpp>
#include <boost/type_traits.hpp>

#include "pyujimage.hpp"

namespace pink
{
  namespace python
  {
    
    /**
    \brief This class casts an 'ujoi<>' image class to 'xvimage*'. If
    the template parameter is not derived from 'pink_image' class, then
    the type is unchanged.

    Example of usage:
    \code
    convert_if<char_image>::type I; // I will be of 'xvimage*' type. 
    convert_if<int_image>::type I;  // I will be of 'xvimage*' type. 
    convert_if<float>::type I;      // I will be of 'float' type.' 
    \endcode
    \tparam T The type to cast
    */    
    template < class T >
    struct convert_if
    {
      typedef typename boost::mpl::if_<
        boost::is_base_of<pink::pink_image, typename boost::remove_reference<T>::type >,
        xvimage*,
        T
        >::type type;
    }; /* convert_if */

   
    /**
    \brief This class is used for C function type decomposition. It
    takes any type as template parameter and if it's 'xvimage*' then
    it returns 'pink_image'. In other cases it returns the parameter
    type unchanged.

    Example:
    \code
    xv2pink<xvimage*>::type I; // I will be of 'pink_image' type
    xv2pink<float>::type I;    // I will be of 'float' type
    \endcode

    \tparam T The type to convert. 
    \return 
    */
    template < class T >
    struct xv2pink
    {
      typedef typename boost::mpl::if_<
        boost::is_same<xvimage*, T>,
        pink::pink_image&,
        T
        >::type type;      
    }; /* xv2pink */

    
    
    /**
       \brief This class is used to delay the instantiation of a template
       function.
       
       \note This is a HACK!
       
       The Pink Python wrapper relies heavily on delayed template
       instantiation. Generally if you write a template function or
       class, this class is instantiated (compiled) only if it becomes
       absolutely inevitable. If you write a template function that you
       never use, then it is not compiled, and it is not included into
       the object file. For example:
       
       \code
       template <class T>
       T max(T a, T b)
       {
         return a < b ? a : b;
       }
       \endcode
       
       Is a C++ equivalent of the macro:
       
       \code 
         #define MAX(a, b) ( (a) < (b) ? (a) : (b) )  
       \endcode
       
       However if you never call the max function, this function is never
       tried to be compiled. In some cases however you don't have this
       type of dependency. For example:  FINISH ME!!!
       \tparam RT result type of mcfunction
       \tparam T parameter type of mcfunction
       \tparam mcfunction pointer to the function to wrap
       \param t parameter of mcfunction
    */    
    template <class RT, class T, RT (*mcfunction)(T)>
    RT ui_cheat(T t)
    {
      return mcfunction(t);
    }; /* ui_cheat */
   

  } /* namespace python */
} /* namespace pink */



/*

 */


















#endif /* UI_CONVERT_IF_HPP */
// LuM end of file
