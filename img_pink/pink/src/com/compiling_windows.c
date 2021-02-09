/** \page compiling_windows Compiling Pink on Microsoft Windows

\brief This page describes the compilation of the Pink library on
Microsoft Windows. Note that Python front-end is ALSO supported on
Microsoft Windows. 

\section dependencies Dependencies
For a successfull compilation on Microsoft Windows you need at least
the following tools:
<ul>
<li> Microsoft Visual Studio Express C++ (tested with 2010)
<li> Python for Microsoft Windows (tested with version 2.7; installer available on <A HREF="http://www.python.org">www.python.org</A>)
<li> Boost Libraries (sources available on <A HREF="http://www.boost.org">boost.org</A>)
</ul>

\section configuration Configuration
There is no configuration utility on Microsoft Windows, so you have to
alter the 'Jamroot', 'boost-config.jam' and 'boost-build.jam' files,
which you can find in the Pink source's root directory. Open up the
three files in question and adjust the directories.

<b> There is no mechanism for creating the built in documentation yet, so
you have to copy 'python_doc.h' from a Linux build. </b>

\section preparing_bjam Preparing the Boost sources
For the compilation on Microsoft Windows Pink uses 'bjam' as it's
build tool. You can create the 'bjam.exe' file by executing
'bootstrap.bat' in Boost's root directory.

\section compilation Compilation
Open up a command prompt and change to the Pink source directory; then
launch the 'bjam' build-tool.

\verbatim
cd C:\Pink-svn
C:\boost_1_45_0\bjam.exe
\endverbatim

In the end of the compilation you will have 'libcpp_pink.pyd' created
somewhere in the 'Pink-svn/bin' directory. 

You can copy this file into the Python package-directory:

\verbatim
copy bin\msvc-10.0\debug\link-static\threading-multi\libcpp_pink.pyd c:\Python27\DLLs\
\endverbatim

Now the library should work the same way as in linux:
\verbatim
import libcpp_pink
\endverbatim

If you want to use the rest of the interface, you should copy the
'Pink-svn/python/pink' directory among the Python modules, or you
can use: 

\verbatim
from sys import path
path.append("C:/Pink-svn/python")
import pink
\endverbatim

enviroment variable (note the slashes insteed of backslashes).

\author UjoImro, 2011








 */
