/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by applicable law.
  
  (C) UjoImro, 2009
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/
// boost python wrapper


#include <pink_python.h>

using namespace boost::python;
using namespace pink;

// typedef std::vector<int> vector_int;


void complex_export()
{
  class_< fcomplex >("fcomplex")

    // .add_property("re",
    //               boost::python::make_function( 
    //                 &get_re,
    //                 boost::python::return_value_policy < boost::python::reference_existing_object >()
    //                 ),
    //               &set_re )
  
    .def_readwrite( "re", &fcomplex::re)    
    .def_readwrite( "im", &fcomplex::im)
    .def("__repr__", &repr_fcomplex)
    
    .def(boost::python::self == boost::python::self)
    .def(boost::python::self != boost::python::self)
    
    .def(boost::python::self + boost::python::self)
    .def(boost::python::self - boost::python::self)
    .def(boost::python::self * boost::python::self)
    .def(boost::python::self / boost::python::self)
    
    .def(boost::python::self += boost::python::self)
    .def(boost::python::self -= boost::python::self)
    .def(boost::python::self *= boost::python::self)
    .def(boost::python::self /= boost::python::self)
    ;
  
  class_< dcomplex >("dcomplex")
    
    .def_readwrite( "re", &dcomplex::re)
    .def_readwrite( "im", &dcomplex::im)
    .def("__repr__", &repr_dcomplex)
    
    .def(boost::python::self == boost::python::self)
    .def(boost::python::self != boost::python::self)
    
    .def(boost::python::self + boost::python::self)
    .def(boost::python::self - boost::python::self)
    .def(boost::python::self * boost::python::self)
    .def(boost::python::self / boost::python::self)
    
    .def(boost::python::self += boost::python::self)
    .def(boost::python::self -= boost::python::self)
    .def(boost::python::self *= boost::python::self)
    .def(boost::python::self /= boost::python::self)
    ;
  
} /* fcomplex_export */


// void xvimage_object_export(){
//   class_< xvimage, PTR<xvimage> >(
//     "xvimage",
//     "The C image struct of pink. It shouldn't be accessed directly from python. "
//     "Use one of the 'ujoi' objects instead."
//     );  
// };


// void shallow_xvimage_object_export() {



//   class_< shallow_xvimage, PTR<shallow_xvimage>, bases< xvimage > >( 
//     "shallow_xvimage",     
//     "The C image struct of pink, armed with some methods, like it can tell it's type. "
//     "Be carefull, it does not contain or allocate any data! It's mainly used in the ujoi objects. "
//     "It shouldn't be accessed directly from python. "
//     "Use one of the 'ujoi' objects instead. "
//     )



//     .def( "imtype", &shallow_xvimage::imtype,
// 	  "returns the image type of the object. Current types are: "
// 	  "uint8_t, int16_t, int32_t, float, double "
//       )
//     ;



// };



// void deep_xvimage_object_export(){



//   class_< deep_xvimage, PTR<deep_xvimage>, bases<xvimage> >( 
//     "deep_xvimage",     
//     "The C image struct of pink, armed with some methods, like it can tell it's type."
//     "This child allocates the data! It's used mainly in readimage. "
//     "It shouldn't be accessed directly from python. "
//     "Use one of the 'ujoi' objects instead."
//     )



//     .def( "imtype", &deep_xvimage::imtype,
// 	  "returns the image type of the object. Current types are: "
// 	  " uint8_t, int16_t, int32_t, float, double"
//       )

    
//     ;

// };






































// LuM end of file
