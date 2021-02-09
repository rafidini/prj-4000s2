/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by applicable law.
  
  (C) UjoImro, 2010
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/

// boost python wrapper

#include <pink_python.h>

/// !!! macros should be named upper case
#undef border

using namespace boost::python;

namespace pink {
  namespace python {


    // This is a very ugly function; to be revised lates! We create a
    // python list of images, where each list will hold one layer of
    // the multi-band image.
    template <class image_type>
    boost::python::list xvimage_to_python_list( xvimage * image )
    {
#     ifdef UJIMAGE_DEBUG
      std::cout << "creating a list of images\n";
#     endif /* UJIMAGE_DEBUG */
      //the original pointer to the first pixel. We set the pointer at
      //the beginning of the current layer at each iteration. Then we
      //copy-create an image from it. NOTE THIS IS A HACK!
      void * first_pixel = image->image_data;
      long int layer_size =
        /*sizeof(typename image_type::pixel_type) * */
        colsize(image) * rowsize(image) * depth(image) * tsize(image);
/*      * nbands(image);*/
      
#     if UJIMAGE_DEBUG > 2
      _DEBUG(sizeof(typename image_type::pixel_type));
      _DEBUG(colsize(image));
      _DEBUG(rowsize(image));
      _DEBUG(depth(image));
      _DEBUG(tsize(image));
      _DEBUG(nbands(image));
#     endif /* UJIMAGE_DEBUG > 2 */
        
      boost::python::list result;
      
      FOR( q, image->num_data_bands )
      {
        // this changes the pointer to the first pixel of the current layer
        image->image_data = &( reinterpret_cast<typename image_type::pixel_type*>(first_pixel)[q*layer_size] );
        char_image layer(*image);
        result.append(layer);
      } /* FOR q in num_data_bands */

      // we set back the image to the original
      image->image_data = first_pixel;
      return result;
    } /* xvimage_to_python_list */
    
    // HT Experiment
#if 0

