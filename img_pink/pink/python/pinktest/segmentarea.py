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


class segmentarea(unittest.TestCase):
    def test_0(self):
        result = pink.cpp.segmentarea( pink.cpp.readimage( IMAGES + '/2dbyte/gray/g2gel.pgm' ), 4, 100)
        gold   = pink.cpp.readimage( RESULTS + '/segmentarea_g2gel_4_100.pgm')
        self.assertTrue( result == gold )


    def test_1(self):
        result = pink.cpp.segmentarea( pink.cpp.readimage( IMAGES + '/2dbyte/gray/g2gel.pgm' ), 8, 100)
        gold   = pink.cpp.readimage( RESULTS + '/segmentarea_g2gel_8_100.pgm')
        self.assertTrue( result == gold )


    def test_2(self):
        result = pink.cpp.segmentarea( pink.cpp.readimage( IMAGES + '/3dbyte/gray/g3a.pgm' ), 6, 100)
        gold   = pink.cpp.readimage( RESULTS + '/segmentarea_g3a_6_100.pgm')
        self.assertTrue( result == gold )


    def test_3(self):
        result = pink.cpp.segmentarea( pink.cpp.readimage( IMAGES + '/3dbyte/gray/g3a.pgm' ), 18, 100)
        gold   = pink.cpp.readimage( RESULTS + '/segmentarea_g3a_18_100.pgm')
        self.assertTrue( result == gold )


    def test_4(self):
        result = pink.cpp.segmentarea( pink.cpp.readimage( IMAGES + '/3dbyte/gray/g3a.pgm' ), 26, 100)
        gold   = pink.cpp.readimage( RESULTS + '/segmentarea_g3a_26_100.pgm')
        self.assertTrue( result == gold )


if __name__ == '__main__':
    unittest.main(testRunner=xmlrunner.XMLTestRunner(output='test-reports'))
