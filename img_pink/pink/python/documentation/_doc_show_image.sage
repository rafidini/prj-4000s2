_doc_show_image
system:python

<!--
UjoImro, 2009

show_image function documentation
-->

<hr>
<h1><font color='#000080'> show_image </font></h1>
<hr>

<pre>
  def show_image( I, scale=0.0, new_size=[0] )
</pre>

<h2> Description </h2>

<p>
  This function shows an image in the notebook. You can also zoom the image if useful.
</p>

{{{id=1|
import sys
sys.path.append("/home/ujoimro/doc/projects/Pink/python/")
testdir="/home/ujoimro/doc/projects/Pink/python/test/"
import pink
}}}

<h2> Usage </h2>

{{{id=2|
marilyn = pink.readimage( testdir + "marilyn.pgm" )
pink.show_image(marilyn)
}}}

<h2> Advanced usage </h2>

{{{id=3|
B = pink.show_image(marilyn, scale=[0.5,0.2])
}}}


<h2> Parameters </h2>
<p>
Note: you may specify at most one property
</p>
{{{id=5|
B = pink.show_image(marilyn, scale=0.3, new_size=[1,2])
}}}

<ul>
  <li> <b>scale</b> Either a scalar value or a list of scalar values
    specifying the scale factor in every dimension. </li>
</ul>

{{{id=6|
B = pink.show_image(marilyn, scale=[0.5,0.2])
}}}

{{{id=7|
C = pink.show_image(marilyn, scale=0.1)
}}}

<ul>    
  <li> <b>new_size</b> A vector specifying the integer dimensions of
    the image to show. </li>
</ul>

{{{id=8|
D = pink.show_image(marilyn, new_size=[200,210])
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
