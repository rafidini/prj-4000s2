_doc_skeleton
system:python

<!--
UjoImro, 2009

skeleton function documentation
-->

<hr>
<h1><font color='#000080'> skeleton </font></h1>
<hr>

<pre>
  def skeleton(image, connex, priority=-1, inhibit=-1)
</pre>

<h2> Description </h2>

<p>
  Ultimate binary skeleton guided by a priority image.
  The lowest values of the priority image correspond to the highest
  priority.
</p>
<h3> Algorithm </h3>
<p>
  Let $X$ be the set corresponding to the input image "input_image".
</p>
<p>
  Let $P$ be the function corresponding to the priority image.
</p>
<p>
  Let $I$ be the set corresponding to the inhibit image, if given or
  the empty set otherwise.
</p>
<p>
  The algorithm is the following:
</p>
  
  <pre>
  repeat until stability:
      select a point $x$ in $X$\ $I$ such that $P[x]$ is minimal
      if $x$ is simple for $X$:
          $X = X$\ $\{x\}$
  return $X$
  </pre>
</p>

{{{id=1|
import sys
sys.path.append("/home/ujoimro/doc/projects/Pink/python/")
testdir="/home/ujoimro/doc/projects/Pink/python/test/"
from pink import show_image, skeleton, char_image, readimage, int_image
}}}

<h2> Usage </h2>

{{{id=2|
bean = readimage( testdir + "bean.pgm" )
show_image( bean )
show_image( skeleton( bean, connex=4, priority=0 ) )
}}}

{{{id=3|
rectangle_rips = readimage( testdir + "rectangle_rips.pgm" )
show_image( rectangle_rips )
show_image( skeleton( rectangle_rips, connex=8, priority=8 ) )
}}}


<h2> Advanced usage </h2>

<p>
Creating an image, which will hold the pixels we want to keep
</p>

{{{id=4|
rectangle = readimage( testdir + "rectangle.pgm" )
}}}

<p>
Let's say we want to keep three additional points
</p>

{{{id=5|
inhibit=char_image( rectangle.size )

inhibit[[ 95,50 ]]=255
inhibit[[ 150,180 ]]=255
inhibit[[ 100,150 ]]=255

show_image( inhibit )
show_image( skeleton( rectangle, connex=4, priority=1, inhibit=inhibit ) )
}}}

<hr>

<p>
Creating a priority image
</p>

{{{id=6|
prio=int_image( rectangle.size )

for q in range( prio.size[0] ):
    for w in range( prio.size[1] ):
            prio[[ q, w ]] = q + w

prio2=int_image( rectangle.size )

for q in range( prio2.size[0] ):
    for w in range( prio2.size[1] ):
            prio2[[ q, w ]] = q^2 + w^2


show_image( rectangle )
show_image( prio )
show_image( skeleton( rectangle, connex=8, priority=prio, inhibit=inhibit ) )
show_image( prio2 )
show_image( skeleton( rectangle, connex=8, priority=prio2, inhibit=inhibit ) )

}}}

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
</ul>

{{{id=9|
circle_hole = readimage( testdir + "circle_hole.pgm" )
show_image( circle_hole, scale=0.5 )
show_image( skeleton( circle_hole, connex=4, priority=0 ), scale=0.5 )
show_image( skeleton( circle_hole, connex=8, priority=0 ), scale=0.5 )
}}}

<ul>
  <li>
    <b>prio</b>: The parameter <b>prio</b> is either an <b><i>_image</i></b>
    (byte, int, float or double) or a numerical code indicating that a
    distance map will be used as a priority image; the possible
    choices are:
    <ul>
      <li> <b>0</b>: approximate euclidean distance </li>
      <li> <b>1</b>: approximate quadratic euclidean distance </li>
      <li> <b>2</b>: chamfer distance </li>
      <li> <b>3</b>: exact quadratic euclidean distance </li>
      <li> <b>4</b>: 4-distance in 2d </li>
      <li> <b>8</b>: 8-distance in 2d </li>
      <li> <b>6</b>: 6-distance in 3d </li>
      <li> <b>18</b>: 18-distance in 3d </li>
      <li> <b>26</b>: 26-distance in 3d </li>
    </ul>
  </li>
</ul>


{{{id=10|
square_hole = readimage( testdir + "square_hole.pgm" )
show_image( square_hole, scale=0.5 )
for q in [ 0, 1, 2, 4, 8 ]:
    show_image( skeleton( square_hole, connex=8, priority=q ), scale=0.5 )
}}}

<ul>
  <li>
    <b>inhibit</b>: If the parameter <b>inhibit</b> is given and is an
    integer, then the points which correspond to
    this priority value will be left unchanged. If the
    parameter <b>inhibit</b> is given and is a binary <b><i>_image</i></b> name, then
    the points of this image will be left unchanged.     
  </li>
</ul>

{{{id=11|
prio=int_image( rectangle.size )

for q in range( prio.size[0] ):
    for w in range( prio.size[1] ):
            prio[[ q, w ]] = q^2 + w^2

check=char_image( rectangle.size )
for q in range( prio.size[0] ):
    for w in range( prio.size[1] ):
        if prio[[ q, w ]] == 190 and rectangle[[ q, w ]]==255:
            check[[ q, w ]]=190

show_image( check )

show_image( rectangle )
show_image( prio )
show_image( skeleton( rectangle, connex=8, priority=prio, inhibit=190 ) )
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
<b> topobin </b>


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
    <b> [BC07] </b> G. Bertrand and
    M. Couprie: <A HREF="http://www.esiee.fr/~coupriem/Pdf/chapitre_topo.pdf">"Transformations
    topologiques discretes"</A>, in <I>G&eacute;om&eacute;trie
    discr&egrave;te et images num&eacute;riques</I>, D. Coeurjolly and
    A. Montanvert and J.M. Chassery, pp.&nbsp;187-209, Herm&egrave;s,
    2007.<BR>
  </li>
</ul>







































<!-- LuM end of file -->
