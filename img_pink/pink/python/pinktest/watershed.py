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


class watershed(unittest.TestCase):
    def test_0(self):
        result = pink.cpp.watershed( pink.cpp.inverse(pink.cpp.readimage( IMAGES + '/2dbyte/gray/g2relief.pgm' )), pink.cpp.readimage( IMAGES + '/2dbyte/binary/b2marqueur.pgm' ), 4)
        gold   = pink.cpp.readimage( RESULTS + '/watershed_g2relief_b2marqueur_4.pgm')
        self.assertTrue( result == pink.cpp.inverse(gold) )


    def test_1(self):
        result = pink.cpp.watershed( pink.cpp.inverse(pink.cpp.readimage( IMAGES + '/2dbyte/gray/g2relief.pgm' )), pink.cpp.readimage( IMAGES + '/2dbyte/binary/b2marqueur.pgm' ), 8)
        gold   = pink.cpp.readimage( RESULTS + '/watershed_g2relief_b2marqueur_8.pgm')
        self.assertTrue( result == pink.cpp.inverse(gold) )


if __name__ == '__main__':
    unittest.main(testRunner=xmlrunner.XMLTestRunner(output='test-reports'))
