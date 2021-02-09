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


class skelcurv(unittest.TestCase):
    def test_0(self):
        result = pink.cpp.skelcurv( pink.cpp.readimage( IMAGES + '/2dbyte/binary/b2hebreu.pgm' ), 0, 8)
        gold   = pink.cpp.readimage( RESULTS + '/skelcurv_b2hebreu_0_8.pgm')
        self.assertTrue( result == gold )


    def test_1(self):
        result = pink.cpp.skelcurv( pink.cpp.readimage( IMAGES + '/2dbyte/binary/b2hebreu.pgm' ), 1, 8)
        gold   = pink.cpp.readimage( RESULTS + '/skelcurv_b2hebreu_1_8.pgm')
        self.assertTrue( result == gold )


    def test_2(self):
        result = pink.cpp.skelcurv( pink.cpp.readimage( IMAGES + '/2dbyte/binary/b2hebreu.pgm' ), 2, 8)
        gold   = pink.cpp.readimage( RESULTS + '/skelcurv_b2hebreu_2_8.pgm')
        self.assertTrue( result == gold )


    def test_3(self):
        result = pink.cpp.skelcurv( pink.cpp.readimage( IMAGES + '/2dbyte/binary/b2hebreu.pgm' ), 3, 8)
        gold   = pink.cpp.readimage( RESULTS + '/skelcurv_b2hebreu_3_8.pgm')
        self.assertTrue( result == gold )


    def test_4(self):
        result = pink.cpp.skelcurv( pink.cpp.readimage( IMAGES + '/2dbyte/binary/b2hebreu.pgm' ), 4, 8)
        gold   = pink.cpp.readimage( RESULTS + '/skelcurv_b2hebreu_4_8.pgm')
        self.assertTrue( result == gold )


    def test_5(self):
        result = pink.cpp.skelcurv( pink.cpp.readimage( IMAGES + '/2dbyte/binary/b2hebreu.pgm' ), 8, 8)
        gold   = pink.cpp.readimage( RESULTS + '/skelcurv_b2hebreu_8_8.pgm')
        self.assertTrue( result == gold )


    def test_6(self):
        result = pink.cpp.skelcurv( pink.cpp.readimage( IMAGES + '/2dbyte/binary/b2hebreu.pgm' ), 0, 4)
        gold   = pink.cpp.readimage( RESULTS + '/skelcurv_b2hebreu_0_4.pgm')
        self.assertTrue( result == gold )


    def test_7(self):
        result = pink.cpp.skelcurv( pink.cpp.readimage( IMAGES + '/2dbyte/binary/b2hebreu.pgm' ), 0, 8, pink.cpp.readimage( IMAGES + '/2dbyte/binary/b2hebreui.pgm' ))
        gold   = pink.cpp.readimage( RESULTS + '/skelcurv_b2hebreu_0_8_i.pgm')
        self.assertTrue( result == gold )


    def test_8(self):
        result = pink.cpp.skelcurv( pink.cpp.readimage( IMAGES + '/3dbyte/binary/b3a.pgm' ), 0, 26)
        gold   = pink.cpp.readimage( RESULTS + '/skelcurv_b3a_0_26.pgm')
        self.assertTrue( result == gold )


    def test_9(self):
        result = pink.cpp.skelcurv( pink.cpp.readimage( IMAGES + '/3dbyte/binary/b3a.pgm' ), 1, 26)
        gold   = pink.cpp.readimage( RESULTS + '/skelcurv_b3a_1_26.pgm')
        self.assertTrue( result == gold )


    def test_10(self):
        result = pink.cpp.skelcurv( pink.cpp.readimage( IMAGES + '/3dbyte/binary/b3a.pgm' ), 2, 26)
        gold   = pink.cpp.readimage( RESULTS + '/skelcurv_b3a_2_26.pgm')
        self.assertTrue( result == gold )


    def test_11(self):
        result = pink.cpp.skelcurv( pink.cpp.readimage( IMAGES + '/3dbyte/binary/b3a.pgm' ), 3, 26)
        gold   = pink.cpp.readimage( RESULTS + '/skelcurv_b3a_3_26.pgm')
        self.assertTrue( result == gold )


    def test_12(self):
        result = pink.cpp.skelcurv( pink.cpp.readimage( IMAGES + '/3dbyte/binary/b3a.pgm' ), 6, 26)
        gold   = pink.cpp.readimage( RESULTS + '/skelcurv_b3a_6_26.pgm')
        self.assertTrue( result == gold )


    def test_13(self):
        result = pink.cpp.skelcurv( pink.cpp.readimage( IMAGES + '/3dbyte/binary/b3a.pgm' ), 18, 26)
        gold   = pink.cpp.readimage( RESULTS + '/skelcurv_b3a_18_26.pgm')
        self.assertTrue( result == gold )


    def test_14(self):
        result = pink.cpp.skelcurv( pink.cpp.readimage( IMAGES + '/3dbyte/binary/b3a.pgm' ), 26, 26)
        gold   = pink.cpp.readimage( RESULTS + '/skelcurv_b3a_26_26.pgm')
        self.assertTrue( result == gold )


    def test_15(self):
        result = pink.cpp.skelcurv( pink.cpp.readimage( IMAGES + '/3dbyte/binary/b3a.pgm' ), 0, 6)
        gold   = pink.cpp.readimage( RESULTS + '/skelcurv_b3a_0_6.pgm')
        self.assertTrue( result == gold )


    def test_16(self):
        result = pink.cpp.skelcurv( pink.cpp.readimage( IMAGES + '/3dbyte/binary/b3a.pgm' ), 0, 26, pink.cpp.readimage( IMAGES + '/3dbyte/binary/b3a2.pgm' ))
        gold   = pink.cpp.readimage( RESULTS + '/skelcurv_b3a_0_26_i.pgm')
        self.assertTrue( result == gold )


if __name__ == '__main__':
    unittest.main(testRunner=xmlrunner.XMLTestRunner(output='test-reports'))
