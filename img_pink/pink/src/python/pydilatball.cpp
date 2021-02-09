/*
  This software is licensed under
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but
  without any warranty to the extent permitted by applicable law.

  (C) UjoImro, 2009-2010
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/


#include <pink_python.h>

using namespace boost::python;
using namespace pink;

namespace pink {
  namespace python {

  //   char_image dilatball(
  //     char_image & image, ///!!!!!!!!!!!!!!! const here
  //     int radius,
  //     int mode=0
  //     )
  //   {

  //     char_image result;
  //     result.copy(image);
  //     xvimage * res;
  //     char_image * result_res;

  //     if ((mode != 0) && (mode != 1) && (mode != 2) && (mode != 3) && (mode != 4) &&
  //         (mode != 8) && (mode != 6) && (mode != 18) && (mode != 26))
  //     {
  //       error("dist = [0|1|2|3|4|8|6|18|26] ");
  //     }

  //     if (result.get_size().size() == 2) // the image is 2D
  //     {
  //       if (radius >= 0)
  //       {
  //         if (! ldilatdisc(result, radius, mode))
  //         {
  //           error("function ldilatdisc failed");
  //         }
  //       }
  //       else
  //       {
  //         if (mode == 3)
  //         {
  //           int_image tmp = byte2long(result);
  //           // if (!convertlong(&result))
  //           // {
  //           //   error("function convertlong failed");
  //           // }

  //           if (! (res = lredt2d(tmp)))
  //           {
  //             error("function lredt2d failed");
  //           }
  //         }
  //         else
  //         {
  //           if (! (res = ldilatdiscloc(result, mode)))
  //           {
  //             error("function ldilatdiscloc failed");
  //           }
  //         }
  //       }
  //     }
  //     else // NOT the image is 2D
  //     {
  //       if (radius >= 0)
  //       {
  //         if (! ldilatball(result, radius, mode))
  //         {
  //           error("function ldilatball failed");
  //         }
  //       }
  //       else
  //       {
  //         if (! (res = ldilatballloc(result, mode)))
  //         {
  //           error("function ldilatballloc failed");
  //         }
  //       }
  //     } // NOT the image is 2D

  //   if (radius >= 0)
  //   {
  //     //writeresult(result, argv[argc-1]);
  //     return result;
  //   }
  //   else
  //   {
  //     result_res = new char_image(*res);
  //     freeimage(res);
  //     return *result_res;
  //     //writeresult(res, argv[argc-1]);
  //   }

  //   // results returned in the previous if-else clause
  // } /* dilatball */


} /* namespace python */
} /* namespace pink */








// UI_EXPORT_ONE_FUNCTION(
//   dilatball,
//   pink::python::dilatball,
//   ( arg("image"), arg("radius"), arg("mode")=0 ),
//   doc__dilatball__c__
//   // end of the documenation
//   );
















































// LuM end of file
