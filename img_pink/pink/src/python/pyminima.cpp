/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by applicable law.
  
  (C) UjoImro, 2010
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/

#include "lminima.h"
#include "pink_python.h"

using namespace boost::python;
using namespace pink;

namespace pink {
  namespace python {


    template<class image_type>
    char_image minima(
      image_type image,
      std::string mode
      )
    {
      char_image result(image.get_size());

      lminima( image.get_output(), const_cast<char*>(mode.c_str()));

      // copying the values back to the result
      // note the values never depass 255 so copying is fine
      std::copy(
        &image(0),
        &image(image.get_size().prod()),
        &result(0)
        );
      
      
      return result;      
    } /* minima */

   
    

  } /* namespace python */
} /* namespace pink */


/** Note: this is a special function, as we use implicit std::copy-conversion
    
 */
void minima_export()
{
  def("minima", pink::python::minima<char_image>,( arg("image"), arg("mode")), doc__minima__c__);
  def("minima", pink::python::minima<short_image>,( arg("image"), arg("mode")), doc__minima__c__);
  def("minima", pink::python::minima<int_image>,( arg("image"), arg("mode")), doc__minima__c__);
  def("minima", pink::python::minima<float_image>,( arg("image"), arg("mode")), doc__minima__c__);
  def("minima", pink::python::minima<double_image>,( arg("image"), arg("mode")), doc__minima__c__);  
} /* minima_export */


















































// LuM end of file
