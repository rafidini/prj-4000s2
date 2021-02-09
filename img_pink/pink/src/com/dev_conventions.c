// -*- latex -*-

/** \page dev_conventions Conventions for developing Pink operators

\brief This page describes the function format for the Pink Image
Processing Library

\section introduction Introduction
To facilitate development, particularly to facilitate others
understanding/improoving your code we use general guidelines for the
functions in Pink. These guidelines ease particularcly the front-end
development, as less types there are less templates are necessary. If
your functions could be transformed in one of the declared formats, it
should be. If, however your function cannot respect any of the known
formats (for example, the size of the result image is different than
the size of the input image), feel free to omit them. 

\section languages Programming Languages
Pink functions can be written in C, C++ and Python. Scripts written in
Tcl or Bash are discouraged as they make the library more difficult to
use and to compile/port. Also except for the Python documentation generation
(which is done by a Python script) you should not invoke any script of
any language during the compilation. This concerns
yacc/bison/lex/flex, sed, swig or any preprocessing languages. These
languages would make the compilation more complex, and would obligate
the potencial developper to learn and master more and more
programming/preprocessing languages. 

\section image_type Image types
Pink deals mostly with black and white images, but color images are
also supported, at least as a format. The main image structure is
xvimage. In C++ xvimage is represented by the ujoi<pixel_type>
object. ujoi<> can be converted (casted) in xvimage* in constant time and memory, so C++
functions can call C without penalties. There are some shortcuts to image
types. At the time of writing the following image types exist in Pink:
\code
struct xvimage; // represents all the supported image types in C format

class ujoi<unsigned byte>  <=> char_image     // represents a byte image
class ujoi<unsigned short> <=> short_image    // represents a short image
class ujoi<int>            <=> int_image      // represents a int image
class ujoi<float>          <=> float_image    // represents a float image
class ujoi<double>         <=> double_image   // represents a double image
class ujoi<fcomplex>       <=> fcomplex_image // represents a complex image
class ujoi<dcomplex>       <=> dcomplex_image // represents a complex image
\endcode

\section general_conventions General conventions
<ul>
<li> Operators have to test function types and constraints
themselves.<br>

The operators don't have to support all Pink image formats,
but they have to test themselves. That is to say, if
the function 'skeleton' can only operate on 2D images, it has to
reject 3D images. It cannot suppose, that the images have the
good type or properties. This is a necessery choice to make the
library usable by non-developers.

<li> Never write anything to the disk unless explicitly asked for.<br>
All the calculation should be carried out in the memory. This also
concerns interactive functions and scripts like threasholding or
rendering. The disk operations slow down the processing considerably,
whereas you only rarely meet a case, where your image does not fit
into the memory. In this case you still have the swap as memory
management automation. If you know that you will certainly not fit
into the swap, you should have a non-default option for memory
mapping. 
</ul>

\section c_functions C functions
To avoid incompatibilities between C and C++ the C functions should
not allocate memory for the result image. Three types of C pink
operators are declared. These functions are automatically wrapped to
Python. For non conventional functions an additional C++ wrapper
function has to be written.
<ul>

<li> <b>Standard functions</b>:

Functions with the same result-type as image type:

\verbatim
int pink_c_function( struct xvimage* image, T1 t1, T2 t2, ... Tn tn);
\endverbatim

Here the types Tk can be 'struct xvimage*' or any other type. At the end
of the function, the result should be copied into the 'image'
variable. The function should return 1 if it succeeded and 0 it had
failed. You can also use exit or (f)printf if you detect some error.

<li><b> Result functions </b>:

If your image type is different than that of the input image, but
otherwise the same for all image types you should use the type:

\verbatim
int pink_c_result(struct xvimage* image, T1 t1, T2 t2, ..., Tn tn, struct xvimage * result);
\endverbatim

In this case the image type of the result should always be the
same. You should use this signature for example if you have a binary
image, and you know, that the result is a distance map (double). You should not
allocate result image inside your function. Instead assume, that
an image with the same size as 'image' is allocated with the
appropriate image type. In the end you copy the result in 'result'.
The function should return 1 if it succeeded and 0 it had
failed. You can also use exit or (f)printf if you detect some error.

<li> <b>Value Functions</b>

These functions calculate a number (integer or float) about the
image. Typically average, max or sum are such operators. The function
used is:

\verbatim
value_type pink_c_value(T0 t0, T1 t1, ..., Tn tn);
\endverbatim

Your function can accept any type Tk 'struct xvimage*' or other and
can have any simple result type. You can use exit or (f)printf if
you detect some error.
</ul>

\section cpp_functions C++ functions
Pink functions written in C++ should use the functional paradigm. That
is they should have only constant parameters, and return a value or an
image. If they need to return more then one value as a result, they
should definitely use 'struct' instead of modifying the paremeters:

\verbatim
result_type pink_cpp_function( const T0 t0, const T1 t1, ..., const Tn tn )
\endverbatim

There are several things to note about this declaration:
<ul>

<li> If there is at least one image parameter, than this parameter
should be t0.

<li> Because ujoi<> is a smart pointer class, even if you call the
copy constructor, it will be in constant time. 

<li> Also if the result_type is an image, then returning it makes a
copy, but because of the smart pointers, this copy is in constant
time. 
</ul>

The function should test the images on the input. If it can, for
example, only work with 2D images, it should test the dimensions of
the input at the beginning. In the case that the image detects an
error it should call the 'error' macro, with the most complete
description  of the errors detected. The error macro declaration can
be found in the beginning of the c++ functions. 


\section python_scripts Python scripts
Python scripts are called directly from python. These scripts should
respect the format of the wrapped function to simplify the
utilization of the library. Generally  a python function looks like:
\verbatim
def pink_python_function(t0, t1, t2, ..., tn)
\endverbatim

If there is an image parameter, t0 should be an image parameter. In
the end the functions should return the result with the 'return'
statement. Possible errors should be raised. 

\author ujoimro, 2011
*/
