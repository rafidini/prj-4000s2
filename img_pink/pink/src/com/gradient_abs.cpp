/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by applicable law.
  
  (C) UjoImro, 2011
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/
// This file is the bash front-end of the ungerflow function

/*!
   \file gradient_abs.cpp

   \brief Computes the absolute value of the gradient of the spline
   approximation of the image.
   
   <B>Usage:</B> gradient_abs( (float_image)image) -> float_image
   
   <B>Description:</B> The an image can be considered as a continuous
   field. This function approximates the image in all directions (x,
   y, z), with a 1D spline. Than it calculates the absolute value of
   the gradient for every point in form

   g[[x,y,z]] = fabs([ pd_x g, pd_y g, pd_z g ])

   here 'pd_x' stands for the partial derivative in the 'x' direction.

   <B>Types supported:</B> float 2d, float 3d
   
   <B>Category:</B> signal, development
   \ingroup signal, development

   <b>References:</b>
   <br>[1] <href url="http://www.gnu.org/software/gsl"> GNU Scientific Library </href>
   \author Laszlo Marak, 2009-2010


 */






//LuM end of file
