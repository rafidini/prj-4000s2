// -*- latex -*-

/** \page python_export Exporting functions to Python

\section introduction Introduction
The Pink library has two front-ends; the Bash and the Python
front-end. Historically almost all of the operators are already
exported into the Bash front-end. For an undetermined transition
period, both front-ends should be maintained. That means, that new
functions should appear in Bash and Python as well. 

Note that all the conventions are respected as the complete Python
wrapper, including the wrapping macros, is written in C++. For further
reading we suggest !!!!

\section bash Bash front-end
To export the functions in Bash, you have to create a file named
'my_function.c' in the 'src/com' directory. The file should contain a
main function, which parses it's parameters and calles <b>exactly
one</b> appropriate Pink operator. The format of the command line
parameters is the following:

\verbatim
operator_name input_image.pgm t1 t2 t3 output_image.pgm
\endverbatim

The main function should look like this:

\code
int main(int argc, char * argv[])
{

  // tests on the number of parameters

  // reading of the input image
  result = readimage(argv[1])

  // conversion (but NOT testing!) of the input parameters
  T1 t1 = convert_or_read(argv[2])
  T2 t2 = convert_or_read(argv[3])
  ...
  Tn tn = convert_or_read(argv[n])

  pink_c_function(result, t1, t2, ..., tn);

  // "returning" the result
  writeimage(result, argv[n+1]);
  return 0;

}
\endcode

The tests for the content of the parameters should be carried out in
'pink_c_function' as specified in the \ref dev_conventions. The
created files are automatically detected and compiled by CMake. 

Note: Do not forget to add the created files to the svn repository.

\section python Python front-end
There are several wrapper functions to facilitate the export of Pink
functions to Python. 

\section doc_export Documentation export
When you are wrapping functions in Python, the wrapper functions allow
you to write your documentation directly at wrapping. This is however
not the suggested way of documenting functions in python. After each
compilation a python script extracts the doxygen documentation of the
operators in the file include/python_doc.h. Each file
'src/com/operator.c' has a macro defined in 'python_doc.h' called
'doc__operator__c__'. You should use these macros, where exporting
documentation in Python.

\section c_functions C functions
There are three wrappers for exporting functions in python. The macros
have to be called in 'src/python/pypink.cpp'. The function types are
as defined in \ref dev_conventions.

<ul>

<li> <b>Standard functions</b>

\code
UI_WRAP_FUNCTION(
  "function_name_in_python",
  function_name,
  (arg("T1"), arg("T2"), ..., arg("Tn")),
  "documentation string" // or doc__function_name__c__
);
\endcode

<li> <b>Result functions</b>

\code
UI_WRAP_RESULT(
  result_type,
  "function_name_in_python",
  function_name,
  (arg("T1"), arg("T2"), ..., arg("Tn")),
  doc__function_name__c__
);
\endcode

<li> <b>Value functions</b>

\code
UI_WRAP_VALUE(
  "function_name_in_python",
  function_name,
  (arg("T1"), arg("T2"), ..., arg("Tn")),
  doc__function_name__c__
);
\endcode
</ul>

After including the appropriate macro call in 'pypink.cpp' the
functions appear in Python. Examples can be found in 'pypinp.cpp'.

\section cpp_export C++ functions. 
C++ functions are exported with the boost export function 'def'. The
parametrisation remains the same as for the WRAP macros:

\code
def(
  "function_name_in_python",
  function_name,
  (arg("T1"), arg("T2"), ..., arg("Tn")),
  doc__function_name__cpp__
);
\endcode


\author ujoimro, 2011

*/
