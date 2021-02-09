/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by applicable law.
  
  (C) UjoImro, 2010-2011
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/



#include "ldist.h"
#include "pink_python.h"
#include "ui_convert.hpp"

#include <boost/cast.hpp>

// you should not use one-letter macro names!
#undef N
#undef D

using namespace boost::python;
using namespace pink;
using boost::polymorphic_cast;


namespace pink {
  namespace python {

    boost::python::object
    distc(
      const char_image & original_image,
      int mode
      )
    {
      int        N;
      uint8_t   *F;
      char_image image;
      image = original_image.clone();

      if ((mode != 0) && (mode != 1) && (mode != 2) && (mode != 3) && (mode != 5) &&
          (mode != 4) && (mode != 8) && (mode != 6) && (mode != 18) && (mode != 26) &&
          (mode != 40) && (mode != 80) && (mode != 60) && (mode != 180) && (mode != 260))
      {
        pink_error("filein.pgm mode fileout.pgm"
              "       mode = 0 (dist. eucl. trunc), 1 (dist. eucl. quad.), 2 (chamfrein),\n"
              "              3 (exact eucl. quad.), 5 (exact eucl.), 4, 8 (2D), 6, 18, 26 (3D)\n"
              "                40, 80 (2D), 60, 180, 260 (3D)\n");
      } /* if mode */

      // if (mode < 40)        
      //   // result = allocimage(NULL, rowsize(image), colsize(image), depth(image), VFF_TYP_4_BYTE);
      //   result = new int_image(image.get_size());      
      // else
      //   // result = allocimage(NULL, rowsize(image), colsize(image), depth(image), VFF_TYP_1_BYTE);
      //   result = new char_image(image.get_size());
      
      // if (result == NULL)
      // {   
      //   pink_error("%s: allocimage failed");
      // }

      N = image.get_size().prod();//rowsize(image) * colsize(image) * depth(image);
      F = UCHARDATA(image.get_output());

      if (mode == 0)
      {

        int_image rimage( image.get_size() );        

        for (int i = 0; i < N; i++) // inverse l'image
        {
          if (F[i]) F[i] = 0; else F[i] = NDG_MAX;
        }
        
        if (depth(image.get_output()) == 1) /* the image is 2D */
        {
          if (! ldisteuc(image, rimage))
          {
            pink_error("%s: ldisteuc failed");
          }
        }
        else /* NOT the image is 2D */
        {
          if (! ldisteuc3d(image, rimage))
          {
            pink_error("%s: ldisteuc3d failed");
          }
        } /* NOT the image is 2D */

        boost::python::object result (rimage);
        return result;

      } /* mode == 0 */
      else if (mode == 1)
      {

        int_image rimage( image.get_size() );

        for (int i = 0; i < N; i++) // inverse l'image
        {
          if (F[i]) F[i] = 0; else F[i] = NDG_MAX;
        }
        
        if (depth(image.get_output()) == 1) /* the image is 2D */
        {
          if (! ldistquad(image, rimage))
          {
            pink_error("%s: ldistquad failed");
          }
        }
        else /* NOT the image is 2D */
        {
          if (! ldistquad3d(image, rimage))
          {
            pink_error("%s: ldistquad3d failed");
          }
        } /* NOT the image is 2D */

        boost::python::object result (rimage);
        return result;

      }
      else if (mode == 2)
      {

        int_image rimage( image.get_size() );

        for (int i = 0; i < N; i++) // inverse l'image
        {
          if (F[i]) F[i] = 0; else F[i] = NDG_MAX;
        }
        
        if (! lchamfrein(image, rimage))
        {
          pink_error("%s: lchamfrein failed");
        }

        boost::python::object result (rimage);
        return result;

      }
      else if ((mode == 3) || (mode == 5))
      {

        int_image rimage( image.get_size() );        

        if (! lsedt_meijster(image, rimage))
        {
          pink_error("%s: lsedt_meijster failed");
        }
        
        if (mode == 5)
        {
          float_image rfimage = pink::convert2float(rimage);  //convertfloat(&result);
       
          for (int i = 0; i < N; i++)
          {
            rfimage(i) = sqrt(rfimage(i)); // D[i] = static_cast<float>(sqrt(D[i]));
          }
          
          boost::python::object result (rfimage);
          return result;
   
        } /* mode == 5 */

        boost::python::object result (rimage);
        return result;
        
      }
      else if (mode < 40)
      {

        int_image rimage( image.get_size() );        

        for (int i = 0; i < N; i++) // inverse l'image
        {
          if (F[i]) F[i] = 0; else F[i] = NDG_MAX;
        }
        
        if (! ldist(image, mode, rimage))
        {
          pink_error("%s: ldist failed");
        }

        boost::python::object result (rimage);
        return result;

      }
      else /* NOT mode < 40 */
      {
        char_image rimage( image.get_size() );        

        if (! ldistbyte(image, mode, rimage))
        {
          pink_error("%s: ldist failed");
        }

        boost::python::object result (rimage);
        return result;
        
      } /* NOT mode < 40 */

      return boost::python::object();
      
    } /* distc */
  
    

