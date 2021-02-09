# -*- coding: utf-8 -*-
#
# This software is licensed under 
# CeCILL FREE SOFTWARE LICENSE AGREEMENT

# This software comes in hope that it will be useful but 
# without any warranty to the extent permitted by applicable law.
  
# (C) UjoImro <ujoimro@gmail.com>, 2009
# Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX

"""
This is the iSize vector iterator. You can feed it vectors and it iterates on them like:
for q in iSize([5,5]):
   print str(q)

output:

[0, 0]
[0, 1]
[0, 2]
[0, 3]
[0, 4]
[1, 0]
[1, 1]
[1, 2]
[1, 3]
[1, 4]
[2, 0]
[2, 1]
[2, 2]
[2, 3]
[2, 4]
[3, 0]
[3, 1]
[3, 2]
[3, 3]
[3, 4]
[4, 0]
[4, 1]
[4, 2]
[4, 3]
[4, 4]
"""

class iSize:
    pos = []
    size = []
    def __init__( self, size ):
        self.size = size
        self.pos = []
        for q in range( len(size) - 1 ):
            self.pos.append( 0 )
        self.pos.append(-1)
                       
    def __iter__( self ):
        for q in range( len( self.pos ) - 1 ):
            self.pos[q] = 0
        self.pos[ len( self.pos ) - 1 ] = -1
        return self
        
    def next( self ):
        d = len( self.size )
        q = d - 1
        while self.pos[q] >= self.size[q] - 1 and q > 0:
            q -= 1
            
        if q == 0 and self.pos[q] >= self.size[q] - 1:
            raise StopIteration
    
        self.pos[q] += 1;
        while q < d-1 :
            q += 1
            self.pos[q]=0
  
        return self.pos







#LuM end of file
