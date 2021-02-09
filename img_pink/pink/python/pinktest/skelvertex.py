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


class skelvertex(unittest.TestCase):
    def test_0(self):
        result = pink.cpp.skelvertex( pink.cpp.readimage( IMAGES + '/2dskel/s2skel4.skel' ), 5)
        gold   = pink.cpp.readimage( RESULTS + '/skelvertex_s2skel4.skel')
        self.assertTrue( result == gold )


    def test_1(self):
        result = pink.cpp.skelvertex( pink.cpp.readimage( IMAGES + '/3dskel/s3skel6.skel' ), 5)
        gold   = pink.cpp.readimage( RESULTS + '/skelvertex_s3skel6.skel')
        self.assertTrue( result == gold )


if __name__ == '__main__':
    unittest.main(testRunner=xmlrunner.XMLTestRunner(output='test-reports'))
