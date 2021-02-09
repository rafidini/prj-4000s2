#! /usr/bin/python

# UjoImro, 2009
# This software is meant to be free.

1+1


from pink.MICCAI import update
update()


from pink.MICCAI import extract_plane_itk, register_two_slices, transsum2D,\
register_a_3D_image, translate_image, make_a_3D_image_from_slices, \
translate_a_3D_image, itk_writeimage, get_a_point
from os import system, makedirs


from glob import glob
from pink import parallel_map, readimage, drawcurve, char_image, vint, show_image
from os import system, makedirs
from pink.MICCAI import iSize, convert_dicom, parse_directory, slices_2D_to_3D, import_validation, image2itk, image2pink, isotropize_itk, itk_image_size, _str
from itk import Image, ImageRegion, Size, Index
import itk


dicom2pgm_exec="/home/ujoimro/doc/projects/MICCAI/Pinktmp/Analyse/dicom2pgm"


# DD="/home/ujoimro/doc/projects/MICCAI/DATA/Challenge Data (Validation)"
# OLD DD="/home/ujoimro/doc/projects/MICCAI/DATA/Challenge Data (Training)"


#convert_dicom( indir = DD + "/SC-HYP-38/DICOM", outdir = DD + "/SC-HYP-38_SEGM" )


#slices = parse_directory( dir = DD + "/SC-HYP-38_SEGM", slices = {} )
#images3D = slices_2D_to_3D(slices, DD + "/SC-HYP-38_SEGM/inversion_data.txt")[0]


#a=[1,2,3]
#print a[:2]





def create_a_frame( input_image, filename, first_slice, last_slice, invert ):
    ### CUT IS HERE
    new_size = [ input_image.size[0], input_image.size[1], ( last_slice - first_slice + 1 ) ]
    frame_image = char_image( new_size )
#    slice = char_image( new_size[:2] )
#    for q in iSize( new_size[:2] ):
#        slice[q] = input_image[ q + [5] ]
        
#    frame = get_a_point( slice )
    
#    print frame
    
    cropped_image = char_image( new_size ) #[ 100, 100, new_size[2] ] )
    
    if invert:
        print "shift =", [ input_image.size[2] - 1 - last_slice, input_image.size[2] - first_slice - 1 ]
        for q in iSize( new_size ): # [ 100, 100, new_size[2] ] ):
            ### CUT IS HERE
            cropped_image[q] = input_image[[  q[0], q[1], q[2] + input_image.size[2] - 1 - last_slice  ]]
    else:  # NOT invert
        print "shift =", [ first_slice, last_slice ]
        for q in iSize( new_size ): # [ 100, 100, new_size[2] ] ):
            ### CUT IS HERE
            cropped_image[q] = input_image[[  q[0], q[1], q[2] + first_slice  ]]
     
    
    #print "( last_slice - first_slice + 1 ) =", ( last_slice - first_slice + 1 )
    print "input_image.size[2] =", input_image.size[2]
    cropped_image.writeimage( filename )


#create_a_frame( images3D[6], "/home/ujoimro/tmp/im01.pgm" )


#images3D[6].size








#image2pink(isotropize_itk( registered_images[q], [1.367, 1.367, 8.0] )).writeamira( "/home/ujoimro/tmp/isoooo"+_str(q)+".am" )


# images3D[q].writeimage( prefix + "_" + _str(q) + ".pgm" )


def save_images( prefix, images3D, first_slice, last_slice, invert ):
    try:
        makedirs(prefix[0:prefix.rfind("/")])
    except:
        1==1
    for q in range(len(images3D)):
        create_a_frame( images3D[q], prefix + "_" + _str(q) + ".pgm", first_slice, last_slice, invert )


def create_non_isotropized_pgms( pgm, first_slice, last_slice, invert ):
    
    print pgm
    
    
    slices = parse_directory( pgm, {} )
    
    images3D = slices_2D_to_3D( slices, pgm + "/inversion_data.txt" )[0]

    
    try:
        makedirs( pgm + "/pgm_non_isotropized" )
    except:
        1==1
        

    save_images( pgm + "/pgm_non_isotropized/images", images3D, first_slice, last_slice, invert )


