#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string>

#include "DirectionalSkeletonizer.hpp"
#include "FIFO.hpp"


#define D6I_ISTHMUS_FLAG 64
#define D6I_ISTHMUS1D_FLAG 64
#define D6I_ISTHMUS2D_FLAG 128
#define D6I_CONSTRAINT 100
#define D6I_INTREATMENT 200
#define D6I_OBJECT 255


unsigned char* loadLUTDir(std::string name, unsigned int size, unsigned int dataSize){
	FILE* file = fopen(name.c_str(),"rb");
	unsigned char* lut = new unsigned char[size*dataSize];
	if(fread((void*)lut, dataSize, size, file)!=size){
		std::cout<<name<<" is uncomplete\n";
		exit(0);	
	}
	fclose(file);
	return lut;
}


DirectionalSkeletonizer::DirectionalSkeletonizer(unsigned int maxWidth, unsigned int maxHeight, unsigned int maxDepth){
	//LUT loading
	unsigned int lutSize = 1<<26;
	char filename[1024];
	sprintf(filename, "%s/src/tables/palaD6I.lut", getenv("PINK"));
    	
	lut = loadLUTDir(filename, lutSize, 1);
	
	
	//neighborhood relative positions computation
        unsigned int tmpopp[26] =  { 1,  0,  3,  2,  5,  4, 11, 10, 13, 12,  7,  6,  9,  8, 17, 16, 15, 14, 25, 24, 23, 22, 21, 20, 19, 18 };
	memcpy(oppositeNb, tmpopp, 26*sizeof(unsigned int));
	
	//bit index computation
	unsigned int index = 1;
	for(unsigned int i = 0; i < 26; i++){
		bitIndex[i] = index;
		index<<=1;
	}
	for(unsigned int i = 0; i < 26; i++)
		bitIndexOp[i] = bitIndex[oppositeNb[i]];


	gridConfig = NULL;
	constraints = NULL;
	setDimensions(maxWidth, maxHeight, maxDepth);
}

void DirectionalSkeletonizer::setDimensions(unsigned int w, unsigned int h, unsigned int d){
	width = w;
	height = h;
	depth = d;
	size = width*height*depth;
	if(gridConfig != NULL)delete[] gridConfig;
	if(constraints != NULL)delete[] constraints;
	gridConfig = new unsigned int[size];
	constraints = new unsigned char[size];
	
	int decX[26]={ 0,  0,  0,  0, -1,  1,  0,  0, -1,  1,  0,  0, -1,  1, -1, -1,  1,  1, -1, -1,  1,  1, -1, -1,  1,  1};
	int decY[26]={ 0,  0, -1,  1,  0,  0, -1,  1,  0,  0, -1,  1,  0,  0, -1,  1, -1,  1, -1,  1, -1,  1, -1,  1, -1,  1};
	int decZ[26]={-1,  1,  0,  0,  0,  0, -1, -1, -1, -1,  1,  1,  1,  1,  0,  0,  0,  0, -1, -1, -1, -1,  1,  1,  1,  1};
	for(unsigned int i = 0; i < 26; i++)
		nbCoordinates[i] = decX[i]+(decY[i]+decZ[i]*height)*width;
}


DirectionalSkeletonizer::~DirectionalSkeletonizer(){
	delete[] lut;
	delete[] gridConfig;
	delete[] constraints;

}






unsigned int DirectionalSkeletonizer::skeletonize(unsigned char* grid, bool curvilinear, unsigned int nbSteps, unsigned char* initialConstraints){
	if(nbSteps == 0) nbSteps = 100000;
	int i, n;
	unsigned int p, res, configNS;
	unsigned int* config;
	unsigned char spec;

	
	unsigned char borderMask = 63;
	unsigned char isthmusMask = 0;	
	if(curvilinear){
			isthmusMask = D6I_ISTHMUS1D_FLAG;
	}else{
			isthmusMask = D6I_ISTHMUS2D_FLAG;
	}
	borderMask|=isthmusMask;


	///////////////////
	// INITIALIZATION
	
	//initialization of constraints
	if(initialConstraints!=NULL)
		std::copy(initialConstraints, initialConstraints+size, constraints);  
	else 
		std::fill(constraints, constraints+size, 0);
	
	
	FIFO<unsigned int> removable(size);//potentially removable for the current iteration
	FIFO<unsigned int> borderNextIteration(size);
	FIFO<unsigned int> borders(size);
	unsigned int nb;
	int vois;

	std::fill(gridConfig, gridConfig+size, 0);
	for(i = 0; i < size; ++i)
		if(grid[i]!=0){
			config = gridConfig+i;
			for(n = 0; n < 26; n++)
				config[nbCoordinates[n]]|=bitIndexOp[n];
			borders.push(i);
		}


	for(i = borders.size(); i > 0; i--){
		p = borders.pop();
		if(grid[p] && !constraints[p]){
			spec = lut[gridConfig[p]];
			if(spec&borderMask){
				if(spec&isthmusMask){
					constraints[p] = 255;
				}else
					borders.push(p);
				grid[p]=D6I_INTREATMENT;
			}else
				grid[p]=D6I_OBJECT;
		}
	}

 
	///////////////////
	// ITERATIONS
	bool stop = false;
	unsigned int iteration = 0;
	while(iteration++ < nbSteps && !stop){
		stop = true;
		
		for(int sub = 0; sub < 6; sub++){
			unsigned char dirMask = 1<<sub;
			for(i = borders.size(); i>0; i--){
				p = borders.pop();
				nb = gridConfig[p];
				if((lut[nb] & dirMask)) removable.push(p);
				else borders.push(p);
			}
			if(!removable.empty())stop = false;
			//we treat all the points which will be removed
			for(i = removable.size(); i>0; i--){
				p = removable.pop();
				grid[p] = 0;//we remove the point
				config = gridConfig+p;
				for(n = 0; n < 6; n++){
					//we add neighbors to border list
					vois = ((int)p)+nbCoordinates[n];
					if(grid[vois]==D6I_OBJECT){
						grid[vois] = D6I_INTREATMENT;
						borderNextIteration.push(vois);
					}
					//for each neighbor of the point
					gridConfig[vois]&=~(bitIndexOp[n]);//we update the configuration
				}
				for(n = 6; n < 26; n++){
					//for each neighbor of the point
					config[nbCoordinates[n]]&=~(bitIndexOp[n]);//we update the configuration
				}
			}
		}
		for(i = borders.size(); i>0; i--){
			p = borders.pop();
			borderNextIteration.push(p);
		}
		for(i = borderNextIteration.size(); i>0; i--){
			p = borderNextIteration.pop();
			if(grid[p] && !constraints[p]){
			spec = lut[gridConfig[p]];
				if(spec&borderMask){
					if(spec&isthmusMask)
						constraints[p] = 255;
					else
						borders.push(p);
				}else
					borderNextIteration.push(p);
			}
		}
	}
	return std::min(iteration, nbSteps);
}
