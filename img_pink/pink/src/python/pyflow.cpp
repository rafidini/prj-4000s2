/*
  This software is licensed under
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but
  without any warranty to the extent permitted by applicable law.

  (C) UjoImro, 2011
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/
// This file contains the wrappers of maxflow functions


#include "pink_python.h"

#include "uiFlow.hpp"
#include "uiUngerFlow.hpp"
#include "ui_flow_simd.hpp"
#ifdef PINK_HAVE_OPENCL
# include "ui_flow_opencl.hpp"
#endif /* PINK_HAVE_OPENCL */
#include "ui_flow_distributed.hpp"

using boost::python::arg;
using boost::python::def;

void fmm_export();



void pymaxflow()
{
  def(
    "maxflow",
    pink::maxflow<pink::float_image>,
    (arg("source_and_sink"),
     arg("constraint image"),
     arg("iterations"),
     arg("tau") = 0.132,
     arg("number_of_threads") = 0,
     arg("packet_size") = 8,
     arg("verbose") = false,
     arg("debug") = false ),    
    doc__maxflow__cpp__
    );

# ifdef PINK_HAVE_NUMA
  
  def(
    "distflow",
    pink::numa::distflow<pink::float_image>,
    (arg("source_and_sink"),
     arg("constraint_image"),
     arg("iterations"),
     arg("tau")=0.132,
     arg("number_of_threads")=0,
     arg("verbose") = false,
     arg("debug") = false ),
    doc__maxflow__cpp__
    );

  {    
#   ifdef PINK_HAVE_SIMD  
    def(
      "simdflow",
      pink::numa::simdflow<pink::float_image>,
      (arg("source_and_sink"),
       arg("constraint_image"),
       arg("iterations"),
       arg("tau") = 0.132,
       arg("number_of_threads") = 0,
       arg("hyper_threading") = false,
       arg("verbose") = false,
       arg("debug") = false ),
      doc__maxflow__cpp__
      );
#   endif /* PINK_HAVE_SIMD */
  }
  
  
# endif /* PINK_HAVE_NUMA */
  
  
# ifdef PINK_HAVE_OPENCL
  def(
    "clflow",
    pink::opencl::clflow<pink::float_image>,
    (arg("source_and_sink"),
     arg("constraint_image"),
     arg("iterations"),
     arg("tau") = 0.132,
     arg("verbose") = false,
     arg("debug")   = false,
     arg("device")  = 0 ),    
    doc__maxflow__cpp__
    );
# endif /* PINK_HAVE_OPENCL */
  
  def(
    "ungerflow",
    pink::lungerflow<pink::float_image>,
    (arg("source_and_sink"),
     arg("gradient_image"),
     arg("lambda_image"),
     arg("guidence_image"),
     arg("number_of_iterations"),
     arg("tau"),
     arg("theta") /*,
                    arg("number of threads")=0*/
      ),
    doc__ungerflow__cpp__
    );

  fmm_export();
    
} /* pymaxflow */















// LuM end of file
