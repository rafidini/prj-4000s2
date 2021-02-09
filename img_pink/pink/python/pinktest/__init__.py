# -*- coding: utf-8 -*-
# 
# This software is licensed under 
# CeCILL FREE SOFTWARE LICENSE AGREEMENT

# This software comes in hope that it will be useful but 
# without any warranty to the extent permitted by applicable law.
  
# (C) UjoImro <ujoimro@gmail.com>, 2012
# ProCarPlan s.r.o.

import unittest
import xmlrunner

## The unit test framework

# from 2dcollapse import *
# from 2dkhalimskize import *
# from 3dcollapse import *
# from 3dkhalimskize import *
from areaopening import *
from areaselnb import *
from bisector import *
from closeball import *
from curve2segments import *
from curve2spline import *
from dilatball import *
from dilation import *
from drawline import *
from drawspline import *
from drawsplines import *
from erosball import *
from erosion import *
from float2byte import *
from gaussianfilter import *
from geodilat import *
from gradientcd import *
from heightmaxima import *
from heightminima import *
from heightselnb import *
from holeclosing import *
from identifyline import *
from identifyparabola2 import *
from identifyparabola3 import *
from label import *
from labelfgd import *
from lambdamedialaxis import *
from lambdamedialaxisauto import *
from long2byte import *
from medialaxis import *
from medianfilter import *
from numpymodule import *
from openball import *
from pca import *
from pgm2curve import *
from pgm2skel import *
from points2spline import *
from rankfilter import *
from redt import *
from rotate3dbin import *
from rotatebin import *
from segmentarea import *
from segmentheight import *
from segmentvol import *
from seuil import *
from skelcurv import *
from skeleton import *
from skeleton_t import *
from skelfindelbows import *
from skelpar import *
from skelpar3d import *
from skelpruning import *
from skelsurf import *
from skelvertex import *
from volmaxima import *
from volselnb import *
from watershed import *
from zoom import *

