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


class holeclosing(unittest.TestCase):
    def test_0(self):
        result = pink.cpp.holeclosing( pink.cpp.readimage( IMAGES + '/3dbyte/binary/b3tores.pgm' ), 26, -1)
        gold   = pink.cpp.readimage( RESULTS + '/holeclosing_b3tores_26__1.pgm')
        self.assertTrue( result == gold )


    def test_1(self):
        result = pink.cpp.holeclosing( pink.cpp.readimage( IMAGES + '/3dbyte/binary/b3tores.pgm' ), 26, 2)
        gold   = pink.cpp.readimage( RESULTS + '/holeclosing_b3tores_26_2.pgm')
        self.assertTrue( result == gold )


    def test_2(self):
        result = pink.cpp.holeclosing( pink.cpp.readimage( IMAGES + '/3dbyte/binary/b3tores.pgm' ), 6, -1)
        gold   = pink.cpp.readimage( RESULTS + '/holeclosing_b3tores_6__1.pgm')
        self.assertTrue( result == gold )


    def test_3(self):
        result = pink.cpp.holeclosing( pink.cpp.readimage( IMAGES + '/3dbyte/binary/b3tores.pgm' ), 6, 2)
        gold   = pink.cpp.readimage( RESULTS + '/holeclosing_b3tores_6_2.pgm')
        self.assertTrue( result == gold )


if __name__ == '__main__':
    unittest.main(testRunner=xmlrunner.XMLTestRunner(output='test-reports'))
