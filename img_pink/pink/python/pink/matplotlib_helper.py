# -*- coding: utf-8 -*-
#
# This software is licensed under 
# CeCILL FREE SOFTWARE LICENSE AGREEMENT

# This software comes in hope that it will be useful but 
# without any warranty to the extent permitted by applicable law.

# (C) H. Talbot <talboth@esiee.fr>, 2011
# Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX

# histograms 

import sys
import pink

try:
    import matplotlib as mpl
except:
    print("error: could not import matplotlib, try to install python-numpy")
    raise python_component_missing

## helper function for matplotlib, in particular image plotting functions
def view(pinkimg):
	mpl.pyplot.imshow(pink.numpy_conv.pink2numpy(pinkimg),cmap=mpl.cm.gray,interpolation='none')

# LuM end of file
