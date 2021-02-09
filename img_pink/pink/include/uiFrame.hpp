/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by applicable law.
  
  (C) UjoImro, 2010
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/

#ifndef UI_FRAME__
#define UI_FRAME__

#include "mcimage.h"
#include "ujimage.hpp"
#include <boost/thread.hpp>
#include <boost/smart_ptr.hpp>

namespace pink { 

  template <class image_type>
  image_type frame(
    const image_type & src_image, 
    typename image_type::pixel_type withval 
    )
  {

    image_type result;
    result = src_image.clone();

    pink::types::vint curr( src_image.get_size() );
    
    FOR(q, src_image.get_size().prod())
    {
      src_image.get_size().next_step( q, curr );
      if (src_image.get_size().on_side( curr ))
      {
	result[q]=withval;	
      } /* if */
    } /* FOR */

    return result;

  } /* frame */


  template <class image_type>
  void frame_around_copy(
    image_type src_image,
    image_type result,
    index_t from,
    index_t to,
    typename image_type::pixel_type withval
    )
  {
    pink::types::vint size(result.get_size());
    index_t d = size.size();
    pink::types::vint curr(d);

    for (index_t q = from; q < to; q++)
    {
      
      // first we treat the case of the sides      
      size.next_step( q, curr );

      if (size.on_side(curr))
      {
        result[curr]=withval;
      }
      else /* NOT size.on_side(curr) */
      {
        FOR(w, d)
        {
          curr[w]-=1;          
        }
        result(q)=src_image[curr];        
      } /* NOT size.on_side(curr) */      
      
    } /* for q in from..to */

    return;    
  } /* frame_around_copy */
  

  
  template <class image_type>
  image_type frame_around(
    const image_type & src_image, 
    typename image_type::pixel_type withval 
    )
  {
    index_t num_cpu = 1;

    // getting the number of cpu-s on the system
    num_cpu = boost::thread::hardware_concurrency();

    num_cpu = std::max<index_t>(1, num_cpu);
    
#   if UJIMAGE_DEBUG > 1
    std::cout << "Framing; I have detected " << num_cpu << " cpu cores" << std::endl;
#   endif /* UJIMAGE_DEBUG */
    
    pink::types::vint new_size(src_image.get_size().size(), "new_size" );
    
    // calculating the size of the framed image
    FOR(q, new_size.size() )
    {
      new_size[q] = src_image.get_size()[q] + 2;      
    } /* FOR */
    
    image_type result(new_size);
    index_t nbp = result.get_size().prod();


    // saving the center of the image
    result.set_center_vint(src_image.get_center());
    
    pink::types::vint curr( src_image.get_size().size(), "curr"  );

    if (nbp > 1000000)
    {
#     ifdef UJIMAGE_DEBUG
      std::cout <<
        "Parallelizing during framing because the size of the image is big enough; "
        "I have detected " << num_cpu << " cpu cores" << std::endl;
#     endif /* UJIMAGE_DEBUG */      

      std::vector< boost::shared_ptr<boost::thread> > threads(num_cpu);
    
      // launching the threads
      FOR(q, num_cpu)
      {
        threads[q].reset(
          new boost::thread(
            frame_around_copy<image_type>,
            src_image,
            result,
            nbp * q / num_cpu, /* from */
            nbp * (q+1) / num_cpu, /* to */
            withval
            )
          );        
      } /* FOR, q, num_cpu */

      FOR(q, num_cpu)
      {
        threads[q]->join();        
      } /* FOR, q, num_cpu */
      
    } /* end of the parallel code */    
    else /* NOT nbp > 1000000*/
    {
      frame_around_copy(src_image, result, 0, nbp, withval);      
    } /* NOT nbp > 1000000*/
    
    return result;

  } /* frame_around */


  
  template <class image_type>
  image_type frame_remove( const image_type & src_image )
  {
    
    pink::types::vint new_size(src_image.get_size().size());
    
    // calculating the size of the framed image
    FOR( q, new_size.size() )
    {
      new_size[q] = src_image.get_size()[q] - 2;
    } /* FOR */
    
    image_type result(new_size);

    // saving the center of the image
    result.set_center_vint(src_image.get_center());
    
    pink::types::vint curr( src_image.get_size() );
        
    // copying the original image into the new image
    FOR( q, result.get_size().prod() )
    {
      result.get_size().next_step(q, curr);
      FOR( w, new_size.size() )
      {
	curr[w]+=1;  // same as with frame around
      } /* FOR */
      result(q)=src_image[curr];
    } /* FOR */
   
    return result;

  } /* remove_frame */



  template <class image_type>
  image_type align_size( const image_type & image, index_t group_size,
                         typename image_type::pixel_type fillval )
  {
    pink::types::vint size( image.get_size() );    
    index_t d = size.size();
    pink::types::vint curr(d);    
    pink::types::vint new_size( d, -1 );    
    FOR(q, d)
    {
      new_size[q] = pink::round_up( size[q] + 2 , group_size ); // rounding up for the simd operations
    } /* q in d*/
    index_t new_prod = new_size.prod();
    index_t prod     = size.prod();
    
    image_type result(new_size);

    // filling up the image with the values from around
    FOR(q, new_prod)
    {
      result(q) = fillval;      
    } /* q in new_prod */
    
    // copying the image. the rest of the pixels remain withval
    FOR(q, prod)
    {
      size.next_step( q, curr );
      FOR(w, d)
      {
        curr[w]++;        
      }

      result[curr] = image(q);
      
    } /* FOR q in prod */

    return result;
  } /* align_size */
  






























  } /* namespace pink */


#endif /* UI_FRAME__ */
/* LuM end of file */
