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

#include "ui_pink_types.hpp"
#include "ujimage.hpp"
#include "mcutil.h"
#include "lminmax.hpp"
#include "llong2byte.h"
#include "ui_convert.hpp"

// you shouldn't use one character macros
#undef N

using namespace std;

namespace pink {


  template <>
  index_t
  maxint<short_image::pixel_type>()
  {
    return USHRT_MAX;    
  }

  template <>
  index_t
  maxint<char_image::pixel_type>()
  {
    return UCHAR_MAX;    
  }

  template <>
  index_t
  maxint<int_image::pixel_type>()
  {
    return INT_MAX;    
  }

  char_image long2byte(
    const int_image & src,
    int mode,
    int nbnewval
    )
  {
    int_image local_copy = src.clone();
  
    xvimage * res = long2byte( local_copy.get_output(), mode, nbnewval );

    char_image result(res);
    free(res); // NOT freeimage!!!!

    return result;    
  } /* long2byte */
  
  char_image float2byte( const float_image & src, int mode )
  {
    return convert_image<float_image, char_image>(src, mode);
  } /* float2byte */

  short_image long2short( const int_image & src, int mode )
  {
    return convert_image<int_image, short_image>(src, mode);
  } /* long2short */
  
  char_image short2byte( const short_image & src, int mode )
  {
    return convert_image<short_image, char_image>(src, mode);
  } /* short2byte */


#ifdef MCNEW
  
  float_image byte2complex( const byte_image & src )
  {

    int N = src.get_size().prod();
    float_image result(2*src.get_size());
    
    FOR( x, N ) 
    {
        result[x] = static_cast<float_image::pixel_type>(src[x]);
    } /* FOR */
    
    FOR( x, N ) 
    {
        result[N+x] = static_cast<float_image::pixel_type>0;
    } /* FOR */

    return result;

  } /* byte2complex */
  
  float_image long2complex( const int_image & src )
  {

    int N = src.get_size().prod();
    float_image result(2*src.get_size());
    
    FOR( x, N ) 
    {
        result[x] = static_cast<float_image::pixel_type>(src[x]);
    } /* FOR */
    
    FOR( x, N ) 
    {
        result[N+x] = static_cast<float_image::pixel_type>0;
    } /* FOR */

    return result;

  } /* float2complex */
  
  float_image float2complex( const float_image & src )
  {

    int N = src.get_size().prod();
    float_image result(2*src.get_size());
    
    FOR( x, N ) 
    {
        result[x] = static_cast<float_image::pixel_type>(src[x]);
    } /* FOR */
    
    FOR( x, N ) 
    {
        result[N+x] = static_cast<float_image::pixel_type>0;
    } /* FOR */

    return result;

  } /* float2complex */
#endif
  
} /* namespace pink */








#endif /*__cplusplus*/
/* LuM end of file */
