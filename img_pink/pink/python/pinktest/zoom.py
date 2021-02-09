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


class zoom(unittest.TestCase):
    # @unittest.skip("Not implemented")
    # def test_0(self):
    #     result = pink.cpp.zoom( pink.cpp.readimage( IMAGES + '/2dbyte/binary/b2hebreu.pgm' ), 0.7)
    #     gold   = pink.cpp.readimage( RESULTS + '/zoom_b2hebreu_07.pgm')
    #     self.assertTrue( result == gold )

    # @unittest.skip("Not implemented")
    # def test_1(self):
    #     result = pink.cpp.zoom( pink.cpp.readimage( IMAGES + '/2dbyte/binary/b2hebreu.pgm' ), 1.7)
    #     gold   = pink.cpp.readimage( RESULTS + '/zoom_b2hebreu_17.pgm')
    #     self.assertTrue( result == gold )


    # @unittest.skip("Not implemented")
    # def test_2(self):
    #     result = pink.cpp.zoom( pink.cpp.readimage( IMAGES + '/2dbyte/binary/b2hebreu.pgm' ), x, 100)
    #     gold   = pink.cpp.readimage( RESULTS + '/zoom_b2hebreu_x_100.pgm')
    #     self.assertTrue( result == gold )


    def test_3(self):
        result = pink.cpp.zoom( pink.cpp.readimage( IMAGES + '/2dbyte/binary/b2hebreu.pgm' ), 0.5, 0.25, 1)
        gold   = pink.cpp.readimage( RESULTS + '/zoom_b2hebreu_05_025_1.pgm')
        self.assertTrue( result == gold )


    # @unittest.skip("Not implemented")
    # def test_4(self):
    #     result = pink.cpp.zoom( pink.cpp.readimage( IMAGES + '/3dbyte/binary/b3a.pgm' ), 0.7)
    #     gold   = pink.cpp.readimage( RESULTS + '/zoom_b3a_07.pgm')
    #     self.assertTrue( result == gold )


    # @unittest.skip("Not implemented")
    # def test_5(self):
    #     result = pink.cpp.zoom( pink.cpp.readimage( IMAGES + '/3dbyte/binary/b3a.pgm' ), 1.7)
    #     gold   = pink.cpp.readimage( RESULTS + '/zoom_b3a_17.pgm')
    #     self.assertTrue( result == gold )


    # @unittest.skip("Not implemented")
    # def test_6(self):
    #     result = pink.cpp.zoom( pink.cpp.readimage( IMAGES + '/3dbyte/binary/b3a.pgm' ), x, 100)
    #     gold   = pink.cpp.readimage( RESULTS + '/zoom_b3a_x_100.pgm')
    #     self.assertTrue( result == gold )

    # @unittest.skip("FAILED")
    # def test_7(self):
    #     result = pink.cpp.zoom( pink.cpp.readimage( IMAGES + '/3dbyte/binary/b3a.pgm' ), 0.5, 0.25, 0.9)
    #     gold   = pink.cpp.readimage( RESULTS + '/zoom_b3a_05_025_09.pgm')
    #     self.assertTrue( result == gold )


if __name__ == '__main__':
    unittest.main(testRunner=xmlrunner.XMLTestRunner(output='test-reports'))
