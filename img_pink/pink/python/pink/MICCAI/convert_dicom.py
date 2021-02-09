#! /usr/bin/python

# UjoImro, 2009
# This software is meant to be free.

1+1


from sys import path
path.append("/home/ujoimro/doc/projects/Pink/python/")
testdir="/home/ujoimro/doc/projects/Pink/python/test/"
from pink import *
from glob import glob
from pink import parallel_map
from os import system, makedirs


dicom2pgm_exec="/home/ujoimro/doc/projects/MICCAI/Pinktmp/Analyse/dicom2pgm"
printfile_exec="/home/ujoimro/doc/projects/MICCAI/GCDM/bin/bin/PrintFile"


def _makedirs(path):
    try:
        makedirs(path)
    except:
        1==1


def convert_dicom( indir, outdir ):
    # cutting of the end '/' if there's one
    indir=normalize( indir )
    outdir=normalize( outdir )
    
    # creating the necessary directories
    _makedirs( outdir + "/Headers" )
    _makedirs( outdir + "/pgm_raw" )
    
    #converting the 'DICOM' files into 'pgm'-s and saving they metadata as textfile  
    params=[]
    for q in glob( indir + "/*.dcm" ):
        params.append( [ q, 
                         outdir + "/pgm_raw/" + cut_ext(q) + ".pgm", 
                         outdir + "/Headers/" + cut_ext(q) + ".txt" ] 
                         )
    return parallel_map( dicom2pgm, params )


def normalize(dirname):
    """
    Normalizes the directory name, that is to say cuts of the / from the end if exists.
    """
    if dirname[len(dirname)-1]=="/":
        return dirname[0:len(dirname)-1]
    else:
        return dirname


# normalize("/home/ujoimro")


# normalize("/home/ujoimro/")


def cut_ext( filename ):
    """
    returns the raw filename, that is to say the filename without the directory prefixe and extention
    """
    return filename[ filename.rfind("/") + 1 : filename.rfind(".") ]


#cut_ext( "/some/dir name/some filename.some.exts" )


def dicom2pgm( dicom, pgm, header ):
    # Converting the files into PGM-s
    system( dicom2pgm_exec + " -n -e 0 255 \"" + dicom + "\" \"" + pgm + "\" &> /dev/null ")
    
    # Dumping the DICOM header into a text FILE
    system( printfile_exec + " filein=\"" + dicom + "\" > \"" + header + "\""  )


#DD="/home/ujoimro/doc/projects/MICCAI/DATA/Challenge Data (Training)"


#convert_dicom( DD + "/SC-N-03/DICOM", DD + "/SC-N-03_SEGM" )


#print parameters[0][0]
#print parameters[0][1]
#convert_dicom( parameters[0][0], parameters[0][1] )


# parameters=[]
# for q in [ "SC-HF-I-01", "SC-HF-I-04", "SC-HF-NI-03", "SC-HF-NI-34", "SC-HYP-01", "SC-HYP-38", "SC-N-02", "SC-N-40", "SC-HF-I-02",  "SC-HF-I-40", "SC-HF-NI-04", "SC-HF-NI-36", "SC-HYP-03", "SC-HYP-40", "SC-N-03" ]:
#    parameters.append([ DD + "/" + q + "/DICOM", DD + "/" + q + "_SEGM" ])


# parallel_map( convert_dicom, parameters )








# LuM end of file
