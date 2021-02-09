/*
  This software is licensed under
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but
  without any warranty to the extent permitted by applicable law.

  (C) UjoImro, 2009-2010
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/



#include "pink_python.h"
#include "liar_fseries.h"


using namespace boost::python;
using namespace pink;

namespace pink {
  namespace python {

    template <class image_t>
    image_t liardilaterect
    (
      const image_t & src,
      const int SEnx,
      const int SEny,
      const int SEnz
    )
    {
    image_t result;
    result = src.clone();

    //pink_error("This function has been switched off!!!! You forgot to add some files!");

    // The low-level function imfdilat_rect etc return 0 to indicate success
    // HT 20120227

       if ( src.get_size().size()==2) // the image is 2D
       {
           struct xvimage *myxvimage = result.get_output();
         if ( imfdilat_rect( myxvimage, SEnx, SEny, myxvimage) )
         {
           pink_error("function imfdilat_rect failed");
         } /* (! ldilateros_ldilat( src, elem_const_away, x, y)) */
       }
       else  // NOT the image is 2D
       {
           struct xvimage *myxvimage = result.get_output();
         if ( imfdilat3D_rect( myxvimage, SEnx, SEny, SEnz, myxvimage) )
         {
           pink_error("function imfdilat3D_rect failed");
         } /* (! ldilat3d( src, elem_const_away, x, y)) */
       } // NOT the image is 2D

      return result;
    } /* liardilat */

    template   <class image_t>
    image_t liardilatepoly
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

    //pink_error("This function has been switched off!!!! You forgot to add some files!");

    // The low-level function imfdilat_rect etc return 0 to indicate success
    // HT 20120227

       if ( src.get_size().size()==2) // the image is 2D
       {
           // It's OK, imfdilate_poly does not modify the input
           struct xvimage *myoutput = result.get_output();
         if ( (errorcode = imfdilate_poly(myoutput, radius, type, sides, myoutput)) != 0)
         {
           pink_error("function imfdilat_poly failed with error = ");
         } /* (! ldilateros_ldilat( src, elem_const_away, x, y)) */
       }
       else  // NOT the image is 2D
       {
           pink_error("function liardilatpoly not available for 3D images");
       } // NOT the image is 2D

      return result;
    } /* liardilatpoly */

  } /* namespace python */
} /* namespace pink */


UI_EXPORT_FUNCTION(
  fdilaterect,
  pink::python::liardilaterect,
  ( arg("src"), arg("SEnx"),arg("SEny"), arg("SEnz") ),
  "Fast dilation by a flat 2D rectangle or 3D rectangle parallelepiped"
  );

UI_EXPORT_FUNCTION(
  fdilatepoly,
  pink::python::liardilatepoly,
  ( arg("src"), arg("Radius"),arg("Type"), arg("Sides") ),
  "Fast 2D dilation by a flat polygon, given a radius, a type of line (0=periodic or 1=Bresenham) and a number of sides (can be zero)"
  );















































// LuM end of file
