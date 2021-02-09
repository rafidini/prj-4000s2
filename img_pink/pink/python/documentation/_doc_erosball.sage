_doc_erosball
system:python

<!--
UjoImro, 2009

erosball function documentation
-->

<hr>
<h1><font color='#000080'> erosball </font></h1>
<hr>

<pre>
  erosball( (char_image)image, (float)r, (int)mode) -> char_image
</pre>

<h2> Description </h2>

<p>
  Morphological binary erosion by a metric ball. This erosion is the
  dual of the dilatball operator, 
  i.e. $\text{erosball}\left(X\right) = \text{inverse}\left(\text{dilatball}\left(\text{inverse}\left(X\right)\right)\right)$.
  The structuring element for this erosion is a ball (or a disc) of radius $r$.
  The erosion is computed by thresholding a distance map.
  The distance used depends on the optional parameter <b>dist</b>.
</p>
<p>
<b>Warning: The input image must be a binary image. No test is done.</b>
</p>

{{{id=1|
import sys
sys.path.append("/home/ujoimro/doc/projects/Pink/python/")
testdir="/home/ujoimro/doc/projects/Pink/python/test/"
import pink
}}}

<h2> Usage </h2>

{{{id=2|
bean = pink.readimage( testdir + "bean.pgm" )
pink.show_image(bean)
eros = pink.erosball( bean, 10, 0 )
pink.show_image(eros)
}}}

<h2> Advanced usage </h2>

<h2> Parameters </h2>
<p>
</p>

<ul> 
  <li>
    <b>r</b> The radius of the image or the disc. 
  </li>
</ul>

{{{id=3|
pink.show_image(bean)
for q in [ 6, 10, 18 ]:
    eros = pink.erosball( bean, q, 0 )
    pink.show_image(eros)
}}}

<ul>
  <li> <b>mode</b> The distance function. Can be one of the following:
    <ul>
      <li> <b>0</b>: approximate euclidean distance </li>
      <li> <b>2</b>: chamfer distance </li>
      <li> <b>4</b>: 4-distance in 2d </li>
      <li> <b>8</b>: 8-distance in 2d </li>
      <li> <b>6</b>: 6-distance in 3d </li>
      <li> <b>18</b>: 18-distance in 3d </li>
      <li> <b>26</b>: 26-distance in 3d </li>
    </ul>
</ul>

{{{id=4|
pink.show_image(bean)
for dist in [ 0, 2, 4, 8 ]:
    eros = pink.erosball( bean, 15, dist )
    pink.show_image(eros)
}}}

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
<b>morpho</b>


<h2>Author</h2> 
<ul>
  <li> <b>Michel Couprie</b>, 2002 </li>
  <li> 
    Laszlo Marak, 2009: Documentation imported to <b>Sage</b>, amended
    with examples.
  </li>
  
</ul>


<h2> References </h2>

<p>

</p>







































<!-- LuM end of file -->
