# UjoImro, 2011

# PARAMETERS
PINK="/home/ujoimro/tmp/build/pink.sh"

"""
This file executes all the tutorials and thus generates a CDE 
package.
"""

import fnmatch
import os
from sys import argv

def find_files(directory, pattern):
    matches = []
    for root, dirnames, filenames in os.walk(directory):
        for filename in fnmatch.filter(filenames, '*.py'):
            matches.append(os.path.join(root, filename))
    return(matches)


# getting the current working directory
CWD = os.getcwd()

# looking for tutorial solutions
tutorials = find_files("python", "*.py")

for q in tutorials:    
    # getting the filename
    filename = os.path.basename(q)
    if filename != "generate_package.py":

        # changing the working directory to that of the Python file
        os.chdir(CWD + "/" + os.path.dirname(q))

        # launching the filename
        print("Launching " + filename)
        execfile(filename)


# going back to the starting directory
os.chdir(CWD)
