/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by applicable law.
  
  (C) UjoImro, 2010
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/

#include "lskeletons.h"
#include "pink_python.h"

// you should not use one-letter macro names!
#undef N
#undef D

using namespace boost::python;
using namespace pink;

namespace pink {
  namespace python {

    template <class image_type>
    char_image skelsurf(
      const char_image & image,
      image_type priority_image,
      char_image inhibit,
      int connexity
      )
    {
      char_image result;
      result = image.clone();
     
      if (! lskelsurf3d(result.get_output(), priority_image.get_output(), /*can_be_null(inhibit)*/ NULL, connexity))
      {
        pink_error("lskelsurf3d failed");        
      } /* if */


      return result;      
    } /* skelsurf */

    template <class image_type>
    char_image skelsurf_short(
      const char_image & image,
      image_type priority_image,
      int connexity
      )
    {     
      char_image result = skelsurf(image, priority_image, char_image(), connexity);
      return result;
    } /* skelsurf */


    
  } /* namespace python */
} /* namespace pink */
                                                                                      
void skelsurf_export()
{

UI_DEFINE_FUNCTION(
  "skelsurf",
  pink::python::skelsurf,
  ( arg("image"), arg("priority_image"), arg("inhibit"), arg("connexity")),
  doc__skelsurf__c__
  // end of the documentation
   );

UI_DEFINE_FUNCTION(
  "skelsurf",
  pink::python::skelsurf_short,
  ( arg("image"), arg("priority_image"), arg("connexity")),
  doc__skelsurf__c__
  // end of the documentation
   );


} /* skelsurf_export */










// LuM end of file
