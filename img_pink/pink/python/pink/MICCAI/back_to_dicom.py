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
import os


from glob import glob
from pink import parallel_map, readimage, drawcurve, char_image, vint, show_image
from os import system, makedirs
from pink.MICCAI import iSize, convert_dicom, parse_directory, slices_2D_to_3D, import_validation, image2itk, image2pink, isotropize_itk, itk_image_size, _str
from itk import Image, ImageRegion, Size, Index
import itk


# OLD DD="/home/ujoimro/doc/projects/MICCAI/DATA/Challenge Data (Training)"


# OLD directories =  [ "SC-HF-I-01", "SC-HF-I-04", "SC-HF-NI-03", "SC-HF-NI-34", "SC-HYP-01", "SC-HYP-38", "SC-N-02", "SC-N-40", "SC-HF-I-02",  "SC-HF-I-40", "SC-HF-NI-04", "SC-HF-NI-36", "SC-HYP-03", "SC-HYP-40", "SC-N-03" ]

#directories =  [ "SC-HF-I-05", "SC-HF-I-07", "SC-HF-NI-07", "SC-HF-NI-33", "SC-HYP-06", "SC-HYP-08", "SC-N-05", "SC-N-07", "SC-HF-I-06", "SC-HF-I-08", "SC-HF-NI-11", "SC-HF-NI-34", "SC-HYP-07", "SC-HYP-37", "SC-N-06" ]

#directories = [ "SC-HF-NI-31" ]


def addconst(filename, val):
    os_system("addconst "+filename+" "+str(val)+" "+filename)


def median(l):
    if len(l) % 2 == 1:
        return l[(len(l)+1)/2-1]
    else:
        return l[len(l)/2-1]


def dupl(j,ds_w,ds):
    return int(round(float(j)/float(ds_w)*float(ds)))


def getdimension(filename):
    s=open(filename,"r")
    s.readline();#P5
    #skipping if there are any comments
    line=s.readline().split(" ")
    while line[0][0]=="#":
        line=s.readline().split(" ")
    return map(int, line)


def redprint(s):
    return 
    print '\033[0;31m'+s+'\033[m'


def gennum(n):
    if n<10:
        return "000"+str(n)+".pgm"
    elif n<100:
        return "00"+str(n)+".pgm"
    elif n<1000:
        return "0"+str(n)+".pgm"
    else: 
        return str(n)+".pgm"


def genimage(w,h,z,outfile):
    redprint( "genimage " + str(w) + " " + str(h) + " " + str(z) + " " + " 255 " + outfile)
    os_system( "genimage " + str(w) + " " + str(h) + " " + str(z) + " " + " 255 " + outfile)


def explode(infile, name_prefix):
    redprint("explode "+infile+" "+name_prefix)
    os_system("explode "+infile+" "+name_prefix)


def insert(sourcefile, destfile, x, y, z):
    redprint("insert "+sourcefile+" "+destfile+" "+str(x)+" "+str(y)+" "+str(z)+" "+destfile)
    os_system("insert "+sourcefile+" "+destfile+" "+str(x)+" "+str(y)+" "+str(z)+" "+destfile)


def os_system( command ):
    print command


def get_inversion_data( filename ):
    result={}
    
    dick = open( filename, "r" )
    
    
    for q in dick.readlines()[2:]:
        dick_pos, t, z = q.split(" ")[0:3]
        result[ (int(z), int(t)) ] = dick_pos
    
    
    
    
    dick.close()
    
    
    
    return result


#inversion_data = get_inversion_data( "/home/ujoimro/doc/projects/MICCAI/DATA/Challenge Data (Training)/SC-HF-I-01_SEGM/inversion_data.txt" )


#inversion_data[(4,16)]


def convert_back_part1_translates( pwd, translates ):

    w=h=z=t=0

    # print "checking the resolution"
    pgmInfo = open( pwd + "/pgm/pgmInfo" , "r" );
    z = int(pgmInfo.readline().split(" ")[0])
    t = int(pgmInfo.readline().split(" ")[0])
    pixel_spacing = float(pgmInfo.readline().split(" ")[0])
    thickness = int(round(float(pgmInfo.readline().split(" ")[0])))
    #print z, t, pixel_spacing, thickness
    pgmInfo.close()

    w,h = getdimension(pwd+"/pgm/rep0/i0000.pgm")[0:2]
    #reading ds_wanted
    ds_wanted = getdimension( pwd + "/IsoVol/res/LVM"+gennum(1) )[2]
    
    # print "w =", w, ", h =", h, ", z =", z, ", t =", t,", pixel_spacing =", \
    pixel_spacing, ", thickness =", thickness, ", ds_wanted =", ds_wanted


    # print "reading translations and crop"
    GeomParam = open(pwd+"/GeomParam")
    slice_min, slice_max = map(int, GeomParam.readline().split(" ")[0:2])
    x_corner, y_corner, x_size, y_size = map(int, GeomParam.readline().split(" ")[0:4])
    translations = [[0,0,0]]
    
    for q in range ( slice_max - slice_min ):
        translations.append( map( int, GeomParam.readline().split(" ")[0:3] ) )
    
    GeomParam.close()
    
    #print "slice_min =", slice_min, ", slice_max =", slice_max, ", x_corner =", x_corner, ", y_corner =", y_corner, ", x_size =", x_size, ", y_size =", y_size
    #print "translations =", translations

    # print "calculating the slices positions"
    
    table=[]
    
    for q in range( slice_min, slice_max + 1 ):
        table.append([])
    
    for q in range(ds_wanted):
        table[ dupl( q, ds_wanted, slice_max - slice_min ) ].append(q)
    
    slices = map( median, table )
    
    
    
    currfile = pwd + "/tmp/LVM_ssc" + gennum(q)

