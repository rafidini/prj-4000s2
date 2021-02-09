#! /usr/bin/python

# UjoImro, 2009
# This software is meant to be free.

from sys import path
path.append("/home/ujoimro/doc/projects/Pink/python/")
from glob import glob
from pink import parallel_map, readimage, drawcurve, char_image, vint, show_image
from os import system, makedirs
from pink.MICCAI.convert_dicom import normalize


dicom2pgm_exec="/home/ujoimro/doc/projects/MICCAI/Pinktmp/Analyse/dicom2pgm"


def parse_line(line):
    result=[]
    #0002|0001 [OB]                                      [File Meta Information Version] [GDCM::Binary data;length = 2] =0x(00\01)
    
    # first part: 
    result.append( line[ 0:line.find(" ")] )
    
    #parentheses
    while 0<=line.find("["):
        result.append(line[ line.find("[") + 1: line.find("]") ] )
        line=line[line.find("]")+1:]
    return result


# parse_line("0002|0001 [OB]                                      [File Meta Information Version] [GDCM::Binary data;length = 2] =0x(00\01)")


def make_dictionary( data ):
    result={}
    for q in data:
        if len(q)>=4:
            result[q[0]]=q[3]
    return result


# make_dictionary([['0002|0002', 'UI', 'Media Storage SOP Class UID','1.2.840.10008.5.1.4.1.1.4 ', 'MR Image Storage']])


class slice:
    instance_number=-1
    slice_location=-1
    pixel_spacing=[ -1., -1. ]
    slice_thickness=-1
    image=-1
    validation=False
    o_contour=-1
    i_contour=-1
    p1_contour=-1
    p2_contour=-1


def file_number( filename ):
    """
    parses the file's number from the filename
    """
    return filename[ filename.rfind("-") + 1 : filename.rfind(".") ]


# file_number("IM-0001-0394.txt")


# file_number("/home/ujoimro/doc/projects/MICCAI/DATA/Challenge Data (Training)/SC-N-03_SEGM/IM-0001-0394.txt")


def parse_directory( dir, slices ):
    dir = normalize( dir )    
    
    ### -------------------------------------------------------------------------
    ### PART1 reading the images into the dictionary
    ### -------------------------------------------------------------------------
    # note: this 'for' is not parallelizable, because it 
    # uses a global variable (slices) which is must be thread safe
    for q in glob( dir + "/pgm_raw/*" ):
        slices[ file_number(q) ] = slice()
        slices[ file_number(q) ].image = readimage(q)
    
    
    ### -------------------------------------------------------------------------
    ### PART2 parsing the headers for parameters
    ### -------------------------------------------------------------------------
    # note: this 'for' is not parallelizable, because it uses a global variable (slices) which is must be thread safe
    for q in glob( dir + "/Headers/*" ):
        s = open(q)
        dict = make_dictionary( map( parse_line, s.readlines() ) )
        slices[ file_number(q) ].instance_number = int(dict["0020|0013"])
        slices[ file_number(q) ].slice_location = float(dict["0020|1041"])
        slices[ file_number(q) ].pixel_spacing = map( float, dict["0028|0030"].split("\\") )
        slices[ file_number(q) ].slice_thickness = float(dict["0018|0050"])
        s.close()
    
    
    return slices


def get_contour( filename ):
    def get_numbers( line ):
        line = line.split(" ")
        return [ int( round( float(line[0]) ) ), int( round ( float( line[1] ) ) ) ]
        
    s = open( filename )
    result = map( get_numbers, s.readlines() )
    s.close()
    return result


#get_contour("/home/ujoimro/doc/projects/MICCAI/Validation/Challenge Contours (Training)/SC-HF-I-01/contours-manual/IRCCI-expert/IM-0001-0048-icontour-manual.txt")


def file_props( filename ):
        """
        parses the filename for the slice number, and the validation type (o_contour, i_..., p1_..., p2_...)
        """
        #/home/ujoimro/doc/projects/MICCAI/Validation/Challenge Contours (Training)/SC-HF-I-01/contours-manual/IRCCI-expert/IM-0001-0048-icontour-manual.txt
        
        filename = filename[ filename.rfind("IM") : filename.rfind(".") ]
        #IM-0001-0048-icontour-manual
        return filename.split("-")[2], filename.split("-")[3]


# file_props( "/home/ujoimro/doc/projects/MICCAI/Validation/Challenge Contours (Training)/SC-N-03/contours-manual/IRCCI-expert/IM-0001-0228-icontour-manual.txt" )


