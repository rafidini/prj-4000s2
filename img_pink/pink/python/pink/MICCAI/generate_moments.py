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
import os


dicom2pgm_exec="/home/ujoimro/doc/projects/MICCAI/Pinktmp/Analyse/dicom2pgm"


VARS = "LD_LIBRARY_PATH=/usr/lib/vtk-5.4:/usr/lib/vtk-5.4:/usr/lib/vtk-5.4:/opt/Julius/bin:/usr/local/s2plot/linux-gnu-i386:/home/ujoimro/Inst/u3d/U3D_A_061228_5/U3D_A_061228_5/Source/:/home/ujoimro/Inst/u3d/U3D_A_061228_5/U3D_A_061228_5/Source/Build/U3D/:/usr/lib/PInK/ \
PATH=/home/ujoimro/doc/projects/MICCAI/Pinktmp/Analyse:/home/ujoimro/doc/projects/MICCAI/Pinktmp/Analyse:/home/ujoimro/doc/projects/MICCAI/Pinktmp/Analyse:/opt/kde3/bin:/home/ujoimro/bin:/usr/local/bin:/usr/bin:/bin:/usr/bin/X11:/usr/X11R6/bin:/usr/games:/usr/lib64/jvm/jre/bin:/usr/lib/mit/bin:/usr/lib/mit/sbin:/usr/lib/qt3/bin:/home/ujoimro/j2sdk_nb/j2sdk1.4.2/bin:/usr/local/lib:/usr/local/maple_su/bin:/home/ujoimro/LuM:/home/ujoimro/LuM/excbook:/home/ujoimro/LuM/immap:/opt/intel/cc/9.1.045/bin:/opt/intel/cce/10.0.023/bin/:/opt/intel/cce/10.0.023/lib/:/usr/local/s2plot/:/usr/local/s2plot:/home/ujoimro/Inst/u3d/U3D_A_061228_5/U3D_A_061228_5/Source/Build/U3D:/home/ujoimro/Inst/u3d/U3D_A_061228_5/U3D_A_061228_5/Source/Build/U3D/:/home/ujoimro/Inst/meshlab/meshlab/meshlab/src/meshlab/:/home/ujoimro/ArtOfIllusion/:/usr/bin/PInK/:/home/ujoimro/j2sdk_nb/j2sdk1.4.2/bin:/usr/local/lib:/usr/local/maple_su/bin:/home/ujoimro/LuM:/home/ujoimro/LuM/excbook:/home/ujoimro/LuM/immap:/opt/intel/cc/9.1.045/bin:/opt/intel/cce/10.0.023/bin/:/opt/intel/cce/10.0.023/lib/:/usr/local/s2plot/:/usr/local/s2plot:/home/ujoimro/Inst/u3d/U3D_A_061228_5/U3D_A_061228_5/Source/Build/U3D:/home/ujoimro/Inst/u3d/U3D_A_061228_5/U3D_A_061228_5/Source/Build/U3D/:/home/ujoimro/Inst/meshlab/meshlab/meshlab/src/meshlab/:/home/ujoimro/ArtOfIllusion/:/usr/bin/PInK/:/home/ujoimro/j2sdk_nb/j2sdk1.4.2/bin:/usr/local/lib:/usr/local/maple_su/bin:/home/ujoimro/LuM:/home/ujoimro/LuM/excbook:/home/ujoimro/LuM/immap:/opt/intel/cc/9.1.045/bin:/opt/intel/cce/10.0.023/bin/:/opt/intel/cce/10.0.023/lib/:/usr/local/s2plot/:/usr/local/s2plot:/home/ujoimro/Inst/u3d/U3D_A_061228_5/U3D_A_061228_5/Source/Build/U3D:/home/ujoimro/Inst/u3d/U3D_A_061228_5/U3D_A_061228_5/Source/Build/U3D/:/home/ujoimro/Inst/meshlab/meshlab/meshlab/src/meshlab/:/home/ujoimro/ArtOfIllusion/:/usr/bin/PInK/:/home/ujoimro/doc/projects/MICCAI/GCDM/bin/bin/:/home/ujoimro/doc/projects/MICCAI/GCDM/bin/bin/:/home/ujoimro/doc/projects/MICCAI/GCDM/bin/bin/ \
PINK=/home/ujoimro/doc/projects/MICCAI/Pinktmp \
TCLLIBPATH=/usr/lib/vtk-5.2 "


