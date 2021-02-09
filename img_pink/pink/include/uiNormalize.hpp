/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by applicable law.
  
  (C) UjoImro, 2010
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/


#ifndef UINORMALIZE_HPP_
#define UINORMALIZE_HPP_

#include "larith.h"
#include "ujimage.hpp"


namespace pink { 

  // float_image lnormalize( const float_image & I,
  //                         float_image::pixel_type vmin = 0. ,
  //                         float_image::pixel_type vmax = 1. 
  //   );
  
  // char_image lnormalize( const char_image & I,
  //                        char_image::pixel_type vmin = 0 ,
  //                        char_image::pixel_type vmax = 255 
  //   );


  template <class image_type, int def_vmin, int def_vmax>
  image_type normalize(
    const image_type & image,
    float vmin = def_vmin,
    float vmax = def_vmax
    )
  {
    image_type result;
    result = image.clone();

    lnormalize( result.get_output(), vmin, vmax );    

    return result;    
  } /* lnormalize */
  




} /* end namespace pink */

#endif /* UINORMALIZE_HPP_ */
/* LuM end of file */

 
