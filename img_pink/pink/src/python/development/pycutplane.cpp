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

using namespace boost::python;
using namespace pink;

#include "uiCutPlane.hpp"

UI_EXPORT_ONE_FUNCTION( cpp_draw_plane, 
			draw_plane, 
			args("image", "coefficient of x", "coefficient of y", "coefficient of z", "absolut part"),
			"this function draws a discreet plane into the image. The "
			"equation of the plane is a*x+b*y+c*z+d==0\n"
  )


UI_EXPORT_ONE_FUNCTION( cpp_project_plane, 
			project_plane, 
			args("the 3D image",
			     "first point of the plane", 
			     "second point of the plane", 
			     "angle of the rotation"),
			"this function projects the given plane to a 2D image"
  )



// LuM end of file
