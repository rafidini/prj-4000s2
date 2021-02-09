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


class dilatball(unittest.TestCase):
    def test_0(self):
        result = pink.cpp.dilatball( pink.cpp.readimage( IMAGES + '/2dbyte/binary/b2hebreu.pgm' ), 3, 0)
        gold   = pink.cpp.readimage( RESULTS + '/dilatball_b2hebreu_3_0.pgm')
        self.assertTrue( result == gold )


    def test_1(self):
        result = pink.cpp.dilatball( pink.cpp.readimage( IMAGES + '/2dbyte/binary/b2hebreu.pgm' ), 15, 2)
        gold   = pink.cpp.readimage( RESULTS + '/dilatball_b2hebreu_15_2.pgm')
        self.assertTrue( result == gold )


    def test_2(self):
        result = pink.cpp.dilatball( pink.cpp.readimage( IMAGES + '/2dbyte/binary/b2hebreu.pgm' ), 3, 3)
        gold   = pink.cpp.readimage( RESULTS + '/dilatball_b2hebreu_3_3.pgm')
        self.assertTrue( result == gold )


    def test_3(self):
        result = pink.cpp.dilatball( pink.cpp.readimage( IMAGES + '/2dbyte/binary/b2hebreu.pgm' ), 3, 4)
        gold   = pink.cpp.readimage( RESULTS + '/dilatball_b2hebreu_3_4.pgm')
        self.assertTrue( result == gold )


    def test_4(self):
        result = pink.cpp.dilatball( pink.cpp.readimage( IMAGES + '/2dbyte/binary/b2hebreu.pgm' ), 3, 8)
        gold   = pink.cpp.readimage( RESULTS + '/dilatball_b2hebreu_3_8.pgm')
        self.assertTrue( result == gold )


    # @unittest.skip("Wrong parameter (-1)")
    # def test_5(self):
    #     result = pink.cpp.dilatball( pink.cpp.readimage( IMAGES + '/2dlong/l2fish1ma3.pgm' ), -1, 3)
    #     gold   = pink.cpp.readimage( RESULTS + '/dilatball_l2fish1ma3__1_3.pgm')
    #     self.assertTrue( result == gold )


    # @unittest.skip("Wrong parameter (-1)")
    # def test_6(self):
    #     result = pink.cpp.dilatball( pink.cpp.readimage( IMAGES + '/2dlong/l2fish1ma4.pgm' ), -1, 4)
    #     gold   = pink.cpp.readimage( RESULTS + '/dilatball_l2fish1ma4__1_4.pgm')
    #     self.assertTrue( result == gold )


    # @unittest.skip("Wrong parameter (-1)")
    # def test_7(self):
    #     result = pink.cpp.dilatball( pink.cpp.readimage( IMAGES + '/2dlong/l2fish1ma8.pgm' ), -1, 8)
    #     gold   = pink.cpp.readimage( RESULTS + '/dilatball_l2fish1ma8__1_8.pgm')
    #     self.assertTrue( result == gold )


    def test_8(self):
        result = pink.cpp.dilatball( pink.cpp.readimage( IMAGES + '/3dbyte/binary/b3a.pgm' ), 3, 0)
        gold   = pink.cpp.readimage( RESULTS + '/dilatball_b3a_3_0.pgm')
        self.assertTrue( result == gold )


    def test_9(self):
        result = pink.cpp.dilatball( pink.cpp.readimage( IMAGES + '/3dbyte/binary/b3a.pgm' ), 15, 2)
        gold   = pink.cpp.readimage( RESULTS + '/dilatball_b3a_15_2.pgm')
        self.assertTrue( result == gold )


    def test_10(self):
        result = pink.cpp.dilatball( pink.cpp.readimage( IMAGES + '/3dbyte/binary/b3a.pgm' ), 3, 3)
        gold   = pink.cpp.readimage( RESULTS + '/dilatball_b3a_3_3.pgm')
        self.assertTrue( result == gold )


    def test_11(self):
        result = pink.cpp.dilatball( pink.cpp.readimage( IMAGES + '/3dbyte/binary/b3a.pgm' ), 3, 6)
        gold   = pink.cpp.readimage( RESULTS + '/dilatball_b3a_3_6.pgm')
        self.assertTrue( result == gold )


    def test_12(self):
        result = pink.cpp.dilatball( pink.cpp.readimage( IMAGES + '/3dbyte/binary/b3a.pgm' ), 3, 18)
        gold   = pink.cpp.readimage( RESULTS + '/dilatball_b3a_3_18.pgm')
        self.assertTrue( result == gold )


    def test_13(self):
        result = pink.cpp.dilatball( pink.cpp.readimage( IMAGES + '/3dbyte/binary/b3a.pgm' ), 3, 26)
        gold   = pink.cpp.readimage( RESULTS + '/dilatball_b3a_3_26.pgm')
        self.assertTrue( result == gold )


    # @unittest.skip("Wrong parameter (-1)")
    # def test_14(self):
    #     result = pink.cpp.dilatball( pink.cpp.readimage( IMAGES + '/3dlong/l3mortierma3.pgm' ), -1, 3)
    #     gold   = pink.cpp.readimage( RESULTS + '/dilatball_l3mortierma3__1_3.pgm')
    #     self.assertTrue( result == gold )


    # @unittest.skip("Wrong parameter (-1)")
    # def test_15(self):
    #     result = pink.cpp.dilatball( pink.cpp.readimage( IMAGES + '/3dlong/l3mortierma6.pgm' ), -1, 6)
    #     gold   = pink.cpp.readimage( RESULTS + '/dilatball_l3mortierma6__1_6.pgm')
    #     self.assertTrue( result == gold )


    # @unittest.skip("Wrong parameter (-1)")
    # def test_16(self):
    #     result = pink.cpp.dilatball( pink.cpp.readimage( IMAGES + '/3dlong/l3mortierma26.pgm' ), -1, 26)
    #     gold   = pink.cpp.readimage( RESULTS + '/dilatball_l3mortierma26__1_26.pgm')
    #     self.assertTrue( result == gold )


if __name__ == '__main__':
    unittest.main(testRunner=xmlrunner.XMLTestRunner(output='test-reports'))
