_doc_long2byte
system:python

<!--
UjoImro, 2009

long2byte function documentation
-->

<hr>
<h1><font color='#000080'> long2byte </font></h1>
<hr>

<pre>
  def long2byte( I, mode='truncate', nb_new_val=-1 )
</pre>

<h2> Description </h2>

<p>
Converts a "int32_t" image to a "byte" image depending on the value
given for the (optional) parameter <B>mode</B>.
</p>

{{{id=1|
import sys
sys.path.append("/home/ujoimro/doc/projects/Pink/python/")
testdir="/home/ujoimro/doc/projects/Pink/python/test/"
from pink import long2byte, int_image, show_image
}}}

<h2> Usage </h2>

{{{id=2|
size=[100,100]
I=int_image(size)

for q in range(size[0]):
    for w in range(size[1]):
        I[[q, w]]=q*q+w*w

BYTE=long2byte(I)

show_image(I, scale=3.)
}}}

<h2> Advanced usage </h2>

{{{id=3|
for q in [ 'truncate', 'modulo', 'scale', 'histogram', 'square_root', 'log' ]:
    show_image(long2byte(I, mode=q, nb_new_val=255), scale=3.)
}}}

<h2> Parameters </h2>
<ul>
  <li> <b>mode</b>
    <ul>
      <li>   
	<B>truncate</B> (default): $\forall x\in I$ do $\text{out}[x]
	= \min(255,I[x])$. 
      </li>
      <li>   
	<B>modulo</B>: $\forall x$ do $\text{out}[x] = I[x] \text{ mod } 256$. 
      </li>
      <li>  
	<B>scale</B>: scales values in the range $0..255$. 
      </li>
      <li>   
	<B>histogram</B>: sorts the values by decreasing number of occurence in the image.
	Replaces the values by their order.
	Only the <B>n</B> (default 255) first values are kept.
	Useful for labeling the images.  
      </li>
      <li>   
	<B>square_root</B>: truncation of the square root in the range
	$0..255$.  
      </li>
      <li>   
	<B>log</B>: truncation of the log in the range $0..255$.  
      </li>
    </ul>
  </li>
  <li> <b>n</b>: If <b>mode</b>=='histogram' than only
    the <B>n</B> (default 255) first values are kept.
    Useful for labeling the images.  
  </li>
</ul>

<h2> See also </h2>
<p>
<!--
    <ul>
      <li>
	<i></i>
      </li>
    </ul>
-->
</p>

<h2>Types supported</h2> 
<p> int32_t 2d, int32_t 3d </p>

<h2>Category</h2> 
<b> convert </b>


<h2>Author</h2> 
<ul>
  <li> <b>Michel Couprie</b>, mai 1998 </li>
  <li> <b>Michel Couprie</b>, decembre 1999: mode histogram</li>

  <li> 
    Laszlo Marak, 2009: Documentation imported to <b>Sage</b>, amended
    with examples.
  </li>
  
</ul>


<h2> References </h2>
<!--
    <ul>
      <li>
      </li>
    </ul>
-->






































<!-- LuM end of file -->
