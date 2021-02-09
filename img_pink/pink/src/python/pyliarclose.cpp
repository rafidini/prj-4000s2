/*
  This software is licensed under
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but
  without any warranty to the extent permitted by applicable law.

  (C) UjoImro, 2009-2010
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/
#include "liar_fseries.h"
#include "pink_python.h"

using namespace boost::python;
using namespace pink;


namespace pink {
  namespace python {
    template <class image_t>
    image_t liarcloserect
    (
      const image_t & src,
      const int SEnx,
      const int SEny,
      const int SEnz
    )
    {

      image_t result;
      result = src.clone();

     // The low-level function imfclose_rect etc return 0 to indicate success
    // HT 20120227

       if ( src.get_size().size()==2) // the image is 2D
       {
         struct xvimage *myxvimage = result.get_output();
         if (imfclose_rect( myxvimage, SEnx, SEny, myxvimage) )
         {
           pink_error("function imfclose_rect failed");
         }
       }
       else  // NOT the image is 2D
       {
         struct xvimage *myxvimage = result.get_output();
         if (imfclose3D_rect( myxvimage, SEnx, SEny, SEnz, myxvimage) )
         {
           pink_error("function imfclose3D_rect failed");
         }
       } // NOT the image is 2D

      return result;
    } /* liarclose */

    template   <class image_t>
    image_t liarclosepoly
    (
      const image_t & src,
      const int radius,
      const int type,
      const int sides
    )
    {
        int errorcode = 0;
        image_t result;
        result = src.clone();

        // The low-level function imfclose_rect etc return 0 to indicate success
        // HT 20120227

       if ( src.get_size().size()==2) // the image is 2D
       {
           // It's OK, imfdilate_poly does not modify the input
           struct xvimage *myoutput = result.get_output();
         if ( (errorcode = imfclose_poly(myoutput, radius, type, sides, myoutput)) != 0)
         {
           pink_error("function imfclose_poly failed with error = ");
         } /* (! ldilateros_ldilat( src, elem_const_away, x, y)) */
       }
       else  // NOT the image is 2D
       {
           pink_error("function liarclosepoly not available for 3D images");
       } // NOT the image is 2D

      return result;
    } /* liardilatpoly */
  } /* namespace python */
} /* namespace pink */


UI_EXPORT_FUNCTION(
  fcloserect,
  pink::python::liarcloserect,
  ( arg("src"), arg("SEnx"),arg("SEny"), arg("SEnz") ),
  "Fast closing by a flat 2D rectangle or 3D rectangle parallelepiped"
  // end of the documenation
  );

UI_EXPORT_FUNCTION(
  fclosepoly,
  pink::python::liarclosepoly,
  ( arg("src"), arg("Radius"),arg("Type"), arg("Sides") ),
  "Fast 2D closing  by a flat polygon, given a radius, a type of line (0=periodic or 1=Bresenham) and a number of sides (can be zero)"
  );






















































// LuM end of file
