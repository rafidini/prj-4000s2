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


# OLD DD="/home/ujoimro/doc/projects/MICCAI/DATA/Challenge Data (Training)"
# DD = "/home/ujoimro/doc/projects/MICCAI/DATA/Challenge Data (Validation)"


# OLD directories =  [ "SC-HF-I-01", "SC-HF-I-04", "SC-HF-NI-03", "SC-HF-NI-34", "SC-HYP-01", "SC-HYP-38", "SC-N-02", "SC-N-40", "SC-HF-I-02",  "SC-HF-I-40", "SC-HF-NI-04", "SC-HF-NI-36", "SC-HYP-03", "SC-HYP-40", "SC-N-03" ]

#directories = [ "SC-HF-I-05", "SC-HF-I-07", "SC-HF-NI-07", "SC-HF-NI-33", "SC-HYP-06", "SC-HYP-08", "SC-N-05", "SC-N-07", "SC-HF-I-06", "SC-HF-I-08", "SC-HF-NI-11", "SC-HF-NI-34", "SC-HYP-07", "SC-HYP-37", "SC-N-06" ]

#directories = [ "SC-HF-NI-31" ]


VARS = "LD_LIBRARY_PATH=/usr/lib/vtk-5.4:/usr/lib/vtk-5.4:/usr/lib/vtk-5.4:/opt/Julius/bin:/usr/local/s2plot/linux-gnu-i386:/home/ujoimro/Inst/u3d/U3D_A_061228_5/U3D_A_061228_5/Source/:/home/ujoimro/Inst/u3d/U3D_A_061228_5/U3D_A_061228_5/Source/Build/U3D/:/usr/lib/PInK/ \
PATH=/home/ujoimro/doc/projects/MICCAI/Pinktmp/Analyse:/home/ujoimro/doc/projects/MICCAI/Pinktmp/Analyse:/home/ujoimro/doc/projects/MICCAI/Pinktmp/Analyse:/opt/kde3/bin:/home/ujoimro/bin:/usr/local/bin:/usr/bin:/bin:/usr/bin/X11:/usr/X11R6/bin:/usr/games:/usr/lib64/jvm/jre/bin:/usr/lib/mit/bin:/usr/lib/mit/sbin:/usr/lib/qt3/bin:/home/ujoimro/j2sdk_nb/j2sdk1.4.2/bin:/usr/local/lib:/usr/local/maple_su/bin:/home/ujoimro/LuM:/home/ujoimro/LuM/excbook:/home/ujoimro/LuM/immap:/opt/intel/cc/9.1.045/bin:/opt/intel/cce/10.0.023/bin/:/opt/intel/cce/10.0.023/lib/:/usr/local/s2plot/:/usr/local/s2plot:/home/ujoimro/Inst/u3d/U3D_A_061228_5/U3D_A_061228_5/Source/Build/U3D:/home/ujoimro/Inst/u3d/U3D_A_061228_5/U3D_A_061228_5/Source/Build/U3D/:/home/ujoimro/Inst/meshlab/meshlab/meshlab/src/meshlab/:/home/ujoimro/ArtOfIllusion/:/usr/bin/PInK/:/home/ujoimro/j2sdk_nb/j2sdk1.4.2/bin:/usr/local/lib:/usr/local/maple_su/bin:/home/ujoimro/LuM:/home/ujoimro/LuM/excbook:/home/ujoimro/LuM/immap:/opt/intel/cc/9.1.045/bin:/opt/intel/cce/10.0.023/bin/:/opt/intel/cce/10.0.023/lib/:/usr/local/s2plot/:/usr/local/s2plot:/home/ujoimro/Inst/u3d/U3D_A_061228_5/U3D_A_061228_5/Source/Build/U3D:/home/ujoimro/Inst/u3d/U3D_A_061228_5/U3D_A_061228_5/Source/Build/U3D/:/home/ujoimro/Inst/meshlab/meshlab/meshlab/src/meshlab/:/home/ujoimro/ArtOfIllusion/:/usr/bin/PInK/:/home/ujoimro/j2sdk_nb/j2sdk1.4.2/bin:/usr/local/lib:/usr/local/maple_su/bin:/home/ujoimro/LuM:/home/ujoimro/LuM/excbook:/home/ujoimro/LuM/immap:/opt/intel/cc/9.1.045/bin:/opt/intel/cce/10.0.023/bin/:/opt/intel/cce/10.0.023/lib/:/usr/local/s2plot/:/usr/local/s2plot:/home/ujoimro/Inst/u3d/U3D_A_061228_5/U3D_A_061228_5/Source/Build/U3D:/home/ujoimro/Inst/u3d/U3D_A_061228_5/U3D_A_061228_5/Source/Build/U3D/:/home/ujoimro/Inst/meshlab/meshlab/meshlab/src/meshlab/:/home/ujoimro/ArtOfIllusion/:/usr/bin/PInK/:/home/ujoimro/doc/projects/MICCAI/GCDM/bin/bin/:/home/ujoimro/doc/projects/MICCAI/GCDM/bin/bin/:/home/ujoimro/doc/projects/MICCAI/GCDM/bin/bin/ \
PINK=/home/ujoimro/doc/projects/MICCAI/Pinktmp \
TCLLIBPATH=/usr/lib/vtk-5.2 "


