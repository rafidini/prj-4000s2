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
// this file opens a file with raw image data as an image of given type

#include <fstream>
#include <iostream>

#include "uiFileIO.hpp"
#include "pink_python.h"


using namespace boost::python;
using namespace pink;

namespace pink {
  namespace python {

    template <class image_type>
    image_type read_raw_image(
      const std::string & filename,
      const boost::python::list & python_dim
      )
    {
  
      typedef typename image_type::pixel_type pixel_type;

      pink::types::vint dim(python_dim);
  
      std::ifstream file;
  
      if ( file_size(filename) != sizeof(pixel_type) * dim.prod() )
      {
        std::cout << "requested file size = " << sizeof(pixel_type) * dim.prod() << "\n";
        std::cout << "but '" << filename << "' is only " << file_size(filename) << " byte-long\n"; 
        pink_error( "the file size does not equal with the image size");
      } /* if */

      file.open( filename.c_str(), std::ios_base::binary );
  
      boost::shared_array<pixel_type> data(new pixel_type[dim.prod()]);
    
      try 
      {    
        file.read( 
          reinterpret_cast<char*>(data.get()), 
          sizeof(pixel_type)*dim.prod()
          );
      }
      catch (...)
      {
        pink_error("the raw data couldn't be read from the file");
      }
  

      file.close();
  
      image_type result( dim, data );

      return result;
  
    } /* read_raw_image */



    boost::python::object py_read_raw_image (
      const std::string & filename,
      const boost::python::list & python_dim,
      const int inttype
      )
    {
      boost::python::object * result;

      // determining the image type and creating
      // the appropriate python object
      switch (inttype)
      {        
      case VFF_TYP_1_BYTE: /* pixels are byte (uint8_t) */
      {
        char_image a = read_raw_image<char_image>(filename, python_dim);
        result = new boost::python::object(a);
      }      
      break;
        
      case VFF_TYP_2_BYTE: /* pixels are two byte (int16_t) */
      {
        short_image a = read_raw_image<short_image>(filename, python_dim);
        result = new boost::python::object(a);                
      }      
      break;
        
      case VFF_TYP_4_BYTE: /* pixels are four byte (integer) */
      {        
        int_image a = read_raw_image<int_image>(filename, python_dim);
        result = new boost::python::object(a);
      }
      break;
        
      case VFF_TYP_FLOAT: /* pixels are float (single precision) */
      {
        float_image a = read_raw_image<float_image>(filename, python_dim);
        result = new boost::python::object(a);

      }
      break;
        
      case VFF_TYP_DOUBLE: /* pixels are float (double precision)*/
      {
        double_image a = read_raw_image<double_image>(filename, python_dim);
        result = new boost::python::object(a);
      }
      break;

      case VFF_TYP_COMPLEX: /* pixels are float (single precision) */
      {
        fcomplex_image a = read_raw_image<fcomplex_image>(filename, python_dim);
        result = new boost::python::object(a);        
      }
      break;
        
      case VFF_TYP_DCOMPLEX: /* pixels are float (double precision)*/
      {
        dcomplex_image a = read_raw_image<dcomplex_image>(filename, python_dim);
        result = new boost::python::object(a);
      }
      break;
      
      default:
        pink_error("c_readimage returned bad image type");
      } /* switch im_type */
      
      return *result;      
    } /* py_read_raw_image */
    



  } /* namespace python */
} /* namespace pink */


void read_raw_image_export()
{
  boost::python::def(
    "read_raw_image",
    pink::python::py_read_raw_image, 
    ( arg("filename"), arg("dimensions of the image"), arg("image mode") = VFF_TYP_1_BYTE),
    "this function reads a raw image from a file the"
    " image type is specified by the suffix and the "
    "size is given in the 'dim' argument\n" );
  
}




// LuM end of file
