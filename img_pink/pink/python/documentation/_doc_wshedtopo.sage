_doc_wshedtopo
system:python

<!--
UjoImro, 2009

wshedtopo function documentation
-->

<hr>
<h1><font color='#000080'> wshedtopo </font></h1>
<hr>

<pre>
  wshedtopo( (char_image)image, (int)connex) -> char_image
</pre>

<h2> Description </h2>

<p>
Topological grayscale watershed (Couprie-Bertrand algorithm).
As defined in [CB97], preserves the minima.
</p>

{{{id=1|
import sys
sys.path.append("/home/ujoimro/doc/projects/Pink/python/")
testdir="/home/ujoimro/doc/projects/Pink/python/test/"
from pink import wshedtopo, show_image, readimage, zoom
}}}

<h2> Usage </h2>

{{{id=2|
linus = readimage( testdir + "linus_pauling.pgm")
linus = zoom(linus, scale=0.5 )
show_image(linus)
show_image(wshedtopo(linus, connex=4))
show_image(wshedtopo(linus, connex=8))
}}}

<h2> Advanced usage </h2>
<p>
</p>

<h2> Parameters </h2>
<ul>
  <li> 
    <b>connex</b>: The parameter <b>connex</b> indicates the
    connectivity of the binary object. Possible choices are:
    <ul>
      <li><b>4</b>: in 2D</li>
      <li><b>8</b>: in 2D</li>
      <li><b>6</b>: in 3D</li>
      <li><b>26</b>: in 3D</li>
    </ul>
  </li>
  <li>
    <b>i</b> If the parameter <B>i</B> is true, then the dual operator
    is applied. <b>NOTE: not yet implemented!</b>   
  </li>
</ul>

<h2> See also </h2>
<p>
  <ul>
    <li>
      <i></i>
    </li>
  </ul>
</p>

<h2>Types supported</h2> 
<p>
byte 2d, byte 3d
</p>

<h2>Category</h2> 
<b>connect</b>


<h2>Author</h2> 
<ul>
  <li> <b>Michel Couprie</b> </li>
  <li> 
    Laszlo Marak, 2009: Documentation imported to <b>Sage</b>, amended
    with examples.
  </li>
  
</ul>


<h2> References </h2>
<ul>
  <li>
    <b>[CB97]</b> M. Couprie and G. Bertrand: ``Topological Grayscale Watershed Transformation'',
    SPIE Vision Geometry V Proceedings, 3168 (136--146), 1997.
  </li>
</ul>







































<!-- LuM end of file -->
