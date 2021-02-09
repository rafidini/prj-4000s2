_doc_zoom
system:python

<!--
UjoImro, 2009

zoom function documentation
-->

<hr>
<h1><font color='#000080'> zoom </font></h1>
<hr>

<pre>
  def zoom( image, scale=0.0, new_size=[0] )
</pre>

<h2> Description </h2>

<p>
  This function zooms an image (shrinking or enlarging it). It uses an
  approximation, so enlarging and shrinking <b>will not</b> give the
  same image.
</p>

{{{id=1|
import sys
sys.path.append("/home/ujoimro/doc/projects/Pink/python/")
testdir="/home/ujoimro/doc/projects/Pink/python/test/"
import pink
}}}

<h2> Usage </h2>

{{{id=2|
gandhi = pink.readimage( testdir + "gandhi.pgm" )
pink.show_image(gandhi)
B = pink.zoom(gandhi, scale=0.5)
pink.show_image(B)  
}}}

<h2> Advanced usage </h2>

{{{id=3|
B = pink.zoom(gandhi, scale=[0.5,0.2])
pink.show_image(B)  
}}}


<h2> Parameters </h2>
<p>
  Note: exactly one of the parameters must be specified
</p>  
{{{id=4|
B = pink.zoom(gandhi)
}}}

{{{id=5|
B = pink.zoom(gandhi, scale=0.3, new_size=[1,2])
}}}

<ul>
  <li> <b>scale</b> Either a scalar value or a list of scalar values
    specifying the scale factor in every dimension. </li>
</ul>

{{{id=6|
B = pink.zoom(gandhi, scale=[0.5,0.2])
pink.show_image(B)
}}}

{{{id=7|
C = pink.zoom(gandhi, scale=0.1)
pink.show_image(C)
}}}

<ul>    
  <li> <b>new_size</b> A vector specifying the integer dimensions of
    the new image. </li>
</ul>

{{{id=8|
D = pink.zoom(gandhi, new_size=[200,210])
pink.show_image(D)
}}}


<h2> See also </h2>
<p>
  <ul>
    <li>
      <i>show_image</i> is a function which uses <i>zoom</i> to
      generate the image for the preview in the notebook.
    </li>
  </ul>
</p>


<h2> References </h2>

<p>

</p>








































<!-- LuM end of file -->
