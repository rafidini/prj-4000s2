/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by applicable law.
  
  (C) UjoImro, 2009
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/

// This software is meant to be free
// boost python wrapper

#include "pink_python.h"
#include "ldraw.h"

using namespace boost::python;
using namespace pink;


namespace pink {
  namespace python {

    char_image drawcurve2D(
      const pink::types::vint & size,
      const pink::types::vint & coordX,
      const pink::types::vint & coordY
      )
    {
      if (size.size()!=2)
      {
        pink_error("only 2D images are supported. Nag the developer.");
      } /* size.size()!=2 */

      if (coordX.size() != coordY.size())
      {
        pink_error("error: The number of X and Y coordinates must be equal.");
      } /* coordX.size() != coordy.size() */
  

      char_image result(size);

      FOR( q, coordX.size()-1 )
      {
        ldrawline( result.get_output(),
                   coordX[q],
                   coordY[q],
                   coordX[q+1],
                   coordY[q+1]
          ); 
      } /* FOR coordX.size()-1 */

      return result;
    } /* drawcurve2D */

  } /* namespace python */
} /* namespace pink */



/*
***********************************************************************************************
***********************************************************************************************
***********************************************************************************************

   exported functions

***********************************************************************************************
***********************************************************************************************
***********************************************************************************************
 */

UI_EXPORT_ONE_FUNCTION(
  drawcurve2D,
  pink::python::drawcurve2D,
  ( arg("image size"), arg("X coordinates"), arg("Y coordinates")),
  "the help is in 'help(pink.drawcurve)'"
  );










































// LuM end of file
