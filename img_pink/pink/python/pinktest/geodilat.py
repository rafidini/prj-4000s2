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


class geodilat(unittest.TestCase):
    def test_0(self):
        result = pink.cpp.geodilat( pink.cpp.readimage( IMAGES + '/2dbyte/binary/b2hebreu_er.pgm' ), pink.cpp.readimage( IMAGES + '/2dbyte/binary/b2hebreu.pgm' ), 4, -1)
        gold   = pink.cpp.readimage( RESULTS + '/geodilat_b2hebreu_er_b2hebreu_4__1.pgm')
        self.assertTrue( result == gold )


    def test_1(self):
        result = pink.cpp.geodilat( pink.cpp.readimage( IMAGES + '/2dbyte/binary/b2hebreu_er.pgm' ), pink.cpp.readimage( IMAGES + '/2dbyte/binary/b2hebreu.pgm' ), 8, -1)
        gold   = pink.cpp.readimage( RESULTS + '/geodilat_b2hebreu_er_b2hebreu_8__1.pgm')
        self.assertTrue( result == gold )


    def test_2(self):
        result = pink.cpp.geodilat( pink.cpp.readimage( IMAGES + '/3dbyte/binary/b3mortier_er.pgm' ), pink.cpp.readimage( IMAGES + '/3dbyte/binary/b3mortier.pgm' ), 6, -1)
        gold   = pink.cpp.readimage( RESULTS + '/geodilat_b3mortier_er_b3mortier_6__1.pgm')
        self.assertTrue( result == gold )


    def test_3(self):
        result = pink.cpp.geodilat( pink.cpp.readimage( IMAGES + '/3dbyte/binary/b3mortier_er.pgm' ), pink.cpp.readimage( IMAGES + '/3dbyte/binary/b3mortier.pgm' ), 26, -1)
        gold   = pink.cpp.readimage( RESULTS + '/geodilat_b3mortier_er_b3mortier_26__1.pgm')
        self.assertTrue( result == gold )


if __name__ == '__main__':
    unittest.main(testRunner=xmlrunner.XMLTestRunner(output='test-reports'))
