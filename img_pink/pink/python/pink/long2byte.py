# -*- coding: utf-8 -*-
#
# This software is licensed under 
# CeCILL FREE SOFTWARE LICENSE AGREEMENT

# This software comes in hope that it will be useful but 
# without any warranty to the extent permitted by applicable law.
  
# (C) UjoImro <ujoimro@gmail.com>, 2009
# Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX

"""
This is the module for 'long2byte' parameterization.
"""

from pink import cpp_long2byte

def long2byte( I, mode='truncate', nb_new_val=255 ):
    """
    the manual is in _doc_long2byte

    """
    
    if mode=='truncate':
        int_mode=0

    elif mode=='modulo':
        int_mode=1

    elif mode=='scale':
        int_mode=2
        
    elif mode=='histogram':
        int_mode=3
        if nb_new_val < 0 or nb_new_val > 255:
            raise "error: nb_new_val should be specified and/or within 0..255"

    elif mode=='square_root':
        int_mode=4

    elif mode=='log':
        int_mode=5

    else:
        raise "error: mode must be one of following: 'truncate', 'modulo', 'scale', 'histogram', 'square_root' or 'log'"
    

    return cpp_long2byte( I, int_mode, nb_new_val )





# LuM end of file

        
