# -*- coding: utf-8 -*-
#
# This software is licensed under the
# CeCILL FREE SOFTWARE LICENSE AGREEMENT

# This software comes in hope that it will be useful but 
# without any warranty to the extent permitted by applicable law.
  
# (C) H. Talbot <h.talbot@esiee.fr>, 2011
# Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX

#
# The Famous Imview Viewer
#
"""
This module prototypes interaction with the imview viewer
"""

import pink

def namestr(obj, namespace):
	return [name for name in namespace if namespace[name] is obj]

def imview(images,debug=False):
	"""A function to display an image in Pink/Python. It works on
	images and lists of images:
	pink.imview(I)
	pink.imview([I,J])
	"""

	if not isinstance(images, list):
		if (debug):
			PLiarEnableDebug()
		# start an imview
		port = pink.cpp.Pimview()
		# login to imview
		conn = pink.cpp.Pimviewlogin("","",port)
		# upload the data
		imagename = namestr(images,locals())[0]
		up = pink.cpp.Pimviewputimage(images,imagename,conn)
		return
	else: # isinstance(image, list)
		num = 0
		viewer = pink.Imview(images[0])
		for q in range(1, len(images)):
			viewer.show(images[q], str(q))


class Imview:
	"""A class to display images in Pink/Python"""
	def __init__(self, image):
		# start an imview
		self.port=pink.cpp.Pimview()
		# login to that imview
		self.conn = pink.cpp.Pimviewlogin("","",self.port)
		# upload the data
		# self.imagename = namestr(image, locals())[0]
		self.up = pink.cpp.Pimviewputimage(image,"image",self.conn)
	def debug(self,d=False):
		self.debugstatus = pink.cpp.Pimview_setdebug(d)
	def show(self,image, name="image"):
		# reup an image
		self.up = pink.cpp.Pimviewputimage(image,name,self.conn)
		if (self.up > 0):
			# reinit oneself
			self.__init__(image)

		

# LuM end of file
