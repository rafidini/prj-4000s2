#!/opt/local/bin/python

from Pix  import Pix
import numpy
import pink
import datetime
import itertools
## This class manages everything that deals with histograms.
class Hist:
	
	## Constructor
	# @param vals The pixels values which we want a histogram of.	
	# @param absmin The lowest value on the abscissa axe
	# @param absmax The lowest value on the abscissa axe
	# @param scale Size of the ordinate axe.
	def __init__(self, vals, absmin = 0, absmax = 255, scale=300):

		self.vals = vals

		if absmin is not None and absmax is not None:
			if absmin > min(self.vals) or absmax < max(self.vals):
				print min(self.vals), max(self.vals)
				raise BaseException("abs out of bounds")

		self.absmin = absmin if absmin is not None else min(self.vals)
		self.absmax = absmax if absmax is not None else max(self.vals)
		
		# Get numpy to calculate the actual histogram			
		nhist = numpy.histogram(self.vals, bins=numpy.arange(self.absmin,self.absmax,(self.absmax-self.absmin)/255.0), range=(self.absmin, self.absmax))
		# self.ord = list(nhist[0])
		# self.abs = list(nhist[1])
		self.ord = [float(x) for x in nhist[0]]
		self.abs = [float(x) for x in nhist[1]]

		self.scale = scale
		self.image = None
		
	
	## Prints a text version of the histogram.
	# @return Returns nothing
	def print_long(self):
		print "---- print_long start"
		for i,(a,o) in enumerate(itertools.izip(self.abs,self.ord)):
			print "%03i\t%5i\t%5i" % (i, a, o), "*"*int(o/self.scale)
		print "---- print_long end"
	
	
	## Finds the abs approximatively by selecting the first point where ord > ordinate
	# @param ordinate The value to seek on the ordinate axe
	# @param reverse If reverse == True, the research goes from left to right eg from the end of the abscissa axe to origin.
	# @param draw_lines If draw_lines == True, the found point will be displayed by axes which cross on its position.
	# @param trim Offset which permits to skip the points from beginning of reseach to abscissa = trim.
	def find_abs(self, ordinate = 600, reverse = False, draw_lines=False, trim=0):
		t_ord = self.ord
		t_abs = self.abs 
		if reverse == True:
			t_ord.reverse()
			t_abs.reverse()
			
		t_ord = t_ord[trim:]	
		t_abs = t_abs[trim:]
		
		if draw_lines:
			self.draw_hline(ordinate)
		
		for a, o in itertools.izip(t_abs, t_ord): 
			if  o > ordinate: 		
				# if draw_lines:
				# 	self.draw_vline(a)
				return a
		return None
				
	## Saves the histogram image at filename.
	def save(self, filename):
		if not self.image:
			self.create_image()
		Pix(self.image, imagetype='gray').save(filename)
		
	## Creates the images representing the histogram, which is stored as a pink image in self.image
	# @return Returns nothing
	def create_image(self):
		self.image = pink.cpp.char_image([256,256])
		for a, o in itertools.izip(self.abs, self.ord):
			# Rescale Abs to [0,255]
			a = int((a-self.absmin) / float(self.absmax-self.absmin) * 255)
			o = int(o/self.scale)
			if o > 255:
				o = 255
			self.image = pink.cpp.drawline(self.image, a, 255, a, 255-o )
			
	## Draws a vertical on the histogram where abscissa = a .
	# @param a The abscissa coordinate of the vertical lign
	# @return Returns nothing
	def draw_vline(self, a):
		if not self.image:
			self.create_image()
		a = int((a-self.absmin) / float(self.absmax-self.absmin) * 255)
		self.image = pink.cpp.drawline(self.image, a, 255, a, 0)		
	
	## Draws an horizontal on the histogram where ordinate = a.
	# @param a The ordinate coordinate of the horizontal lign
	# @return Returns nothing	
	def draw_hline(self,o):	
		if not self.image:
			self.create_image()
		o = int(o/self.scale)
		self.image = pink.cpp.drawline(self.image, 0, 255-o, 255, 255-o)		
		
if __name__=="__main__":
	if False:
		p = Pix.load('xxx_0.pgm')
		h = Hist(p.get_list(), scale=150)
		h.save("hist.pgm")
		h.find_abs(ordinate=6000, reverse=False, draw_lines=True)
		h.save("hist_with_threshold.pgm")
		
	import random
	x = [random.gauss(20,5) for x in range(1000)]
	h = Hist(x, scale=1)
	h.print_long()
	h.save('hist.pgm')
	









		
		