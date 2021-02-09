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
#include "python_doc.h"
#include "ui_skeleton.hpp"


void pyskeleton()
{
            
    boost::python::class_< pink::skel_t<index_t> > (
        "skeleton_t",
        "This is the class for manipulating skeletons.",            
        boost::python::init<>(
            boost::python::args("self"), "Creates an empty skeleton object." )
        )   

        .def( boost::python::init< uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t > (
                  boost::python::args("self","rs", "cs", "ds", "nbvertex", "nbptcell", "nbadjcell", "connex" ),
                  "Creates a skeleton with appropriate parameters" )
            )

        .def( boost::python::init< std::string > (
                  boost::python::args("self","filename"), "Reads a skeleton from a file." )
            )
        

        // SKC_pt_pcell
        // skeladdptcell ( int32_t val, SKC_pt_pcell next ) {
        //     return skeladdptcell( m_skel, next );            
        // }
        
        // SKC_adj_pcell
        // skeladdadjcell( int32_t val, SKC_adj_pcell next ) {
        //     return skeladdadjcell( m_skel, val, next );
        // }

        .def( "addadjlist", &pink::skel_t<pink::char_image>::addadjlist,
              ( boost::python::arg("k"), boost::python::arg("v") ),
              "WRITE ME!!!"
            )

        .def( "addptslist", &pink::skel_t<index_t>::addptslist,
              ( boost::python::arg("k"), boost::python::arg("v") ),
              "WRITE ME!!!"
            )

        .def( "nb_adjacent_elts", &pink::skel_t<index_t>::nb_adjacent_elts,
              ( boost::python::arg("i") ),
              "WRITE ME!!!"
            )

        // .def( "", &pink::skel_t<index_t>::,
        //       ( boost::python::arg("") ),
        //       "WRITE ME!!!"
        //     )

        .def( "writeskel", &pink::skel_t<index_t>::writeskel,
              ( boost::python::arg("filename") ),
              "WRITE ME!!!"
            )
        
        // .def( "writevskel", &pink::skel_t<index_t>::writevskel<char_image>,
        //       ( boost::python::arg("") ),
        //       "WRITE ME!!!"
        //     )

        
        // template <class MT0>
        // void writevskel( std::string filename, MT0 & image ) {
        //     writeskel( m_skel, filename, image );            
        // }

        .def( "skeldelete", &pink::skel_t<index_t>::skeldelete,
              ( boost::python::arg("i") ),
              "WRITE ME!!!"
            )

        .def( "__repr__", &pink::skel_t<index_t>::repr,
              boost::python::args("self"),
              "Prints the skeleton."
            )


        ; // pink::skel_t (skeleton)


    
    boost::python::def( "image2skel", pink::image2skel<pink::char_image>,
                        ( boost::python::arg("image"), boost::python::arg("connex"), boost::python::arg("len") ),
                        doc__pgm2skel__c__
        );

    boost::python::def( "image2skel", pink::image2skel_short<pink::char_image>,
                        ( boost::python::arg("image"), boost::python::arg("connex") ),
                        doc__pgm2skel__c__
        );


} // pyskeleton
        


//LuM end of file
