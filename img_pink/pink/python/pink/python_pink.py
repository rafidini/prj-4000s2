# -*- coding: utf-8 -*-
#
# This software is licensed under 
# CeCILL FREE SOFTWARE LICENSE AGREEMENT

# This software comes in hope that it will be useful but 
# without any warranty to the extent permitted by applicable law.
  
# (C) UjoImro <ujoimro@gmail.com>, 2009
# Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX

print("note: This package is obsolete!")

import pink

def readimage(filename):
    XV = pink.cpp_readimage(filename)
    if XV.imtype()=="uint8_t":
        return pink.char_image(XV)
    elif XV.imtype()=="int16_t":
        return pink.short_image(XV)
    elif XV.imtype()=="int32_t":
        return pink.int_image(XV)
    elif XV.imtype()=="float":
        return pink.float_image(XV)
    elif XV.imtype()=="double":
        return pink.double_image(XV)
    else:
        print "error: bad image type '" + XV.imtype() + "'"
        raise "Bad image type"


def skeleton(image, connex, priority=-1, inhibit=-1):
    """
      \brief ultimate binary skeleton guided by a priority image

      <B>Usage:</B> skeleton in.pgm prio connex [inhibit] out.pgm
      
      <B>Description:</B>
      Ultimate binary skeleton guided by a priority image.
      The lowest values of the priority image correspond to the highest priority.

      The parameter \b prio is either an image (byte, int32_t, float or double), or a numerical code
      indicating that a distance map will be used as a priority image; 
      the possible choices are:
      \li 0: approximate euclidean distance
      \li 1: approximate quadratic euclidean distance
      \li 2: chamfer distance
      \li 3: exact quadratic euclidean distance
      \li 4: 4-distance in 2d
      \li 8: 8-distance in 2d
      \li 6: 6-distance in 3d
      \li 18: 18-distance in 3d
      \li 26: 26-distance in 3d
      
      The parameter \b connex indicates the connectivity of the binary object.
      Possible choices are 4, 8 in 2d and 6, 26 in 3d.
      
      If the parameter \b inhibit is given and is an integer different from -1,
      then the points which correspond to this priority value will be left unchanged. 
      If the parameter \b inhibit is given and is a binary image name,
      then the points of this image will be left unchanged. 
      
      Let X be the set corresponding to the input image \b in.pgm.
      Let P be the function corresponding to the priority image.
      Let I be the set corresponding to the inhibit image, if given, or the empty
      set otherwise.
      The algorithm is the following:
      
      \verbatim
      Repeat until stability
      Select a point x in X \ I such that P[x] is minimal
      If x is simple for X then
      X = X \ {x}
      Result: X
      \endverbatim
      
      References:<BR> 
      [BC07] G. Bertrand and M. Couprie: <A HREF="http://www.esiee.fr/~coupriem/Pdf/chapitre_topo.pdf">"Transformations topologiques discretes"</A>, in <I>G&eacute;om&eacute;trie discr&egrave;te et images num&eacute;riques</I>, D. Coeurjolly and A. Montanvert and J.M. Chassery, pp.&nbsp;187-209, Herm&egrave;s, 2007.<BR>
      <B>Types supported:</B> byte 2d, byte 3d
      
      <B>Category:</B> topobin
      \ingroup  topobin
      
      \author Michel Couprie

      <B>Example:</B>
      
      skeleton circuit1 8 8 circuit1_skeleton
      
      """

    ### def skeleton(image, connex, priority=-1, inhibit=-1):
    
    if priority==-1:
        raise "Priority must be defined. See 'help(pink.skeleton)'"
    
    if type(priority)==type(-1): # priority is an integer

        if inhibit==-1: # nothing is inhibited

            return pink.cpp_py_skeleton_im_int_int_void( image, priority, connex )

        else: # something is inhibited

            if type(inhibit)==type(-1): # we have an inhibited value

                return pink.cpp_py_skeleton_im_int_int_int( image, priority, connex, inhibit )

            else: # we have an inhibited image

                return pink.cpp_py_skeleton_im_int_int_im( image, priority, connex, inhibit )

    else: # priority is NOT an integer (it is an image)

        if inhibit==-1: # nothing is inhibited

            return pink.cpp_py_skeleton_im_im_int_void( image, priority, connex )

        else: # something is inhibited

            if type(inhibit)==type(-1): # we have an inhibited value

                return pink.cpp_py_skeleton_im_im_int_int( image, priority, connex, inhibit )

            else: # we have an inhibited image

                return pink.cpp_py_skeleton_im_im_int_im( image, priority, connex, inhibit )







































# LuM end of file