    // This is an even uglier function; to be revised later! 
    // it does the reverse of the previous one, so that we can display 
    // colour images for instance
    xvimage * python_list_to_xvimage( boost::python::list imglist )
    {
#     ifdef UJIMAGE_DEBUG
      std::cout << "creating an image from a list\n";
#     endif /* UJIMAGE_DEBUG */
      //the original pointer to the first pixel. We set the pointer at
      //the beginning of the current layer at each iteration. Then we
      //copy-create an image from it. NOTE THIS IS A HACK!
      void * first_pixel = image->image_data;
      long int layer_size =
        /*sizeof(typename image_type::pixel_type) * */
        colsize(image) * rowsize(image) * depth(image) * tsize(image);
/*      * nbands(image);*/
      
#     if UJIMAGE_DEBUG > 2
      _DEBUG(sizeof(typename image_type::pixel_type));
      _DEBUG(colsize(image));
      _DEBUG(rowsize(image));
      _DEBUG(depth(image));
      _DEBUG(tsize(image));
      _DEBUG(nbands(image));
#     endif /* UJIMAGE_DEBUG > 2 */
        
      //boost::python::list result;
      
      int n = boost::python::extract<int>(imglist->attr("__len__")());

      FOR( p, n) {
	xvimage *image = boost::python::extract<char_image>(imglist[p]);
      }

      FOR( q, image->num_data_bands )
      {
        // this changes the pointer to the first pixel of the current layer
        image->image_data = &( reinterpret_cast<typename image_type::pixel_type*>(first_pixel)[q*layer_size] );
        char_image layer(*image);
        result.append(layer);
      } /* FOR q in num_data_bands */

      // we set back the image to the original
      image->image_data = first_pixel;
      return result;
    } /* xvimage_to_python_list */

#endif // zero 
    
// NOTE THIS FUNCTION NEEDS CLEAN-UP !!!!!!    
    boost::python::object py_readimage( std::string filename )
    {
      //boost::python::object * result;
            
      xvimage * tmp = NULL;
      tmp = readimage( filename.c_str() ); // reading the image
      
      if ( tmp == NULL )
      {
        pink_error("error reading file: '" + filename + "'" );
      }

      // there's a special treatment (hack) for multi-band images!
      if (tmp->num_data_bands!=1)
      {
        // in this block the images are multi-band! In python we
        // return a list

#       ifdef UJIMAGE_DEBUG
        std::cout << "there are " << tmp->num_data_bands << " data bands.\n";
#       endif /* UJIMAGE_DEBUG */

        // determining the image type and creating the appropriate
        // python object
        switch (tmp->data_storage_type)
        {        
        case VFF_TYP_1_BYTE: /* pixels are byte (uint8_t) */
        {
          boost::python::list images;
          images = xvimage_to_python_list<char_image>(tmp);
          
          boost::python::object result(images);
          return result;
        }      
        break;
        
        case VFF_TYP_2_BYTE: /* pixels are two byte (int16_t) */
        {        
          boost::python::list images;
          images = xvimage_to_python_list<short_image>(tmp);
                
          boost::python::object result(images);
          return result;
        }      
        break;
        
        case VFF_TYP_4_BYTE: /* pixels are four byte (integer) */
        {
          boost::python::list images;
          images = xvimage_to_python_list<int_image>(tmp);
                
          boost::python::object result(images);
          return result;
        }
        break;
        
        case VFF_TYP_FLOAT: /* pixels are float (single precision) */
        {        
          boost::python::list images;
          images = xvimage_to_python_list<float_image>(tmp);
                
          boost::python::object result(images);
          return result;
        }
        break;
        
        case VFF_TYP_DOUBLE: /* pixels are float (double precision)*/
        {        
          boost::python::list images;
          images = xvimage_to_python_list<double_image>(tmp);
                
          boost::python::object result(images);
          return result;
        }
        break;

        case VFF_TYP_COMPLEX: /* pixels are float (single precision) */
        {         
          boost::python::list images;
          images = xvimage_to_python_list<fcomplex_image>(tmp);
                
          boost::python::object result(images);
          return result;
        }
        break;
        
        case VFF_TYP_DCOMPLEX: /* pixels are float (double precision)*/
        {        
          boost::python::list images;
          images = xvimage_to_python_list<dcomplex_image>(tmp);
                
          boost::python::object result(images);
          return result;
        }
        break;
      
        default:
          pink_error("c_readimage returned bad image type");
        } /* switch im_type */
      
        return boost::python::object(); // default return ( never happens )
      }
      else /* NOT tmp->num_data_bands!=1 */
      {
        // determining the image type and creating
        // the appropriate python object
        switch (tmp->data_storage_type)
        {        
        case VFF_TYP_1_BYTE: /* pixels are byte (uint8_t) */
        {
          char_image a(tmp);
          free(tmp); // NOT freeimage!!!
          boost::python::object result(a); // if there are some exceptions
          return result;
        }
        break;
        
        case VFF_TYP_2_BYTE: /* pixels are two byte (int16_t) */
        {        
          short_image a(tmp);
          free(tmp); // NOT freeimage!!!
          boost::python::object result(a); // if there are some exceptions
          return result;
        }      
        break;
        
        case VFF_TYP_4_BYTE: /* pixels are four byte (integer) */
        {        
          int_image a(tmp);
          free(tmp); // NOT freeimage!!!
          boost::python::object result(a); // if there are some exceptions
          return result;
        }
        break;
        
        case VFF_TYP_FLOAT: /* pixels are float (single precision) */
        {        
          float_image a(tmp);
          free(tmp); // NOT freeimage!!!
          boost::python::object result(a); // if there are some exceptions
          return result;
        }
        break;
        
        case VFF_TYP_DOUBLE: /* pixels are float (double precision)*/
        {        
          double_image a(tmp);
          free(tmp); // NOT freeimage!!!
          boost::python::object result(a); // if there are some exceptions
          return result;
        }
        break;

        case VFF_TYP_COMPLEX: /* pixels are float (single precision) */
        {        
          fcomplex_image a(tmp);
          free(tmp); // NOT freeimage!!!
          boost::python::object result(a); // if there are some exceptions
          return result;
        }
        break;
        
        case VFF_TYP_DCOMPLEX: /* pixels are float (double precision)*/
        {        
          dcomplex_image a(tmp);
          free(tmp); // NOT freeimage!!!
          boost::python::object result(a); // if there are some exceptions
          return result;
        }
        break;
      
        default:
          pink_error("c_readimage returned bad image type");
        } /* switch im_type */
      
        return boost::python::object();

      } /* NOT tmp->num_data_bands!=1 */
      
    } /* py_readimage */
    
  } /* namespace python */
} /* namespace pink */

UI_EXPORT_ONE_FUNCTION (
  readimage,
  pink::python::py_readimage,
  ( arg("filename") ),
  "reads an image from a 'pgm' file and returns an object with the appropriate "
  "type"  
  );






// LuM end of file