def main():
    alltests = unittest.TestSuite()
    # alltests.addTest(unittest.TestLoader().loadTestsFromTestCase(2dcollapse));
    # alltests.addTest(unittest.TestLoader().loadTestsFromTestCase(2dkhalimskize));
    # alltests.addTest(unittest.TestLoader().loadTestsFromTestCase(3dcollapse));
    # alltests.addTest(unittest.TestLoader().loadTestsFromTestCase(3dkhalimskize));
    alltests.addTest(unittest.TestLoader().loadTestsFromTestCase(areaopening));
    # alltests.addTest(unittest.TestLoader().loadTestsFromTestCase(areaselnb));
    # alltests.addTest(unittest.TestLoader().loadTestsFromTestCase(bisector));
    # alltests.addTest(unittest.TestLoader().loadTestsFromTestCase(closeball));
    # alltests.addTest(unittest.TestLoader().loadTestsFromTestCase(curve2segments));
    # alltests.addTest(unittest.TestLoader().loadTestsFromTestCase(curve2spline));
    alltests.addTest(unittest.TestLoader().loadTestsFromTestCase(dilatball));
    #alltests.addTest(unittest.TestLoader().loadTestsFromTestCase(dilation));
    alltests.addTest(unittest.TestLoader().loadTestsFromTestCase(drawline));
    #alltests.addTest(unittest.TestLoader().loadTestsFromTestCase(drawspline));
    # alltests.addTest(unittest.TestLoader().loadTestsFromTestCase(drawsplines));
    alltests.addTest(unittest.TestLoader().loadTestsFromTestCase(erosball));
    # alltests.addTest(unittest.TestLoader().loadTestsFromTestCase(erosion));
    # alltests.addTest(unittest.TestLoader().loadTestsFromTestCase(float2byte));
    # alltests.addTest(unittest.TestLoader().loadTestsFromTestCase(gaussianfilter));
    alltests.addTest(unittest.TestLoader().loadTestsFromTestCase(geodilat));
    alltests.addTest(unittest.TestLoader().loadTestsFromTestCase(gradientcd));
    alltests.addTest(unittest.TestLoader().loadTestsFromTestCase(heightmaxima));
    alltests.addTest(unittest.TestLoader().loadTestsFromTestCase(heightminima));
    #alltests.addTest(unittest.TestLoader().loadTestsFromTestCase(heightselnb));
    #alltests.addTest(unittest.TestLoader().loadTestsFromTestCase(holeclosing));
    #alltests.addTest(unittest.TestLoader().loadTestsFromTestCase(identifyline));
    # alltests.addTest(unittest.TestLoader().loadTestsFromTestCase(identifyparabola2));
    # alltests.addTest(unittest.TestLoader().loadTestsFromTestCase(identifyparabola3));
    # alltests.addTest(unittest.TestLoader().loadTestsFromTestCase(label));
    # alltests.addTest(unittest.TestLoader().loadTestsFromTestCase(labelfgd));
    # alltests.addTest(unittest.TestLoader().loadTestsFromTestCase(lambdamedialaxis));
    # alltests.addTest(unittest.TestLoader().loadTestsFromTestCase(lambdamedialaxisauto));
    # alltests.addTest(unittest.TestLoader().loadTestsFromTestCase(long2byte));
    # alltests.addTest(unittest.TestLoader().loadTestsFromTestCase(medialaxis));
    # alltests.addTest(unittest.TestLoader().loadTestsFromTestCase(medianfilter));
    alltests.addTest(unittest.TestLoader().loadTestsFromTestCase(numpymodule));
    alltests.addTest(unittest.TestLoader().loadTestsFromTestCase(openball));
    # alltests.addTest(unittest.TestLoader().loadTestsFromTestCase(pca));
    # alltests.addTest(unittest.TestLoader().loadTestsFromTestCase(pgm2curve));
    # alltests.addTest(unittest.TestLoader().loadTestsFromTestCase(pgm2skel));
    # alltests.addTest(unittest.TestLoader().loadTestsFromTestCase(points2spline));
    #alltests.addTest(unittest.TestLoader().loadTestsFromTestCase(rankfilter));
    # alltests.addTest(unittest.TestLoader().loadTestsFromTestCase(redt));
    # alltests.addTest(unittest.TestLoader().loadTestsFromTestCase(rotate3dbin));
    #alltests.addTest(unittest.TestLoader().loadTestsFromTestCase(rotatebin));
    #alltests.addTest(unittest.TestLoader().loadTestsFromTestCase(segmentarea));
    #alltests.addTest(unittest.TestLoader().loadTestsFromTestCase(segmentheight));
    #alltests.addTest(unittest.TestLoader().loadTestsFromTestCase(segmentvol));
    alltests.addTest(unittest.TestLoader().loadTestsFromTestCase(seuil));
    # alltests.addTest(unittest.TestLoader().loadTestsFromTestCase(skelcurv));
    alltests.addTest(unittest.TestLoader().loadTestsFromTestCase(skeleton));
    alltests.addTest(unittest.TestLoader().loadTestsFromTestCase(skeleton_t));
    # alltests.addTest(unittest.TestLoader().loadTestsFromTestCase(skelfindelbows));
    #alltests.addTest(unittest.TestLoader().loadTestsFromTestCase(skelpar));
    # alltests.addTest(unittest.TestLoader().loadTestsFromTestCase(skelpar3d));
    # alltests.addTest(unittest.TestLoader().loadTestsFromTestCase(skelpruning));
    #alltests.addTest(unittest.TestLoader().loadTestsFromTestCase(skelsurf));
    #alltests.addTest(unittest.TestLoader().loadTestsFromTestCase(skelvertex));
    alltests.addTest(unittest.TestLoader().loadTestsFromTestCase(volmaxima));
    alltests.addTest(unittest.TestLoader().loadTestsFromTestCase(volselnb));
    alltests.addTest(unittest.TestLoader().loadTestsFromTestCase(watershed));
    alltests.addTest(unittest.TestLoader().loadTestsFromTestCase(zoom));

    xmlrunner.XMLTestRunner(output='test-reports').run(alltests)


#LuM end of file
