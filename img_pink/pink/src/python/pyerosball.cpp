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

#include <pink_python.h>

#undef error
#define error(msg) {std::stringstream fullmessage; fullmessage << "in pyerosball.cpp: " << msg; call_error(fullmessage.str());}

using namespace boost::python;
using namespace pink;

namespace pink {
  namespace python {

    // char_image erosball(
    //   const char_image & src,
    //   float r,
    //   int mode=0
    //   )
    // {

    //   char_image result;
    //   result.copy(src);

    //   if ( src.get_size().size() == 2 ) // the image is 2D
    //   {
    //     if (! lerosdisc( result.get_output(), r, mode ) )
    //     {
    //       error("erosball: lerosdisc failed");
    //     }
    //   }
    //   else /* NOT src.get_size().size() == 2 */
    //   {
    //     if ( src.get_size().size() == 3 ) // the image is 3D
    //     {
    //       if (! lerosball( result.get_output(), r, mode ) )
    //       {
    //         error("erosball: lerosball failed");
    //       }
    //     }
    //     else /* NOT src.get_size().size() == 3 */
    //     {
    //       error("erosball: only 2D and 3D images are supported");
    //     }  /* NOT src.get_size().size() == 3 */
    //   }  /* NOT src.get_size().size() == 2 */


    //   return result;
    // } /* py_erosball */

  } /* namespace python */
} /* namespace pink */


// UI_EXPORT_ONE_FUNCTION(
//   erosball,
//   pink::python::erosball,
//   ( arg("image"),arg("r"),arg("mode")=0 ),
//   doc__erosball__c__
//   // end of the documenation
//   );



















































// LuM end of file
