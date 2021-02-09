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
#ifndef UI_CONVERT_HPP_
#define UI_CONVERT_HPP_

#include <cmath>

#include "ujimage.hpp"

namespace pink {

  template <class T>
  index_t
  maxint()
  {
    pink_error("could not determine the type's maximum value");    
  }
  
  template <class T0>
  int64_t uiround(T0 val)
  {
    return floor(val + 0.5);
  }

  char_image  float2byte(  const float_image & src, int mode=0 );
  short_image long2short( const int_image &   src, int mode=0 );
  char_image  short2byte( const short_image &   src, int mode=0 );

  char_image long2byte(
    const int_image & src,
    int mode,
    int nbnewval
    );
    
  
#ifdef MCNEW
  template<class image_type>
  float_image convert2complex( const image_type & image )
  {
    float_image result(image.get_size());
    int N = src.get_size().prod();
    
    FOR(q, N)
    {
      result[q] = image[q];        
    } /* FOR */
    
    FOR(q, N)
    {
      result[N+q] = 0;        
    } /* FOR */
    
    return result;      
  } /* convert2complex */
#endif

  template <class image_type, class result_type>
  result_type simple_convert( const image_type & image )
  {
    result_type result(image.get_size());
    
    index_t nbp = image.get_size().prod();
    
    FOR(q, nbp)
    {
      result[q]=image[q];      
    }
    
    return result;      
  } /* simple_convert */

  
  template <class image_type>
  int_image convert2long( const image_type & image )
  {
    return simple_convert<image_type, int_image>(image);    
  }

  template <class image_type>
  float_image convert2float( const image_type & image )
  {
    return simple_convert<image_type, float_image>(image);    
  }
  

  

  /**
     \brief This function converts long images to short or byte
     images.
     
  description Although it could be used to convert between different
  types, it has to be examined before used to do so. 

  \param image The image to convert
  
  \param mode The conversion mode. At the time of this documentation
  the valid modes are: mode = 0 (trunc)  1 (modulo)  2 (scale)
   4 (square root)  5 (log)"

  \return The converted image.
  */  
  template <class image_type, class result_type>
  result_type convert_image( const image_type & image, int mode=0 )
  {
    index_t nbp =       image.get_size().prod();
    result_type result( image.get_size() );

    typedef typename image_type::pixel_type pixel_type;
        
    index_t maxval = maxint<typename result_type::pixel_type>();
    
    double t, tmp, T;

    switch(mode)
    {

    case 0:
    
      FOR(q, nbp) 
      {
        result[q] = std::min<pixel_type>( uiround(image[q]), maxval );
      } /* FOR */
      break;

    case 1:

      FOR(q, nbp) 
      {
        result[q] = uiround( image[q] ) %  (maxval + 1);
      } /* FOR */
      break;

    case 2:
    {
      
      pixel_type min, max;
          
      std::pair<pixel_type, pixel_type> minmax = lminmaxval(image);
      min = minmax.first;
      max = minmax.second;
            
      FOR(q, nbp) 
      {
        t = ( ( image[q] - min ) * maxval ) / static_cast<double>( max - min );
        tmp = uiround(t);
        result[q] = std::min<typename result_type::pixel_type>( tmp, maxval );
      } /* FOR */
      break;
    }
    
    case 4:

      FOR(q, nbp) 
      {

        T = std::sqrt( static_cast<double>( image[q]) );
        tmp = uiround(T);
        tmp = std::min<double>( maxval, tmp );
        tmp = std::max<double>( 0, tmp );
        result[q] = tmp;
      } /* FOR */

      break;

    case 5:

      FOR(q, nbp) 
      {

        T = log(static_cast<double>(image[q]));
        tmp = uiround(T);
        tmp = std::min<double>(maxval, tmp);
        tmp = std::max<double>(0,      tmp);
        result[q] = tmp;
      } /* FOR */

      break;



    default:
      pink_error( "mode must be: \nmode = 0 (trunc) \n 1 (modulo) \n 2 (scale) \n 4 (square root) \n 5 (log)\n" );

    } /* switch(mode) */
  

    return result;

  }
  
} /* namespace pink */
#endif /* UI_CONVERT_HPP_ */
#endif /*__cplusplus*/
/* LuM end of file */
