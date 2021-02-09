#!/usr/bin/python
# -*- coding: utf-8 -*-

# This software is licensed under 
# CeCILL FREE SOFTWARE LICENSE AGREEMENT

# This software comes in hope that it will be useful but 
# without any warranty to the extent permitted by applicable law.
  
# (C) UjoImro, 2011
# Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
# ujoimro@gmail.com

# (C) UjoImro, 2012
# ProCarPlan s.r.o.
# ujoimro@gmail.com

#

""" 
This program searches all the source files in the given directory and
extracts the first level doxygen documentation from it.  The first
level documentation corresponds to the statement \file. From
'filename.*', then the program generates a macro

#define doc__filename__*__ "documentation for\n"
"the file filename.* "

The program should be used as:
./python_documenter directory documentation.c
"""

import sys
from sys import argv
from os import listdir
from os.path import join
from time import strftime

def header():
    result = ""
    result += "// This file has been generated automatically by the \n"
    result += "// python_documenter, part of the PInK package (UjoImro, 2011)\n"
    result += "// the date of generation: " + strftime("%Y-%m-%dT%H:%M:%SZ")+"\n"
    result += "// warning: DO NOT EDIT. All your changes will be lost at \n"
    result += "// the next generation\n\n"
    result += "#ifndef PINK__PYTHON__DOCUMENTER__HPP__\n"
    result += "#define PINK__PYTHON__DOCUMENTER__HPP__\n"

    return result

def footer():
    result = ""
    result += "#endif /* PINK__PYTHON__DOCUMENTER__HPP__ */\n"
    result += "// LuM end of file\n"

    return result

def simple_filter(text):
    result = text
    result = result.replace( "\n", ""  )
    result = result.replace( "/*!", "" )
    result = result.replace( "*/", ""  )
    return result

def advanced_filter(text):
    result = ""

    rep = False
    xml = False

    # the first cases are beginings of the eliminations \li <B> ... are eliminated
    for current in text:
        if current=='\\':
            rep = True
            
        elif current=='<':
            xml = True
      

        if (not rep) and (not xml):                
            # here we filter some inconvenient characters as '\"'
            if current =='\"':
                result += "\\\""

            # newline is handled by readline
            # # the newline must be handled carefully
            # elif (current == '\n'):
            #     result += "\\n\"    \\\n\""

            else:        
                result += current;          
            

        # these are the ends of eliminations \li' ' <B'>' ... note the end of the eliminations
        if (current=='>') and (xml):
            xml = False;        
        
        elif (current==' ') and (rep):
            rep = False;
            result += " "
    
    return result

def parse_file(filedesc, instance_name):
    result = instance_name
    rawdoc = get_raw_text(filedesc);

    for q in rawdoc:
        result += "\"" + advanced_filter(simple_filter(q)) + "\\n\"         \\\n"

    result += "\"\\n\""
    return result

def get_raw_text(filedesc):
    result = []
    
    inside_documentation = False
    documented = False

    q = filedesc.readline()
    while q!="":

        if not inside_documentation and (q.find("/*!")!=-1):
            documented = True
            inside_documentation = True            

        if inside_documentation:            
            result.append(q)

        if inside_documentation and (q.find("*/")!=-1):
            inside_documentation = False

        q = filedesc.readline()

    if not documented:
        result = ["WRITE ME!!!"]

    return result


def parse_filename(filename):
    return "#define doc__" + filename.replace(".", "__").replace("#", "") + "__ "


### **********************************************************************
### **********************************************************************
### **********************************************************************
### ***                            MAIN                                ***
### **********************************************************************
### **********************************************************************
### **********************************************************************

if len(argv)!=3:
#                                0               1           2
    print("error: usage: ./python_documenter directory documentation.c")
    exit(1)

print("Boost python documenter")

dir = argv[1]
if sys.version_info[0] >= 3:
    outfile = open( argv[2], "w", encoding="latin1" )
else:
    outfile = open( argv[2], "w" )

outfile.write(header())

for q in listdir(dir):
    opened = False
    try:
        if sys.version_info[0] >= 3:
            filedesc = open( join(dir, q), "r", encoding="latin1" )
        else:
            filedesc = open( join(dir, q), "r" )
        opened = True
    except:
        print("skipping " + join(dir, q))

    if opened:
        print("processing file " + join(dir, q))
        outfile.write( parse_file( filedesc, parse_filename(q) ) )
        filedesc.close()
        outfile.write("\n\n\n")

outfile.write(footer())
outfile.close()





































# Lum end of file
