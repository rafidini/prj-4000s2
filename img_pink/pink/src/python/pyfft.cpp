/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by applicable law.
  
  (C) UjoImro, 2011
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/

#include "lfft.h"
#include "lcrop.h"
#include "pink_python.h"

using namespace boost::python;
using namespace pink;

namespace pink {
  namespace python {

    fcomplex_image
    py_fft(
      fcomplex_image image,
      int32_t dir
      )
    {
      if (image.get_size().size()!=2)
      {
        pink_error("Only 2D complex images are supported by FFT.");        
      } /* if */

      int32_t rs, cs, cs2, rs2;
      fcomplex_image * result;
      
      cs = image.get_size()[0];            /* Number of rows */
      rs = image.get_size()[1];            /* Number of columns */
  
      rs2 = cs2 = 1;

      // We are looking for the smallest power of two greater or equal
      // then the image
      while (rs2 < rs) rs2 = rs2 << 1;
      while (cs2 < cs) cs2 = cs2 << 1;

      if ((rs2 != rs) || (cs2 != cs))
      {
        pink::types::vint new_size(2);
        new_size << rs2, cs2;
        
        result = new fcomplex_image(new_size);

        result->get_output()->xdim = image.get_output()->xdim;
        result->get_output()->ydim = image.get_output()->ydim;
        result->get_output()->zdim = image.get_output()->zdim;
        
        razimage(result->get_output());
        if (!linsert(image.get_output(), result->get_output(), 0, 0, 0))
        {
          pink_error("function linsert failed");
        }
        
        // NOTE: these commands are not necessary in C++
        ///freeimage(image);
        //image = image2;
      }
      else /* NOT rs2!=rs or cs2!=cs */
      {
        result = new fcomplex_image();
        (*result) = image.clone();
      } /* NOT rs2!=rs or cs2!=cs */
      
  
      /* if (! lfft(image, dir)) */
      /* { */
      /*   fprintf(stderr, "%s: function lfft failed\n", argv[0]); */
      /*   exit(1); */
      /* } */
      lfft(result->get_output(), dir);

      return *result;      
    } /* py_fft */

  } /* namespace python */
} /* namespace pink */


UI_EXPORT_ONE_FUNCTION( fft, 
                        pink::python::py_fft, 
                        (arg("image"), arg("direction")=0),
                        doc__fft__c__
  );

































// LuM end of file