def import_validation( dir, slices ):

    def file_props( filename ):
        """
        parses the filename for the slice number, and the validation type (o_contour, i_..., p1_..., p2_...)
        """
        #/home/ujoimro/doc/projects/MICCAI/Validation/Challenge Contours (Training)/SC-HF-I-01/contours-manual/IRCCI-expert/IM-0001-0048-icontour-manual.txt
        
        filename = filename[ filename.rfind("IM") : filename.rfind(".") ]
        #IM-0001-0048-icontour-manual
        return filename.split("-")[2], filename.split("-")[3]

    dir = normalize(dir)

    # Creating blank files
    for q in slices:
        tmp = vint( slices[q].image.size )
        slices[q].o_contour  = char_image( tmp )
        slices[q].i_contour  = char_image( tmp )
        slices[q].p1_contour = char_image( tmp )
        slices[q].p2_contour = char_image( tmp )

    # importing the validation files
    for q in glob( dir + "/*"):

        coords = get_contour(q)
        slice_num, ftype = file_props(q)
        slices[slice_num].validation=True

        if ftype=="ocontour":
            slices[slice_num].o_contour = drawcurve( slices[slice_num].image, coords=coords )

        elif ftype=="icontour":
            slices[slice_num].i_contour = drawcurve( slices[slice_num].image, coords=coords )

        elif ftype=="p1contour":
            slices[slice_num].p1_contour = drawcurve( slices[slice_num].image, coords=coords )

        elif ftype=="p2contour":
            slices[slice_num].p2_contour = drawcurve( slices[slice_num].image, coords=coords )

        else:
            raise("error: The type of the validation is unknown")
        
        
        
    
    return slices


# slices["0328"].slice_thickness


def elem_frequency(l):
    curr=l[0]
    result=[]
    count=0
    for q in l:
        if curr==q:
            count+=1
        else: 
            result.append(count)
            count=1
            curr=q
            
    result.append(count)
    return result


# elem_frequency([1,1,1,1,2,2,2,2,2,3,3,3,3,3,4,4,4,4])


def elem_order(list):
    curr=list[0]
    result={}
    count=0
    for q in list:
        if curr!=q:
            result[curr]=count
            count+=1
            curr=q
    result[curr]=count
    return result


# elem_order([1,1,1,1,2,2,2,2,2,3,3,3,3,3,4,4,4,4])


def copy_2D_to_3D(im2D, im3D, pos3D):
    result = char_image(im3D)
    for q in range(im2D.size[0]):
        for w in range(im2D.size[1]):
            result[[ pos3D[0] + q, pos3D[1] + w, pos3D[2] ]]=im2D[[ q, w ]]
            
    return result


def elem_group(set_of, list):    
    curr=list[0]
    result={}
    pos_glob=0
    pos_loc=0
    for q in list:
        if curr!=q:
            pos_loc=0
            curr=q

        result[set_of[pos_glob]]=pos_loc
        pos_glob+=1
        pos_loc+=1
    return result


# elem_group(range(18),[1,1,1,1,2,2,2,2,2,3,3,3,3,3,4,4,4,4])


def ___slices_2D_to_3D(slices):
    instances=[]    
    for q in slices:
        instances.append(q)
    instances.sort()

    locations=[]
    for q in instances:
        locations.append(slices[q].slice_location)
    
    # the moments represents the number of pictures in different times at the same position
    moments = elem_frequency(locations) 
    z_pos=elem_order(locations)
    t_pos=elem_group(instances, locations)
    
    
    axe_x = slices["0001"].image.size[0]
    axe_y = slices["0001"].image.size[1]
    axe_z = len(moments)
    axe_t = moments[0]
    
    images3D=[]
    o_ims=[]
    i_ims=[]
    p1_ims=[]
    p2_ims=[]
    for q in range( moments[0] ):
        images3D.append( char_image([ axe_x, axe_y, axe_z ]) )
        o_ims.append( char_image([ axe_x, axe_y, axe_z ]) )
        i_ims.append( char_image([ axe_x, axe_y, axe_z ]) )
        p1_ims.append( char_image([ axe_x, axe_y, axe_z ]) )
        p2_ims.append( char_image([ axe_x, axe_y, axe_z ]) )

        
    
    for q in instances:
        images3D[ t_pos[q] ] = copy_2D_to_3D( slices[q].image, images3D[ t_pos[q] ], [ 0, 0, z_pos[ slices[q].slice_location ] ] )
        if slices[q].validation:           
            o_ims[ t_pos[q] ] = copy_2D_to_3D( slices[q].o_contour, o_ims[ t_pos[q] ], [ 0, 0, z_pos[ slices[q].slice_location ] ] )
            i_ims[ t_pos[q] ] = copy_2D_to_3D( slices[q].i_contour, i_ims[ t_pos[q] ], [ 0, 0, z_pos[ slices[q].slice_location ] ] )
            p1_ims[ t_pos[q] ] = copy_2D_to_3D( slices[q].p1_contour, p1_ims[ t_pos[q] ], [ 0, 0, z_pos[ slices[q].slice_location ] ] )
            p2_ims[ t_pos[q] ] = copy_2D_to_3D( slices[q].p2_contour, p2_ims[ t_pos[q] ], [ 0, 0, z_pos[ slices[q].slice_location ] ] )
        
        
    return images3D, o_ims, i_ims, p1_ims, p2_ims


