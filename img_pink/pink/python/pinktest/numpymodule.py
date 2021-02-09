# -*- coding: utf-8 -*- 
#  
# This software is licensed under  
# CeCILL FREE SOFTWARE LICENSE AGREEMENT 
 
# This software comes in hope that it will be useful but  
# without any warranty to the extent permitted by applicable law. 
   
# (C) UjoImro <ujoimro@gmail.com>, 2012 
# ProCarPlan s.r.o. 

## unittest for the NumPy wrapper in Python 

import os 
import pink 
import numpy
import random
import unittest 
import xmlrunner 
 
try:   
    IMAGES  = os.environ["PINKTEST"] + "/images" 
    RESULTS = os.environ["PINKTEST"] + "/results_prev" 
except KeyError:  
    raise Exception, "PINKTEST environment variable must be defined for the testing module. It must point to the testimages directory" 


class numpymodule(unittest.TestCase):
    def test_static(self): ## simple static test

        ## Part 1 true copy from pink to numpy
        I = pink.cpp.char_image([10, 14]);
        A = pink.cpp.pink2numpy(I); # this command makes a true copy
        self.assertTrue( A.dtype == numpy.uint8 )
        I[[2,3]]=10
        A[2,3]=207
        self.assertTrue( I[[2,3]] == 10 )
        self.assertTrue( A[2,3] == 207 )

        ## Part 2 true copy from numpy to pink
        B = numpy.ones([44,20], dtype=numpy.float32)
        J = pink.cpp.numpy2pink(B)
        self.assertTrue( J.imtype() == 'float' )
        B[4,5]=1.1
        J[[4,5]]=-7
        self.assertTrue( B[4,5] - 1.1 < 0.0001 )
        self.assertTrue( J[[4,5]] == -7 )
        
        ## Part 3 wrapper
        J = pink.cpp.int_image([90, 100])
        C = pink.cpp.wrap2numpy(J)
        
        J[[4,5]] = -111
        C[4,5] = 91
        self.assertTrue( J[[4,5]] == 91 )
        self.assertTrue( C[4,5] == 91 )

        C[9,3]=-1424
        J[[9,3]]=-33
        self.assertTrue( C[9,3]==-33 )
        self.assertTrue( J[[9,3]] == -33)
        
    def test_dynamic(self): ## test with random matrices
        
        size = [ random.randint(200,300), random.randint(300, 400) ]

        I = pink.cpp.int_image(size);

        print("filling up the matrix with random data")

        for q in range(size[0]):
            for w in range(size[1]):
                I[[q,w]] = random.randint(-1000,1000)

        print("Copy invariance")
        A = pink.cpp.pink2numpy(I)
        J = pink.cpp.numpy2pink(A)

        self.assertTrue( I==J )

        A[[199, 144]] = 23232
        J[[199, 144]] = -11231
        I[[199, 144]] = 434

        self.assertTrue( A[199, 144] == 23232 )
        self.assertTrue( J[[ 199, 144]] == -11231 )
        self.assertTrue( I[[ 199, 144]] == 434 )

        print("Wrapper")
        B = pink.cpp.wrap2numpy(I)
        for q in range(size[0]):
            for w in range(size[1]):
                B[q,w] = random.randint(-1000,1000)
        
        J = pink.cpp.numpy2pink(B)

        self.assertTrue(J==I)

    def test_garbage_collection(self): ## this would only work if the image is properly destroyed in each iteration
        size = [ random.randint(200,300), random.randint(300, 400) ]

        I = pink.cpp.int_image(size);

        print("filling up the matrix with random data")

        for q in range(size[0]):
            for w in range(size[1]):
                I[[q,w]] = random.randint(-1000,1000)
        

        for q in range(1000):
            B = pink.cpp.pink2numpy(I)

        self.assertTrue(True) ## if it gets up here, the memory was never filled



if __name__ == '__main__':
    unittest.main(testRunner=xmlrunner.XMLTestRunner(output='test-reports'))
