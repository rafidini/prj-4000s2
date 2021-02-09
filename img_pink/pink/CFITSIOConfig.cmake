# - Find CFITSIO library
# Find the native CFITSIO includes and library
# This module defines
#  CFITSIO_INCLUDE_DIR, where to find fitsio.h, etc.
#  CFITSIO_LIBRARIES, libraries to link against to use CFITSIO.
#  CFITSIO_FOUND, If false, do not try to use CFISTIO.
# also defined, but not for general use are
#  CFITSIO_LIBRARY, where to find the CFITSIO library.

#=============================================================================
# Copyright 2011 H. Talbot IGM / ESIEE / Universite Paris Est
#
# Distributed under the OSI-approved BSD License (the "License");
# see accompanying file Copyright.txt for details.
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the License for more information.
#=============================================================================
# (To distribute this file outside of CMake, substitute the full
#  License text for the above reference.)

FIND_PATH(CFITSIO_INCLUDE_DIR fitsio.h)

SET(CFITSIO_NAMES ${CFITSIO_NAMES} cfitsio libcfitsio)
FIND_LIBRARY(CFITSIO_LIBRARY NAMES ${CFITSIO_NAMES} )

SET(MYCFITSIO_FOUND FALSE)
# handle the QUIETLY and REQUIRED arguments and set CFITSIO_FOUND to TRUE if 
# all listed variables are TRUE
#INCLUDE(${CMAKE_CURRENT_LIST_DIR}/FindPackageHandleStandardArgs.cmake)
INCLUDE(${CMAKE_ROOT}/Modules/FindPackageHandleStandardArgs.cmake)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(CFITSIO  DEFAULT_MSG CFITSIO_LIBRARY  CFITSIO_INCLUDE_DIR)

IF(CFITSIO_FOUND)
#  message("cfitsio was found, what is going on ?")
    SET( CFITSIO_LIBRARIES ${CFITSIO_LIBRARY} )
ENDIF(CFITSIO_FOUND)

#ELSE(CFITSIO_FOUND)
# message("cfitsio was not found")
#ENDIF(CFITSIO_FOUND)

SET(HT_CFITSIO_FOUND ${CFITSIO_FOUND})

MARK_AS_ADVANCED(CFITSIO_INCLUDE_DIR CFITSIO_LIBRARY)
