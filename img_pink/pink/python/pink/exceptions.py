# -*- coding: utf-8 -*-
#
# This software is licensed under 
# CeCILL FREE SOFTWARE LICENSE AGREEMENT

# This software comes in hope that it will be useful but 
# without any warranty to the extent permitted by applicable law.
  
# (C) UjoImro, 2011
# Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
# ujoimro@gmail.com


"""
This file contains Pink's exceptions called from Python.
"""

try:
    class python_component_missing: BaseException
except: # python 2.4 in debian 4 (etch) does not have the BaseException class
    class python_component_missing:
        pass
















# LuM end of file
