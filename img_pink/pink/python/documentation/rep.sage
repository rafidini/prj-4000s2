PInK demo
system:python

<!--
UjoImro, 2009

Python DEMO in sage
-->

<hr>
<center>
  <h1><font color='#000080'>PInK and Sage</font></h1>
</center>
<hr>

<h2> Introduction </h2>
<p>
  This worksheet demonstrates a proposition of porting <b>PInK</b>
  into the <b>Sage</b> mathematics software. 
</p>
<h3> Sage </h3>

<p>
  Sage is a software application which covers many aspects of
  mathematics. It is written in <b>Python</b> and <b>Cython</b> and
  integrates an included distribution of specialized mathematics
  software into a common experience.
</p>

<p>
  As described in [1], <b>Sage</b> has the following major features, that can be
  useful in <b>Pink</b>:
</p>

<ul>
  <li> It provides a <b>notebook</b> document interface. </li>
  <li> It has a support for <b>LaTeX</b> formulas for function
  documentation.
  <li> It provides a text-based command line interface. </li>
  <li> It includes MoinMoin as a Wiki system for knowledge management. </li>
  <li> It is possible to embed Sage inside LaTeX documents. </li>
  <li> It is possible to use sliders as input for calculations.[2] </li>
  <li> 
    Sage provides interfaces to some third-party software like
    Mathematica, Magma, and Maple, which allows users to combine
    software and compare output and performance. It is thus also a
    "front-end" to other mathematical tools similar to GNU TeXmacs.
  </li>
</ul>

<p>
  One of the key features of <b>Sage</b> is, that is uses <b>Python</b>
  as it's programming language. As <b>Python</b> is a real-time interpreted
  dynamically casted language, it allows calling <b>Pink</b>'s functions
  as-you-type. This also means that given a code, you can change
  parameterization of the function on-the-fly. This speeds up learning
  <b>PInK</b>, as you can see the results of the command
  instantenously, while always keeping the complete history of
  commands. It also speeds up development, as the functions can be first
  implemented in <b>Python</b> and ported later to <b>c++</b>, when the
  speed boost is necessary.
</p>

<h3> PInK </h3>
<p>
  <b>PInK</b> is an image processing library developed mainly by Michel
  Couprie at ESIEE/Paris. It contains approximately 150 functions of
  image processing, many of which are unique implementations. It uses
  Linux bash as its main interface. 
</p>

<h3> UJIF </h3>
<p>
  <b>UJIF</b> is a true database file format, that is to say it is a set of tables (entities)
  and their connections (relations). Every IO operation is done through the Database
  Engine. All the functions of the database are accessible, including indexed search
  and atomic transactions. The configuration can be expanded according to future
  needs.
</p>
<p>
  For several years the problem of digital image representation has
  been a major concern of software developers. There are more then 150
  individual image file formats
  listed on Wikipedia [3] as of mars 2009. However most of the image formats support
  directly only two dimensional images. This is inconvenient, as 3D images created
  today in medical imaging or other tomography can be up to 1000 slices in depth.
  This would mean, that any individual directory would contain a thousand separate
  files, each of which metadata would have to be synchronized manually or through
  single purpose scripts.
<p>
</p>
  Furthermore, as in image processing often arrives, a single image can have
  several "versions" or "states", like pre-filtered state or segmentation results. These
  would also mean a new set of images or new directories, and maintaining the relations
  between them is also left to the user.
</p>
<p>
  As there is no file format that would address this problem (to our knowledge),
  we would propose a new file format that would be more convenient for storing multi-
  dimensional images.
</p>

<h2> Proposition </h2>
<p>
  For further use, I would like to initiate the creation of
  a <b>Python</b> interface, a <b>Sage</b>-based documentation and a
  new, advanced file format of <b>PInK</b>. Note, that as the
  documentation language of <b>Sage</b> is <b>HTML</b>, you can
  copycat all the <b>Doxygen</b> documentation available in </b>PInK<b>.
</p>

<h3>Interface</h3>