CONFIGFILE = "/home/ujoimro/doc/projects/MICCAI/DATA/Challenge Data (Training)/ALL_IN_ONE/BVSeg.paf"
ISOT="  /home/ujoimro/doc/projects/MICCAI/Pinktmp/tclCoeur/isotropise0.4.tcl "
BVSEGM="  /home/ujoimro/doc/projects/MICCAI/Pinktmp/tclCoeur/BVSegmenter0.4.tcl "
PREPIM="  /home/ujoimro/doc/projects/MICCAI/Pinktmp/tclCoeur/PrepareImage0.4.tcl "


#sglob( BD + "/" + directories[0] + "_SEGM/pgm_non_isotropized/*" )


#BD + "/" + directories[0] + "/pgm_non_isotropized/"


#"/home/ujoimro/doc/projects/MICCAI/DATA/Challenge Data (Training)/SC-HF-I-01/pgm_non_isotropized/"


#ALL_FILES=[]
#for q in directories:
#    ALL_FILES+=glob( BD + "/" + q + "_SEGM/pgm_non_isotropized/*" )


#makedirs(AIO + "/" + _str(1))


#"cp \"" + CONFIGFILE + "\" \"" + AIO + "/" + _str(q) + "/\""


# returns the glob sorted
def sglob( pattern ):
    result = glob( pattern )
    result.sort()
    return result


#print MOMENT + "/" + directories[0] + "_SEGM"
#print sglob( BD + "/" + directories[0] + "_SEGM/pgm_non_isotropized/*" )


