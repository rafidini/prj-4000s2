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


class 2dkhalimskize(unittest.TestCase):
    def test_0(self):
        result = pink.cpp.2dkhalimskize( pink.cpp.readimage( IMAGES + '/2dbyte/binary/b2fish1.pgm' ), 0)
        gold   = pink.cpp.readimage( RESULTS + '/2dkhalimskize_b2fish1_0.k')
        self.assertTrue( result == gold )


    def test_1(self):
        result = pink.cpp.2dkhalimskize( pink.cpp.readimage( IMAGES + '/2dbyte/binary/b2fish1.pgm' ), 4)
        gold   = pink.cpp.readimage( RESULTS + '/2dkhalimskize_b2fish1_4.k')
        self.assertTrue( result == gold )


    def test_2(self):
        result = pink.cpp.2dkhalimskize( pink.cpp.readimage( IMAGES + '/2dbyte/binary/b2fish1.pgm' ), 8)
        gold   = pink.cpp.readimage( RESULTS + '/2dkhalimskize_b2fish1_8.k')
        self.assertTrue( result == gold )


    def test_3(self):
        result = pink.cpp.2dkhalimskize( pink.cpp.readimage( IMAGES + '/2dbyte/binary/b2fish1.pgm' ), h)
        gold   = pink.cpp.readimage( RESULTS + '/2dkhalimskize_b2fish1_h.k')
        self.assertTrue( result == gold )


    def test_4(self):
        result = pink.cpp.2dkhalimskize( pink.cpp.readimage( IMAGES + '/2dbyte/gray/g2fish1.pgm' ), m)
        gold   = pink.cpp.readimage( RESULTS + '/2dkhalimskize_g2fish1_m.k')
        self.assertTrue( result == gold )


    def test_5(self):
        result = pink.cpp.2dkhalimskize( pink.cpp.readimage( IMAGES + '/2dbyte/gray/g2fish1.pgm' ), M)
        gold   = pink.cpp.readimage( RESULTS + '/2dkhalimskize_g2fish1_M.k')
        self.assertTrue( result == gold )


    def test_6(self):
        result = pink.cpp.2dkhalimskize( pink.cpp.readimage( IMAGES + '/2dlong/l2fish1.pgm' ), g)
        gold   = pink.cpp.readimage( RESULTS + '/2dkhalimskize_l2fish1_g.k')
        self.assertTrue( result == gold )


    def test_7(self):
        result = pink.cpp.2dkhalimskize( pink.cpp.readimage( IMAGES + '/2dlong/l2fish1.pgm' ), G)
        gold   = pink.cpp.readimage( RESULTS + '/2dkhalimskize_l2fish1_G.k')
        self.assertTrue( result == gold )


    def test_8(self):
        result = pink.cpp.2dkhalimskize( pink.cpp.readimage( IMAGES + '/2dfloat/f2fish1.pgm' ), a)
        gold   = pink.cpp.readimage( RESULTS + '/2dkhalimskize_f2fish1_a.k')
        self.assertTrue( result == gold )


    def test_9(self):
        result = pink.cpp.2dkhalimskize( pink.cpp.readimage( IMAGES + '/2dbyte/binary/b2fish1.k' ), R)
        gold   = pink.cpp.readimage( RESULTS + '/2dkhalimskize_b2fish1_R.pgm')
        self.assertTrue( result == gold )


if __name__ == '__main__':
    unittest.main(testRunner=xmlrunner.XMLTestRunner(output='test-reports'))