<p>
  The functionality of PInK can be ported reasonably easily with
  the <b>BOOST</b> software package. After porting the image object,
  the functions of <b>PInK</b> can be ported on a one line per
  function bases, provided, that they are in a:
<pre>
<font color="#2040a0">PTR</font><font color="4444FF">&lt;</font><font color="#2040a0">_image</font><font color="4444FF">&gt;</font> <font color="#2040a0">function_name</font><font color="4444FF">(</font> <strong>const</strong> <font color="#2040a0">_image</font> <font color="4444FF">&amp;</font> <font color="#2040a0">src_image_1</font>,
                           <strong>const</strong> <font color="#2040a0">_image</font> <font color="4444FF">&amp;</font> <font color="#2040a0">src_image_2</font>,
                           ...
                           <strong>const</strong> <font color="#2040a0">_image</font> <font color="4444FF">&amp;</font> <font color="#2040a0">src_image_n</font>,

                           <font color="#2040a0">some_type</font> <font color="#2040a0">parameter_1</font>,
                           <font color="#2040a0">some_type</font> <font color="#2040a0">parameter_2</font>,
                           ...
                           <font color="#2040a0">some_type</font> <font color="#2040a0">parameter_m</font>,
  <font color="4444FF">)</font><font color="4444FF">;</font>
</pre>
  format, where <b>PTR</b> is the boost smart pointer, handled
  completely automatically, and <b>_image</b> is one of the image formats
  (e.g. char_image, short_image, int_image, float_image or
  double_image).
</p>

<h1> Demo of <b>PInK</b> in <b>Sage</b> </h1>
<p>
  This notebook provides a sneak-peek into the future functionality and
  behavior to <b>PInK</b>. Many, though not all of the functionality is
  demonstrated here and in the demo main pages. 
</p>
<p>
  One of the main features is <b>INTERACTIVITY</b>, that is to say, that
  you can alter every command in the notebook (including the text by
  clicking on the edit bar). You can check this way different
  parameterizations or input images.
</p>

<h2> Basic features </h2>

<p>
  First we have to set up some system dependent variables. Hopefully
  these will be set automatically in the future.
</p>

{{{id=1|
import sys
sys.path.append("/home/ujoimro/doc/projects/Pink/python/")
testdir="/home/ujoimro/doc/projects/Pink/python/test/"
}}}

<p>
  We call the <b>PInK</b> package with the <i>import</i> command.
</p>

{{{id=2|
import pink
}}}

<p>
  For reading "pgm" images we have the <i>readimage</i>
  function. After a successful reading it returns an image object. The
  image can be shown with the <i>show_image</i> function.
</p>

{{{id=3|
A = pink.readimage( testdir + "cercle11.pgm" )
pink.show_image( A, scale=30 )
}}}

<p>
  The pixels of the image can be accessed through intuitive list-like
  interface. We can <i>print</i> all the pixel values of the image.
</p>

{{{id=4|
for q in range(11):
    l = []
    for w in range(11):
        l.append(A[[ q, w ]])
    print l
}}}

<p>
  We can also change the values of the pixels.
</p>

{{{id=5|
A[[ 5, 5 ]] = 255
}}}

<p>
Check if the value have successfully changed?
</p>

{{{id=6|
for q in range(11):
    l = []
    for w in range(11):
        l.append(A[[ q, w ]])
    print l
}}}

{{{id=7|
pink.show_image(A, scale=30)
del A
}}}

<h1>Functions</h1>
<p>
  In this part of the presentation we look at some of <b>PInK</b>'s
  functions and see how they are accessed and how they behave in <b>Sage</b>.
</p>

<hr>
<h2><font color='#000080'>Topological Watershed</font></h2>

{{{id=8|
B = pink.readimage( testdir + "em.pgm" )
pink.show_image(B)
}}}

<p>
We can call the function intuitively, with giving the source image and
parameterization.
</p>

{{{id=9|
B_wshed = pink.wshedtopo( B, 4 )
}}}

