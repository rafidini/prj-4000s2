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
#ifndef UI_EXTRACTPLANE_HPP_
#define UI_EXTRACTPLANE_HPP_

#include "ui_pink_types.hpp"

namespace pink {

  template<class image_type>
  image_type extractplane(
    const image_type & image,
    int n, /*the number of the plane*/
    std::string mode
    )
  {
    int32_t i, j, k, t, offset, rs, cs, ds;

    if (! (((mode[0] == 'x') && (mode[1] == 'y')) ||
           ((mode[0] == 'y') && (mode[1] == 'x')) ||
           ((mode[0] == 'x') && (mode[1] == 'z')) ||
           ((mode[0] == 'z') && (mode[1] == 'x')) ||
           ((mode[0] == 'y') && (mode[1] == 'z')) ||
           ((mode[0] == 'z') && (mode[1] == 'y'))))
    {
        // fprintf(stderr, "usage: filein.pgm n plane fileout.pgm (plane=xy|yx|xz|zx|yz|zy)");
      pink_error("extractplane called with incorrect mode " << mode );
    }

    rs = image.get_size()[0]; //rowsize(image);
    cs = image.get_size()[1]; //colsize(image);
    ds = image.get_size()[2]; //depth(image);

    /* ---------------------------------------------------------- */
    /* extraction d'un plan */
    /* ---------------------------------------------------------- */

    if ((mode[0] == 'x') && (mode[1] == 'y'))
    {
      pink::types::vint size(2,0);
      size << rs,cs;      
      image_type result(size);
      if ((n < 0) || (n >= ds))
      {
        pink_error("bad plane number " << n);
      }
      t = rs * cs;
      offset = n * t;
      for (i = 0; i < t; i++) result(i) = image(i + offset);

      return result;      
    } 
    else if ((mode[0] == 'y') && (mode[1] == 'x'))
    {
      pink::types::vint size(2,0);
      size << cs,rs;      
      image_type result(size);
      if ((n < 0) || (n >= ds))
      {
        pink_error("bad plane number " << n);
      }
      t = rs * cs;
      for (j = 0; j < cs ; j++)
        for (i = 0; i < rs ; i++)
          result(i*cs + j) = image(n*t + j*rs + i);

      return result;      
    }
    else if ((mode[0] == 'x') && (mode[1] == 'z'))
    {
      pink::types::vint size(2,0);
      size << rs,ds;      
      image_type result(size);

      if ((n < 0) || (n >= cs))
      {
        pink_error("bad plane number " << n);
      }
      t = rs * cs;
      for (k = 0; k < ds ; k++)
        for (i = 0; i < rs ; i++)
          result(k*rs + i) = image(k*t + n*rs + i);

      return result;      
    }
    else if ((mode[0] == 'z') && (mode[1] == 'x'))
    {
      pink::types::vint size(2,0);
      size << ds,rs;      
      image_type result(size);
      
      if ((n < 0) || (n >= cs))
      {
        pink_error("bad plane number " << n ); //,n
      }
      t = rs * cs;
      for (k = 0; k < ds ; k++)
        for (i = 0; i < rs ; i++)
          result(i*ds + k) = image(k*t + n*rs + i);
      
      return result;      
    }
    else if ((mode[0] == 'y') && (mode[1] == 'z'))
    {
      pink::types::vint size(2,0);
      size << cs,ds;      
      image_type result(size);
     
      if ((n < 0) || (n >= rs))
      {
        pink_error("bad plane number " << n);
      }
      t = rs * cs;
      for (k = 0; k < ds ; k++)
        for (j = 0; j < cs ; j++)
          result(k*cs + j) = image(k*t + j*rs + n);

      return result;      
    }
    else if ((mode[0] == 'z') && (mode[1] == 'y'))
    {
      pink::types::vint size(2,0);
      size << ds,cs;      
      image_type result(size);
      
      if ((n < 0) || (n >= rs))
      {
        pink_error("bad plane number " << n);
      }
      t = rs * cs;
      for (k = 0; k < ds ; k++)
        for (j = 0; j < cs ; j++)
          result(j*ds + k) = image(k*t + j*rs + n);

      return result;      
    }
    else
    {
      pink_error("Valid modes are (mode=xy|yx|xz|zx|yz|zy) " << mode);
    }
 
  } /* extractplane */
  

  




} /* namespace pink */
#endif /* UI_EXTRACTPLANE_HPP_ */
#endif /*__cplusplus*/
/* LuM end of file */
