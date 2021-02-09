# -*- coding: utf-8 -*- 
#  
# This software is licensed under  
# CeCILL FREE SOFTWARE LICENSE AGREEMENT 
 
# This software comes in hope that it will be useful but  
# without any warranty to the extent permitted by applicable law. 
   
# (C) UjoImro <ujoimro@gmail.com>, 2012 
# ProCarPlan s.r.o. 
 
import os 
import pink 
import filecmp
import unittest 
import xmlrunner 
 
try:   
    IMAGES  = os.environ["PINKTEST"] + "/images" 
    RESULTS = os.environ["PINKTEST"] + "/results_prev" 
except KeyError:  
    raise Exception, "PINKTEST environment variable must be defined for the testing module. It must point to the testimages directory" 

class skeleton_t(unittest.TestCase):

    def test_image2skel(self):
        im = pink.cpp.readimage( IMAGES + '/skeleton_t/uo_w.pgm' )
        imf = pink.cpp.frame(im, 255)       # adds a white frame inside the image
        imf = pink.cpp.frame_around(imf, 0) # adds a black frame outside the image
        skelim = pink.cpp.skeleton(imf, 0, 8)    # get rid of simple points

        result = pink.cpp.image2skel( skelim, 8 )
        result.writeskel('_result.skel');
                
        self.assertTrue( filecmp.cmp('_result.skel', IMAGES + '/skeleton_t/s.skel') )


    def test_skel2graph(self):

        skel = pink.cpp.skeleton_t( IMAGES + '/skeleton_t/s.skel' )
        #skel.writeskel("_skelpy.skel")
        graph = pink.cpp.skel2graph( skel, 1 )
        graph.writegraph("_graphpy.graph")
        graph.writeps("_graphpy.ps" , 0.3)

        self.assertTrue( filecmp.cmp('_graphpy.graph', IMAGES + '/skeleton_t/s.graph') )
        #self.assertTrue( filecmp.cmp('_graphpy.ps', IMAGES + '/skeleton_t/s.ps') ) // the result is slightly different due tu double/float collision
        self.assertTrue( filecmp.cmp('_graphpy.ps', IMAGES + '/skeleton_t/graphpy.ps') )


if __name__ == '__main__':
    unittest.main(testRunner=xmlrunner.XMLTestRunner(output='test-reports'))
