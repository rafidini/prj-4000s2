/** \page compiling_make Compiling Pink with make

\brief This page contains the installation instructions with the
classical make framework.

\section Linux

To install Pink in Linux, type:
\verbatim
tar zxvf pink.tgz
mv Pinktmp Pink
cd Pink
makelin
\endverbatim

The executables will be created in Pink/linux/bin,
the scripts in Pink/scripts and in Pink/tcl
Update your environment variables accordingly. For example:

\verbatim
setenv PINK ~coupriem/Pink
set path=( $path $PINK/linux/bin $PINK/scripts $PINK/tcl )
\endverbatim

\section Microsoft Windows

Begin by installing <A HREF="http://www.mingw.org/">MinGW</A>
(Minimalist GNU For Windows), together with "make" (part of MinGW, but
separate installation)

Untar pink.tgz (you can use WinZip or WinRar).

Modify "mgw.make" and "makemgw.bat" to match your configuration.

Open a command promt and change to Pink 's directory.

type:
\verbatim
makemgw
\endverbatim

The executables will be created in Pink/bin;
update the PATH variable accordingly, for example:

\verbatim
set PATH=c:\Pink\bin;%PATH%
\endverbatim


\section comperror Possible compilation errors and solutions:

you get an error message containing "M_PI"<br>
<b>cause</b>: obsolete MinGW version<br>
<b>solution</b>: copy Pink/tools/pinkconst.h in MinGW/include/math.h<br>

you get an error message containing "uint8_t", "int8_t", "uint16_t", etc <br>
<b>cause</b>: obsolete MinGW version<br>
<b>solution</b>: copy Pink/tools/pinktypes.h in MinGW/include/math.h<br>

message d'erreur contenant "chrono"<br>
<b>cause</b>: compilation flag forgotten<br>
<b>solution</b>: delete the line "#define CHRONO" from the source files.<br>

*/
