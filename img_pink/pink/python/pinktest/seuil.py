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


class seuil(unittest.TestCase):

    def test_0(self):
        result = pink.cpp.seuil( pink.cpp.readimage( IMAGES + '/2dbyte/gray/g2fish1.pgm' ), 3)
        gold   = pink.cpp.readimage( RESULTS + '/seuil_g2fish1_3.pgm')
        self.assertTrue( result == gold )


    # @unittest.skip("No testfile")
    # def test_1(self):
    #     result = pink.cpp.seuil( pink.cpp.readimage( IMAGES + '/2dlong/l2fish1.pgm' ), 3)
    #     gold   = pink.cpp.readimage( RESULTS + '/seuil_l2fish1_3.pgm')
    #     self.assertTrue( result == gold )


    # @unittest.skip("No testfile")
    # def test_2(self):
    #     result = pink.cpp.seuil( pink.cpp.readimage( IMAGES + '/2dfloat/f2fish1.pgm' ), 3)
    #     gold   = pink.cpp.readimage( RESULTS + '/seuil_f2fish1_3.pgm')
    #     self.assertTrue( result == gold )


if __name__ == '__main__':
    unittest.main(testRunner=xmlrunner.XMLTestRunner(output='test-reports'))
