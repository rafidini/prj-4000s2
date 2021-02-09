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


class heightminima(unittest.TestCase):
    def test_0(self):
        result = pink.cpp.heightminima( pink.cpp.readimage( IMAGES + '/2dbyte/gray/g2gel.pgm' ), 4, 20)
        gold   = pink.cpp.readimage( RESULTS + '/heightminima_g2gel_4_20.pgm')
        self.assertTrue( result == gold )


    def test_1(self):
        result = pink.cpp.heightminima( pink.cpp.readimage( IMAGES + '/2dbyte/gray/g2gel.pgm' ), 8, 20)
        gold   = pink.cpp.readimage( RESULTS + '/heightminima_g2gel_8_20.pgm')
        self.assertTrue( result == gold )


    def test_2(self):
        result = pink.cpp.heightminima( pink.cpp.readimage( IMAGES + '/3dbyte/gray/g3a.pgm' ), 6, 20)
        gold   = pink.cpp.readimage( RESULTS + '/heightminima_g3a_6_20.pgm')
        self.assertTrue( result == gold )


    def test_3(self):
        result = pink.cpp.heightminima( pink.cpp.readimage( IMAGES + '/3dbyte/gray/g3a.pgm' ), 18, 20)
        gold   = pink.cpp.readimage( RESULTS + '/heightminima_g3a_18_20.pgm')
        self.assertTrue( result == gold )


    def test_4(self):
        result = pink.cpp.heightminima( pink.cpp.readimage( IMAGES + '/3dbyte/gray/g3a.pgm' ), 26, 20)
        gold   = pink.cpp.readimage( RESULTS + '/heightminima_g3a_26_20.pgm')
        self.assertTrue( result == gold )


if __name__ == '__main__':
    unittest.main(testRunner=xmlrunner.XMLTestRunner(output='test-reports'))
