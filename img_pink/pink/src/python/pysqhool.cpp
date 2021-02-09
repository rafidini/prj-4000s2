// UjoImro, 2009
// This software is meant to be free
// this file contains the prototype for wrapping the uisqhool object using different pixel types
// I think it's doable with templates, but the way is not quite clear, 
// so in the mean time I'll just use c++'s define directive
// and here is how it's done


#include "uiSqhool.hpp"
#include "pink_python.h"


using namespace boost::python;
using namespace pink;


/*
***************************************************************************************
***************************************************************************************
***************************************************************************************


Python class pink.uiSqhool / c++ pink::uiSqhool


***************************************************************************************
***************************************************************************************
***************************************************************************************
*/

//void (uiSqhool::*char_insert_image)(const char_image &, const std::string &, std::string ) = &uiSqhool::template insert_image<char_image>;

void uiSqhool_object_export() {

  class_< uiSqhool, boost::shared_ptr< uiSqhool > > (
    "cpp_uiSqhool",
    "This class provides an interface for the ujif (UJoimro Image File) file type.",
    init<>(args("self"), "Default constructor. Creates an empty unattached 'uiSqhool' object.")
    )
    
    
    
    .def( init< const std::string &, const std::string &, const std::string &, bool > (
	    args( "filename", "creator = ''", "description = ''", "create = false" ),
	    "Creation of a sqhool object and attaching it to a file. If the file does not \n"
	    "exist it will be created, if the file exists then it will be rewritten or reused \n"
	    "according to the attributes. If the create parameter is false, then creator and \n"
	    "description are ignored. \n"
	    "@param  filename The filename of the image with the relative path. \n"
	    "@param  creator The creator of the image. Can be arbitrary, but tipically \n"
	    "`whoami`/uiFibres \n"
	    "@param  description The description of the image. \n"
	    "@param  create If true a new file will be created. If the file already exists, \n"
	    "than it will be deleted first. \n"
	    "If false then the file will be opened read and write. If the file does not exist \n"
	    "it will complain."
	    )
      )



    .def( "init", &uiSqhool::init, 
	  args( "filename", "creator = ''", "description = ''", "create = false" ),
	  "This is the initialization function. It can be used when the class has been \n"
	  "created with the empty constructor. If the class is created with \n"
	  "sqhool(filename, create), then this function is called automatically. It uses \n"
	  "the same parametrization as uiSqhool(...). \n"
	  "It returns true if the operation succeded. \n"
	  "@return bool \n"
	  "@param  filename The filename of the image with the full path. \n"
	  "@param  creator \n"
	  "@param  description \n"
	  "@param  create"
      )





    .def( "set_comment", &uiSqhool::set_comment, 
	  args("self", "ID", "comment"),
	  "////////!!!!!!!"
      )


    .def( "set_log", &uiSqhool::set_log, 
	  args("self", "ID", "comment"),
	  "////////!!!!!!!"
      )

    .def( "set_command", &uiSqhool::set_command, 
	  args("self", "ID", "command"),
	  ""
	  "////////!!!!!!!"
      )

    .def( "set_substitution", &uiSqhool::set_substitution, 
	  args("self", "ID_final_image", "ID_source_image", "name_source_in_command"),
	       ""
	       "////////!!!!!!!"
	    
      )


    .def( "get_image_type", &uiSqhool::get_image_type, 
	  args("self", "ID"),
	  "returns the image type of the image with ID"
      )

    .def( "get_commands", &uiSqhool::get_commands, 
	  args("self", "ID"),
	  "////////!!!!!!!" 
      )


    .def( "list_images", &uiSqhool::list_images, 
	  args("self"),	  
	  "////////!!!!!!!" 
      )


    .def( "insert_image_char", &uiSqhool::insert_image<char_image>, 
	 args("self", "image", "relative_path", "full_path"),
	  "////////!!!!!!!" 
      )
	  
   .def( "insert_image_short", &uiSqhool::insert_image<short_image>, 
	 args("self", "image", "relative_path", "full_path"),
	  "////////!!!!!!!" 
      )

   .def( "insert_image_int", &uiSqhool::insert_image<int_image>, 
	 args("self", "image", "relative_path", "full_path"),
	  "////////!!!!!!!" 
      )

   .def( "insert_image_float", &uiSqhool::insert_image<float_image>, 
	 args("self", "image", "relative_path", "full_path"),
	  "////////!!!!!!!" 
      )

   .def( "insert_image_double", &uiSqhool::insert_image<double_image>, 
	 args("self", "image", "relative_path", "full_path"),
	  "////////!!!!!!!" 
      )




   .def( "get_image_char", &uiSqhool::get_image<char_image>, 
	  args("self", "ID" ),
	  "////////!!!!!!!" 
      )
	  
   .def( "get_image_short", &uiSqhool::get_image<short_image>, 
	  args("self", "ID" ),
	  "////////!!!!!!!" 
      )

   .def( "get_image_int", &uiSqhool::get_image<int_image>, 
	  args("self", "ID" ),
	  "////////!!!!!!!" 
      )

   .def( "get_image_float", &uiSqhool::get_image<float_image>, 
	  args("self", "ID" ),
	  "////////!!!!!!!" 
      )

   .def( "get_image_double", &uiSqhool::get_image<double_image>, 
	  args("self", "ID" ),
	  "////////!!!!!!!" 
     )
    
    ;



}; /* uiSqhool_object_export */











































































//LuM end of file
