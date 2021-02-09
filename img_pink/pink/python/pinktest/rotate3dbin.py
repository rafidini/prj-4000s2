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
import unittest 
import xmlrunner 
 
try:   
    IMAGES  = os.environ["PINKTEST"] + "/images" 
    RESULTS = os.environ["PINKTEST"] + "/results_prev" 
except KeyError:  
    raise Exception, "PINKTEST environment variable must be defined for the testing module. It must point to the testimages directory" 


class rotate3dbin(unittest.TestCase):
    def test_0(self):
        result = pink.cpp.rotate3dbin( pink.cpp.readimage( IMAGES + '/3dbyte/binary/b3_se_5_7_9.pgm' ), 25, x)
        gold   = pink.cpp.readimage( RESULTS + '/rotate3dbin_b3_se_5_7_9_x_25.pgm')
        self.assertTrue( result == gold )


    def test_1(self):
        result = pink.cpp.rotate3dbin( pink.cpp.readimage( IMAGES + '/3dbyte/binary/b3_se_5_7_9.pgm' ), 25, y)
        gold   = pink.cpp.readimage( RESULTS + '/rotate3dbin_b3_se_5_7_9_y_25.pgm')
        self.assertTrue( result == gold )


    def test_2(self):
        result = pink.cpp.rotate3dbin( pink.cpp.readimage( IMAGES + '/3dbyte/binary/b3_se_5_7_9.pgm' ), 25, z)
        gold   = pink.cpp.readimage( RESULTS + '/rotate3dbin_b3_se_5_7_9_z_25.pgm')
        self.assertTrue( result == gold )


if __name__ == '__main__':
    unittest.main(testRunner=xmlrunner.XMLTestRunner(output='test-reports'))
