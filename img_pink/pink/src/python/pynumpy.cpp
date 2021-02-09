/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by applicable law.
  
  (C) UjoImro, 2012 <ujoimro@gmail.com>
  ProCarPlan s.r.o.
*/

// Pink NumPy Conversion routine

#ifdef PINK_HAVE_NUMPY
# include <Python.h>
# include <boost/python.hpp>
# include <boost/cstdint.hpp>
# include <numpy/arrayobject.h>
# include <boost/smart_ptr.hpp>

# include "pink_python.h"
#endif /* PINK_HAVE_NUMPY */

namespace pink {
  namespace python {

    // part of this code has been borrowed from
    // Copyright (c) 2008, Michael Droettboom
#   ifdef PINK_HAVE_NUMPY

    namespace detail {

      template<class T>
      class numpy_type_map {
      public:
        static const int typenum;
      };

      template<>
      const int numpy_type_map<float>::typenum = NPY_FLOAT;

      template<>
      const int numpy_type_map<std::complex<float> >::typenum = NPY_CFLOAT;

      template<>
      const int numpy_type_map<double>::typenum = NPY_DOUBLE;

      template<>
      const int numpy_type_map<std::complex<double> >::typenum = NPY_CDOUBLE;

      template<>
      const int numpy_type_map<long double>::typenum = NPY_LONGDOUBLE;

      template<>
      const int numpy_type_map<std::complex<long double> >::typenum = NPY_CLONGDOUBLE;

      template<>
      const int numpy_type_map<boost::int8_t>::typenum = NPY_INT8;

      template<>
      const int numpy_type_map<boost::uint8_t>::typenum = NPY_UINT8;

      template<>
      const int numpy_type_map<boost::int16_t>::typenum = NPY_INT16;

      template<>
      const int numpy_type_map<boost::uint16_t>::typenum = NPY_UINT16;

      template<>
      const int numpy_type_map<boost::int32_t>::typenum = NPY_INT32;

      template<>
      const int numpy_type_map<boost::uint32_t>::typenum = NPY_UINT32;

      template<>
      const int numpy_type_map<boost::int64_t>::typenum = NPY_INT64;

      template<>
      const int numpy_type_map<boost::uint64_t>::typenum = NPY_UINT64;
    }   

    template <class image_type>
    boost::python::object
    wrap2numpy( image_type & image )
    {
      // the dimension of the image
      index_t nd = image.get_size().size();
      // the sizes of the image
      boost::shared_array<npy_intp> dims( new npy_intp[nd] );            
      for ( index_t q=0; q<nd; ++q ) dims[q] = image.get_size()[q];
            
      PyObject* numpy_array;
      numpy_array =
        PyArray_SimpleNewFromData (
          nd,
          dims.get(),
          detail::numpy_type_map<typename image_type::pixel_type>::typenum,
          reinterpret_cast<void*>(image.get_pixels().get())
          );
      
      if (numpy_array == NULL)
        pink_error("Couldn't allocate the NumPy array.");

      boost::python::handle<> handle(numpy_array); // making boost handle the array as a smart pointer
      boost::python::object result(handle);
      return result;      
    }


    
    template <class image_type>
    boost::python::object
    pink2numpy( image_type & image )
    {
      // the dimension of the image
      index_t nd = image.get_size().size();
      // the sizes of the image
      boost::shared_array<npy_intp> dims( new npy_intp[nd] );            
      for ( index_t q=0; q<nd; ++q ) dims[q] = image.get_size()[q];
            
      PyObject* numpy_array;
      numpy_array =
        PyArray_SimpleNew (
          nd,
          dims.get(),
          detail::numpy_type_map<typename image_type::pixel_type>::typenum
          );

      if (numpy_array == NULL)
        pink_error("Couldn't allocate the NumPy array.");

      if (! PyArray_Check(numpy_array) )
        pink_error("Internal error during the conversion 01.");

      PyArrayObject * tmparray = reinterpret_cast<PyArrayObject*>(numpy_array);

      if (!PyArray_ISCONTIGUOUS(tmparray))
        pink_error("Internal error during the conversion 02.");      
       
      // copying the values to the numpy array
      typedef typename image_type::pixel_type type;      
      type * data = reinterpret_cast<type*>(PyArray_DATA(tmparray));
      
      std::copy( (&image[0]), (&image[image.get_size().prod()]), data );      

      boost::python::handle<> handle(numpy_array); // making boost handle the array as a smart pointer
      boost::python::object result(handle);
      return result;      
    }


