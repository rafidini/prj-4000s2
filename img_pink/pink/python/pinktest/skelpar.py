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


class skelpar(unittest.TestCase):
    def test_0(self):
        result = pink.cpp.skelpar( pink.cpp.readimage( IMAGES + '/2dbyte/binary/b2hebreu.pgm' ), 2, -1)
        gold   = pink.cpp.readimage( RESULTS + '/skelpar_b2hebreu_2__1.pgm')
        self.assertTrue( result == gold )


    def test_1(self):
        result = pink.cpp.skelpar( pink.cpp.readimage( IMAGES + '/2dbyte/binary/b2hebreu.pgm' ), 3, -1)
        gold   = pink.cpp.readimage( RESULTS + '/skelpar_b2hebreu_3__1.pgm')
        self.assertTrue( result == gold )


    def test_2(self):
        result = pink.cpp.skelpar( pink.cpp.readimage( IMAGES + '/2dbyte/binary/b2hebreu.pgm' ), 4, -1)
        gold   = pink.cpp.readimage( RESULTS + '/skelpar_b2hebreu_4__1.pgm')
        self.assertTrue( result == gold )


    def test_3(self):
        result = pink.cpp.skelpar( pink.cpp.readimage( IMAGES + '/2dbyte/binary/b2hebreu.pgm' ), 22, -1)
        gold   = pink.cpp.readimage( RESULTS + '/skelpar_b2hebreu_22__1.pgm')
        self.assertTrue( result == gold )


    def test_4(self):
        result = pink.cpp.skelpar( pink.cpp.readimage( IMAGES + '/2dbyte/binary/b2hebreu.pgm' ), 23, -1)
        gold   = pink.cpp.readimage( RESULTS + '/skelpar_b2hebreu_23__1.pgm')
        self.assertTrue( result == gold )


    def test_5(self):
        result = pink.cpp.skelpar( pink.cpp.readimage( IMAGES + '/2dbyte/binary/b2hebreu.pgm' ), 2, 3)
        gold   = pink.cpp.readimage( RESULTS + '/skelpar_b2hebreu_2_3.pgm')
        self.assertTrue( result == gold )


    def test_6(self):
        result = pink.cpp.skelpar( pink.cpp.readimage( IMAGES + '/2dbyte/binary/b2hebreu.pgm' ), 2, -1, pink.cpp.readimage( IMAGES + '/2dbyte/binary/b2hebreui.pgm' ))
        gold   = pink.cpp.readimage( RESULTS + '/skelpar_b2hebreu_2__1_i.pgm')
        self.assertTrue( result == gold )


if __name__ == '__main__':
    unittest.main(testRunner=xmlrunner.XMLTestRunner(output='test-reports'))
