#!/opt/local/bin/python2.6

import os, sys, re, ctypes
import pink
import datetime
import cv
import itertools
import numpy, Image, array
import scipy.misc

## The Pix class is for handling per-pixel information.
# The two data formats are pink image or a numpy array of pixel values.
# Whether the image is in 8 or 16 bytes format, it will be loaded then stored as an int32_t pink image.
# It is stored in the highest format in order to be able to get 8 or 16 from it without loosing data.
# The numpy is always 1Dimensionnal. If needed, the reshaped() function is used to make it 2Dimensionnal (see get_numpy_2d() )

class Pix:
	
	## Constructor that needs the width and height parameters when called with a numpy array type.
	#
	# @param data The pink image or numpy array containing the pixels value.
	# @param size The size of the numpy array. It has to be a [width,height] list, which is the format used within Pink  
	# Numpy and openCV go with a (height,widht) tuple. This format of size is stored in self.size_np.
	# @param format The format of the stored pixels.
	# @return Returns a Pix objet.
	
	# dataformats
	# byte,char 	- uint8_t   1 byte
	# long      	- int32_t   4 bytes
	# float     	- float     4 bytes
	# double,short	- uint16_t	2 bytes
	
	# imagetype
	# binary
	# labelled
	# gray
	
	pinktype_to_numpydtype  = {
		'uint8_t':numpy.dtype('u1'),  # 0 to 255
		'float'  :numpy.dtype('f4'), 
		'int32_t':numpy.dtype('i4'),
	}
	numpydtype_to_pinktype  = dict((v,k) for k, v in pinktype_to_numpydtype.iteritems())
	
	def __init__(self, data, size=None, imagetype=None, debug=False):
		self.debug = debug
		self.data = {}
		self.data['pink_image']  = None
		self.data['numpy_array'] = None
		
		
		if imagetype is None:
			raise BaseException("imagetype must be set to binary,labelled or gray")
		
		# if the data is a python list there must be a size given
		if type(data).__name__=='ndarray':
			self.type = 'numpy_array'
			if size is None:
				raise BaseException("No size given for numpy array pix object initialisation")
			self.size   = size  # list [width, height]
			# self.size = data.shape # data.shape if 2D
			self.dataformat = Pix.numpydtype_to_pinktype[data.dtype]	
			self.imagetype = imagetype
		else :
			self.type       = "pink_image"
			self.size       = data.size # list [width, height]
			self.dataformat = data.imtype()  # uint8_t etc
			self.imagetype  = imagetype

			
		self.data[self.type] = data
		self.npixels = self.size[0] * self.size[1]
		self.size_np = (self.size[1], self.size[0]) # tuple (height, width)
		
		if self.dataformat not in ['uint8_t', 'int32_t', 'float']:
			raise BaseException("dataformat [%s] is weird" % self.dataformat)
		if self.imagetype not in ['binary', 'labelled', 'gray']:
			raise BaseException("imagetype [%s] is weird" % self.imagetype)

	
	## Loads data from the given filename. The input image has to be a .pgm file. 
	# Whether the image is in 8 or 16 bytes format, it will be loaded then stored as an int32_t pink image in the returned Pix object.
	# It is stored in the highest format in order to be able to get 8 or 16 from it without losing data.
	# @ param filename The filename of the slide.
	# @ return Returns a Pix object containing an int32_t pink_image.
	@staticmethod
	def load(filename, imagetype='gray', debug=False):
		im = pink.cpp.readimage(filename)
		im = pink.cpp.convert2long(im)
		return Pix( im, imagetype=imagetype, debug=debug)
		
	
	## Save the image under the filename.
	# The purpose of saving being to check the results, the outputs have to make sense.
	# For example, the real output of a binary image would be a pitch black image (0's and 1's).
	# This is why the default value of expand_range has been set so that the actual data is first converted before saving.
	# This has to be taken into account when checking the values on the output image.
	# If actual values are to be privileged over visibilty, set the expand_range parameter to False.
	# @param filename The filename under which the image is to be saved.
	# @param expand_range If False, output will contain the real values without conversion. 
	# Default value is True. Keep True if visible output is wanted.
	def save(self, filename, expand_range=True):
		if (self.imagetype=='labelled' or self.imagetype=='binary') and expand_range:
			# convert to float and back via pink
			# this will scale it into 0-255
			self.convert_to('float').convert_to('uint8_t').get_pink().writeimage( filename ) 
		else:
			self.convert_to('uint8_t').get_pink().writeimage( filename )
		

	## Opens imview to show the current image, whatever its format is.
	def view(self):
		pink.imview( self.get_pink() )
	
	## The get_pink() function returns a pink_image from the data contained in the current Pix.
	# This function has been created to use the morphological operations from Pink.
	# It finds its application in EImage where Pink is mainly used. 
	# If the calling Pix already contains a pink_image, no new object is created.
	# Otherwise, it generates a pink object out of the numpy array contained in the Pix.
	# @return Returns an object of type pink_image.	
	def get_pink(self):
		if self.data['pink_image'] is None:
			self.create_pink_representation()
		return self.data['pink_image']

	#===============================================================================================
	#	Numpy manipulation 
	#===============================================================================================

	## The get_numpy_whatever() function returns a numpy_array from the data contained in the current Pix.
	# Numpy arrays permit to easily manipulate the pixels values of an image.
	# If the calling object already contains a numpy_array, no new object is created.
	# Otherwise, it generates a numpy_array out of the pink_image contained in the Pix, 
	# in which case the data format is determined by the pink_image.
	# The pixels values in the array can be in whatever format (uint8_t, float, uint32_t).
	# This takes its whole interest when the array is created out of a pink_image, which is not necessarily an uint8_t.
	# @return Returns a 1D numpy_array.	
	def get_numpy_whatever(self):
		if self.data['numpy_array'] is None:
			self.create_numpy_representation()
		return self.data['numpy_array']		

	## The get_numpy8() returns an uint8_t numpy_array from the data contained in the current Pix.
	# It is not currently called anywhere, but has to be considered as a way to gain accuracy/clarity.
	# It would permit to know when a uint8_t is needed, where it has no importance,
	# and which type of data is currently being used. 
	# It has been created after the get_numpy16() has been implemented in order to avoid confusion.
	# @return Returns the 1D numpy array contained in the current Pix as an uint8_t array.
	def get_numpy8(self):
		return self.get_numpy_whatever().astype('u1')

	## The get_numpy16() function returns a 2D uint16_t numpy_array from the data contained in the current Pix.
	# If has been created for particular functions which cannot handle in32t_t data.
	# It is only called in Features, where the calculate Moments from opencv does not support the 32 format.
	# It could deal with uint8_t format. However, results can be improved using a higher data format.
	# Because the only functions that cannot handle int32_t are opencv functions, and that
	# opencv uses 2D arrays, only 2D uint16_t are needed.
	# @return Returns a 2D uint16_t numpy array.
	def get_numpy16(self):
		array_1d = self.get_numpy_whatever().astype('u2')
		return numpy.reshape(array_1d, self.size_np)	
			
	## Returns the numpy array contained in the Pix object as a 2Dimensionnal array (instead of 1D)
	# the reshaped array is the SAME object as the 1D object. They share the same pointer.
	# What modifies the 2d array mofifies the 1D too.	
	def get_numpy_2d(self):
		array_1d = self.get_numpy_whatever()
		array_2d = numpy.reshape(array_1d, self.size_np)
		return array_2d

	# get_numpy_1d() does not deal with the numpy values in a Pix object, since it would never contain a 2D.
	# This function returns a 1dimensionnal array version of the 2d array in parameters	
	@staticmethod
	def get_numpy_1d( np_array_2d ):
		return numpy.reshape( np_array_2d, (1, np_array_2d.size) )[0]

	
	#====================================================================================================
	#  Convert numpy-to-pink and pink-to-numpy for the internal representation
	#====================================================================================================
	
	
	## This function creates the self.data['pink_image'] item if it has not been previously created.
	# For futher explanation on its utility, see get_pink()
	########### check if i'm not saying total crap ###########################
	# Depending on the dataformat of the numpy array, a different function is called to create the pink object.
	# The pointers that permits to access data in Pink are from C++
	# The datas are taken from the array and directly put at the adresses pointed by these pointers.
	# The process of conversion from numpy to pink has been optimised to be the fast.
	# It involves delicate pointers manipulation and should not be changed.
	# @return Returns nothing
	def create_pink_representation(self):
		if self.data['pink_image'] is None:
			if self.dataformat=='uint8_t':	
				self.data['pink_image'] = pink.cpp.char_image( self.size )
				pointer_type            = ctypes.c_uint8
			elif self.dataformat=='int32_t':
				self.data['pink_image'] = pink.cpp.int_image( self.size )
				pointer_type            = ctypes.c_int32
			elif self.dataformat=='float':
				self.data['pink_image'] = pink.cpp.float_image( self.size )
				pointer_type            = ctypes.c_float			
			else:
				raise BaseException("Can't yet convert from [%s] numpy array to pink" % self.dataformat)

			if self.dataformat=='float':
				p = self.data['pink_image'].get_pixels()
				match = re.search("ptr (0x.*?),.*?at (0x.*?)>", p.__repr__())
				ptr = int(match.group(1), 16)
				at  = int(match.group(2), 16)		
				pp = ctypes.cast(ptr, ctypes.POINTER(pointer_type))

				python_list = self.get_numpy_whatever()
				for i in range(self.npixels):
					pp[i] = python_list[i]
				return

			p = self.data['pink_image'].get_pixels()
			match = re.search("ptr (0x.*?),.*?at (0x.*?)>", p.__repr__())
			pink_ptr = int(match.group(1), 16)

			numpy_array   = self.get_numpy_whatever()
			numpy_pointer = numpy_array.ctypes.data

			ctypes.memmove(pink_ptr, numpy_pointer, numpy_array.nbytes)
	
	
	## This function creates the self.data['numpy_array'] item if it has not been previously created.
	# For futher explanation on its utility, see get_numpy_whatever()
	########### check if i'm not saying total crap ###########################
	# Uses a numpy wrapper to get directly the data from the Pink buffer which memory is allocated in C++.
	# The process of conversion from pink to numpy has been optimised to be the fast.
	# It involves delicate pointers manipulation and should not be changed.
	# @return Returns nothing					
	def create_numpy_representation(self):
		if self.debug: print "DEBUG: Pix.create_numpy_representation()"
		if self.data['numpy_array'] is not None:
			return
		if self.data['pink_image']:
			# do a slow copy
			if self.debug: print "DEBUG: Pix.create_numpy_representation()", "slowcopy", self.dataformat
			self.data['numpy_array'] = numpy.frombuffer( self.data['pink_image'].get_pixels()[:] , dtype=Pix.pinktype_to_numpydtype[self.dataformat])
			if self.data['numpy_array'].size != self.npixels:
				raise BaseException("xxxxxxxxxxxxxxxxxxxxx  " + self.dataformat + "  " +  Pix.pinktype_to_numpydtype[self.dataformat].__str__())
				
	#====================================================================================================
	#  Ask for new Pix objects in different formats
	#====================================================================================================
	
	## This function creates a new Pix from the current one.
	# The new Pix contains the same data as the current one, converted to the dataformat given in parameters.
	# Its main purpose is to have a clean way of converting data before saving images (see save() )
	# It is also used to make int32_t -> uint8_t conversions in watershed.
	# Indeed, some pink functions always return a in32_t pink object, when watershed processes everything with uint8_t images. 
	# The conversion is made by calling Pink functions.
	# @parameter dataformat The dataformat wanted for new Pix. It can be 'uint8_t', 'float' or 'int32_t'
	def convert_to(self, dataformat='uint8_t'):
		# mode from 0 to 5 are available.
		# However, weird values may result from a mode set to another value than 2.
		# That is why choice is not provided anymore in the convert_to() parameters.
		float2byte_mode=2
		long2byte_mode=2
		
		if dataformat == self.dataformat:
			return self
		elif dataformat == 'float':
			return Pix( pink.cpp.convert2float(self.get_pink()), imagetype=self.imagetype, debug=self.debug )
		elif dataformat == 'int32_t':
			return Pix( pink.cpp.convert2long(self.get_pink()), imagetype=self.imagetype, debug=self.debug )
		elif dataformat == 'uint8_t':
			if self.dataformat == 'int32_t':
				# long2byte can do the conversion in a few ways. 
				# We are mostly going to be converting int32_t labelled images
				# into bytes, so mode=0 is the default 
				#    mode = 0 (default) : for all x, out[x] = min(255, in[x]).
		        #    mode = 1 : for all x, out[x] = in[x] modulo 256.
		        #    mode = 2 : scales values in the range 0-255.
		        #    mode = 3 : sorts the values by decreasing number of occurence in the image.
		        # Replaces the values by their order.
		        # Only the n (default 255) first values are kept.
		        # Useful for label images.
		        #    mode = 4 : truncation of the square root in the range 0-255.
		        #    mode = 5 : truncation of the log in the range 0-255.
				return Pix( pink.cpp.long2byte(self.get_pink(), mode=long2byte_mode, nb_new_val=0), imagetype=self.imagetype, debug=self.debug )
			elif self.dataformat == 'float':
				# mode = 0  : for all x, out[x] = min(255, arrondi(in[x])).
				# mode = 1 : for all x, out[x] = arrondi(in[x]) modulo 256.
				# mode = 2 : scales values in the range 0-255.  ################ DEFAULT ###########
				# mode = 4 : truncation of the square root in the range 0-255.
				# mode = 5 : truncation of the log in the range 0-255.
				return Pix( pink.cpp.float2byte(self.get_pink(), mode=float2byte_mode), imagetype=self.imagetype, debug=self.debug )
				
		else:
			raise BaseException("No method implemented to convert %s to %s" % (self.dataformat, dataformat))
			

	#===================================================================================================
	# Masking
	#===================================================================================================
	
	## Creates a Pix containing a int32_t pink_image which is a labelled version of the input image.
	# The labelling puts different values on each separated element, using 8 connectivity.
	# 0's where background is. 1's for the first element found, 2's for the second ect .....
	# This function is called in find_center_blob() to differenciate the blobs
	# The pink function labelfgd does the labelling by itself.
	# @return Returns a Pix containing an int32_t labelled pink_image
	def label(self):
		return Pix( pink.cpp.labelfgd(self.get_pink(), 8), imagetype='labelled') 	
	
	## Creates a Pix which only contains the pixels of the current image where mask == val.
	# The other pixels are set to 0.
	# This functions permits, once a mask has been created, to apply it on a Pix containing a gray image,
	# and get the grey values only where the element of interest is. 
	# If returned result was saved, it would display the gray element corresponding to the mask, surrounded by black.
	# The only difference with get_value_via_mask() is that result is given as a Pix.
	# This finds a single application in Features, where an array is needed to create an opencv Mat.
	# When calculating hu moments on the color_channel of the element, we only want the element values to be taken into account,
	# but we can not call the opencv function on a list of values.
	# In order for the values to be given as an array without affecting the results, the 'void' between the actual element and the 'box'
	# surrounding it to create an array can be filled with 0's.
	# It is also called in the hypothetic l_channel_mean(), l_channel_mean_old() functions
	# @param mask The Pix containing the mask (binary or labelled image)
	# @param val  The value in the mask image which will be used to select pixels
	# @return Returns a Pix.
	def mask(self, mask, val=1):		
		# bin_mask == 1 where mask == val, else 0
		bin_mask  = mask.create_binary_mask_via_value(val).get_pink()	
		masked_im = pink.cpp.mult(self.get_pink(), bin_mask)  
	
		return Pix(masked_im, imagetype=self.imagetype, debug=self.debug)
		
	## Uses the mask to get the values of the pixels of the current image where mask == val.
	# This functions permits, once a mask has been created, to apply it on a Pix containing a gray image,
	# and get the grey values only where the element of interest is.
	# In our case, the interesting grey values are the ones from the R,G or B Pix components.
	# Indeed, the amount of red, green or blue in an element is a good way to caracterize it.
	# @param mask The Pix containing the mask (binary or labelled image)
	# @param val  The value in the mask image which will be used to select pixels	
	# @return The informations are returned as a 1D numpy array of values
	def get_value_via_mask(self, mask, val=1):
		mask_bool = mask.get_numpy_whatever() == val
		values    = self.get_numpy_whatever()[mask_bool]

		return values			
		
	## Returns a Pix object containing a binary image (0/1) with the value set to 1
	# wherever the current Pix (self) is set to val.	
	# It is used to created a new mask out of a labelled image.
	# The most obvious place where it proves useful is when creating the nucleus, cytoplasm and RBC markers
	# out of the watershed in the watershed() function of EImage.
	# @param val  The value in the image which will be used to select pixels 
	# @ return Retuns a Pix containing a binary image.
	def create_binary_mask_via_value(self, val):
		im_bool = self.get_numpy_whatever() == val

		return Pix(im_bool.astype(numpy.dtype('u1')), size=self.size, imagetype='binary', debug=self.debug)
			
	
	## Creates a Pix containing a binary mask.
	# The mask is generated by applying the threshold on the current image.
	# The element of interest (1's in the resulting mask) is composed of the values below or above threshold.
	# This function finds its application in EImage, after findind thresholds with histograms.
	# If the threshold is accurate enough, applying it on the corresponding image creates a new marker	
	# containing a type of element. 
	# @param reverse If False, the element of interest is composed of the values strictly below threhold. 
	# If True, the element of interest is composed of the values above or equal to threhold. 
	# @param threshold The threshold value.
	# @return Returns a Pix object containing the binary mask .
	def create_binary_mask_via_threshold(self, threshold, reverse=False):
		if reverse:
			im = self.get_pink()
			mask_pink = pink.cpp.seuil(im, threshold, 0,1)
		else:
			mask_pink = pink.cpp.seuil(self.get_pink(), threshold, 1,0)
		return Pix(mask_pink, imagetype='binary', debug=self.debug)						

	#=========================================================================================
	# Morphology
	#=========================================================================================
		
	## Creates a Pix containing a filtered version of the input binary image.  
	# It is only used once in EImage fo slightly filter the background instead of a heavy close_hole.
	# Uses pink erosion to remove small blobs.
	# Uses pink dilation to fill in small holes.
	# @return Returns a Pix with the same type as input .
	def filter_noise(self):
		eroded   = pink.cpp.erosion( self.get_pink(), pink.cpp.genball(2))
		recons1  = pink.cpp.geodilat(eroded, self.get_pink(), 8, 10)
		dilated  = pink.cpp.dilation(recons1,pink.cpp.genball(2))
		filtered = pink.cpp.geoeros(dilated, recons1, 8, 10)
		return Pix(filtered, imagetype=self.imagetype, debug=self.debug)
	
				
	## Creates a Pix containing the input binary image without any element touching the borders.	
	# Every element which is touching the edge of the image becomes part of the background.
	# Add a frame to a copy of the image (1 pixel around the edge),
	# Find anything which touches this frame (by using geodilation),
	# and then subtract it from the image,
	# @return Returns a Pix with the same type as input.
	def remove_border_objects(self):
		frame = pink.cpp.frame( pink.cpp.char_image(self.size), 255 ) 
		border_objects = pink.cpp.geodilat( frame, self.get_pink(), 8 )
		result = self.get_pink() - border_objects
		return Pix(result, imagetype=self.imagetype, debug=self.debug)	
		
	## Closes the holes in a binary image where 0 is background and 1 is foreground
	# Most of the time, the RBCs have a very light centre, which show up as holes in the FG image. These holes should be closed
	# so that the interior of RBCs is considered to be part of the foreground.
	# This technique uses geodilation.
	# @return Returns a Pix containing the closed_hole image.	 
	def close_holes(self):
		if self.imagetype!='binary':
			raise BaseException("Can only close_holes on a binary imagetype")

		#filt= self.filter_noise().get_pink() 
		topval = 255
		frame = pink.cpp.frame( pink.cpp.char_image(self.size), topval)
		inv     = pink.cpp.inverse(self.get_pink())
		dilated = pink.cpp.geodilat(frame, inv, 8)
		result  = pink.cpp.inverse(dilated)
		
		return Pix(result, imagetype=self.imagetype, debug=self.debug)
		
	## Wrapper for the pink erosion function.
	# just because it's such an ugly call. We need to investigate on how to modify
	# the genball values according to the size of the image
	# @return Returns a Pix containing the eroded pink_image.
	def erode(self, ballsize):
		if ballsize=='small':
			#print "ERODE small"
			#ballsize = self.size[0]/100
			ballsize = 4
		elif ballsize=='large':
			#print "ERODE large"
			ballsize = self.size[0]/40
		#print "Eroding with:", ballsize
		return Pix ( pink.cpp.erosion(self.get_pink(), pink.cpp.genball(ballsize) ) , imagetype=self.imagetype, debug=self.debug)
	
	#===========================================================================================
	# Image manipulation
	#===========================================================================================
	
	## input image must be binary (filled with 1 & 0's).
	# @return Returns a Pix containing the inverse 0 & 1's.
	def bin_inverse(self):                            
		return Pix(pink.cpp.seuil(self.get_pink(),1,1,0), imagetype='binary', debug=self.debug)	
		
	## Scale the image by the given coefficient
	# The scaling automatically converts to uint8_t
	# This has two main reasons :
	# - pink.cpp.zoom does not handle correctly the scaling of an int32_t image. (round the values)
	# - scale() is only used in the create_watershed() function (see EImage), where everything is done in 8bytes.
	# @param coeff shrink if coeff < 1  
	# @return Returns a Pix containing a pink of type uint8_t, regardless of the input format.
	def scale(self, coeff):
		if coeff==1:
			return self
		return Pix( pink.cpp.zoom(self.convert_to('uint8_t').get_pink(),coeff,coeff,1), imagetype=self.imagetype, debug=self.debug )	
	
	## Scale the image to exactly fit the size given in parameters.
	# @param new_size Has to be given as follow : list [width, height]
	# @return Returns a Pix containing an array of same type as input
	def scale_by_size(self, new_size):
		size_scipy = (new_size[1], new_size[0]) # tuple (height, width)
		
		origim_array = self.get_numpy_2d()

		scaled_array_2d = scipy.misc.imresize(origim_array, size_scipy )
		scaled_array_1d = Pix.get_numpy_1d(scaled_array_2d)
		scaled_im =  Pix(scaled_array_1d, size = new_size, imagetype = self.imagetype)
		
		return scaled_im
	
	# Creates a new pix, using the bounding box parameters to crop the current image.
	# @param box The box in blobstats['bounding_rect'], the dict blobstats{} being returned by one of the calc_features() function
	# @return Returns a new Pix containing the cropped image
	def crop(self, box):
		# be careful !! While we are used to size : widht, height. In numpy, it goes Height, Width !!!
		# and it has to be a list to create a Pix, but a tuple when using numpy.
		x1,y1,width,height = box
		box_size = [ width+1, height+1 ]
		
		array = self.get_numpy_2d()
		#array = numpy.reshape(array,(self.size[1], self.size[0]))
		
		# reduce the  binary image to the bounding box around the element.
		# box_bin = numpy.zeros((height+1, width+1) , dtype = int)
		# box_bin[:,:] = array[ y1 : y1+height+1 , x1 : x1+width+1 ]
		box_bin = array[ y1 : y1+height+1 , x1 : x1+width+1 ]

		# Convert the box_bin back to the right type
		target_dtype = Pix.pinktype_to_numpydtype[self.dataformat]
		box_bin = box_bin.astype( target_dtype ) #2D 
		box_bin = Pix.get_numpy_1d(box_bin)

		return Pix(box_bin, size = box_size, imagetype = self.imagetype)

	
	#==========================================================================================
	# Calculation
	#==========================================================================================
	
	## Get the area and center coordinates of a element in the image, 
	# this element being the set of pixels equal to val.
	# @return Return area, x_center, y_center	
	def get_area_and_center_by_val(self, val=1):
		coords = numpy.where( self.get_numpy_2d()==val )
		coord = zip(*coords)
		area = len(coord)
		x_center = None
		y_center = None
		
		if area!=0:
			rows, cols = zip(*coord)
			x_center = sum(cols)/area
			y_center = sum(rows)/area
		
		return area , x_center, y_center

		
	## Given the labelled nucleated_cells markers (see EImage), find the nucleated cell in the center.	
	
	# Once watershed has been created,the binary image containing several white cells can be extracted.
	# The process is as follows :
	# For each nucleated cell marker (with only one blob on it) :
	#		Check if area is greated than a threshold size (min_size)
	#		If distance from center is shorter than before, make it the closest blob
	# @param nucleated_cells nucleated list containing all the nucleated cell markers
	# @return Returns the marker containing the center blob.
	# input : Pix binary image with all the blobs, 0's being background, 1's being the elements.
	# output : Pix binary image with the center element being 1's
	def find_center_blob(self):
		min_size = 5000
		im_coord_center =[  self.size[0]/2, self.size[1]/2]
		min_dist = self.npixels**2
		closest_blob_index = 0
				
		print"		labelling each blob on image"
		# label each different nucleated cell from the binary image  
		labelled_im_32 = self.label()
		labelled_im_8 = labelled_im_32.convert_to('uint8_t')  # Check if conversion if really needed
		# find the number of nucleated cell on this image
		numblobs = pink.cpp.minmax(labelled_im_8.get_pink())[1]
		if numblobs == 0:
			print"		No blob found !"
			return -1
				
		print"		finding blob in center"
		for i in range(numblobs) :
			print "		processing a blob "
			area_nucl, x_center, y_center = labelled_im_8.get_area_and_center_by_val( i+1 ) # does not start at index 0, 0 being background
			if area_nucl > min_size :			
				x_dist = x_center - im_coord_center[0]
				y_dist = y_center - im_coord_center[1]
				new_dist = x_dist**2 + y_dist**2
				if new_dist < min_dist:
					min_dist = new_dist
					closest_blob_index = i+1

		if closest_blob_index == 0 :
			print"		No blob found in center"
			return -1
			
		return labelled_im_8.create_binary_mask_via_value(closest_blob_index) 
		
	## Creates a mask using a k-means (k=2) on inpu image ( Green channel )
	# @return Returns a Pix containing the binary mask : 0-255 ?? or 1-0 ?		
	def split_im_by_lightness(self):	
		size         = self.size

		green_pixels = self.get_numpy_whatever()
		kmeans_out   = scipy.cluster.vq.kmeans2(green_pixels, k=2)

		centers      = list(kmeans_out[0])
		mask_np    = numpy.array(kmeans_out[1]).astype( numpy.dtype('u1') )

		# Ensure the higher index is fg
		if centers.index( min(centers) ) == 1:
		 	return Pix(mask_np, size, imagetype='binary')
		else:
		 	return Pix(mask_np, size, imagetype='binary').bin_inverse()
		
	## The calc_gradient function creates the gradient image from the choosen plane.
	# @return Returns a Pix object containing the gradient image (pink type).
	def calc_gradient(self):	
		norm     = pink.cpp.normalize(self.get_pink())
		gradient = pink.cpp.gradientcd(norm,1)
		return Pix(gradient, imagetype=self.imagetype, debug=self.debug)

if __name__=="__main__":
	pass
	

	
	