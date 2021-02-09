#! /usr/bin/python
# UjoImro, 2009
# This software is meant to be free





"""
This file converts a sage notebook to a python module. This essentially means removing 
all the text and only leaving the code in the cells. 

note: This file can also be imported as a module!
note: The order of the cells (if different from the chronologic order ) will not be respected.
"""

from sys import argv


def sage2python( infile, outfile ):
    _if = open( infile, "r" )
    _of = open( outfile, "w" )


    incell=False
    incode=False
    first=False

    _of.write( "#! /usr/bin/python\n\n# UjoImro, 2009\n# This software is meant to be free.\n\n")

    for q in _if.readlines():

        if q[0:3] == "{{{":
            first = True
            incell = True
            incode = True

        elif q[0:3] == "///":
            incode = False
            _of.write("\n\n")

        elif q[0:3] == "}}}":

            if incode == True:
                _of.write("\n\n")

            incode = False
            incell = False

        if first:
            first = False
        else:
            if incode:
                _of.write(q)

    _of.writelines("# LuM end of file\n")

    _if.close()
    _of.close()



# if len(argv) != 3:
#     print """

# error: usage

# ./sage2python.py infile outfile

# """
# else:
#     sage2python( infile=argv[1], outfile=argv[2] )




































# LuM end of file