def slices_2D_to_3D( slices, filename ):
    instances=[]    
    for q in slices:
        instances.append(q)
    instances.sort()

    locations=[]
    for q in instances:
        locations.append(slices[q].slice_location)
    
    # the moments represents the number of pictures in different times at the same position
    moments = elem_frequency(locations) 
    z_pos=elem_order(locations)
    t_pos=elem_group(instances, locations)
    
    
    axe_x = slices["0001"].image.size[0]
    axe_y = slices["0001"].image.size[1]
    axe_z = len(moments)
    axe_t = moments[0]
    
    images3D=[]
    o_ims=[]
    i_ims=[]
    p1_ims=[]
    p2_ims=[]
    for q in range( moments[0] ):
        images3D.append( char_image([ axe_x, axe_y, axe_z ]) )
        o_ims.append( char_image([ axe_x, axe_y, axe_z ]) )
        i_ims.append( char_image([ axe_x, axe_y, axe_z ]) )
        p1_ims.append( char_image([ axe_x, axe_y, axe_z ]) )
        p2_ims.append( char_image([ axe_x, axe_y, axe_z ]) )

    
    s = open( filename, "w" )
    s.write("# Positions of the DICOM images in the space and time\n\n")
    
    for q in instances:
        
        s.write( str(q) + " " + str(t_pos[q]) + " " + str(z_pos[ slices[q].slice_location ])
                        + " " + str(slices[q].slice_location) + " " +
                 "# q, t_pos[q], z_pos[ slices[q].slice_location ], slices[q].slice_location \n" )
        
        images3D[ t_pos[q] ] = copy_2D_to_3D( 
                                              slices[q].image, 
                                              images3D[ t_pos[q] ], 
                                              [ 0, 0, z_pos[ slices[q].slice_location ] ] 
                                              )
        
        if slices[q].validation:           
            o_ims[ t_pos[q] ] = copy_2D_to_3D( slices[q].o_contour, o_ims[ t_pos[q] ], [ 0, 0, z_pos[ slices[q].slice_location ] ] )
            i_ims[ t_pos[q] ] = copy_2D_to_3D( slices[q].i_contour, i_ims[ t_pos[q] ], [ 0, 0, z_pos[ slices[q].slice_location ] ] )
            p1_ims[ t_pos[q] ] = copy_2D_to_3D( slices[q].p1_contour, p1_ims[ t_pos[q] ], [ 0, 0, z_pos[ slices[q].slice_location ] ] )
            p2_ims[ t_pos[q] ] = copy_2D_to_3D( slices[q].p2_contour, p2_ims[ t_pos[q] ], [ 0, 0, z_pos[ slices[q].slice_location ] ] )
        
    s.close()
    
    return images3D, o_ims, i_ims, p1_ims, p2_ims


#images3D=slices_2D_to_3D(slices)


def _str(n):
    result=str(n)
    if len(result) < 4:
        for q in range(len(result), 4):
            result="0" + result
    return result


# print _str(12)
# print _str(7)
# print _str(123)
# print _str(1023)


def save_images( prefix, images3D ):
    try:
        makedirs(prefix[0:prefix.rfind("/")])
    except:
        1==1
    for q in range(len(images3D)):
        images3D[q].writeamira( prefix + "_" + _str(q) + ".am" )


# save_images("/home/ujoimro/tmp/test3D/images", images3D)


def create_amira( pgm, validation ):
    
    slices = parse_directory( pgm, {} )
    slices = import_validation( validation, slices )
    
    images3D, o_ims, i_ims, p1_ims, p2_ims = slices_2D_to_3D(slices)
    
    try:
        makedirs( pgm + "/test3D" )
    except:
        1==1
    
    save_images( pgm + "/test3D/images", images3D )
    save_images( pgm + "/test3D/ocont", o_ims )
    save_images( pgm + "/test3D/icont", i_ims )
    save_images( pgm + "/test3D/p1cont", p1_ims )
    save_images( pgm + "/test3D/p2cont", p2_ims )


# SEGM="/home/ujoimro/doc/projects/MICCAI/DATA/Challenge Data (Training)"
# VAL="/home/ujoimro/doc/projects/MICCAI/Validation/Challenge Contours (Training)"


# parameters=[]
# for q in [ "SC-HF-I-01", "SC-HF-I-04", "SC-HF-NI-03", "SC-HF-NI-34", \
#           "SC-HYP-01", "SC-HYP-38", "SC-N-02", "SC-N-40", "SC-HF-I-02", \
#           "SC-HF-I-40", "SC-HF-NI-04", "SC-HF-NI-36", "SC-HYP-03", "SC-HYP-40", "SC-N-03" ]:
#               
#     parameters.append([ SEGM + "/" + q + "_SEGM", VAL + "/" + q + "/contours-manual/IRCCI-expert" ])


# print parameters[0][0]
# print parameters[0][1]
# create_amira( parameters[0][0], parameters[0][1] )


# parallel_map( create_amira, parameters )




















# LuM end of file
