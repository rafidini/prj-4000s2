#! /usr/bin/python

# UjoImro, 2009
# This software is meant to be free.

1+1


#from pink.MICCAI import update
#update()


from glob import glob
from pink import parallel_map, readimage, drawcurve, char_image, vint, show_image
from os import system, makedirs
from pink.MICCAI import iSize, convert_dicom, parse_directory, slices_2D_to_3D, import_validation, image2itk, image2pink, isotropize_itk, itk_image_size
from itk import Image, ImageRegion, Size, Index
import itk


testdir="/home/ujoimro/doc/projects/Pink/python/test/"
filename="/home/ujoimro/doc/projects/MICCAI/DATA/Challenge Data (Training)/SC-HF-I-01_SEGM/IsoVol/res/LVCM0018.pgm"
directory="/home/ujoimro/doc/projects/MICCAI/DATA/Challenge Data (Training)/SC-HF-I-01_SEGM/IsoVol/res"
dir4D="/home/ujoimro/doc/projects/MICCAI/DATA/Challenge Data (Training)/SC-N-03_SEGM"
dicom2pgm_exec="/home/ujoimro/doc/projects/MICCAI/Pinktmp/Analyse/dicom2pgm"
val_dir="/home/ujoimro/doc/projects/MICCAI/Validation/Challenge Contours (Training)/SC-N-03/contours-manual/IRCCI-expert"


def extract_plane_itk( input_image, slice_number ):
    import itk
    extractor=itk.ExtractImageFilter.IF3IF2.New()
    input_region=input_image.GetLargestPossibleRegion()
    size=input_region.GetSize()
    size.SetElement(2,0)
    
    start=input_region.GetIndex()    
    start.SetElement(2, slice_number)
    
    desired_region=extractor.GetExtractionRegion()
    desired_region.SetSize(size)
    desired_region.SetIndex(start)
    
    extractor.SetExtractionRegion(desired_region)
    extractor.SetInput(input_image)
    extractor.Update()
    
    return extractor.GetOutput()


def region2itk( region ):
   result=itk.ExtractImageFilter.IF2IF2.New().GetExtractionRegion()
   result.SetIndex(region["start"])
   result.SetSize(region["size"])
   
   return result


def register_two_slices( fixed_image, moving_image, region, verbose=False ):
    
    def iterationUpdate():
        currentParameter = transform.GetParameters()
        print "M: %f   P: %f %f " % ( optimizer.GetValue(), 
                                      currentParameter.GetElement(0),
                                      currentParameter.GetElement(1) 
                                      )
  
    transform=itk.TranslationTransform.D2.New()
    optimizer=itk.RegularStepGradientDescentOptimizer.New()
    #metric=itk.MeanReciprocalSquareDifferenceImageToImageMetric.IF2IF2.New()
    #metric=itk.MeanSquaresImageToImageMetricMetric.IF2IF2.New()
    metric=itk.MattesMutualInformationImageToImageMetric.IF2IF2.New()
    itk.MattesMutualInformationImageToImageMetric
    interpolator=itk.LinearInterpolateImageFunction.IF2D.New()
    registration=itk.ImageRegistrationMethod.IF2IF2.New()
    
    registration.SetMetric( metric.GetPointer() )
    registration.SetOptimizer( optimizer.GetPointer() )
    registration.SetTransform( transform.GetPointer() )
    registration.SetInterpolator( interpolator.GetPointer() )
    
    registration.SetFixedImage( fixed_image )
    registration.SetMovingImage( moving_image )
    
    registration.SetFixedImageRegion( region2itk( region ) )
    
    transform.SetIdentity()
    
    initial_parameters=transform.GetParameters()
    initial_parameters.SetElement(0,0.0)
    initial_parameters.SetElement(1,0.0)
    registration.SetInitialTransformParameters( initial_parameters )
    
    
    if verbose:
        iterationCommand = itk.PyCommand.New()
        iterationCommand.SetCommandCallable( iterationUpdate )
        optimizer.AddObserver( itk.IterationEvent(), iterationCommand.GetPointer() )
        
    optimizer.SetMaximumStepLength( 1.00 )
    optimizer.SetMinimumStepLength( 0.001 )
    optimizer.SetNumberOfIterations( 200 )
    
    registration.Update()
    
    result=[0.0,0.0]
    result[0]=registration.GetLastTransformParameters().GetElement(0)
    result[1]=registration.GetLastTransformParameters().GetElement(1)
    
    return result


def transsum2D( vect, n ):
    result=[0.0,0.0]
    
    if len(vect) < n:
        raise "error: The vector is shorter than the sum should be"
    
    for q in range(n):
        result[0]+=vect[q][0] 
        result[1]+=vect[q][1]
        
    return result


def register_a_3D_image( input_image, region ):
    size = itk_image_size( input_image )
    result=[]
    for q in range(size[2]-1):
        result.append( register_two_slices(
                                            extract_plane_itk( input_image = input_image, slice_number = q ),
                                            extract_plane_itk( input_image = input_image, slice_number = q+1 ),
                                            region
                     ) 
        )
    return result


def translate_image( input_image, translation_vector ):
    
    resampler=itk.ResampleImageFilter.IF2IF2.New()

    translation_vector=map( int, translation_vector )
    translate = itk.AffineTransform.D2.New()
    translate.SetIdentity()

    translate.Translate( translation_vector[0], translation_vector[1] )
    
    interpolator=itk.LinearInterpolateImageFunction.IF2D.New()
    
    direction = itk.Image.F2.New().GetDirection()
    direction.SetIdentity()
    resampler.SetOutputDirection( direction )
    resampler.SetTransform( translate.GetPointer() )
    resampler.SetInterpolator( interpolator.GetPointer() )
    resampler.SetInput( input_image )

    resampler.SetSize( input_image.GetLargestPossibleRegion().GetSize() )
    resampler.SetOutputOrigin( input_image.GetOrigin() )
    resampler.SetOutputSpacing( input_image.GetSpacing() )
    resampler.SetDefaultPixelValue( 255 )

    resampler.Update()
    
    result = resampler.GetOutput()
    
    result.Update()
  
    return result


def make_a_3D_image_from_slices( slices ):
    joint = itk.JoinSeriesImageFilter.IF2IF3.New()
    for q in range(len(slices)):
        joint.SetInput(q, slices[q])
    joint.Update()
    return joint.GetOutput()


def translate_a_3D_image( input_image, translations ):
    size = itk_image_size( input_image )
    print "size =", size
    if size[2]!=len(translations):
        raise "error: You've specified wrong number of translations"
        
        
    translated_slices=[]
        
    for q in range( len( translations ) ):
        translated_slices.append( 
                   translate_image( 
                                    input_image = extract_plane_itk( input_image=input_image, slice_number=q ),
                                    translation_vector = translations[q]    
                                  )
                                )
                                
                                
    return translated_slices#make_a_3D_image_from_slices( translated_slices )


def itk_writeimage( input_image, filename ):
    caster = itk.CastImageFilter.IF2IUC2.New()
    writer = itk.ImageFileWriter.IUC2.New()
          
    caster.SetInput( input_image )
    writer.SetFileName( filename )
    writer.SetInput( caster.GetOutput() )
    
    writer.Update()








itk.MattesMutualInformationImageToImageMetric




































































# LuM end of file
