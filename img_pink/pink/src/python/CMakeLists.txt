# -*- sh -*-
# UjoImro, 2010

# ------------------------------------
# -------- The Python front-end ------
# ------------------------------------ 
SET( py_documentation ${CMAKE_SOURCE_DIR}/include/python_doc.h )

FILE( GLOB py_src_cpp *.cpp )

if (DEVELOPMENT)
    FILE( GLOB py_src_development_cpp development/*.cpp )
    add_definitions( -DPINK_DEVELOPMENT )
endif (DEVELOPMENT)

add_library( cpp_pink SHARED ${py_src_cpp} ${py_src_development_cpp} ${py_documentation} )
if (NOT THIS_IS_MSVC)
    add_dependencies( cpp_pink pink  )
    target_link_libraries( cpp_pink pink ${LIBS} )
else (NOT THIS_IS_MSVC)
    add_dependencies( cpp_pink pinkstatic  )
    target_link_libraries( cpp_pink pinkstatic ${LIBS} )
endif (NOT THIS_IS_MSVC)

# unix defines
if (CMAKE_HOST_UNIX)
    add_definitions( -DUNIXIO -DPC )
endif (CMAKE_HOST_UNIX)

set( CMAKE_SHARED_LINKER_FLAGS ${CMAKE_SHARED_LINKER_FLAGS} ${PYTHON_FLAG} )
include_directories( ${PYTHON_INCLUDE}  ${BOOST_INCLUDE} )
set( CMAKE_INCLUDE_DIRS_CONFIG ${CMAKE_REQUIRED_INCLUDES} )

if (NOT PYTHON_EXE)
    find_program(PYTHON_EXE ipython)
    if (NOT PYTHON_EXE)
	find_program(PYTHON_EXE python)
    endif (NOT PYTHON_EXE)
endif (NOT PYTHON_EXE)

if (NOT PYTHON_DOCUMENTATION_EXE )
    find_program( PYTHON_DOCUMENTATION_EXE python )
endif (NOT PYTHON_DOCUMENTATION_EXE )

if (NOT PYTHON_EXE)
    message( WARNING "python executable not found. If you have it installed than adjust the PYTHON_EXE variable. You still may be able to compile, but the python front-end will not work." )
endif (NOT PYTHON_EXE)

#python -c "from distutils.sysconfig import get_python_lib; print get_python_lib()"
if (NOT PYTHON_MODULE_INSTALL_PATH )
    if ( NOT PYTHON_DOCUMENTATION_EXE )
	set( PYTHON_MODULE_INSTALL_PATH "UNKNOWN" CACHE STRING "The place to put the modules after the installation" )
    else ( NOT PYTHON_DOCUMENTATION_EXE )
	execute_process ( COMMAND python -c "from distutils.sysconfig import get_python_lib; path=get_python_lib().replace('\\\\','/'); print(path[ path[1:].find('/') + 2 :])" OUTPUT_VARIABLE PYTHON_SITE_PACKAGES OUTPUT_STRIP_TRAILING_WHITESPACE )
	#message( "PYTHON_SITE_PACKAGES = '" ${PYTHON_SITE_PACKAGES} "'" )
	set( PYTHON_MODULE_INSTALL_PATH ${PYTHON_SITE_PACKAGES} CACHE STRING "The place to put the modules after the installation" )
	#message( "PYTHON_MODULE_INSTALL_PATH = '" ${PYTHON_MODULE_INSTALL_PATH} "'" )
    endif ( NOT PYTHON_DOCUMENTATION_EXE )
endif (NOT PYTHON_MODULE_INSTALL_PATH )

if ( NOT CMAKE_INSTALL_PREFIX )
    set( CMAKE_INSTALL_PREFIX "/usr/local" CACHE STRING "The place to put the Pink shared library after the installation" )
endif ( NOT CMAKE_INSTALL_PREFIX )

### **********************************************************************************************
### **********************************************************************************************
### ********************************* PYTHON Documentation ***************************************
### **********************************************************************************************
### **********************************************************************************************

FILE( GLOB pink_commands ${CMAKE_SOURCE_DIR}/src/com/* )

#MESSAGE( "custom command = " ${CMAKE_SOURCE_DIR}/include/python_doc.h  )
#MESSAGE(" pink_commands =" ${pink_commands})
#MESSAGE(" output =" ${CMAKE_SOURCE_DIR}/include/python_doc.h  )
#MESSAGE(" command =" COMMAND ${PYTHON_DOCUMENTATION_EXE} ${CMAKE_SOURCE_DIR}/src/com ${CMAKE_SOURCE_DIR}/include/python_doc.h  )

add_custom_command( OUTPUT ${py_documentation}
                    COMMAND ${PYTHON_DOCUMENTATION_EXE} ${CMAKE_SOURCE_DIR}/python_documenter.py ${CMAKE_SOURCE_DIR}/src/com ${CMAKE_SOURCE_DIR}/include/python_doc.h
		    DEPENDS ${pink_commands}
                    )

# add_custom_command( TARGET pink PRE_LINK 
#                     COMMAND seq 100
#                     COMMAND ${PYTHON_DOCUMENTATION_EXE} ${CMAKE_SOURCE_DIR}/python_documenter.py ${CMAKE_SOURCE_DIR}/src/com ${CMAKE_SOURCE_DIR}/include/python_doc.h
#                     )

add_dependencies( cpp_pink ${py_documentation} )

### **********************************************************************************************
### **********************************************************************************************
### ******************************** Pink.sh Generation for **************************************
### ******************************** local testing ***********************************************
### **********************************************************************************************

if (THIS_IS_LINUX)
add_custom_command( TARGET cpp_pink POST_BUILD 
		    COMMAND cp -r ${CMAKE_SOURCE_DIR}/python/* ${CMAKE_BINARY_DIR}/python/
		    COMMAND rm -r -f `find ${CMAKE_BINARY_DIR}/python/ -type d -name ".svn"`
		    COMMAND echo "\\#\\!" /bin/bash > ${CMAKE_BINARY_DIR}/pink.sh
		    COMMAND echo "\\# UjoImro, 2010"  >> ${CMAKE_BINARY_DIR}/pink.sh
		    COMMAND echo "\\# Generated automatically on"`date --iso-8601` >> ${CMAKE_BINARY_DIR}/pink.sh
		    COMMAND echo "" >> ${CMAKE_BINARY_DIR}/pink.sh
		    COMMAND echo PINKTEST=${PINKTEST_DIRECTORY} "PATH=\\\${PATH}:${CMAKE_BINARY_DIR}/bash LD_LIBRARY_PATH=\\\${LD_LIBRARY_PATH}:${CMAKE_BINARY_DIR}/python:${CMAKE_BINARY_DIR}/lib PYTHONPATH=\\\${PYTHONPATH}:${CMAKE_BINARY_DIR}/python" ${PYTHON_EXE} $$\\@  >> ${CMAKE_BINARY_DIR}/pink.sh
		    COMMAND chmod u+x ${CMAKE_BINARY_DIR}/pink.sh
		    )



elseif (CMAKE_HOST_APPLE)
add_custom_command( TARGET cpp_pink POST_BUILD 
                    COMMAND ln -sf ${CMAKE_BINARY_DIR}/python/libcpp_pink.dylib ${CMAKE_BINARY_DIR}/python/libcpp_pink.so
		    # .svn directories cause permission denied
		    COMMAND cp -r ${CMAKE_SOURCE_DIR}/python/* ${CMAKE_BINARY_DIR}/python/
		    COMMAND rm -r -f `find ${CMAKE_BINARY_DIR}/python/ -type d -name ".svn"`
		    COMMAND echo "\\#\\!" /bin/bash > ${CMAKE_BINARY_DIR}/pink.sh
		    COMMAND echo "\\# UjoImro, 2010"  >> ${CMAKE_BINARY_DIR}/pink.sh
    		    COMMAND echo "\\# Generated automatically on"`date` >> ${CMAKE_BINARY_DIR}/pink.sh
		    COMMAND echo "" >> ${CMAKE_BINARY_DIR}/pink.sh
		    COMMAND echo PINKTEST=${PINKTEST_DIRECTORY} "DYLD_LIBRARY_PATH=\\\${LD_LIBRARY_PATH}:${CMAKE_BINARY_DIR}/python:${CMAKE_BINARY_DIR}/lib PYTHONPATH=\\\${PYTHONPATH}:${CMAKE_BINARY_DIR}/python" ${PYTHON_EXE}  >> ${CMAKE_BINARY_DIR}/pink.sh
		    COMMAND chmod u+x ${CMAKE_BINARY_DIR}/pink.sh
		    )



elseif (THIS_IS_CYGWIN)
add_custom_command( TARGET cpp_pink POST_BUILD 
		    COMMAND cp -r ${CMAKE_SOURCE_DIR}/python/* ${CMAKE_BINARY_DIR}/python/
		    COMMAND echo "\\#\\!" /bin/bash > ${CMAKE_BINARY_DIR}/pink.sh
		    COMMAND echo "\\# UjoImro, 2010"  >> ${CMAKE_BINARY_DIR}/pink.sh
		    COMMAND echo "\\# Generated automatically on"`date --iso-8601` >> ${CMAKE_BINARY_DIR}/pink.sh
		    COMMAND echo "" >> ${CMAKE_BINARY_DIR}/pink.sh
		    COMMAND echo PINKTEST=${PINKTEST_DIRECTORY} "LD_LIBRARY_PATH=\\\${LD_LIBRARY_PATH}:${CMAKE_BINARY_DIR}/python:${CMAKE_BINARY_DIR}/lib PYTHONPATH=\\\${PYTHONPATH}:${CMAKE_BINARY_DIR}/python" ${PYTHON_EXE}  >> ${CMAKE_BINARY_DIR}/pink.sh
		    COMMAND chmod u+x ${CMAKE_BINARY_DIR}/pink.sh
		    )

elseif (THIS_IS_MSVC)
add_custom_command( TARGET cpp_pink POST_BUILD 
		    COMMAND cp -r ${CMAKE_SOURCE_DIR}/python/* ${CMAKE_BINARY_DIR}/python/
		    COMMAND cp ${CMAKE_BINARY_DIR}/python/cpp_pink.dll ${CMAKE_BINARY_DIR}/python/libcpp_pink.pyd
		    )
endif (THIS_IS_LINUX)

### **********************************************************************************************
### **********************************************************************************************
### ******************************** PYTHON Module Install ***************************************
### **********************************************************************************************
### **********************************************************************************************

### The scripts written in Python
INSTALL(
    DIRECTORY ${CMAKE_BINARY_DIR}/python/pink 
    DESTINATION ${PYTHON_MODULE_INSTALL_PATH}
)

if (THIS_IS_MSVC)
    INSTALL(
	FILES ${CMAKE_BINARY_DIR}/python/libcpp_pink.pyd
	DESTINATION ${PYTHON_MODULE_INSTALL_PATH}
	PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE )
else (THIS_IS_MSVC) # on unix like system
    INSTALL(TARGETS cpp_pink
	DESTINATION ${PYTHON_MODULE_INSTALL_PATH}
	PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE )
endif (THIS_IS_MSVC)















# LuM end of file
