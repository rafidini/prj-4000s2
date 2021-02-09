# -*- sh -*-
# UjoImro, 2010

#message("Abandon every hope, ye who entereth here.")

#message(CMAKE_SYSTEM_TYPE\ ${CMAKE_SYSTEM_TYPE} )
#message(CMAKE_HOST_SYSTEM_NAME\ ${CMAKE_HOST_SYSTEM_NAME} )
#message(CMAKE_HOST_SYSTEM\ ${CMAKE_HOST_SYSTEM} )
#message( APPLE\ ${APPLE} )
#message( CMAKE_HOST_APPLE\ ${CMAKE_HOST_APPLE} )
#message( CMAKE_HOST_WIN32\ ${CMAKE_HOST_WIN32} )
#message( CMAKE_HOST_UNIX\ ${CMAKE_HOST_UNIX} )

### **********************************************************************************************
### **********************************************************************************************
### *************************** Generalities *****************************************************
### **********************************************************************************************
### **********************************************************************************************

cmake_minimum_required (VERSION 2.6)
project(pink-image)
INCLUDE (UJ_CheckIncludeFile.cmake)
INCLUDE (UJ_CheckIncludeFileCXX.cmake)
#INCLUDE (FindCFITSIO.cmake)

set( POTENTIAL_LIBDIRS /usr/lib /usr/local/lib /opt/local/lib /usr/lib64 /usr/local/lib64 /opt/local/lib64 /Library )
set( INCLUDES include src/python /usr/include /usr/local/include )

include_directories( ${INCLUDES} )

## This does not work
### I didn't think of this myself (HT)
##set(CMAKE_MODULE_PATH "${CMAKE_ROOT}/Modules")
###message("1- CMAKE module path = " ${CMAKE_MODULE_PATH})
##set(EXTRA_MODULES "${pink_SOURCE_DIR}")
###message("2- EXTRA_MODULES = " ${EXTRA_MODULES})
##set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}  "${EXTRA_MODULES}" )
##message("3- new CMAKE module path = " ${CMAKE_MODULE_PATH})

set(CFITSIO_DIR "${pink_SOURCE_DIR}")


# --- Library extention (.so, .dylib, .a) ---

string( COMPARE EQUAL ${CMAKE_HOST_SYSTEM_NAME} Linux THIS_IS_LINUX )
set( THIS_IS_MSVC False )
set( THIS_IS_CYGWIN False )

if (THIS_IS_LINUX)
    set( LIBEXT so )
elseif (CMAKE_HOST_APPLE)
    set( LIBEXT dylib )
elseif (CMAKE_HOST_WIN32)
    set( LIBEXT lib ) # extension for MSVC
    if (CMAKE_HOST_CYGWIN)
	set( THIS_IS_CYGWIN True )
    else (CMAKE_HOST_CYGWIN)
	set( THIS_IS_MSVC True )
    endif(CMAKE_HOST_CYGWIN)
endif (THIS_IS_LINUX)




### **********************************************************************************************
### **********************************************************************************************
### *************************** General Options **************************************************
### **********************************************************************************************
### **********************************************************************************************

if(NOT BOOST_INCLUDEDIR)
    SET(BOOST_INCLUDEDIR "/usr/include" CACHE PATH "The directory of the boost headers. Remember in the case of /usr/local/include/boost, you specify /usr/local/include." )
endif(NOT BOOST_INCLUDEDIR)

if(NOT GSL_INCLUDE)
    SET(GSL_INCLUDE "/usr/include" CACHE PATH "The directory of the gsl library headers." )
endif(NOT GSL_INCLUDE)

option( 64BIT "Compiles Pink in 64bit mode. This is necessary for files which could be larger than 2GB. Note that the library always obey the system architecture." ON )

