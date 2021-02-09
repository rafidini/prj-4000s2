#! /usr/bin/python

# UjoImro, 2009
# This software is meant to be free.

from sys import path
path.append("/home/ujoimro/doc/projects/Pink/python/")
testdir="/home/ujoimro/doc/projects/Pink/python/test/"
filename="/home/ujoimro/doc/projects/MICCAI/DATA/Challenge Data (Training)/SC-HF-I-01_SEGM/IsoVol/res/LVCM0018.pgm"
from glob import glob
from pink import parallel_map, readimage, drawcurve, char_image, vint, show_image
from os import system, makedirs
from pink.MICCAI.convert_dicom import normalize
from pink.MICCAI import itk_image_size
import itk
from math import sqrt


def isotropize_itk( input_image, spacing ):
    
    # intensity filter
    intensity_filter = itk.IntensityWindowingImageFilter.IF3IF3.New()
    intensity_filter.SetWindowMaximum(255)
    intensity_filter.SetWindowMinimum(0)    
    intensity_filter.SetOutputMaximum(255)
    intensity_filter.SetOutputMinimum(0)
    
    input_image.SetSpacing( spacing )
    input_image.SetOrigin([0,0,0])
    
    direction = input_image.GetDirection()
    direction.SetIdentity()
    input_image.SetDirection( direction )
    
    intensity_filter.SetInput( input_image )
    
    #smoother_X = itk.RecursiveGaussianImageFilter.IF3IF3.New()
    #smoother_Y = itk.RecursiveGaussianImageFilter.IF3IF3.New()

    #smoother_X.SetInput( intensity_filter.GetOutput() )
    #smoother_Y.SetInput( smoother_X.GetOutput() )
    
    
    iso_spacing = sqrt( spacing[0] * spacing[2] )
    
    #smoother_X.SetSigma( sqrt( spacing[0] * spacing[2] ) )
    #smoother_X.SetSigma( sqrt( spacing[1] * spacing[2] ) )
    
    #smoother_X.SetDirection(0)
    #smoother_Y.SetDirection(1)
    
    #smoother_X.SetNormalizeAcrossScale( True )
    #smoother_Y.SetNormalizeAcrossScale( True )
    
    resampler = itk.ResampleImageFilter.IF3IF3.New()
    
    transform = itk.IdentityTransform.D3.New()
    transform.SetIdentity()
    
    resampler.SetTransform( transform.GetPointer() )
    
    #interpolator = itk.BSplineInterpolateImageFunction.IF3DF.New()
    interpolator = itk.NearestNeighborInterpolateImageFunction.IF3D.New()
    resampler.SetInterpolator( interpolator.GetPointer() )
    
    
    resampler.SetDefaultPixelValue( 255 )
    resampler.SetOutputSpacing( [ spacing[0], spacing[0], spacing[0] ] ) #[ iso_spacing, iso_spacing, iso_spacing ] )
    
    
    resampler.SetOutputOrigin([0,0,0])
    resampler.SetOutputDirection( input_image.GetDirection() )
    
    
    size=itk_image_size(input_image)
    #d_x = 256 * 1.367188  / 3.307
    #d_y = 256 * 1.367188  / 3.307
    #d_z = ( 20 - 1 ) * 8.0  / 3.307
    d_x=size[0]
    d_y=size[1]
    d_z=size[2] * spacing[2] / spacing[0]
    
    
    resampler.SetSize( [ int(d_x), int(d_y), int(d_z) ] )
    resampler.SetInput( intensity_filter.GetOutput() ) # smoother_Y.GetOutput() )
    
    resampler.Update()
    
    return resampler.GetOutput()





# LuM end of file
