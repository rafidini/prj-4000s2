/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by applicable law.
  
  (C) UjoImro, 2012 <ujoimro@gmail.com>
  ProCarPlan s.r.o.
*/

// this file contains the wrapper for Pink's skeleton class


#ifndef PINK_DEVELOPMENT
#  include "pink_python.h"
#else /* PINK_DEVELOPMENT */
#  include "pink_development.h"
#endif /* PINK_DEVELOPMENT */

#include "ujimage.hpp"
#include "ui_graph.hpp"
#include "python_doc.h"
#include "ui_skeleton.hpp"


void pygraph()
{
            
    boost::python::class_< pink::graph_t<index_t> > (
        "graph_t",
        "This is the class for manipulating graphs.",
        boost::python::init<>(
            boost::python::args("self"), "Creates an empty graph object." )
        )   

        .def( boost::python::init< std::string > (
                  boost::python::args("self","filename"), "Reads a skeleton from a file." )
            )
        
        .def( "writegraph", &pink::graph_t<index_t>::writegraph,
              ( boost::python::arg("filename") ),
              "WRITE ME!!!"
            )

        .def( "writeps", &pink::graph_t<index_t>::writeps,
              ( boost::python::arg("filename"), boost::python::arg("scale")=20 ),
              "WRITE ME!!!"
            )

        // .def( "", &pink::graph_t<index_t>::,
        //       ( boost::python::arg("") )
        //       "WRITE ME!!!"
        //     )

        .def( "clone", &pink::graph_t<index_t>::clone,
              "WRITE ME!!!"
            )

        .def( "nb_edges", &pink::graph_t<index_t>::nb_edges,
              "WRITE ME!!!"
            )

        .def( "nb_maxedges", &pink::graph_t<index_t>::nb_maxedges,
              "WRITE ME!!!"
            )

        .def( "nb_nodes", &pink::graph_t<index_t>::nb_nodes,
              "WRITE ME!!!"
            )

        
        // .def( "__repr__", &pink::skel_t<index_t>::repr,
        //       boost::python::args("self"),
        //       "Prints the graph."
        //     )


        ; // pink::skel_t (skeleton)
    
    boost::python::def( "skel2graph", pink::skel2graph<index_t>,
                        ( boost::python::arg("skeleton"), boost::python::arg("mode"), boost::python::arg("param") ),
                        doc__skel2graph__c__
        );

    boost::python::def( "skel2graph", pink::skel2graph_short<index_t>,
                        ( boost::python::arg("skeleton"), boost::python::arg("mode")=0 ),
                        doc__skel2graph__c__
        );



} // pyskeleton
        


//LuM end of file
