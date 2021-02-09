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


class lambdamedialaxis(unittest.TestCase):
    def test_0(self):
        result = pink.cpp.lambdamedialaxis( pink.cpp.readimage( IMAGES + '/2dbyte/binary/b2hebreu.pgm' ))
        gold   = pink.cpp.readimage( RESULTS + '/lambdamedialaxis_b2hebreu.pgm')
        self.assertTrue( result == gold )


    def test_1(self):
        result = pink.cpp.lambdamedialaxis( pink.cpp.readimage( IMAGES + '/3dbyte/binary/b3a.pgm' ))
        gold   = pink.cpp.readimage( RESULTS + '/lambdamedialaxis_b3a.pgm')
        self.assertTrue( result == gold )


if __name__ == '__main__':
    unittest.main(testRunner=xmlrunner.XMLTestRunner(output='test-reports'))
