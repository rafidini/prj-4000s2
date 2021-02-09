/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by applicable law.
  
  (C) UjoImro, 2011
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/

#ifdef __cplusplus
#ifndef UI_ANY_TO_COMPLEX_HPP_
#define UI_ANY_TO_COMPLEX_HPP_

#include "pink.h"

namespace pink {

  template <class image_type>
  fcomplex_image lany2complex(
    const image_type & re,
    const image_type & im
    )
  {
    //COMPARE_SIZE(re.get_output(), im.get_output());
    if (re.get_size() != im.get_size())
    {
      pink_error("The image sizes must be equal!");      
    }
        
    fcomplex_image result(re.get_size());

    FOR(q, result.get_size().prod())
    {
      result[q].im=im[q];
      result[q].re=re[q];      
    }

    return result;
    
  } /* lany2complex */
  










  
} /* namespace pink */

#endif /* UI_ANY_TO_COMPLEX_HPP_ */
#endif /* __cplusplus */
/* LuM end of file */
