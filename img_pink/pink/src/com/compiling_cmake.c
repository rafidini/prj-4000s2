/** \page compiling_cmake Compiling Pink with CMake

\brief This page describes the compilation of the Pink library
including the compilation of the library, core functions and the
Python front-end

\section configuration Configuration
Pink uses CMake as its configuration and build automation tool. Before
you could compile the library you need to configure the source
tree. During the configuration CMake checks if all the necessery
dependencies are installed on your system. For further reading look at
<A HREF="https://secure.wikimedia.org/wikipedia/en/wiki/Dependency_hell">dependency
hells</A> on Wikipedia. 

Generally the compilation is carried out in a separate directory. To
call CMake you can use the following commands:
\verbatim
ls Pink_sources.tar.bz2
tar xvf Pink_sources.tar.bz2
\endverbatim

The above commands create a directory 'Pink_svn' on your system. You
create a build directory and configure the repository:

\verbatim
mkdir build
cd build
ccmake ../Pink-svn
\endverbatim

The above-latest command will start the CMake in configuration
mode. To configure the repository you type 'c'.

Here you have to choose your compilation options. There are four
major options you can choose from:

\verbatim
BASH_FRONT_END                   OFF
BUILD_DOCUMENTATION              OFF
DEVELOPMENT                      OFF
PYTHON_FRONT_END                 OFF
\endverbatim

For a standard compilation you have to set PYTHON_FRONT_END to ON:

\verbatim
BASH_FRONT_END                   OFF
BUILD_DOCUMENTATION              OFF
DEVELOPMENT                      OFF
PYTHON_FRONT_END                 ON
\endverbatim

After setting the options you type 'c' twice to invoke a second
and a third configuration. 

\section generation Generation
If the configuration succeeds each time you are ready to generate the
build code. Type 'g' and the necessary makefiles will be generated.

Note: If there's an error during one of these configurations, then an
appropriate error message is presented. Try to update your system
according to these error messages. 

\section compilation Compilation
Type 

\verbatim
make
\endverbatim

to carry out necessary compilation, building and script
generation. If the compilation succeeds without and error message, you
can consult the page \ref using_python.

\section installation Installation The configuration is capable of
installing the Pink-Python library on your system. For the
installation you need to check two variables:

\verbatim
CMAKE_INSTALL_PREFIX             /usr/local
PYTHON_MODULE_INSTALL_PATH       /usr/lib/python2.6/site-packages
\endverbatim

The 'PYTHON_MODULE_INSTALL_PATH' variable is detected automatically,
so you shouldn't have any problems. The 'CMAKE_INSTALL_PREFIX' is the
GNU standard '/usr/local', you probably want to change it to
'/usr'. Alternatively, you can specify different directories depending
your system. If you are content with the configuration, type

\verbatim
sudo make install
\endverbatim

and the Pink-Python library will be installed on your system. After
the installation you can consult the page \ref using_python.

\author UjoImro, 2011








 */
