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


class drawsplines(unittest.TestCase):
    def test_0(self):
        result = pink.cpp.drawsplines( pink.cpp.readimage( IMAGES + '/2dbyte/binary/b2empty_30_40.pgm' ), pink.cpp.readimage( IMAGES + '/2dlist/binary/l2splines1.list' ))
        gold   = pink.cpp.readimage( RESULTS + '/drawsplines_l2splines1.pgm')
        self.assertTrue( result == gold )


    def test_1(self):
        result = pink.cpp.drawsplines( pink.cpp.readimage( IMAGES + '/3dbyte/binary/b3empty_20_30_40.pgm' ), pink.cpp.readimage( IMAGES + '/3dlist/binary/l3splines1.list' ))
        gold   = pink.cpp.readimage( RESULTS + '/drawsplines_l3splines1.pgm')
        self.assertTrue( result == gold )


if __name__ == '__main__':
    unittest.main(testRunner=xmlrunner.XMLTestRunner(output='test-reports'))
