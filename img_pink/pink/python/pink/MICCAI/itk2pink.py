#! /usr/bin/python

# UjoImro, 2009
# This software is meant to be free.

from sys import path
path.append("/home/ujoimro/doc/projects/Pink/python/")
path.append("/usr/lib/InsightToolkit/WrapITK/Python")
testdir="/home/ujoimro/doc/projects/Pink/python/test/"
filename="/home/ujoimro/doc/projects/MICCAI/DATA/Challenge Data (Training)/SC-HF-I-01_SEGM/IsoVol/res/LVCM0018.pgm"
from glob import glob
from pink import parallel_map, readimage, drawcurve, char_image, vint, show_image
from os import system, makedirs
from pink.MICCAI.convert_dicom import normalize
from itk import Image, ImageRegion, Size, Index, UC, F


directory="/home/ujoimro/doc/projects/MICCAI/DATA/Challenge Data (Training)/SC-HF-I-01_SEGM/IsoVol/res"
dir4D="/home/ujoimro/doc/projects/MICCAI/DATA/Challenge Data (Training)/SC-N-03_SEGM"
dicom2pgm_exec="/home/ujoimro/doc/projects/MICCAI/Pinktmp/Analyse/dicom2pgm"
val_dir="/home/ujoimro/doc/projects/MICCAI/Validation/Challenge Contours (Training)/SC-N-03/contours-manual/IRCCI-expert"


class iSize:
    pos = []
    size = []
    def __init__( self, size ):
        self.size = size
        self.pos = []
        for q in range( len(size) - 1 ):
            self.pos.append( 0 )
        self.pos.append(-1)
                       
    def __iter__( self ):
        for q in range( len( self.pos ) - 1 ):
            self.pos[q] = 0
        self.pos[ len( self.pos ) - 1 ] = -1
        return self
        
    def next( self ):
        d = len( self.size )
        q = d - 1
        while self.pos[q] >= self.size[q] - 1 and q > 0:
            q -= 1
            
        if q == 0 and self.pos[q] >= self.size[q] - 1:
            raise StopIteration
    
        self.pos[q] += 1;
        while q < d-1 :
            q += 1
            self.pos[q]=0
  
        return self.pos


#for q in iSize([2,4,2]):
#   print q


def vint2int_list( vect ):
    result=[]
    for q in range(len(vect)):
        result.append( int( vect[q] ) )
    return result


def plus( x ): # adds 1 to x
    return x+1


def image2itk( image ):
    
    # defining the basic image type
    #if image.imtype()=="uint8_t":
    #    dim = len( image.size )
    #    im_type=Image[ UC, dim ]
    #else:
    #    raise "This image type is not yet implemented."
     
     
    dim = len( image.size )
    im_type=Image[ F, dim ]   
        

    # merging the size and the center into a region
    image_region=ImageRegion[ dim ]()
    image_region.SetSize( vint2int_list ( image.size ) )
    #image_region.SetIndex( range( dim ) ) # [ 0, 1, 2 ]
    

    result = im_type.New()
    result.SetRegions( image_region )
    result.Allocate()
    result.FillBuffer(0.0)
    
    if len( image.size ) == 2:
        raise "error: 2D images are not supported"

    for q in iSize( image.size ):
        #print map( plus, q )
        result.SetPixel( q, image[q] )
     
    result.Update()
     
    return result


def itk_image_size( image ):
    size=[]
    biggest_region = image.GetLargestPossibleRegion()
    region_size = biggest_region.GetSize()
    for q in range( region_size.GetSizeDimension() ):
        size.append( region_size.GetElement(q) )
    return size


def image2pink( image ):
    # float image is assumed

    def scale( min, max, x ):
        return int( 255 * (x - min) / (max - min) )

    result = char_image( itk_image_size( image ) )
    
    print "result.size =", result.size
    
    if result.size[0]==0:
        raise "One of the dimensions is 0"


    _max = -float('inf')             #-infinity
    _min = float('inf')              # infinity    
    for q in iSize( result.size ):
        pix=image.GetPixel(q)
        if pix < _min:
            _min = pix
        elif pix > _max:
            _max = pix

    for q in iSize( result.size ):
        result[q] = scale( _min, _max, image.GetPixel(q))
        
    return result


#for q in iSize([4,6]):
#    print q








# LuM end of file
