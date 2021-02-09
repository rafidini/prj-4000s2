/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by applicable law.
  
  (C) UjoImro, 2010
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/

#ifdef __cplusplus
#ifndef UIINSERT_HPP_
#define UIINSERT_HPP_

#ifdef PINK_HAVE_PYTHON
# include <boost/python.hpp>
#endif /* PINK_HAVE_PYTHON */

#include "ujimage.hpp"

namespace pink {

# ifdef PINK_HAVE_PYTHON
  template <class image_type >
  image_type insert_image( 
    const image_type & big_image,
    const image_type & small_image,
    const boost::python::list & shift
    )
  {
    image_type result;
    result = big_image.clone();
    int d = big_image.get_size().size();
    pink::types::vint shift_vec(shift);

    pink::types::vint curr(d);
    pink::types::vint new_pos(d);

    DEBUG(shift_vec.repr());
    DEBUG(big_image.get_size().repr());
    DEBUG(result.get_size().repr());
    DEBUG(small_image.get_size().repr());
    

    FOR(q, small_image.get_size().prod())
    {
      small_image.get_size().next_step( q, curr );
      
      FOR(w, d)
      {
	new_pos[w]=curr[w]+shift_vec[w];
      } /* FOR */
      
      if (big_image.get_size().inside(new_pos))
      {
	result[new_pos]=small_image[curr];
      } /* if */

    } /* FOR */

    return result;
  } /* insert_image */

  template <class image_type >
  image_type merge_max_image( 
    const image_type & big_image,
    const image_type & small_image,
    const boost::python::list & shift
    )
  {
    image_type result;
    result = big_image.clone();    
    int d = big_image.get_size().size();
    pink::types::vint shift_vec(shift);

    pink::types::vint curr(d);
    pink::types::vint new_pos(d);

    DEBUG(shift_vec.repr());
    DEBUG(big_image.get_size().repr());
    DEBUG(result.get_size().repr());
    DEBUG(small_image.get_size().repr());
    

    FOR(q, small_image.get_size().prod())
    {
      small_image.get_size().next_step( q, curr );
      
      FOR(w, d)
      {
	new_pos[w]=curr[w]+shift_vec[w];
      } /* FOR */
      
      if (big_image.get_size().inside(new_pos))
      {
	result[new_pos]=
	  result[new_pos]>small_image[curr]?
	  result[new_pos]:small_image[curr];
      } /* if */

    } /* FOR */
   

    return result;
  } /* merge_max_image */



# endif /* PINK_HAVE_PYTHON */

} /* namespace pink */
#endif /* UIINSERT_HPP_ */
#endif /*__cplusplus*/
/* LuM end of file */