    template <class image_type>
    boost::python::object
    numpy2pink_detail( boost::python::object & array )
    {
      if (! PyArray_Check(array.ptr())) // verifying that the object is indeed a numpy array
        pink_error("This function can only convert numpy arrays");
          
      PyArrayObject* original_array = reinterpret_cast<PyArrayObject*>(array.ptr());
      PyArrayObject* tmparray = PyArray_GETCONTIGUOUS(original_array); // making sure that the object is continuous; // if it is continuous than no copy is done here

      if (!PyArray_ISCONTIGUOUS(tmparray)) // the array is not continuous
        pink_error("Internal error, the array should be continuous by now.");

      npy_intp nd = PyArray_NDIM(tmparray);
      npy_intp * dims = PyArray_DIMS(tmparray);
      pink::types::vint dim( nd, 0 );

      FOR(q, nd) dim[q]=dims[q];

      typedef typename image_type::pixel_type pixel_type;
            
      if ( detail::numpy_type_map<pixel_type>::typenum == PyArray_TYPE(tmparray) ) // one more check to be sure
      {
        pixel_type * data = reinterpret_cast<pixel_type *>(PyArray_DATA(tmparray));
        image_type result_image( dim, data, "numpy image" );
        boost::python::object result(result_image);
        return result;
      }
      else
      {
        pink_error("Cannot convert into Pink Image. Internal error 03.");        
      }

      return boost::python::object();      
    } // numpy2pink

    
    
        
    boost::python::object
    numpy2pink( boost::python::object & array )
    {
      if (! PyArray_Check(array.ptr())) // verifying that the object is indeed a numpy array
          pink_error("This function can only convert numpy arrays");

      PyArrayObject* original_array = reinterpret_cast<PyArrayObject*>(array.ptr());
      
      if (PyArray_TYPE(original_array) == NPY_UINT8)
        return numpy2pink_detail<char_image>(array);
      else if (PyArray_TYPE(original_array) == NPY_UINT16)
        return numpy2pink_detail<short_image>(array);
      else if (PyArray_TYPE(original_array) == NPY_INT32)
        return numpy2pink_detail<int_image>(array);
      else if (PyArray_TYPE(original_array) == NPY_FLOAT)
        return numpy2pink_detail<float_image>(array);
      else if (PyArray_TYPE(original_array) == NPY_DOUBLE)
        return numpy2pink_detail<double_image>(array);
      else
        pink_error("Cannot convert into Pink Image. The type of the array is not supported in Pink");        

      return boost::python::object();      
    } // numpy2pink

#   endif /* PINK_HAVE_NUMPY */
  } /* namespace python */
} /* namespace pink */


const char * wrap2numpy_doc =
                     "Converts a pink::ujimage object to a numpy array. The data is NOT copied"
                     ", only the pointer is carried, which means that if you change the numpy "
                     "array the image values change too. In exchange the conversion does no copy and "
                     "is therefore in constant time.";

const char * pink2numpy_doc =
                     "Converts a pink::ujimage object to a numpy array. The data IS copied. If You would like a shallow copy, use wrap2numpy.";

const char * numpy2pink_doc =
                     "Converts a pink::ujimage object to a numpy array. The data IS copied.";

                             
void numpy_export()
{
# ifdef PINK_HAVE_NUMPY
  boost::python::def( "wrap2numpy", &pink::python::wrap2numpy<pink::char_image>,     ( boost::python::arg("image") ), wrap2numpy_doc );
  boost::python::def( "wrap2numpy", &pink::python::wrap2numpy<pink::short_image>,    ( boost::python::arg("image") ), wrap2numpy_doc );
  boost::python::def( "wrap2numpy", &pink::python::wrap2numpy<pink::int_image>,      ( boost::python::arg("image") ), wrap2numpy_doc );
  boost::python::def( "wrap2numpy", &pink::python::wrap2numpy<pink::float_image>,    ( boost::python::arg("image") ), wrap2numpy_doc );
  boost::python::def( "wrap2numpy", &pink::python::wrap2numpy<pink::double_image>,   ( boost::python::arg("image") ), wrap2numpy_doc );
//  boost::python::def( "wrap2numpy", &pink::python::wrap2numpy<pink::fcomplex_image>, ( boost::python::arg("image") ), wrap2numpy_doc );
//  boost::python::def( "wrap2numpy", &pink::python::wrap2numpy<pink::dcomplex_image>, ( boost::python::arg("image") ), wrap2numpy_doc );

  boost::python::def( "numpy2pink", &pink::python::numpy2pink, ( boost::python::arg("array") ), numpy2pink_doc );

  boost::python::def( "pink2numpy", &pink::python::pink2numpy<pink::char_image>,     ( boost::python::arg("image") ), pink2numpy_doc );
  boost::python::def( "pink2numpy", &pink::python::pink2numpy<pink::short_image>,    ( boost::python::arg("image") ), pink2numpy_doc );
  boost::python::def( "pink2numpy", &pink::python::pink2numpy<pink::int_image>,      ( boost::python::arg("image") ), pink2numpy_doc );
  boost::python::def( "pink2numpy", &pink::python::pink2numpy<pink::float_image>,    ( boost::python::arg("image") ), pink2numpy_doc );
  boost::python::def( "pink2numpy", &pink::python::pink2numpy<pink::double_image>,   ( boost::python::arg("image") ), pink2numpy_doc );
//  boost::python::def( "pink2numpy", &pink::python::pink2numpy<pink::fcomplex_image>, ( boost::python::arg("image") ), pink2numpy_doc );
//  boost::python::def( "pink2numpy", &pink::python::pink2numpy<pink::dcomplex_image>, ( boost::python::arg("image") ), pink2numpy_doc );
  
  import_array();  // numpy initialization
# endif /* PINK_HAVE_NUMPY */
} // numpy_export 

// LuM end of file
