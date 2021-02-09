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


#include <pink_python.h>

#include "ui_systest.hpp"

using boost::python::arg;
using boost::python::def;

void pymemtest()
{
  def(
    "memspeed64",
    pink::benchmark::memspeed<double>,
    (arg("memory size (MiB)")=100, arg("number_of_threads")=0),
    "Tests non-numa parallel memory speed of the system."
    );


  def(
    "barispeed",
    pink::benchmark::barispeed<index_t, index_t>,
    (arg("number of threads")=1, arg("number of iterations")=1000),
    "Tests the synchronization time needed for n threads. It is useful to benchmark the "
    "fine-grained--parallel algorithms. "
    );
  
    
#   ifdef PINK_HAVE_NUMA

  def(
    "numaspeed64",
    pink::benchmark::numa::memspeed<double>,
    (arg("memory size (MiB)")=100, arg("number_of_threads")=0, arg("node")=-1),
    "Tests numa parallel memory speed of the system. If the node is specified, than the \n"
    "thread will be restricted to that specific NUMA node. If it's omitted the threads \n"
    "will be optimally distributed.\n"
    );


  def(
    "barinuma",
    pink::benchmark::numa::barispeed<index_t, index_t>,
    (arg("number of threads")=1, arg("number of iterations")=1000),
    "Tests the synchronization time needed for n threads. It is useful to benchmark the "
    "fine-grained--parallel algorithms. This function restricts the threads to their cores. "
    );

  
#   endif /* PINK_HAVE_NUMA */

    
}; /* pymemtest */
















// LuM end of file
