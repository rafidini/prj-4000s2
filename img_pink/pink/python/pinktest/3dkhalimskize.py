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


class 3dkhalimskize(unittest.TestCase):
    def test_0(self):
        result = pink.cpp.3dkhalimskize( pink.cpp.readimage( IMAGES + '/3dbyte/binary/b3a.pgm' ), 6)
        gold   = pink.cpp.readimage( RESULTS + '/3dkhalimskize_b3a_6.k')
        self.assertTrue( result == gold )


    def test_1(self):
        result = pink.cpp.3dkhalimskize( pink.cpp.readimage( IMAGES + '/3dbyte/binary/b3a.pgm' ), 26)
        gold   = pink.cpp.readimage( RESULTS + '/3dkhalimskize_b3a_26.k')
        self.assertTrue( result == gold )


    def test_2(self):
        result = pink.cpp.3dkhalimskize( pink.cpp.readimage( IMAGES + '/3dbyte/binary/b3a.pgm' ), h)
        gold   = pink.cpp.readimage( RESULTS + '/3dkhalimskize_b3a_h.k')
        self.assertTrue( result == gold )


    def test_3(self):
        result = pink.cpp.3dkhalimskize( pink.cpp.readimage( IMAGES + '/3dbyte/gray/g3a.pgm' ), m)
        gold   = pink.cpp.readimage( RESULTS + '/3dkhalimskize_g3a_m.k')
        self.assertTrue( result == gold )


    def test_4(self):
        result = pink.cpp.3dkhalimskize( pink.cpp.readimage( IMAGES + '/3dlong/l3a.pgm' ), M)
        gold   = pink.cpp.readimage( RESULTS + '/3dkhalimskize_l3a_M.k')
        self.assertTrue( result == gold )


    def test_5(self):
        result = pink.cpp.3dkhalimskize( pink.cpp.readimage( IMAGES + '/3dfloat/f3a.pgm' ), a)
        gold   = pink.cpp.readimage( RESULTS + '/3dkhalimskize_f3a_a.k')
        self.assertTrue( result == gold )


    def test_6(self):
        result = pink.cpp.3dkhalimskize( pink.cpp.readimage( IMAGES + '/3dbyte/binary/b3a.k' ), R)
        gold   = pink.cpp.readimage( RESULTS + '/3dkhalimskize_b3a_R.pgm')
        self.assertTrue( result == gold )


if __name__ == '__main__':
    unittest.main(testRunner=xmlrunner.XMLTestRunner(output='test-reports'))
