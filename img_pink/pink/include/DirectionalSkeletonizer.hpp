#ifndef _DirectionalSkeletonizer_HPP_
#define _DirectionalSkeletonizer_HPP_
class DirectionalSkeletonizer{
	private:
		//Look Up Tables 
		unsigned char*      lut;

		//Grid dimensions
		unsigned int 		width;
		unsigned int		height;
		unsigned int        depth;
		unsigned int        size;


		//neighbors relatives coordinates
		int nbCoordinates[26];

		//index of neighbor flag
		unsigned int bitIndex[26];
		unsigned int bitIndexOp[26];

		//index of neighbor by symetry
		unsigned int oppositeNb[26];

		//temporary grids
		unsigned int* gridConfig;
		unsigned char* constraints;
		unsigned char* status;


	public:
			//Constructor
			DirectionalSkeletonizer(unsigned int maxWidth, unsigned int maxHeight, unsigned int maxDepth);
			//Destructor
			~DirectionalSkeletonizer();
			void setDimensions(unsigned int width, unsigned int height, unsigned int depth);
			unsigned int skeletonize(unsigned char* grid, bool curvilinear=true, unsigned int nbsteps=0, unsigned char* initialConstraints=NULL);
};

#endif
