# -*- sh -*-
# UjoImro, 2010

# ------------------------------------
# -------- Bash front-end ------------
# ------------------------------------

#-lgsl -lgslcblas -lm -lX11 -lXext /usr/lib64/libboost_thread-il-mt-1_39.so.1.39.0
FILE( GLOB com_src_c *.c )
FILE( GLOB com_src_cpp *.cpp )
# I think we should use only one extension for C++
FILE( GLOB com_src_cxx *.cxx ) # cpp files that compile without boost

# unix defines
add_definitions( -DUNIXIO -DPC )
set( CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS} ${PYTHON_FLAG} )
include_directories( ${PYTHON_INCLUDE}  ${BOOST_INCLUDE} )

# -------- Iteration over the executables -------- 

if ( NOT PYTHON_FRONT_END )
    set( com_src ${com_src_c}  ${com_src_cxx} )
else ( NOT PYTHON_FRONT_END )    
    set( com_src ${com_src_c} ${com_src_cpp} ${com_src_cxx} )
endif ( NOT PYTHON_FRONT_END )

foreach( q ${com_src} )
         string( REGEX MATCH [^/]*.c[^.]*$ filename ${q} )
	 string( REGEX REPLACE .c[^.]*$ "" executable ${filename} )
	 add_executable( pink.${executable} ${q} )
	 add_dependencies( pink.${executable} pink )
	 target_link_libraries( pink.${executable} pink ${LIBS} )
	 install( TARGETS pink.${executable} DESTINATION bin )
endforeach(q)





















# LuM end of file