ISOT="  /home/ujoimro/doc/projects/MICCAI/Pinktmp/tclCoeur/isotropise0.4.tcl "
BVSEGM="  /home/ujoimro/doc/projects/MICCAI/Pinktmp/tclCoeur/BVSegmenter0.4.tcl "
PREPIM="  /home/ujoimro/doc/projects/MICCAI/Pinktmp/tclCoeur/PrepareImage0.4.tcl "
MOMENT="/home/ujoimro/doc/projects/MICCAI/DATA/Challenge Data (Validation)/GIVE_ME_ONE_MOMENT_IN_TIME"


#range(2,10)[1:3]


def parse_a_shift_file( filename ):
    s = open( filename, "r" )
    lines=s.readlines()
    
    center = map( int, lines[1].split(" ")[0:2])
    
    result=[[0,0]]
    for q in range(2,len(lines)):
        result.append( map( int, lines[q].split(" ")[1:3] ) )   
    
    s.close()
    
    return center, result


#center, vects = parse_a_shift_file( MOMENT + "/" + directories[0] + "_SEGM/0000/GeomParam" )
#print "vects =", vects
#print "center =", center


def shift_an_image( input_slices, center, vects ):    
    def pos_in( x, val ):
        return x >= 0 and x <= val -1
        
    #size = [ input_slices[0].size[0], input_slices[0].size[1], len( input_slices ) ] 
    size = [ 100, 100, len( input_slices ) ] 

    result = char_image( size )
    
    for q in iSize( size ):
    
        pos_x = center[0] + q[0] - vects[q[2]][0]
        pos_y = center[1] + q[1] - vects[q[2]][1]
    
        if pos_in( pos_x, input_slices[0].size[0] ) and pos_in( pos_y, input_slices[0].size[1] ):
            result[q] = input_slices[ q[2] ][[ pos_x, pos_y ]]
        else:
            result[q] = 255
            
    return result


def move_a_dir( directory, vects, center ):
    slices_to_move=[]
    for q in range( len( vects ) ):
        slices_to_move.append( readimage( directory + "/pgm/rep" + str(q) + "/i0000.pgm" ) )
    
    
    moved_slices = shift_an_image( input_slices = slices_to_move, center=center, vects = vects )
    
    moved_slices.writeimage( directory + "/Volume/V3d001.pgm" )
    moved_slices.writeimage( directory + "/Volume/V3d002.pgm" )


#move_a_dir( MOMENT + "/" + directories[0] + "_SEGM/0001", vects )


def sglob( pattern ):
    result = glob(pattern)
    result.sort()
    return result


#for q in directories:
#    current_dir = MOMENT + "/" + q + "_SEGM"    
#    print current_dir

#    b_dirs = sglob( current_dir + "/0*" )
#    #print b_dirs
    
#    center, vects = parse_a_shift_file( current_dir + "/0000/GeomParam" )
    
#    for w in b_dirs[1:]:
#        #print w
#        move_a_dir( directory = w, vects = vects, center = center )


#for q in ["SC-HYP-08"]:
#    current_dir = MOMENT + "/" + q + "_SEGM"    
#    print current_dir
#
#    b_dirs = sglob( current_dir + "/0*" )
#    #print b_dirs
#    
#    center, vects = parse_a_shift_file( current_dir + "/0000/GeomParam" )
#    
#    for w in b_dirs[1:]:
#        #print w
#        move_a_dir( directory = w, vects = vects, center = center )






































# LuM end of file