    boost::python::object
    dist(
      const char_image & original_image,
      int mode
      )
    {
      int N;
      uint8_t *F;
      char_image image;
      image = original_image.clone();

      if ((mode != 0) && (mode != 1) && (mode != 2) && (mode != 3) && (mode != 5) &&
          (mode != 4) && (mode != 8) && (mode != 6) && (mode != 18) && (mode != 26) &&
          (mode != 40) && (mode != 80) && (mode != 60) && (mode != 180) && (mode != 260))
      {
        pink_error("filein.pgm mode fileout.pgm"
              "       mode = 0 (dist. eucl. trunc), 1 (dist. eucl. quad.), 2 (chamfrein),\n"
              "              3 (exact eucl. quad.), 5 (exact eucl.), 4, 8 (2D), 6, 18, 26 (3D)\n"
              "                40, 80 (2D), 60, 180, 260 (3D)\n");
      } /* if mode */

      // if (mode < 40)        
      //   // result = allocimage(NULL, rowsize(image), colsize(image), depth(image), VFF_TYP_4_BYTE);
      //   result = new int_image(image.get_size());      
      // else
      //   // result = allocimage(NULL, rowsize(image), colsize(image), depth(image), VFF_TYP_1_BYTE);
      //   result = new char_image(image.get_size());
      
      // if (result == NULL)
      // {   
      //   pink_error("%s: allocimage failed");
      // }

      N = image.get_size().prod();//rowsize(image) * colsize(image) * depth(image);
      F = UCHARDATA(image.get_output());

      if (mode == 0)
      {
        int_image rimage( image.get_size() );
        
        if (depth(image.get_output()) == 1) /* image is 2D */
        {
          if (! ldisteuc(image, rimage))
          {
            pink_error("%s: ldisteuc failed");
          }
        }
        else /* NOT image is 2D */
        {
          if (! ldisteuc3d(image, rimage))
          {
            pink_error("%s: ldisteuc3d failed");
          }
        } /* NOT image is 2D */

        boost::python::object result (rimage);
        return result;
      }
      else if (mode == 1)
      {
        int_image rimage( image.get_size() );        

        if (depth(image.get_output()) == 1) /* image is 2D */
        {
          if (! ldistquad(image, rimage))
          {
            pink_error("%s: ldistquad failed");
          }
        }
        else /* NOT image is 2D */
        {
          if (! ldistquad3d(image, rimage))
          {
            pink_error("%s: ldistquad3d failed");
          }
        } /* NOT image is 2D */

        boost::python::object result (rimage);
        return result;

      }
      else if (mode == 2)
      {
        
        int_image rimage( image.get_size() );

        if (! lchamfrein(image, rimage))
        {
          pink_error("%s: lchamfrein failed");
        }
        
        boost::python::object result (rimage);
        return result;
        
      }
      else if ((mode == 3) || (mode == 5))
      {

        int_image rimage( image.get_size() );  
        
        for (int i = 0; i < N; i++) // inverse l'image
          if (F[i]) F[i] = 0; else F[i] = NDG_MAX;

        if (! lsedt_meijster(image, rimage))
        {
          pink_error("%s: lsedt_meijster failed");
        }
        
        if (mode == 5)
        {

          float_image rfimage = pink::convert2float(rimage);  //convertfloat(&result);
          
          for (int i = 0; i < N; i++)
          {
            rfimage(i) = sqrt(rfimage(i)); // D[i] = static_cast<float>(sqrt(D[i]));
          }
          
          boost::python::object result (rfimage);
          return result;
 
        } /* mode == 5 */

        boost::python::object result (rimage);
        return result;
  
      }
      else if (mode < 40)
      {

        int_image rimage( image.get_size() );        

        if (! ldist(image, mode, rimage))
        {
          pink_error("%s: ldist failed");
        }

        boost::python::object result (rimage);
        return result;

      }
      else /* NOT mode < 40 */
      {
        char_image rimage( image.get_size() );        

        if (! ldistbyte(image, mode, rimage))
        {
          pink_error("%s: ldist failed");
        }
        
        boost::python::object result (rimage);
        return result;

      } /* NOT mode < 40 */

      return boost::python::object();
      
    } /* dist */
  
        
    


  } /* namespace python */
} /* namespace pink */

void distc_export()
{

  UI_DEFINE_ONE_FUNCTION(
    dist,
    pink::python::dist,
    ( arg("image"), arg("mode")),
    doc__dist__c__
    // end of the documenation    
    );
  
  
  UI_DEFINE_ONE_FUNCTION(
    distc,
    pink::python::distc,
    ( arg("image"), arg("mode")),
    doc__distc__c__
    // end of the documenation
    );
  
  
} /* distc_export */











// LuM end of file
