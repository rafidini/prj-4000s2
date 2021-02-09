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


class float2byte(unittest.TestCase):
    def test_0(self):
        result = pink.cpp.float2byte( pink.cpp.readimage( IMAGES + '/2dfloat/f2fish1.pgm' ), 0)
        gold   = pink.cpp.readimage( RESULTS + '/float2byte_f2fish1_0.pgm')
        self.assertTrue( result == gold )


    def test_1(self):
        result = pink.cpp.float2byte( pink.cpp.readimage( IMAGES + '/2dfloat/f2fish1.pgm' ), 1)
        gold   = pink.cpp.readimage( RESULTS + '/float2byte_f2fish1_1.pgm')
        self.assertTrue( result == gold )


    def test_2(self):
        result = pink.cpp.float2byte( pink.cpp.readimage( IMAGES + '/2dfloat/f2fish1.pgm' ), 2)
        gold   = pink.cpp.readimage( RESULTS + '/float2byte_f2fish1_2.pgm')
        self.assertTrue( result == gold )


    def test_3(self):
        result = pink.cpp.float2byte( pink.cpp.readimage( IMAGES + '/2dfloat/f2fish1.pgm' ), 4)
        gold   = pink.cpp.readimage( RESULTS + '/float2byte_f2fish1_4.pgm')
        self.assertTrue( result == gold )


    def test_4(self):
        result = pink.cpp.float2byte( pink.cpp.readimage( IMAGES + '/2dfloat/f2fish1.pgm' ), 5)
        gold   = pink.cpp.readimage( RESULTS + '/float2byte_f2fish1_5.pgm')
        self.assertTrue( result == gold )


if __name__ == '__main__':
    unittest.main(testRunner=xmlrunner.XMLTestRunner(output='test-reports'))
