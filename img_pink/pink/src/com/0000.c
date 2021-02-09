// -*- mode: C; coding: utf-8 -*-
/*! \mainpage 

Pink is an image processing library developed
at <A HREF="http://www.esiee.fr">ESIEE Engineering</A> for research
and teaching purposes. It contains implementations of over 200
algorithms for image segmentation and filtering. Most of the operators
come from mathematical morphology, but it contains operators from
different fields. It is a free software licensed under the
<A HREF="http://www.cecill.info/index.en.html"> CeCILL </A>
license. 

In august 2011 Pink has been presented at the European Python
Scientific Conference as a poster. Below you can see the conference
poster (also in <a href="pink_euroscipy_web.pdf"> pdf </a> and hi-res <a href="pink_euroscipy_web.jpeg"> jpeg </a>).

\image html pink_euroscipy_web_thumb.jpeg "EuroSciPy 2011 poster"

\section screenshots Screenshots

<A HREF="http://developer.berlios.de/screenshots/?group_id=12429">
Screenshots can be found here. </A>

\section download Download

The source code and the binary packages can be accessed from the \ref
download page.

\section mailing Mailing list 

If you are interested in the daily life of Pink, please subscribe to
the <a href="https://lists.berlios.de/mailman/listinfo/pink-image-devel">
Pink Developer mailing list </a>. You can also post questions
concerning the usage or raise issues. 

\section bughunt Bug-hunt!

If you have discovered a bug, please report it at
the <a href="https://developer.berlios.de/bugs/?group_id=12429" >
bug tracker</a>.

\section credits Credits

<ul>
  <li> Code licensed under CeCILL license
  <ul> 
    <li> <a href="http://www.esiee.fr/~coupriem"><b>Michel Couprie</b> </a>
    <li> <a href="http://www.laurentnajman.org"> <b>Laurent Najman</b> </a>: localextrema, saliency
    <li> <a href="http://hugues.zahlt.info">     <b>Hugues Talbot </b> </a>: fmm 
    <li> <a href="http://www.esiee.fr/~coustyj"> <b>Jean
    Cousty</b> </a>: redt 3d (reverse euclidean distance transform -
    algo de D. Coeurjolly), watershedthin, opérateurs sur les graphes
    d'arêtes (GA), forêts de poids min (MSF), waterfall,
    recalagerigide_translateplane
    <li> <b>Xavier Daragon</b>: dist, distc (distance euclidienne quadratique 3D)
    <li> <b>André Vital Saude</b>: radialopening, divers scripts tcl, hma
    <li> <b>Nicolas Combaret</b>: toposhrinkgray, ptselectgray
    <li> <b>John Chaussard</b>: lballincl, cropondisk, shrinkondisk
    <li> <b>Christophe Doublier</b>: zoomint
    <li> <b>Hildegard Koehler</b>: lintophat
    <li> <b>Cédric Allène</b>: gettree, histolisse, labeltree, nbcomp, pgm2vtk, seuilauto
    <li> <b>Gu Jun</b>: maxdiameter
    <li> <b>Sébastien Couprie</b>: mcsplines.c
    <li> <b>Rita Zrour</b>: medialaxis (axe médian euclidien exact - algo de Rémy-Thiel),
      dist, distc (distance euclidienne quadratique exacte - algo de Saito-Toriwaki)
    <li> <b>Laurent Mercier</b>: gestion d'un masque dans delaunay
    <li> <a href="http://www.ujoimro.com"> <b>Laszlo Marak (ujoimro)</b> </a>: continuous maximum flows,
    Unger flows, Python front-end, native Microsoft Windows port
    <li><b>Benjamin Raynal</b>: parallel 3D thinning
    <li><b>Nivando Bezerra</b>: parallel grayscale thinning
   </ul>
  <li> Code under different free software licenses
    <ul>
      <li> <b>David Coeurjolly</b>: lvoronoilabelling.c
      <li> <b>Dario Bressanini</b>: mcpowell.c
      <li> <b>Andrew W. Fitzgibbon</b>: lbresen.c
      <li> <b>Lilian Buzer</b>: lbdigitalline.cxx
   </ul>
</ul>
\section compiling Compiling

Pink can be compiled with two sets of tools. For the compilation with
dependency checking and Python front-end, visit the page \ref
compiling_cmake. For the classical compilation look at \ref
compiling_make. You can also compile Pink on windows (including the
Python front-end). For details on compiling on windows look at \ref
compiling_windows. 


\section development Development
For developing Pink look at the \ref dev_conventions page. For
exporting functions in Python look at the \ref python_export page. 

\section dependencies Dependencies
For optimal use, the following packages should be installed:
<br><br>
<A HREF="http://hugues.zahlt.info/Imview.html">imview</A><br>
<A HREF="http://www.python.org">Python</A><br>
<A HREF="http://www.doxygen.org">Doxygen</A><br>
<A HREF="http://dev.scriptics.com/">ActiveTcl 8.3</A><br>
<A HREF="http://public.kitware.com/VTK/">VTK</A><br>
<A HREF="http://www.mplayerhq.hu/design7/news.html">MPlayer</A><br>
<A HREF="http://www.gnuplot.info/">Gnuplot</A><br>

Note, that the detailed dependencies can be found in file
'dependencies'.

The Python front-end has been financially supported
by <a href="http://www.edf.fr"> EDF </a>.  The
project management is alpha-hosted
at <a href="http://developer.berlios.de">
<img src="http://developer.berlios.de/bslogo.php?group_id=12429"
width="124" height="32" border="0" alt="BerliOS Logo" />BerliOs</a>.

<HR>
Michel Couprie  -  Professeur  -  ESIEE Paris
Laboratoire d'Informatique Gaspard-Monge, Université Paris-Est
ESIEE 2, Bd Blaise Pascal - B.P. 99
93162 Noisy-Le-Grand CEDEX
m(dot)couprie(at)esiee(dot)fr 
url:  http://www.esiee.fr/~coupriem
 */

/*!
\defgroup inter Interactive operators
 */

/*!
\defgroup arith Arithmetic operators
 */

/*!
\defgroup convert Format and type conversion 
*/

/*!
\defgroup morpho Mathematical morphology
*/

/*!
\defgroup connect Digital connectivity
*/

/*!
\defgroup topobin Digital topology (binary)
*/

/*!
\defgroup topogray Digital topology (grayscale)
*/

/*!
\defgroup orders Orders topology
*/

/*!
\defgroup geo Geometrical operators
*/

/*!
\defgroup draw Graphic primitives
*/

/*!
\defgroup histo Histogram-based operators
*/

/*!
\defgroup signal Signal processing
*/

/*!
\defgroup stats Statistics
*/

/*!
\defgroup mesh3d Three-dimensional meshing
*/

/*!
\defgroup development Development
*/


// LuM end of file