{{{id=10|
pink.show_image(B_wshed)
del B
del B_wshed
}}}

<hr>
<h2><font color='#000080'>Erosion by a ball</font></h2>

<p>
  We can try the same functionality on a different function.
</p>

{{{id=11|
C = pink.readimage( testdir + "bean.pgm" )
pink.show_image(C, scale=3)
}}}

{{{id=12|
C_erosball = pink.erosball( C,  10.,  0 )
}}}

{{{id=14|
pink.show_image(C_erosball, scale=3)
del C
del C_erosball
}}}

<hr>
<h2><font color='#000080'>Median Filter</font></h2>

<p>
  At some functions we have an additional images as parameters. These
  can be specified simply as other parameters could be.
</p>

{{{id=15|
D = pink.readimage( testdir + "em.pgm")
pink.show_image(D)
SE = pink.readimage( testdir + "disque11.pgm")
}}}

<p>
  In some cases the structuring element has a <b>center point</b>,
  which is used by the function. The <b>_image</b> object provides
  this functionality by the center property.
</p>

{{{id=16|
SE.center = [5,5]
}}}

{{{id=17|
D_median = pink.medianfilter( D, SE )
}}}

{{{id=18|
pink.show_image(D_median)
del D
del D_median
}}}

<h2><font color='#000080'>Skeleton</font></h2>

<h3>Skeleton without inhibited object</h3>

<p>
  Some functions can have more complex parameterizations. For example
  in the case of the median filter, we can have an automatically
  calculated priority image or and arbitrarily defined one. Also,
  optionally we can inhibit the deletion of some pixels. This example
  shows the different parameterizations of the function.
</p>

{{{id=19|
E = pink.readimage( testdir + "bean.pgm")
pink.show_image(E)
}}}

{{{id=20|
E_skeleton = pink.skeleton( E, connex=4, priority=1  ) #priority = 1 ( approximate quadratic Euclidean distance )
}}}

{{{id=21|
pink.show_image(E_skeleton)
}}}

<h3>Skeleton with an inhibited image</h3>

{{{id=22|
inhibit_image = pink.char_image( E.size )
}}}

<!--
size = inhibit.size;
for q in range(size[0]):
    l = []
    for w in range(size[1]):
        l.append( inhibit[[ q, w ]] )
    print l
-->

{{{id=23|
inhibit_image[[ 20,20 ]] = 255
inhibit_image[[ 100, 20 ]] = 255
}}}

{{{id=24|
pink.show_image(inhibit_image)
}}}

{{{id=25|
E_skeleton = pink.skeleton( E, connex=4, priority=1, inhibit=inhibit_image )
}}}

{{{id=26|
for q in range(E.size[0]):
    for w in range(E.size[1]):
        if E_skeleton[[ q, w ]]>0:
            E[[ q, w ]]=100
pink.show_image(E_skeleton)
pink.show_image(E)
del E
del E_skeleton
del inhibit_image
}}}

<hr>
<h1><font color='#000080'>Conclusion</font></h1>
<p>
  As <b>Mathematica</b> has proven during the past decade, there is a
  huge advantage of using a notebook environment as a front-end of an
  interpreted language. This combined with <b>Python</b>'s run-time
  features can create a much more convenient way of using <b>PInK</b>
  than the traditional <b>Bash</b> interface.
</p>


<h1> References </h1>
<ol>
  <li>  
    <a href='http://en.wikipedia.org/wiki/Sage_(mathematics_software)'>
      http://en.wikipedia.org/wiki/Sage_(mathematics_software)
    </a>
  </li>
  
  <li>  
    <a href='http://wiki.sagemath.org/interact'>
      http://wiki.sagemath.org/interact
    </a>
  </li>
  
  <li>  
    <a href='http://en.wikipedia.org/wiki/Comparison_of_graphics_file_formats'>
      http://en.wikipedia.org/wiki/Comparison_of_graphics_file_formats
    </a>
  </li>
  
</ol>










<!-- LuM end of file -->
