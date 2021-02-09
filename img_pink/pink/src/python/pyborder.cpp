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


#include "pink_python.h"
#include "lborder.h"
#include "mctopo3d.h"

/// !!! macros should be named upper case
#undef border

using namespace boost::python;

namespace pink {
  namespace python {


    char_image border(const char_image & src, int connex)
    {
      char_image result;
      result = src.clone();

      if (!mctopo3d_lborder(result.get_output(), connex))
      {
        pink_error("mctopo3d_lborder failed");        
      }

      return result;
    } /* py_border */

  } /* namespace python */
} /* namespace pink */


UI_EXPORT_ONE_FUNCTION(
  border, 
  pink::python::border, 
  (arg("image"), arg("connexity")),
  doc__border__c__
  );






// LuM end of file