if (CMAKE_HOST_UNIX) ### We build with gcc
    # default optimization options
    SET( CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -msse -msse2" CACHE STRING "Optimization flags for the C++ compiler in Release mode" FORCE )
    SET( CMAKE_C_FLAGS_RELEASE   "-O3 -DNDEBUG -msse -msse2" CACHE STRING "Optimization flags for the C   compiler in Release mode" FORCE )
else (CMAKE_HOST_UNIX) ## We build with MSVC
    SET( CMAKE_CXX_FLAGS_RELEASE " /O2 -DNDEBUG /MT " CACHE STRING "Optimization flags for the C++ compiler in Release mode" FORCE )
    SET( CMAKE_C_FLAGS_RELEASE   " /O2 -DNDEBUG /MT " CACHE STRING "Optimization flags for the C   compiler in Release mode" FORCE )
    SET( PINK_LINKING_FLAGS " /STACK:10000000 /machine:x64 /NODEFAULTLIB:msvcrt.lib /NODEFAULTLIB:libc.lib, /NODEFAULTLIB:msvcrt.lib, /NODEFAULTLIB:libcd.lib, /NODEFAULTLIB:libcmtd.lib, /NODEFAULTLIB:msvcrtd.lib /NODEFAULTLIB:msvcprt.lib /VERBOSE:LIB " )
    SET( CMAKE_LINKER_FLAGS ${PINK_LINKING_FLAGS} CACHE STRING "default linking flags" FORCE )
    SET( CMAKE_LINKER_FLAGS_RELEASE ${PINK_LINKING_FLAGS} CACHE STRING "default linking flags" FORCE )
    SET( CMAKE_MODULE_LINKER_FLAGS ${PINK_LINKING_FLAGS} CACHE STRING "default linking flags" FORCE )
    SET( CMAKE_MODULE_LINKER_FLAGS_RELEASE ${PINK_LINKING_FLAGS} CACHE STRING "default linking flags" FORCE )
    SET( CMAKE_SHARED_LINKER_FLAGS ${PINK_LINKING_FLAGS} CACHE STRING "default linking flags" FORCE )
    SET( CMAKE_SHARED_LINKER_FLAGS_RELEASE ${PINK_LINKING_FLAGS} CACHE STRING "default linking flags" FORCE )
endif (CMAKE_HOST_UNIX)

if(NOT CMAKE_BUILD_TYPE)
    SET( CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build, options are: Debug Release" FORCE )
endif(NOT CMAKE_BUILD_TYPE)

if (CMAKE_MINOR_VERSION>=8) # cmake 2.6 required; cmake 2.8 or newer desirable
SET_PROPERTY( CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Release" "Debug" )
endif (CMAKE_MINOR_VERSION>=8)

### **********************************************************************************************
### **********************************************************************************************
### *************************** Functions in development *****************************************
### **********************************************************************************************
### **********************************************************************************************

option( DEVELOPMENT "Compiles additional functions which are not-yet part of the core library (Mostly because they require additional dependencies). " OFF )
option( OPENCL "Compiles the functions which use OpenCL. " OFF )
option( NUMA "Compiles the functions which use The NUMA api. " OFF )
option( SIMD "Compiles the functions with the SIMD (SSE2, SSE3, SSE4, SSE4.1) instruction set. " ON )

## LIAR is incomporated in the PYTHON_FRONT_END
# option( LIAR "Compiles additional functions adapted from the Library of Image Analysis Routines," OFF)

option( PYTHON_FRONT_END "Compile the python front-end and the cpp functions" OFF )

### **********************************************************************************************
### **********************************************************************************************
### *************************** Library **********************************************************
### **********************************************************************************************
### **********************************************************************************************

if (64BIT)
    add_definitions( -DMC_64_BITS )
endif (64BIT)

if ( PYTHON_FRONT_END )
    add_definitions(-DPINK_HAVE_PYTHON)
endif( PYTHON_FRONT_END )

if (CMAKE_HOST_UNIX)

    CHECK_INCLUDE_FILE( X11/Xlib.h  HTEST1 ) 
    if (NOT HTEST1) 
	message( FATAL_ERROR "error X11 header file X11/Xlib.h not found") 
    endif(NOT HTEST1)

    CHECK_INCLUDE_FILE( curses.h  HTEST101 ) 
    if (NOT HTEST101) 
	message( FATAL_ERROR "error ncurses header file curses.h not found") 
    endif(NOT HTEST101)
	
    CHECK_INCLUDE_FILE( X11/Xutil.h  HTEST2 )
    if (NOT HTEST2)
	message( FATAL_ERROR "error X11 header file X11/Xutil.h not found")
    endif(NOT HTEST2)

    # unix defines
    add_definitions( -DUNIXIO -DPC )

    find_library( MATH_LIB libm.${LIBEXT} PATHS ${POTENTIAL_LIBDIRS} )
    set( LIBS ${MATH_LIB} )

endif (CMAKE_HOST_UNIX)

if (THIS_IS_MSVC)
    add_definitions( -D_WINDOWS -DWIN32 )
endif(THIS_IS_MSVC)
    
##
## We need the optional CFITSIO library for reading the 
## FITS image format, widely used in astronomy
## Hugues Talbot 9 Mar 2011
##
find_package(CFITSIO)

if (HT_CFITSIO_FOUND)
    #message("cfitsio found, library= " ${CFITSIO_LIBRARY})
    add_definitions(-DHAVE_FITS_LIB)
    set( LIBS ${LIBS} ${CFITSIO_LIBRARY})
else (HT_CFITSIO_FOUND)
    #message("cfitsio library not found, FITS handling disabled")
endif (HT_CFITSIO_FOUND)

#find_library( STDCPP_LIB libstdc++.${LIBEXT} PATHS ${POTENTIAL_LIBDIRS} )
#set( LIBS ${STDCPP_LIB} )


if (CMAKE_HOST_UNIX) # These packages certainly will not be available   
    # Libraries required to compile the pink library
    find_package( X11 REQUIRED )
    find_package( Threads REQUIRED )

    set( LIBS ${LIBS} ${X11_LIBRARIES} )
    set( LIBS ${LIBS} ${THREADS_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} )
endif (CMAKE_HOST_UNIX)

### 
### Option LIAR
###
    
## no longer an option
##    if (PYTHON_FRONT_END) # LIAR obsolete
	
## we need POSIX semaphores to communicate with imview
CHECK_INCLUDE_FILE(semaphore.h SEMTEST)
if (NOT SEMTEST)
    message("POSIX semaphores not found")
else (NOT SEMTEST)
    add_definitions( -DHAVE_POSIX_IPC )
endif(NOT SEMTEST)

CHECK_INCLUDE_FILE(sys/sem.h SYSVTEST)
if (NOT SYSVTEST)
    message("SysV IPC not found")
else (NOT SYSVTEST)
    add_definitions(-DHAVE_SYSV_IPC)
endif(NOT SYSVTEST)

if (THIS_IS_LINUX)
    find_library( POSIX_REALTIME_LIB librt.${LIBEXT} PATHS ${POTENTIAL_LIBDIRS} )
    set( LIBS ${LIBS} ${POSIX_REALTIME_LIB} )
endif (THIS_IS_LINUX)

if (THIS_IS_MSVC)
    find_library( WINSOCK_LIBRARY winsock.lib )
    find_library( WSOCK32_LIBRARY wsock32.lib )
    find_library( WSOCK64_LIBRARY wsock64.lib )

    SET( PINK_WINSOCK_FOUND FALSE )
    if (WINSOCK_LIBRARY)
	set( LIBS ${LIBS} ${WINSOCK_LIBRARY} )
	set( PINK_WINSOCK_FOUND TRUE )
    endif (WINSOCK_LIBRARY)

    if (WSOCK32_LIBRARY)
	set( LIBS ${LIBS} ${WSOCK32_LIBRARY} )
	set( PINK_WINSOCK_FOUND TRUE )
    endif (WSOCK32_LIBRARY)

    if (WSOCK64_LIBRARY)
	set( LIBS ${LIBS} ${WSOCK64_LIBRARY} )
	set( PINK_WINSOCK64_FOUND TRUE )
    endif (WSOCK64_LIBRARY)

    if (NOT PINK_WINSOCK_FOUND)
	message( FATAL "error: The Windows(tm) socket library could not be found. The compilation will fail." )
    endif (NOT PINK_WINSOCK_FOUND)
    

endif (THIS_IS_MSVC)

FIND_PACKAGE(TIFF)

if (TIFF_FOUND)
    #message("Found libtiff headers in ${TIFF_INCLUDE_DIR}, library at ${TIFF_LIBRARIES}")
    ADD_DEFINITIONS(-DHAVE_TIFF_LIB)
    INCLUDE_DIRECTORIES(${TIFF_INCLUDE_DIR})
    set( LIBS ${LIBS} ${TIFF_LIBRARIES} )
else(TIFF_FOUND)
    message( "Libtiff not found. Please check your installation")
endif(TIFF_FOUND)

### the LIAR is no longer an option 
### endif (PYTHON_FRONT_END) # end of LIAR

## the GNU Scientific Library is no longer an option either
## this is unfortunate.

include_directories( ${GSL_INCLUDE} )

find_library( GSL_LIB libgsl.${LIBEXT} ${POTENTIAL_LIBDIRS} )
set( LIBS ${LIBS} ${GSL_LIB} )

find_library( GSLCBLAS_LIB libgslcblas.${LIBEXT} ${POTENTIAL_LIBDIRS} )
set( LIBS ${LIBS} ${GSLCBLAS_LIB} )

find_library( LAPACK_LIB liblapack.${LIBEXT} ${POTENTIAL_LIBDIRS} )
set( LIBS ${LIBS} ${LAPACK_LIB} )

if (THIS_IS_MSVC)
    find_library( BLAS_LIB blas.${LIBEXT} ${POTENTIAL_LIBDIRS} )
    set( LIBS ${LIBS} ${BLAS_LIB} )

    find_library( LIBF2C_LIB libf2c.${LIBEXT} ${POTENTIAL_LIBDIRS} )
    set( LIBS ${LIBS} ${LIBF2C_LIB} )
endif (THIS_IS_MSVC)

CHECK_INCLUDE_FILE( gsl/gsl_interp.h HTEST )
if (NOT HTEST)
    message( FATAL_ERROR "gsl header file failed to compile. If it is installed, than check the gsl flags") 
endif(NOT HTEST)

### ******************************************************
### *************** Option Development *******************
### ******************************************************
if (DEVELOPMENT)
    if(NOT EIGEN_INCLUDE)
        SET(EIGEN_INCLUDE "/usr/include" CACHE PATH "The directory of the eigen2 headers. Remember in the case of /usr/local/include/eigen2, you specify /usr/local/include." )
    endif(NOT EIGEN_INCLUDE)

    include_directories( ${EIGEN_INCLUDE} )

    SET( CMAKE_REQUIRED_INCLUDES ${CMAKE_REQUIRED_INCLUDE} ${EIGEN_INCLUDE} )

    CHECK_INCLUDE_FILE_CXX( eigen2/Eigen/Core HTEST12 ${CMAKE_REQUIRED_FLAGS} )
    if (NOT HTEST12)
 	message( FATAL_ERROR "error eigen2/Eigen/Core does not compile. Check if its correctly installed. You may need a newer version. ")
    endif(NOT HTEST12)

    CHECK_INCLUDE_FILE_CXX( eigen2/Eigen/LU HTEST14  ${CMAKE_REQUIRED_FLAGS} )
    if (NOT HTEST14)
 	message( FATAL_ERROR "error eigen2/Eigen/LU does not compile. Check if its correctly installed. You may need a newer version. ")
    endif(NOT HTEST14)

endif (DEVELOPMENT)

if (OPENCL)
    # header
    CHECK_INCLUDE_FILE_CXX( CL/cl.h HTEST17 )
    if (NOT HTEST17)
 	message( FATAL_ERROR "error CL/cl.h does not compile. Check if its correctly installed. You may need a newer version. ")
    endif(NOT HTEST17)
	    
    # library 
    find_library( OPENCL_LIB libOpenCL.${LIBEXT} ${POTENTIAL_LIBDIRS} )
    set( LIBS ${LIBS} ${OPENCL_LIB} )
	    
    # signalling the compilation that we desire the opencl functions
    ADD_DEFINITIONS(-DPINK_HAVE_OPENCL)
endif(OPENCL)

if (NUMA)
    find_library( NUMA_LIB libnuma.${LIBEXT} ${POTENTIAL_LIBDIRS} )
    set( LIBS ${LIBS} ${NUMA_LIB} )

    CHECK_INCLUDE_FILE_CXX( numa.h HTEST16 )
    if (NOT HTEST16)
 	message( FATAL_ERROR "error numa.h does not compile. Check if its correctly installed. You may need a newer version. ")
    endif(NOT HTEST16)
    ADD_DEFINITIONS(-DPINK_HAVE_NUMA)
endif (NUMA)

if (SIMD)
    ADD_DEFINITIONS(-DPINK_HAVE_SIMD)
endif (SIMD)

add_subdirectory( src/lib lib )


### **********************************************************************************************
### **********************************************************************************************
### *************************** Bash front-end ***************************************************
### **********************************************************************************************
### **********************************************************************************************

option( BASH_FRONT_END "Compiles the executable files if you switch this of you can still compile the Python front-end and the shared library." OFF )

if (BASH_FRONT_END)
    add_subdirectory( src/com bash )
endif(BASH_FRONT_END)


### **********************************************************************************************
### **********************************************************************************************
### *************************** Python front-end *************************************************
### **********************************************************************************************
### **********************************************************************************************

if ( PYTHON_FRONT_END )

    add_definitions(-DPINK_HAVE_PYTHON)
    option( NUMPY "Compiles numpy<->pink conversion utilities." OFF )

    find_package(PythonInterp)

    if(NOT PYTHON_LIBRARY)
	find_package(PythonLibs)
	SET(PYTHON_LIBRARY ${PYTHON_LIBRARIES} CACHE FILEPATH "The python library.")
    endif(NOT PYTHON_LIBRARY)

    if(NOT PYTHON_INCLUDE)
	find_package(PythonLibs)
	SET(PYTHON_INCLUDE ${PYTHON_INCLUDE_PATH} CACHE PATH "The directory of the python headers." )
    endif(NOT PYTHON_INCLUDE)

    find_package(Boost COMPONENTS python thread )

    ## The following test fails on centos: cmake 2.6 patch 2
    # if ( (NOT Boost_FOUND) AND ( (NOT Boost_PYTHON) OR (NOT Boost_THREAD) ) )
    if(NOT Boost_FOUND) 
	if (NOT Boost_PYTHON)
	    #${Boost_PYTHON_LIBRARY} ${Boost_THREAD_LIBRARY}
	    SET( Boost_PYTHON "NOT FOUND" CACHE FILEPATH "The boost.python library.")
	    SET( Boost_THREAD "NOT FOUND" CACHE FILEPATH "The boost.thread library.")
	    message( FATAL_ERROR "Boost libraries where not found. If they are installed, please set the Boost_PYTHON and the Boost_THREAD variables.")
	endif(NOT Boost_PYTHON)
    endif(NOT Boost_FOUND)

    # # required version of BOOST 1.35 or newer
    #set( Boost_MAJOR_VERSION_REQUIRED 1 )
    #set( Boost_MINOR_VERSION_REQUIRED 37 )

    #    if ( Boost_MAJOR_VERSION LESS Boost_MAJOR_VERSION_REQUIRED )
    #        message( FATAL_ERROR "For the core components you need a boost version at least " ${Boost_MAJOR_VERSION_REQUIRED} "." ${Boost_MINOR_VERSION_REQUIRED} )
    #    elseif ( Boost_MINOR_VERSION LESS Boost_MINOR_VERSION_REQUIRED )
    #        message( FATAL_ERROR "For the core components you need a boost version at least " ${Boost_MAJOR_VERSION_REQUIRED} "." ${Boost_MINOR_VERSION_REQUIRED} )
    #    endif ( Boost_MAJOR_VERSION LESS Boost_MAJOR_VERSION_REQUIRED )

    set( LIBS ${LIBS} ${Boost_LIBRARIES} ${Boost_PYTHON} ${Boost_THREAD} ${PYTHON_LIBRARY} )

    #message( STATUS "Boost_LIBRARIES=" ${Boost_LIBRARIES} )
    #message( STATUS "Boost_LIBRARIES=" ${Boost_LIBRARIES} )
    #message( STATUS "Boost_PYTHON_LIBRARY=" ${Boost_PYTHON_LIBRARY} )

    # looking for the python library 
    if (NOT THIS_IS_MSVC)
	set( CMAKE_REQUIRED_FLAGS ${CMAKE_REQUIRED_FLAGS} ${PYTHON_LIBRARY} ${LIBS} ${Boost_PYTHON_LIBRARY} ${Boost_THREAD_LIBRARY} ${Boost_PYTHON} ${Boost_THREAD} )
    else (NOT THIS_IS_MSVC)
	set( CMAKE_REQUIRED_FLAGS ${CMAKE_REQUIRED_FLAGS} ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE}  )
    endif (NOT THIS_IS_MSVC)
    set( CMAKE_REQUIRED_INCLUDES  ${Boost_INCLUDE_DIR} ${CMAKE_REQUIRED_INCLUDES} ${INCLUDES} ${PYTHON_INCLUDE} ${EIGEN_INCLUDE} )
    include_directories( ${CMAKE_REQUIRED_INCLUDES} )


    CHECK_INCLUDE_FILE( ${PYTHON_INCLUDE}/pyconfig.h HTEST )
    if (NOT HTEST)
	message( ${PYTHON_INCLUDE}/pyconfig.h )
	message( FATAL_ERROR " pyconfig.h header file failed to compile. If it is installed, than check the python flags and directories") 
    endif(NOT HTEST)

    CHECK_INCLUDE_FILE( curses.h HTEST )
    if (NOT HTEST)
	message( FATAL_ERROR " curses.h header file failed to compile. If it is installed, than check the include directories") 
    endif(NOT HTEST)

    CHECK_INCLUDE_FILE_CXX( boost/shared_ptr.hpp  HTEST3 )
    if (NOT HTEST3)
	message( FATAL_ERROR "error boost/shared_ptr.hpp does not compile. Check if its correctly installed. You may need a newer version. ")
    endif(NOT HTEST3)
		
    CHECK_INCLUDE_FILE_CXX( boost/shared_array.hpp HTEST4 )
    if (NOT HTEST4)
	message( FATAL_ERROR "error boost/shared_array.hpp does not compile. Check if its correctly installed. You may need a newer version. ")
    endif(NOT HTEST4)
	 
    if (NOT HTEST5)
	CHECK_INCLUDE_FILE_CXX( boost/python/list.hpp  HTEST5 )
    endif (NOT HTEST5)

    if (NOT HTEST5)
	message( FATAL_ERROR "error boost/python/list.hpp does not compile. You need at least version 1.37 or newer. Check if its correctly installed. There is a known issue. If "
 	    "PYTHON_LIBRARY='" ${PYTHON_LIBRARY} "' does not point to the correct location of python's libraries, please specify them manually"
	    " or delete the file in question. (seen on OpenSuSE 11.2)" )
    endif(NOT HTEST5)

    if (NOT HTEST6)
	CHECK_INCLUDE_FILE_CXX( boost/python/extract.hpp  HTEST6 )
    endif(NOT HTEST6)

    if (NOT HTEST6)
	message( FATAL_ERROR "error boost/python/extract.hpp does not compile. Check if its correctly installed. You may need a newer version or set up python paths correctly.")
    endif(NOT HTEST6)
			    
    CHECK_INCLUDE_FILE_CXX( boost/lexical_cast.hpp  HTEST7 )
    if (NOT HTEST7)
	message( FATAL_ERROR "error boost/lexical_cast.hpp does not compile. Check if its correctly installed. You may need a newer version or set up python paths correctly.")
    endif(NOT HTEST7)
	 
    CHECK_INCLUDE_FILE_CXX( boost/thread.hpp  HTEST8 )
    if (NOT HTEST8)
	message( FATAL_ERROR "error boost/thread.hpp does not compile. Check if its correctly installed. You may need a newer version. ")
    endif(NOT HTEST8)
				    
    CHECK_INCLUDE_FILE_CXX( boost/algorithm/minmax_element.hpp  HTEST9 )
    if (NOT HTEST9)
	message( FATAL_ERROR "error boost/algorithm/minmax_element.hpp does not compile. Check if its correctly installed. You may need a newer version. ")
    endif(NOT HTEST9)

    CHECK_INCLUDE_FILE_CXX( boost/operators.hpp  HTEST921 )
    if (NOT HTEST921)
	message( FATAL_ERROR "error boost/operators.hpp does not compile. Check if its correctly installed. You may need a newer version. ")
    endif(NOT HTEST921)

    if (NUMPY)
	## Try to determine the directory of numpy headers
	execute_process ( COMMAND python -c "from numpy import distutils; path=distutils.misc_util.get_numpy_include_dirs(); print(path[0])" OUTPUT_VARIABLE NUMPY_INCLUDEDIR OUTPUT_STRIP_TRAILING_WHITESPACE )
	if (NOT NUMPY_INCLUDE)
	    SET(NUMPY_INCLUDE ${NUMPY_INCLUDEDIR} CACHE PATH "Directory for the numpy headers. If it is not set automatically it is determined by python distutils." )
        endif( NOT NUMPY_INCLUDE)

	include_directories( ${NUMPY_INCLUDE} )
	if( NOT EXISTS ${NUMPY_INCLUDE}/numpy/arrayobject.h )
	    message( FATAL_ERROR "${NUMPY_INCLUDE}/numpy/arrayobject.h header is not found. If it is installed, than check the python flags and numpy header directories") 
	endif( NOT EXISTS ${NUMPY_INCLUDE}/numpy/arrayobject.h )
	add_definitions( -DPINK_HAVE_NUMPY )
    endif (NUMPY)

    ### ******************************************************
    ### *************** Option Development *******************
    ### ******************************************************
    if (DEVELOPMENT)

    # required version of BOOST 1.39 or newer
	set( Boost_MAJOR_VERSION_REQUIRED 1 )
	set( Boost_MINOR_VERSION_REQUIRED 39 )

	if ( Boost_MAJOR_VERSION LESS Boost_MAJOR_VERSION_REQUIRED )
	    message( FATAL_ERROR "For the development components you need a boost version at least " ${Boost_MAJOR_VERSION_REQUIRED} "." ${Boost_MINOR_VERSION_REQUIRED} )
	elseif ( Boost_MINOR_VERSION LESS Boost_MINOR_VERSION_REQUIRED )
	    message( FATAL_ERROR "For the development components you need a boost version at least " ${Boost_MAJOR_VERSION_REQUIRED} "." ${Boost_MINOR_VERSION_REQUIRED} )
        endif ( Boost_MAJOR_VERSION LESS Boost_MAJOR_VERSION_REQUIRED )


	CHECK_INCLUDE_FILE_CXX( boost/accumulators/accumulators.hpp  HTEST10)
	if (NOT HTEST10)
 	    message( FATAL_ERROR "error boost/accumulators/accumulators.hpp does not compile. Check if its correctly installed. You may need a newer version. ")
	endif(NOT HTEST10)

	CHECK_INCLUDE_FILE_CXX( boost/accumulators/statistics.hpp HTEST11 )
	if (NOT HTEST11)
 	    message( FATAL_ERROR "error boost/accumulators/statistics.hpp does not compile. Check if its correctly installed. You may need a newer version. ")
	endif(NOT HTEST11)

    endif (DEVELOPMENT)

    add_subdirectory( src/python python )

endif ( PYTHON_FRONT_END )

### **********************************************************************************************
### **********************************************************************************************
### *************************** Package Generation ***********************************************
### **********************************************************************************************
### **********************************************************************************************

if( NOT PINK_PACKAGE_TYPE )
    SET( PINK_PACKAGE_TYPE "TBZ2" CACHE STRING "The binary package to create after the compilation with the 'make package' commang." )
endif( NOT PINK_PACKAGE_TYPE )

if (CMAKE_MINOR_VERSION>=8) # cmake 2.6 required; cmake 2.8 or newer desirable
    SET_PROPERTY( CACHE PINK_PACKAGE_TYPE PROPERTY STRINGS "TBZ2" "ZIP" "TGZ" "RPM-GENERIC" "DEB-GENERIC" "DEB-DEBIAN4" "DEB-DEBIAN6" "RPM-FEDORA17" "RPM-OpenSuSE12.1" "RPM-CENTOS6" )
endif(CMAKE_MINOR_VERSION>=8)

INCLUDE(UJ_GenerateRPM.cmake)

### **********************************************************************************************
### **********************************************************************************************
### ******************************** DOXYGEN Documentation ***************************************
### **********************************************************************************************
### **********************************************************************************************

find_package(Doxygen)
if ( NOT DOXYGEN_FOUND )
    option( BUILD_DOCUMENTATION "Builds the documentation files with Doxygen" OFF )
else ( NOT DOXYGEN_FOUND )
    #message( "not building documentation" )
    option( BUILD_DOCUMENTATION "Builds the documentation files with Doxygen" OFF )
endif ( NOT DOXYGEN_FOUND )

if (BUILD_DOCUMENTATION)
    add_subdirectory( doc doc )
endif(BUILD_DOCUMENTATION)


### **********************************************************************************************
### **********************************************************************************************
### ********************************** Testing ***************************************************
### **********************************************************************************************
### **********************************************************************************************

if(NOT PINKTEST_DIRECTORY)
    SET( PINKTEST_DIRECTORY ${CMAKE_SOURCE_DIR}/"../Pinktest" CACHE PATH "The directory of the imput files for the unit testing. Mostly images, but skeletons and graph are also tested." )
endif(NOT PINKTEST_DIRECTORY)



















# LuM end of file
