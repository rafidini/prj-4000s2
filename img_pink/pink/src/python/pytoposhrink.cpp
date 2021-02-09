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


#include "lskeletons.h"
#include "pink_python.h"

using namespace boost::python;
using namespace pink;

namespace pink {
  namespace python {

    char_image toposhrink(
      const char_image & image,
      int_image priority_image,
      int connexity,
      int tmin,
      int tmax,
      int tbmin,
      int tbmax,
      char_image inhibit
      )
    {
      char_image result;
      result = image.clone();
      // ???????
      // char_image loc_inhibit;
      
      // if (inhibit!=0)
      // {
      //   loc_inhibit = inhibit;        
      // }
      
      
      if (image.get_size().size()==2)
      {
        if (! ltoposhrink(result, priority_image, connexity, tmin, tmax, tbmin, tbmax, can_be_null(inhibit)))
        {
          pink_error("ltoposhrink failed");
        }
      }
      else /* NOT image.get_size().size()==2 */
      {
        if (! ltoposhrink3d(result, priority_image, connexity, tmin, tmax, tbmin, tbmax, can_be_null(inhibit)))
        {
          pink_error("ltoposhrink3d failed");
        }
      }  /* NOT image.get_size().size()==2 */

      return result;      
    } /* toposhrink */
    

    char_image toposhrink_small(
      const char_image & image,
      int_image priority_image,
      int connexity,
      int tmin,
      int tmax,
      int tbmin,
      int tbmax
      )
    {
      char_image result = toposhrink( image, priority_image, connexity, tmin, tmax, tbmin, tbmax, char_image() );
      return result;      
    } /* toposhrink_small */
    

    
  } /* namespace python */
} /* namespace pink */


void toposhrink_export()
{

  UI_DEFINE_ONE_FUNCTION(
    toposhrink,
    pink::python::toposhrink,
    ( arg("image"),
      arg("priority_image"),
      arg("connexity"),
      arg("tmin"),
      arg("tmax"),
      arg("tbmin"),
      arg("tbmax"),
      arg("inhibit")
      ),
    doc__toposhrink__c__
    //end of documentation
    );

  UI_DEFINE_ONE_FUNCTION(
    toposhrink,
    pink::python::toposhrink_small,
    ( arg("image"),
      arg("priority_image"),
      arg("connexity"),
      arg("tmin"),
      arg("tmax"),
      arg("tbmin"),
      arg("tbmax")
      ),
    doc__toposhrink__c__
    // end of documentation
    );
  

}







// LuM end of file
