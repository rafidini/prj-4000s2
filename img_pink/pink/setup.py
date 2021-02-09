# UjoImro, 2011
# Pink's Python installer
# This file is used to generate a Python installer
# executable on Microsoft Windows

# on windows
# copy libcpp_pink.pyd to Pink-svn/python
# cd Pink-svn/python
# c:\python27\python.exe ..\setup.py bdist --format=wininst

from distutils.core import setup

setup( name         = 'pink-image',
       version      = '0.9.r566',
       description  = 'Pink Image Processing Library',
       author       = 'Michel Couprie et al.',
       author_email = 'pink-image-devel@lists.berlios.de',
       license      = 'CeCILL',
       url          = 'http://www.pinkhq.com',
       packages     = [ 'pink', 'pink.cpp', 'pink.MICCAI' ],
       data_files = [ ('DLLs', ['libcpp_pink.pyd'])]
     )




# LuM end of file
