/**

\page using_python Using the Python front-end

\brief This page describes the launching and basic utilisation of
Pink's Python front-end.

\section compilation_aftermath After the compilation
If you have succeded with \ref compiling_cmake, there is a script file
'pink.sh' generated on the top of your Pink build directory. You can
invoke the Python interpreter by typing

\verbatim
./pink.sh
\endverbatim

If you have also installed Pink after the compilation, you can get the
same environment by launching the Python interpreter

\verbatim
ipython
\endverbatim

you should get the following prompt:

\verbatim
Python 2.6.2 (r262:71600, Oct 28 2010, 20:54:41)
Type "copyright", "credits" or "license" for more information.

IPython 0.10 -- An enhanced Interactive Python.
?         -> Introduction and overview of IPython's features.
%quickref -> Quick reference.
help      -> Python's own help system.
object?   -> Details about 'object'. ?object also works, ?? prints more.

In [1]:
\endverbatim

This is a standard Python interpreter. The script sets up the
necessary directories so that the interpreter would find the Pink
library. The ultimate test of a successfull compilation is the
importing of the library in Python:

\verbatim
In [1]: import pink

In [2]:
\endverbatim

The [2] promtline indicates that the import have succeded. You can now
list the available functions:

\verbatim
In [2]: pink.cpp.
Display all 201 possibilities? (y or n)
pink.cpp.2dthin                 pink.cpp.barycentre             pink.cpp.exp                    pink.cpp.matchrect              pink.cpp.seltype2d
pink.cpp.3dthin                 pink.cpp.beta3d                 pink.cpp.extractplane           pink.cpp.max                    pink.cpp.seltype3d
pink.cpp.Pimview                pink.cpp.border                 pink.cpp.float2byte             pink.cpp.maxdiameter            pink.cpp.seuil    
pink.cpp.Pimview_force_socket   pink.cpp.border2d               pink.cpp.float_image            pink.cpp.mcube                  pink.cpp.shencastan
pink.cpp.Pimview_sendcommand    pink.cpp.boxmin                 pink.cpp.frame                  pink.cpp.medianfilter           pink.cpp.short_image
pink.cpp.Pimview_setdebug       pink.cpp.ccv                    pink.cpp.frame_around           pink.cpp.merge_max_image        pink.cpp.simplepair 
pink.cpp.Pimviewlogin           pink.cpp.char_image             pink.cpp.frame_remove           pink.cpp.min                    pink.cpp.skelcurv   
pink.cpp.Pimviewputimage        pink.cpp.closeball              pink.cpp.gaussianfilter         pink.cpp.minima                 pink.cpp.skeleton   
pink.cpp.__builtins__           pink.cpp.closebeta2d            pink.cpp.genball                pink.cpp.minmax                 pink.cpp.skelsurf   
pink.cpp.__class__              pink.cpp.closing                pink.cpp.generate_rgb_image     pink.cpp.nbvois                 pink.cpp.skelsurf3d 
pink.cpp.__delattr__            pink.cpp.color2d                pink.cpp.geodilat               pink.cpp.normalize              pink.cpp.skew       
pink.cpp.__dict__               pink.cpp.color3d                pink.cpp.geoeros                pink.cpp.openball               pink.cpp.sphere3d   
pink.cpp.__doc__                pink.cpp.contours               pink.cpp.gradientcd             pink.cpp.opening                pink.cpp.squel      
pink.cpp.__file__               pink.cpp.convert2float          pink.cpp.gradmorph              pink.cpp.pardircollapse2d       pink.cpp.squelbin   
pink.cpp.__format__             pink.cpp.convol3                pink.cpp.grayskel               pink.cpp.pardircollapse3d       pink.cpp.stretch    
pink.cpp.__getattribute__       pink.cpp.cpp_uiSqhool           pink.cpp.greet                  pink.cpp.plane3d                pink.cpp.surfacerestoration
pink.cpp.__hash__               pink.cpp.deletecomp             pink.cpp.heightmaxima           pink.cpp.pow                    pink.cpp.surimp            
pink.cpp.__init__               pink.cpp.deriche                pink.cpp.heightminima           pink.cpp.print_image            pink.cpp.surrect           
pink.cpp.__name__               pink.cpp.detectcercles          pink.cpp.heightselnb            pink.cpp.ptcurve                pink.cpp.sym               
pink.cpp.__new__                pink.cpp.dilatball              pink.cpp.histscal               pink.cpp.ptend                  pink.cpp.t26pp             
pink.cpp.__package__            pink.cpp.dilatbin               pink.cpp.holeclosing            pink.cpp.pthseparatinggray      pink.cpp.t4pp              
pink.cpp.__path__               pink.cpp.dilation               pink.cpp.houghcircles           pink.cpp.ptinterior             pink.cpp.t8pp              
pink.cpp.__reduce__             pink.cpp.dilatnum               pink.cpp.hthiniso               pink.cpp.ptisolated             pink.cpp.taf               
pink.cpp.__reduce_ex__          pink.cpp.dir                    pink.cpp.htkern                 pink.cpp.ptjunction             pink.cpp.tlf               
pink.cpp.__repr__               pink.cpp.directions             pink.cpp.identifyline           pink.cpp.ptmultiple             pink.cpp.toposhrink
pink.cpp.__setattr__            pink.cpp.dist                   pink.cpp.insert_image           pink.cpp.ptseparating           pink.cpp.translate
pink.cpp.__sizeof__             pink.cpp.distc                  pink.cpp.int_image              pink.cpp.ptseparatinggray       pink.cpp.tuf
pink.cpp.__str__                pink.cpp.distgeo                pink.cpp.inverse                pink.cpp.ptsimple               pink.cpp.variance
pink.cpp.__subclasshook__       pink.cpp.double_image           pink.cpp.isthmus3d              pink.cpp.quasishear             pink.cpp.vector_int
pink.cpp.alpha                  pink.cpp.drawball               pink.cpp.jones                  pink.cpp.rankfilter             pink.cpp.vint
pink.cpp.alpha3d                pink.cpp.drawcurve2D            pink.cpp.kern                   pink.cpp.read_raw_image         pink.cpp.volmaxima
pink.cpp.amont                  pink.cpp.drawline               pink.cpp.lambdaskel             pink.cpp.readimage              pink.cpp.volselnb
pink.cpp.any_image              pink.cpp.drawtorus              pink.cpp.lerosbin               pink.cpp.reconsplateaux         pink.cpp.watershed
pink.cpp.area                   pink.cpp.dynrecons              pink.cpp.levialdi               pink.cpp.remspnoise             pink.cpp.wshedtopo
pink.cpp.areaclosing            pink.cpp.ecarttype              pink.cpp.line                   pink.cpp.segmentarea            pink.cpp.wshedval
pink.cpp.areaopening            pink.cpp.egalise                pink.cpp.lmedialaxis_lbisector  pink.cpp.segmentheight          pink.cpp.xor
pink.cpp.areaselnb              pink.cpp.ellipticite            pink.cpp.log                    pink.cpp.segmentlines           pink.cpp.zoom
\endverbatim

The operator can be called the standard Python call:

\verbatim
I=pink.cpp.char_image([10,10])
inverted_image = pink.cpp.inverse(I)
\endverbatim

\section documentation The documentation of the operators

You can look at the documentation of a function (operator) by calling
the help function:

\verbatim
help(pink.cpp.geoeros)
\endverbatim

The help looks like this:

\verbatim
Help on built-in function geoeros:

geoeros(...)
    geoeros( (char_image)G image, (char_image)F image, (int)connexity [, (int)number of iterations=-1]) -> char_image :
          geoeros.c

         geodesic (grayscale or binary) erosion

        Usage: geoeros g.pgm f.pgm connex niter out.pgm

        Description:
        Geodesic erosion of g.pgm over f.pgm.
        Let G and F be the two input images. If G is not over F, then
        G is replaced initially by max(G,F).
        The structuring element is specified by the value of the parameter connex,
        which can be one of the following ones: 4, 8 in 2d, or 6, 18, 26 in 3d.
        The parameter  niter sets the number of iterations. If  niter = -1,
        then the iterations continue until stability.

        Types supported: byte 2d, byte 3d.

        Category: connect, morpho
          connect, morpho

         Michel Couprie




        C++ signature :
            pink::ujoi<unsigned char> geoeros(pink::ujoi<unsigned char>,pink::ujoi<unsigned char>,int [,int=-1])
\endverbatim

The documentation contains three parts. The signature of the python
function
\verbatim
geoeros( (char_image)G image, (char_image)F image, (int)connexity [, (int)number of iterations=-1]) -> char_image :
geoeros.c
\endverbatim

The description of the usage and the c++ signature of the operator:
\code 
pink::ujoi<unsigned char> geoeros(pink::ujoi<unsigned char>,pink::ujoi<unsigned char>,int [,int=-1])
\endcode 



\author UjoImro, 2011

 */
