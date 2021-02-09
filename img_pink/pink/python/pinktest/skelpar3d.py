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


class skelpar3d(unittest.TestCase):
    def test_0(self):
        result = pink.cpp.skelpar3d( pink.cpp.readimage( IMAGES + '/3dbyte/binary/b3a.pgm' ), 0, -1)
        gold   = pink.cpp.readimage( RESULTS + '/skelpar3d_b3a_0__1.pgm')
        self.assertTrue( result == gold )


    def test_1(self):
        result = pink.cpp.skelpar3d( pink.cpp.readimage( IMAGES + '/3dbyte/binary/b3a.pgm' ), 1, -1)
        gold   = pink.cpp.readimage( RESULTS + '/skelpar3d_b3a_1__1.pgm')
        self.assertTrue( result == gold )


    def test_2(self):
        result = pink.cpp.skelpar3d( pink.cpp.readimage( IMAGES + '/3dbyte/binary/b3a.pgm' ), 2, -1)
        gold   = pink.cpp.readimage( RESULTS + '/skelpar3d_b3a_2__1.pgm')
        self.assertTrue( result == gold )


    def test_3(self):
        result = pink.cpp.skelpar3d( pink.cpp.readimage( IMAGES + '/3dbyte/binary/b3a.pgm' ), 3, -1)
        gold   = pink.cpp.readimage( RESULTS + '/skelpar3d_b3a_3__1.pgm')
        self.assertTrue( result == gold )


    def test_4(self):
        result = pink.cpp.skelpar3d( pink.cpp.readimage( IMAGES + '/3dbyte/binary/b3a.pgm' ), 4, -1)
        gold   = pink.cpp.readimage( RESULTS + '/skelpar3d_b3a_4__1.pgm')
        self.assertTrue( result == gold )


    def test_5(self):
        result = pink.cpp.skelpar3d( pink.cpp.readimage( IMAGES + '/3dbyte/binary/b3a.pgm' ), 5, -1)
        gold   = pink.cpp.readimage( RESULTS + '/skelpar3d_b3a_5__1.pgm')
        self.assertTrue( result == gold )


    def test_6(self):
        result = pink.cpp.skelpar3d( pink.cpp.readimage( IMAGES + '/3dbyte/binary/b3a.pgm' ), 6, -1)
        gold   = pink.cpp.readimage( RESULTS + '/skelpar3d_b3a_6__1.pgm')
        self.assertTrue( result == gold )


    def test_7(self):
        result = pink.cpp.skelpar3d( pink.cpp.readimage( IMAGES + '/3dbyte/binary/b3a.pgm' ), 7, -1)
        gold   = pink.cpp.readimage( RESULTS + '/skelpar3d_b3a_7__1.pgm')
        self.assertTrue( result == gold )


    def test_8(self):
        result = pink.cpp.skelpar3d( pink.cpp.readimage( IMAGES + '/3dbyte/binary/b3a.pgm' ), 8, -1)
        gold   = pink.cpp.readimage( RESULTS + '/skelpar3d_b3a_8__1.pgm')
        self.assertTrue( result == gold )


    def test_9(self):
        result = pink.cpp.skelpar3d( pink.cpp.readimage( IMAGES + '/3dbyte/binary/b3a.pgm' ), 9, -1)
        gold   = pink.cpp.readimage( RESULTS + '/skelpar3d_b3a_9__1.pgm')
        self.assertTrue( result == gold )


    def test_10(self):
        result = pink.cpp.skelpar3d( pink.cpp.readimage( IMAGES + '/3dbyte/binary/b3a.pgm' ), 10, -1)
        gold   = pink.cpp.readimage( RESULTS + '/skelpar3d_b3a_10__1.pgm')
        self.assertTrue( result == gold )


    def test_11(self):
        result = pink.cpp.skelpar3d( pink.cpp.readimage( IMAGES + '/3dbyte/binary/b3a.pgm' ), 11, -1)
        gold   = pink.cpp.readimage( RESULTS + '/skelpar3d_b3a_11__1.pgm')
        self.assertTrue( result == gold )


    def test_12(self):
        result = pink.cpp.skelpar3d( pink.cpp.readimage( IMAGES + '/3dbyte/binary/b3a.pgm' ), 12, -1)
        gold   = pink.cpp.readimage( RESULTS + '/skelpar3d_b3a_12__1.pgm')
        self.assertTrue( result == gold )


    def test_13(self):
        result = pink.cpp.skelpar3d( pink.cpp.readimage( IMAGES + '/3dbyte/binary/b3a.pgm' ), 3, 4)
        gold   = pink.cpp.readimage( RESULTS + '/skelpar3d_b3a_0_4.pgm')
        self.assertTrue( result == gold )


    def test_14(self):
        result = pink.cpp.skelpar3d( pink.cpp.readimage( IMAGES + '/3dbyte/binary/b3a.pgm' ), 3, -1, pink.cpp.readimage( IMAGES + '/3dbyte/binary/b3a2.pgm' ))
        gold   = pink.cpp.readimage( RESULTS + '/skelpar3d_b3a_0__1_i.pgm')
        self.assertTrue( result == gold )


if __name__ == '__main__':
    unittest.main(testRunner=xmlrunner.XMLTestRunner(output='test-reports'))
