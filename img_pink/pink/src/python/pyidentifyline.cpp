/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by applicable law.
  
  (C) UjoImro, 2010
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/

// boost python wrapper
// this file opens a file with raw image data as an image of given type

#include "mclin.h"
#include "pink_python.h"

using namespace boost::python;
using namespace pink;

namespace pink {
  namespace python {
    
    boost::python::list
    identifyline( const boost::python::list & X,
                     const boost::python::list & Y 
      )
    {
      boost::python::list result;
  

      int length=boost::python::len(X);
  
      #if UJIMAGE_DEBUG >= 1

      int check=boost::python::len(Y);

      if (length != check)
      {    
        pink_error("identifyline: the sizes of the vectors must be equal");
      } /* length != check */
  
      #endif /* UJIMAGE_DEBUG */


      boost::shared_array<double> cX( new double[length] );
      boost::shared_array<double> cY( new double[length] );

      FOR(q, length)
        try
        {
          cX[q]=boost::python::extract<double>(X[q]);
          cY[q]=boost::python::extract<double>(Y[q]);
        } 
        catch (...) 
        {
          pink_error("identifyline: the elements of the list must be convertible to doubles.");
        }

      double a,b;
  
      if ( !lidentifyline( cX.get(), cY.get(), length, &a, &b) )
      {
        pink_error("lidentifyline failed");
      } /* if */
  
  
      result.append(a);
      result.append(b);

      return result;
    } /* py_border */

  } /* namespace python */
} /* namespace pink */


UI_EXPORT_ONE_FUNCTION(
  identifyline, 
  pink::python::identifyline, 
  ( arg("list of X coordinates"), arg("list of Y coordinates") ),
  doc__identifyline__c__
  );






// LuM end of file
