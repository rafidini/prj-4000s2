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


class closeball(unittest.TestCase):
    def test_0(self):
        result = pink.cpp.closeball( pink.cpp.readimage( IMAGES + '/2dbyte/binary/b2hebreu.pgm' ), 3, 0)
        gold   = pink.cpp.readimage( RESULTS + '/closeball_b2hebreu_3_0.pgm')
        self.assertTrue( result == gold )


    def test_1(self):
        result = pink.cpp.closeball( pink.cpp.readimage( IMAGES + '/2dbyte/binary/b2hebreu.pgm' ), 15, 2)
        gold   = pink.cpp.readimage( RESULTS + '/closeball_b2hebreu_15_2.pgm')
        self.assertTrue( result == gold )


    def test_2(self):
        result = pink.cpp.closeball( pink.cpp.readimage( IMAGES + '/2dbyte/binary/b2hebreu.pgm' ), 3, 3)
        gold   = pink.cpp.readimage( RESULTS + '/closeball_b2hebreu_3_3.pgm')
        self.assertTrue( result == gold )


    def test_3(self):
        result = pink.cpp.closeball( pink.cpp.readimage( IMAGES + '/2dbyte/binary/b2hebreu.pgm' ), 3, 4)
        gold   = pink.cpp.readimage( RESULTS + '/closeball_b2hebreu_3_4.pgm')
        self.assertTrue( result == gold )


    def test_4(self):
        result = pink.cpp.closeball( pink.cpp.readimage( IMAGES + '/2dbyte/binary/b2hebreu.pgm' ), 3, 8)
        gold   = pink.cpp.readimage( RESULTS + '/closeball_b2hebreu_3_8.pgm')
        self.assertTrue( result == gold )


    def test_5(self):
        result = pink.cpp.closeball( pink.cpp.readimage( IMAGES + '/3dbyte/binary/b3a.pgm' ), 3, 0)
        gold   = pink.cpp.readimage( RESULTS + '/closeball_b3a_3_0.pgm')
        self.assertTrue( result == gold )


    def test_6(self):
        result = pink.cpp.closeball( pink.cpp.readimage( IMAGES + '/3dbyte/binary/b3a.pgm' ), 15, 2)
        gold   = pink.cpp.readimage( RESULTS + '/closeball_b3a_15_2.pgm')
        self.assertTrue( result == gold )


    def test_7(self):
        result = pink.cpp.closeball( pink.cpp.readimage( IMAGES + '/3dbyte/binary/b3a.pgm' ), 3, 3)
        gold   = pink.cpp.readimage( RESULTS + '/closeball_b3a_3_3.pgm')
        self.assertTrue( result == gold )


    def test_8(self):
        result = pink.cpp.closeball( pink.cpp.readimage( IMAGES + '/3dbyte/binary/b3a.pgm' ), 3, 6)
        gold   = pink.cpp.readimage( RESULTS + '/closeball_b3a_3_6.pgm')
        self.assertTrue( result == gold )


    def test_9(self):
        result = pink.cpp.closeball( pink.cpp.readimage( IMAGES + '/3dbyte/binary/b3a.pgm' ), 3, 18)
        gold   = pink.cpp.readimage( RESULTS + '/closeball_b3a_3_18.pgm')
        self.assertTrue( result == gold )


    def test_10(self):
        result = pink.cpp.closeball( pink.cpp.readimage( IMAGES + '/3dbyte/binary/b3a.pgm' ), 3, 26)
        gold   = pink.cpp.readimage( RESULTS + '/closeball_b3a_3_26.pgm')
        self.assertTrue( result == gold )


if __name__ == '__main__':
    unittest.main(testRunner=xmlrunner.XMLTestRunner(output='test-reports'))
