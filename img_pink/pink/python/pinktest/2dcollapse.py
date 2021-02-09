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


class 2dcollapse(unittest.TestCase):
    def test_0(self):
        result = pink.cpp.2dcollapse( pink.cpp.readimage( IMAGES + '/2dbyte/binary/b2fish1.k' ), 0)
        gold   = pink.cpp.readimage( RESULTS + '/2dcollapse_b2fish1_0.k')
        self.assertTrue( result == gold )


    def test_1(self):
        result = pink.cpp.2dcollapse( pink.cpp.readimage( IMAGES + '/2dbyte/binary/b2fish1.k' ), 1)
        gold   = pink.cpp.readimage( RESULTS + '/2dcollapse_b2fish1_1.k')
        self.assertTrue( result == gold )


    def test_2(self):
        result = pink.cpp.2dcollapse( pink.cpp.readimage( IMAGES + '/2dbyte/binary/b2fish1.k' ), 2)
        gold   = pink.cpp.readimage( RESULTS + '/2dcollapse_b2fish1_2.k')
        self.assertTrue( result == gold )


    def test_3(self):
        result = pink.cpp.2dcollapse( pink.cpp.readimage( IMAGES + '/2dbyte/binary/b2fish1.k' ), 3)
        gold   = pink.cpp.readimage( RESULTS + '/2dcollapse_b2fish1_3.k')
        self.assertTrue( result == gold )


    def test_4(self):
        result = pink.cpp.2dcollapse( pink.cpp.readimage( IMAGES + '/2dbyte/binary/b2fish1.k' ), 4)
        gold   = pink.cpp.readimage( RESULTS + '/2dcollapse_b2fish1_4.k')
        self.assertTrue( result == gold )


    def test_5(self):
        result = pink.cpp.2dcollapse( pink.cpp.readimage( IMAGES + '/2dbyte/binary/b2fish1.k' ), 8)
        gold   = pink.cpp.readimage( RESULTS + '/2dcollapse_b2fish1_8.k')
        self.assertTrue( result == gold )


    def test_6(self):
        result = pink.cpp.2dcollapse( pink.cpp.readimage( IMAGES + '/2dbyte/binary/b2fish1.k' ), 0, 3)
        gold   = pink.cpp.readimage( RESULTS + '/2dcollapse_b2fish1_0_3.k')
        self.assertTrue( result == gold )


    def test_7(self):
        result = pink.cpp.2dcollapse( pink.cpp.readimage( IMAGES + '/2dbyte/binary/b2fish1.k' ), pink.cpp.readimage( IMAGES + '/2dfloat/f2fish1lam.k' ), 10)
        gold   = pink.cpp.readimage( RESULTS + '/2dcollapse_b2fish1_f2fish1lam_10.k')
        self.assertTrue( result == gold )


    def test_8(self):
        result = pink.cpp.2dcollapse( pink.cpp.readimage( IMAGES + '/2dbyte/binary/b2fish1.k' ), 0, pink.cpp.readimage( IMAGES + '/2dbyte/binary/b2fish1i.k' ))
        gold   = pink.cpp.readimage( RESULTS + '/2dcollapse_b2fish1_0_i.k')
        self.assertTrue( result == gold )


if __name__ == '__main__':
    unittest.main(testRunner=xmlrunner.XMLTestRunner(output='test-reports'))
