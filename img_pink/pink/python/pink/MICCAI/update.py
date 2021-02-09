#! /usr/bin/python
# UjoImro, 2009
# This software is meant to be free

from pink.MICCAI import sage2python


def update():

    # Converting a directory of DICOM images 
    sage2python( 
        infile="/home/ujoimro/.sage/sage_notebook/worksheets/admin/21/worksheet.txt", 
        outfile="/home/ujoimro/doc/projects/Pink/python/pink/MICCAI/convert_dicom.py" 
        )

    # parallel map
    sage2python( 
        infile="/home/ujoimro/.sage/sage_notebook/worksheets/admin/22/worksheet.txt", 
        outfile="/home/ujoimro/doc/projects/Pink/python/pink/parallel_map.py" 
        )
    

    # Parse DICOM headers to a 4D volume 
    sage2python( 
        infile="/home/ujoimro/.sage/sage_notebook/worksheets/admin/23/worksheet.txt", 
        outfile="/home/ujoimro/doc/projects/Pink/python/pink/MICCAI/headers_to_4D.py" 
        )


    # ITK object conversion 
    sage2python( 
        infile="/home/ujoimro/.sage/sage_notebook/worksheets/admin/26/worksheet.txt", 
        outfile="/home/ujoimro/doc/projects/Pink/python/pink/MICCAI/itk2pink.py" 
        )
    


    sage2python(
        infile="/home/ujoimro/.sage/sage_notebook/worksheets/admin/31/worksheet.txt", 
        outfile="/home/ujoimro/doc/projects/Pink/python/pink/MICCAI/isotropization.py" 
        )


    sage2python(
        infile="/home/ujoimro/.sage/sage_notebook/worksheets/admin/32/worksheet.txt", 
        outfile="/home/ujoimro/doc/projects/Pink/python/pink/MICCAI/registration.py" 
        )

    sage2python(
        infile="/home/ujoimro/.sage/sage_notebook/worksheets/admin/36/worksheet.txt", 
        outfile="/home/ujoimro/doc/projects/Pink/python/pink/MICCAI/get_a_point.py" 
        )

    sage2python(
        infile="/home/ujoimro/.sage/sage_notebook/worksheets/admin/37/worksheet.txt",
        outfile="/home/ujoimro/doc/projects/Pink/python/pink/MICCAI/create_non_isotropized.py"
        )

    sage2python(
        infile="/home/ujoimro/.sage/sage_notebook/worksheets/admin/38/worksheet.txt",
        outfile="/home/ujoimro/doc/projects/Pink/python/pink/MICCAI/generate_moments.py"
        )

    sage2python(
        infile="/home/ujoimro/.sage/sage_notebook/worksheets/admin/39/worksheet.txt",
        outfile="/home/ujoimro/doc/projects/Pink/python/pink/MICCAI/move_the_rest.py"
        )

    sage2python(
        infile="/home/ujoimro/.sage/sage_notebook/worksheets/admin/40/worksheet.txt",
        outfile="/home/ujoimro/doc/projects/Pink/python/pink/MICCAI/back_to_dicom.py"
        )











# LuM end of file
