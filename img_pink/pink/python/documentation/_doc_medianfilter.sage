_doc_medianfilter
system:python

<!--
UjoImro, 2009

medianfilter function documentation
-->

<hr>
<h1><font color='#000080'> medianfilter </font></h1>
<hr>

<pre>
  medianfilter( (char_image)image, (char_image)structuring_element) -> char_image
</pre>

<h2> Description </h2>

<p>
Let $F$ be the input image, $G$ be the output image, and $E$ the
structuring element. Then for each pixel $p$, $G[p]$ is the median element
of the sorted list (by increasing order) of the pixel values in the
set ${ F[q], q \in E[p] }$.

The median filter is a particular case of <b>rank_filter</b> (rank_filter), 
with rank = $0.5$ .
</p>

{{{id=1|
import sys
sys.path.append("/home/ujoimro/doc/projects/Pink/python/")
testdir="/home/ujoimro/doc/projects/Pink/python/test/"
from pink import medianfilter, char_image, readimage, show_image
}}}

<h2> Usage </h2>

{{{id=2|
def dist2(a,b):
    return (a[0]-b[0])*(a[0]-b[0]) + (a[1]-b[1])*(a[1]-b[1])
}}}

{{{id=3|
model=readimage( testdir + "model.pgm" )
show_image(model, scale=0.3)
}}}

{{{id=4|
WD=41
r=20

SE=char_image([ WD, WD ])
for q in range(WD):
    for w in range(WD):
        if dist2([q, w], [WD/2, WD/2]) <= r*r:
            SE[[q, w]]=255
show_image(SE, scale=6.)
}}}

{{{id=5
SE.center=[20, 20]
show_image( medianfilter(model, SE), scale=0.3 )
}}}

<h2> Advanced usage </h2>

<h2> Parameters </h2>
<ul>
  <li> 
    <b>structuring_element</b> This is a binary image, which is used
    select the pixels for the population. Its <b>center</b> property
    must be set. The <b>SE</b> will be put on the pixel by its
    center point and the median will be selected from the
    covered pixels.
  </li>
</ul>

<h2> See also </h2>
<p>
  <ul>
    <li>
      <i>rank_filter</i>
    </li>
  </ul>
</p>

<h2>Types supported</h2> 
<p>
byte 2d, byte 3d
</p>

<h2>Category</h2> 
<b> morpho </b>


<h2>Author</h2> 
<ul>
  <li> <b>Michel Couprie</b>, 1997 december </li>
  <li> 
    Laszlo Marak, 2009: Documentation imported to <b>Sage</b>, amended
    with examples.
  </li>
  
</ul>


<h2> References </h2>
<ul>
  <li>
  </li>
</ul>







































<!-- LuM end of file -->