#    SOLUTION = readimage( pwd + "/IsoVol/res/LVM" + gennum(q+1) )
        
    # print pwd + "/IsoVol/res/LVM" + gennum(q+1)
        
    for e in range(z):
        if ( slice_min <= e ) and ( e <= slice_max ):
            eps = slices[e] #estimated position of the slice 
            
            #SOLUTION[[  eps ]]
            
            #print "e =", e, "gennum(eps)", gennum(eps), \
            #      "x_corner - translations[e-1][1]", x_corner - translations[e-1][1], \
            #      "y_corner - translations[e-1][2]", y_corner - translations[e-1][2], \
            #      "e", e
                  
            translates[e] = [ x_corner - translations[e][1], y_corner - translations[e][2], eps ]
                    
    return [ w, h, z ], translates


#dicom_size, translates = convert_back_part1_translates( pwd = MOMENT + "/" + "SC-HF-I-05" + "_SEGM/0000", translates={} )


#print "dicom_size =", dicom_size
#print "translates =", translates


#inversion_data





def solution_back_part2( directory, inversion_data, translates, dicom_size, first_slice ):
    
    print directory
    
    solution_dir = directory + "/dicom_solution"
    
    try:
        makedirs( solution_dir )
    except:
        1==1
        
    for q in range(20): # q is the TIME in this case
        
        if os.path.exists( directory + "/" + _str(q) + "/IsoVol/res/LVC0001.pgm" ) and os.path.exists( directory + "/" + _str(q) + "/IsoVol/res/LVCM0001.pgm" ):
        
            i_solution = readimage( directory + "/" + _str(q) + "/IsoVol/res/LVC0001.pgm" )
            #i_solution.writeamira( directory + "/" + _str(q) + "/IsoVol/res/LVC0001.am" )
            o_solution = readimage( directory + "/" + _str(q) + "/IsoVol/res/LVCM0001.pgm" )
            
            #print directory + "/" + _str(q) + "/IsoVol/res/LVC0001.pgm"
            
            
            for w in range( dicom_size[2] ): # w is the Z in this case, shifted with 3
            
                i_dick = char_image( dicom_size[:2] )
                o_dick = char_image( dicom_size[:2] )
                           
                for e in iSize([100,100]):
                    # i_dick's positions [ translate_x, translate_y, eps ]
                    
                    
                    # the position in the 3D image has to be calculated manually due to a bug in pinkuin.
                    
                    try:
                        i_dick[[ translates[w][0] + e[0], translates[w][1] + e[1] ]] \
                                       = i_solution[ e + [translates[w][2]] ]
                                       
                        
                    except:
                        print str( translates[w][0]) + "= translates[w][0]" 
                        print str( translates[w][1]) + "= translates[w][1]" 
                        print str( e[0] ) + "=e[0]"
                        print str( e[1] ) + "=e[1]"
                        
                        print str([ translates[w][0] + e[0], translates[w][1] + e[1] ])
                        print str(e)+"+"+str(translates[w][2])
                          
                    o_dick[[ translates[w][0] + e[0], translates[w][1] + e[1] ]] \
                                   = o_solution[ e + [translates[w][2]] ]
    
                ### CUT IS HERE
                i_dick.writeimage( solution_dir + "/" + _str( inversion_data[( w + first_slice, q )] ) + "_i.pgm" )
                o_dick.writeimage( solution_dir + "/" + _str( inversion_data[( w + first_slice, q )] ) + "_o.pgm" )


#i_dick=solution_back_part2( MOMENT + "/" + "SC-HYP-38" + "_SEGM", inversion_data, translates, dicom_size )
#i_dick=solution_back_part2( MOMENT + "/" + "SC-HF-I-04" + "_SEGM", inversion_data, translates, dicom_size )
#SC-HF-I-04_SEGM


#parameters=[]
#for q in [ \
#                 [ "SC-HF-I-05",  3, 9  ], \
#                 [ "SC-HF-I-06",  1, 10 ], \
#                 [ "SC-HF-I-07",  1, 7  ], \
#                 [ "SC-HF-I-08",  2, 10 ], \
#                 [ "SC-HF-NI-07", 2, 11 ], \
#                 [ "SC-HF-NI-11", 2, 11 ], \
#                 [ "SC-HF-NI-33", 3, 9 ], \
#                 [ "SC-HF-NI-34", 3, 8 ], \
#                 [ "SC-HYP-06",   2, 7 ], \
#                 [ "SC-HYP-07",   3, 9 ], \
#                 [ "SC-HYP-08",   4, 10 ], \
#                 [ "SC-HYP-37",   3, 7 ], \
#                 [ "SC-N-05",     2, 5 ], \
#                 [ "SC-N-06",     3, 7 ],  \
#                 [ "SC-N-07",     2, 8 ] \
#                 ]:
#    parameters.append( q )

#parameters.append(["SC-HF-NI-31", 4, 9])


#for q in parameters:
#  
#    
#    try:
#    
#        print q
#        #print MOMENT + "/" + q[0] + "_SEGM/0000"
#        
#        dicom_size, translates = convert_back_part1_translates( \
#                                      pwd = MOMENT + "/" + q[0] + "_SEGM/0000", translates={} )
#
#    
#        inversion_data = get_inversion_data( BD + "/" + q[0] + "_SEGM/inversion_data.txt" )
#  
#        solution_back_part2( MOMENT + "/" + q[0] + "_SEGM", inversion_data, translates, dicom_size, first_slice=q[1] )
#        
#        print "success"        
#
#    except:
#        1==1


#dicom_size, translates = convert_back_part1_translates( \
#                                      pwd = MOMENT + "/" + 'SC-HF-NI-11' + "_SEGM/0000", translates={} )


#dicom_size


#translates

































































# LuM end of file