def generate_moments( AIO, ALL_FILES  ):

    try:
        makedirs( AIO )
    except:
        1==1

    # file for segmentation
    file = open( AIO + "/commands.sh", "w")
    file.write("#! /bin/bash\n\n")
    file.write("# UjoImro, 2009\n")
    file.write("# This software is meant to be free.\n\n\n\n\n")


    # file for registration
    register = open( AIO + "/register.sh", "w" )
    register.write("#! /bin/bash\n\n")
    register.write("# UjoImro, 2009\n")
    register.write("# This software is meant to be free.\n\n\n\n\n")


    for q in range(len(ALL_FILES)):
    
        current_dir = AIO + "/" + _str(q)
        #print "current_dir =", current_dir
    
    
        try:
            makedirs( AIO + "/" + _str(q) )
        except:
            1==1
            
        try:
            makedirs( AIO + "/" + _str(q) + "/IsoVol" )
        except:
            1==1

        try:
            makedirs( AIO + "/" + _str(q) + "/Volume"  )
        except:
            1==1
        
        try:
            makedirs( AIO + "/" + _str(q) + "/pgm"   )
        except:
            1==1
        
        try:
            makedirs( AIO + "/" + _str(q) + "/tmp" )
        except:
            1==1
    
        # os.system( "ln -s \"" + CONFIGFILE + "\" \"" + AIO + "/" + _str(q) + "/\"" )
        
        
        # pgmInfo is generated now for each image, because the number of slices changes
        # os.system( "cp \"" + PGMINFO + "\" \"" + AIO + "/" + _str(q) + "/pgm/\"" )
    
        #print "explode  \"" + ALL_FILES[q] + "\" \"" + current_dir + "/tmp/__vol\""
    
        os.system("explode  \"" + ALL_FILES[q] + "\" \"" + current_dir + "/tmp/__vol\"")
    
    
        #os.system( "cp \"" + ALL_FILES[q] + "\" \"" + AIO + "/" + _str(q) + "/Volume/V3d001.pgm\"" )
    
    
    
        slices=sglob( current_dir + "/tmp/__vol*")


        infofile=open( AIO + "/" + _str(q) + "/pgm/pgmInfo", "w" )

        infofile.write( str(len(slices))+" ; number of 2D+t sequences\n" )
        infofile.write( "2 ; number of images per 2D+t sequence\n" )
        infofile.write( "1.367188 ; pixel spacing (along x and y axis) of the 2D images\n" )
        infofile.write( "8 ; thickeness of the 2D images\n" )

        infofile.close()
        #print slices

        for w in range(len(slices)):
            try:
                makedirs( current_dir + "/pgm/rep" + str(w) )
            except:
                1==1
            # there are two slices with the same content
            #print "cp " + "\"" + slices[w] + "\"" + " \"" + current_dir + "/pgm/rep" + str(w) + "/i0000.pgm\""
            os.system( "cp " + "\"" + slices[w] + "\"" + " \"" + current_dir + "/pgm/rep" + str(w) + "/i0000.pgm\"" )
            os.system( "cp " + "\"" + slices[w] + "\"" + " \"" + current_dir + "/pgm/rep" + str(w) + "/i0001.pgm\"" )
        

        # We only register the first moment        
        if q==0:
            register.write( VARS )
            register.write( PREPIM )
            register.write( "\"" + current_dir + "\""  + "\n\n\n" )
    
        file.write( VARS )
        file.write( ISOT )
        file.write( "\"" + AIO + "/" + _str(q) + "/Volume/V3d\" " )
        file.write( "\"" + AIO + "/" + _str(q) + "/pgm/pgmInfo\" " )
        file.write( "\"" + AIO + "/" + _str(q) + "/IsoVol\" " )
        file.write( "\"" + AIO + "/" + _str(q) + "\" \n\n")
    
        ws = open( AIO + "/" + _str(q) + "/check.sh", "w" )
    
        ws.write("#! /bin/bash\n\n")
        ws.write("# UjoImro, 2009\n")
        ws.write("# This software is meant to be free.\n\n\n\n\n")
    
        ws.write( VARS )
        ws.write( BVSEGM )
        ws.write( "\"" + AIO + "/" + _str(q) + "/IsoVol/IsoV3d\" " )
        ws.write( "\"" + AIO + "/" + _str(q) + "/pgm/pgmInfo\" " )
        ws.write( "\"" + AIO + "/" + _str(q) + "/IsoVol/res\" " )
        ws.write( "\"" + AIO + "/" + _str(q) + "\" \n\n\n\n\n\n\n\n")
    
        ws.write("\n\n\n\n\n#LuM end of file\n")
    
        ws.close()
    
    
        file.write( VARS )
        file.write( BVSEGM )
        file.write( "\"" + AIO + "/" + _str(q) + "/IsoVol/IsoV3d\" " )
        file.write( "\"" + AIO + "/" + _str(q) + "/pgm/pgmInfo\" " )
        file.write( "\"" + AIO + "/" + _str(q) + "/IsoVol/res\" " )
        file.write( "\"" + AIO + "/" + _str(q) + "\" \n\n\n\n\n\n\n\n")
    
    

    file.write("\n\n\n\n\n#LuM end of file\n")
    file.close()

    register.write("\n\n\n\n\n#LuM end of file\n")
    register.close()


#generate_moments( AIO = MOMENT + "/" + directories[0] + "_SEGM", ALL_FILES=sglob( BD + "/" + directories[0] + "_SEGM/pgm_non_isotropized/*" ) )


#for q in directories:
#    print q
#    generate_moments( AIO = MOMENT + "/" + q + "_SEGM", ALL_FILES=sglob( BD + "/" + q + "_SEGM/pgm_non_isotropized/*" ))





# LuM end of file
