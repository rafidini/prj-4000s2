/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by applicable law.
  
  (C) UjoImro, 2009
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/

// boost python wrapper

#include "lwshedtopo.h"
#include "pink_python.h"

using namespace boost::python;
using namespace pink;




// int32_t lwshedtopo_lwshedtopo( struct xvimage *image, int32_t connex );

namespace pink {
  namespace python {

    char_image wshedtopo( const char_image & orig, int connex )
    {

      char_image result;
      result = orig.clone();
      if (! lwshedtopo_lwshedtopo(  result.get_output(), connex ) )
      {
        pink_error("lwshedtopo_lwshedtopo failed");
      }
  
      return result;
    } /* wshedtopo */

  } /* namespace python */
} /* namespace pink */


UI_EXPORT_ONE_FUNCTION(
  wshedtopo,
  pink::python::wshedtopo, 
  ( arg("image"), arg("connex") ),
  doc__wshedtopo__c__
  );



































// LuM end of file
