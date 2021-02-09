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


class label(unittest.TestCase):
    def test_0(self):
        result = pink.cpp.label( pink.cpp.readimage( IMAGES + '/2dbyte/binary/b2hebreu.pgm' ), 8, fgd)
        gold   = pink.cpp.readimage( RESULTS + '/label_b2hebreu_8_fgd.pgm')
        self.assertTrue( result == gold )


    def test_1(self):
        result = pink.cpp.label( pink.cpp.readimage( IMAGES + '/2dbyte/binary/b2hebreu.pgm' ), 4, fgd)
        gold   = pink.cpp.readimage( RESULTS + '/label_b2hebreu_4_fgd.pgm')
        self.assertTrue( result == gold )


    def test_2(self):
        result = pink.cpp.label( pink.cpp.readimage( IMAGES + '/2dbyte/binary/b2hebreu.pgm' ), 8, bgd)
        gold   = pink.cpp.readimage( RESULTS + '/label_b2hebreu_8_bgd.pgm')
        self.assertTrue( result == gold )


    def test_3(self):
        result = pink.cpp.label( pink.cpp.readimage( IMAGES + '/2dbyte/binary/b2hebreu.pgm' ), 4, bgd)
        gold   = pink.cpp.readimage( RESULTS + '/label_b2hebreu_4_bgd.pgm')
        self.assertTrue( result == gold )


    def test_4(self):
        result = pink.cpp.label( pink.cpp.readimage( IMAGES + '/2dbyte/gray/g2hebreu.pgm' ), 4, min)
        gold   = pink.cpp.readimage( RESULTS + '/label_g2hebreu_4_min.pgm')
        self.assertTrue( result == gold )


    def test_5(self):
        result = pink.cpp.label( pink.cpp.readimage( IMAGES + '/2dbyte/gray/g2hebreu.pgm' ), 4, max)
        gold   = pink.cpp.readimage( RESULTS + '/label_g2hebreu_4_max.pgm')
        self.assertTrue( result == gold )


    def test_6(self):
        result = pink.cpp.label( pink.cpp.readimage( IMAGES + '/2dbyte/gray/g2hebreu.pgm' ), 4, pla)
        gold   = pink.cpp.readimage( RESULTS + '/label_g2hebreu_4_pla.pgm')
        self.assertTrue( result == gold )


    def test_7(self):
        result = pink.cpp.label( pink.cpp.readimage( IMAGES + '/3dbyte/gray/g3a.pgm' ), 6, pla)
        gold   = pink.cpp.readimage( RESULTS + '/label_g3a_6_pla.pgm')
        self.assertTrue( result == gold )


    def test_8(self):
        result = pink.cpp.label( pink.cpp.readimage( IMAGES + '/3dbyte/gray/g3a.pgm' ), 18, pla)
        gold   = pink.cpp.readimage( RESULTS + '/label_g3a_18_pla.pgm')
        self.assertTrue( result == gold )


    def test_9(self):
        result = pink.cpp.label( pink.cpp.readimage( IMAGES + '/3dbyte/gray/g3a.pgm' ), 26, pla)
        gold   = pink.cpp.readimage( RESULTS + '/label_g3a_26_pla.pgm')
        self.assertTrue( result == gold )


if __name__ == '__main__':
    unittest.main(testRunner=xmlrunner.XMLTestRunner(output='test-reports'))
