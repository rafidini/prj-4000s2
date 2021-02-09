#! /usr/bin/python

# UjoImro, 2009
# This software is meant to be free.

import sys
sys.path.append("/home/ujoimro/doc/projects/Pink/python/")
testdir="/home/ujoimro/doc/projects/Pink/python/test/"
filename="/home/ujoimro/doc/projects/MICCAI/DATA/Challenge Data (Training)/SC-HF-I-01_SEGM/IsoVol/res/LVCM0018.pgm"
from glob import glob
from threading import Thread, activeCount
from random import random
from time import sleep
MAX_THREAD_NUM=16
THREAD_NUMBER_PRECISION=0.01


directory="/home/ujoimro/doc/projects/MICCAI/DATA/Challenge Data (Training)/SC-HF-I-01_SEGM/IsoVol/res"
dicom2pgm_exec="/home/ujoimro/doc/projects/MICCAI/Pinktmp/Analyse/dicom2pgm"


class iteration(Thread):
    
    def __init__(self, function, parameters):
        Thread.__init__(self)
        self.parameters = parameters
        self.function = function
        
    def run(self):
        self.result = self.function(*self.parameters)


def parallel_map(function, parameters):
    """
    This function calls the function 'function' with the parameters from the 
    set 'parameters'. It calls all in parallel, so the order can be arbitrary. 
    In the end it returns a list of results with the order respected.
    """
    def create_iteration( parameter ): 
        return iteration( function, parameter )
        
    def schedule( iteration ):
        """
        Schedules the iteration. It waits until some threads finish, and there 
        are at most MAX_THREAD_NUM threads running. 
        note: Not yet implemented: Later for avoid dead locks it should launch 
        the thread after some time, even if it'd depasse the MAX_THREAD_NUM
        """
        while activeCount() > MAX_THREAD_NUM:
            sleep(THREAD_NUMBER_PRECISION)
        iteration.start()

    def join_and_get_result( iteration ):    
        iteration.join()
        return iteration.result        

        
    # Creating iterations as thread objects    
    iterations = map( create_iteration, parameters )
        
    # All the iterations created, now we have to run them    
    # first thread is always started without scheduling, because, if parallel_map 
    # is called from another parallel_map, then there're already MAX_THREAD_NUM 
    # threads, so the iteration cannot begin 
    iterations[0].start()
    map( schedule, iterations[1:] )
            
    # All the iterations are finished, so we only need to collect the results
    return map( join_and_get_result, iterations )


#def sleepy(x,y,z):
#    print "started " + str(x) + ", " + str(y) + ", " + str(z)
#    sleep(x+y+z)
#    print "finished " + str(x) + ", " + str(y) + ", " + str(z)
#    return [x, y, z]


#sleepy(1,1,1)


#A=iteration(sleepy, [1,1,1])
#A.start()
#A.join()
#print A.result


#parameters=[]
#for q in range(30):
#    parameters.append([random(),random(),random()])


#parameters


#parallel_map(sleepy, parameters)








# LuM end of file