# SEGM="/home/ujoimro/doc/projects/MICCAI/DATA/Challenge Data (Validation)"
# OLD SEGM="/home/ujoimro/doc/projects/MICCAI/DATA/Challenge Data (Training)"





# parameters=[]
# OLD for q in [ "SC-HF-I-01", "SC-HF-I-04", "SC-HF-NI-03", "SC-HF-NI-34", "SC-HYP-01", "SC-HYP-38", "SC-N-02", "SC-N-40", "SC-HF-I-02",  "SC-HF-I-40", "SC-HF-NI-04", "SC-HF-NI-36", "SC-HYP-03", "SC-HYP-40", "SC-N-03" ]:
# OLD   parameters.append([ DD + "/" + q + "/DICOM", DD + "/" + q + "_SEGM" ])

#for q in [ "SC-HF-I-05", "SC-HF-I-07", "SC-HF-NI-07", "SC-HF-NI-33", "SC-HYP-06", "SC-HYP-08", "SC-N-05", "SC-N-07", "SC-HF-I-06", "SC-HF-I-08", "SC-HF-NI-11", "SC-HF-#NI-34", "SC-HYP-07", "SC-HYP-37", "SC-N-06" ]:
    #parameters.append([ DD + "/" + q + "/DICOM", DD + "/" + q + "_SEGM" ])
    
# parameters.append([ DD + "/" + "SC-HF-NI-31" + "/DICOM", DD + "/" + "SC-HF-NI-31" + "_SEGM" ])


# parallel_map( convert_dicom, parameters )


# parameters=[]
# OLDfor q in [ "SC-HF-I-01", "SC-HF-I-04", "SC-HF-NI-03", "SC-HF-NI-34", \
#           "SC-HYP-01", "SC-HYP-38", "SC-N-02", "SC-N-40", "SC-HF-I-02", \
#           "SC-HF-I-40", "SC-HF-NI-04", "SC-HF-NI-36", "SC-HYP-03", "SC-HYP-40", "SC-N-03" ]:               
#    parameters.append( SEGM + "/" + q + "_SEGM" )

#for q in [ \
#                 [ "SC-HF-I-05",  3, 9, False ], \
#                 [ "SC-HF-I-06",  0, 9, True ], \
#                 [ "SC-HF-I-07",  0, 6, True ], \
#                 [ "SC-HF-I-08",  1, 9, True ], \
#                 [ "SC-HF-NI-07", 1, 10,True  ], \
#                 [ "SC-HF-NI-11", 2, 11,False ], \
#                 [ "SC-HF-NI-33", 2, 8, True ], \
#                 [ "SC-HF-NI-34", 3, 8, False ], \
#                 [ "SC-HYP-06",   2, 7, False ], \
#                 [ "SC-HYP-07",   3, 9, False ], \
#                 [ "SC-HYP-08",   1, 7, True ], \
#                 [ "SC-HYP-37",   3, 7, False ], \
#                 [ "SC-N-05",     2, 5, True ], \
#                 [ "SC-N-06",     3, 7, False ],  \
#                 [ "SC-N-07",     2, 8, False ] \
#
#                 ]:
#    parameters.append( [ SEGM + "/" + q[0] + "_SEGM", q[1], q[2], q[3] ] )



# parameters.append( [ SEGM + "/" + "SC-HF-NI-31" + "_SEGM", 4, 9, False ] ) # Orientation unknown


#create_non_isotropized_pgms( parameters[14] )


#map( create_non_isotropized_pgms, parameters )


# for q in parameters:
#     print q


# for q in parameters:
#    create_non_isotropized_pgms( q[0], first_slice=q[1], last_slice=q[2], invert=q[3] )


# parameters[10]


# create_non_isotropized_pgms( parameters[10][0], first_slice=0, last_slice=7 )


#slices











# LuM end of file
