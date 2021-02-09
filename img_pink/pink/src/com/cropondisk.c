//Librairies standard
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


#include <mcimage.h>
#include <mccodimage.h>

#include <libcrop.h>


#define USAGE "<input_image_file> <raw|pgm> <ouput_pgm_file> x y z w h d <for raw input only: width height depth header_size type(uint_8|uint_16|uint_32|float|double)>"


int32_t main(int argc, char* argv[])
{
	FILE *f;
	uint64_t rs, cs, d, nrs, ncs, nd, cx, cy, cz, h, b, size;
	struct xvimage *output, *input_header;
	char mode, type;
	struct stat st;



	if(argc!=10 && argc!=15)
	{
		fprintf(stderr, "usage: %s %s\n", argv[0], USAGE);
		exit(1);
	}

	f=fopen(argv[1], "rb");
	if(f==NULL)
	{
		fprintf(stderr, "%s: Could not open file %s.\n", argv[0], argv[1]);
		exit(1);
	}

	if(strcmp(argv[2],"pgm")==0)
		mode=1; //pgm
	else if(strcmp(argv[2],"raw")==0)
		mode=0; //raw
	else
	{
		fprintf(stderr, "%s: Chosen mode not recognised (pgm or raw)\n", argv[0]);
		exit(1);
	}


	//Read info from input file

	if(mode==0)
	{
		if(argc!=15)
		{
			fprintf(stderr, "usage: %s %s\n", argv[0], USAGE);
			exit(1);
		}

		rs=atoi(argv[10]); cs=atoi(argv[11]); d=atoi(argv[12]);
		h=atoi(argv[13]);

		if(strcmp(argv[14],"uint_8")==0)
		{
			b=sizeof(uint8_t);
			type=VFF_TYP_1_BYTE;
		}
		else if(strcmp(argv[14],"uint_16")==0)
		{
			b=sizeof(uint16_t);
			type=VFF_TYP_2_BYTE;
		}
		else if(strcmp(argv[14],"uint_32")==0)
		{
			b=sizeof(uint32_t);
			type=VFF_TYP_4_BYTE;
		}
		else if(strcmp(argv[14],"float")==0)
		{
			b=sizeof(float);
			type=VFF_TYP_FLOAT;
		}
		else if(strcmp(argv[14],"double")==0)
		{
			b=sizeof(double);
			type=VFF_TYP_DOUBLE;
		}
		else
		{
			fprintf(stderr, "%s: Image type not implemented\n", argv[0]);
			exit(1);
		}
	}
	else
	{
		input_header=readheader(argv[1]);

		rs=rowsize(input_header);
		cs=rowsize(input_header);
		d=depth(input_header);

		switch(datatype(input_header))
		{
			case(VFF_TYP_1_BYTE):
				b=sizeof(uint8_t);
				type=VFF_TYP_1_BYTE;
				break;

			case(VFF_TYP_2_BYTE):
				b=sizeof(uint16_t);
				type=VFF_TYP_2_BYTE;
				break;

			case(VFF_TYP_4_BYTE):
				b=sizeof(uint32_t);
				type=VFF_TYP_4_BYTE;
				break;

			case(VFF_TYP_FLOAT):
				b=sizeof(float);
				type=VFF_TYP_FLOAT;
				break;

			case(VFF_TYP_DOUBLE):
				b=sizeof(double);
				type=VFF_TYP_DOUBLE;
				break;

			default:
				fprintf(stderr, "%s: Input pgm file has unsupported type\n", argv[0]);
				exit(1);
		}

		stat(argv[1], &st);
		size = st.st_size;
		h=size-(rs*cs*d)*b;
	}

	cx=atoi(argv[4]); cy=atoi(argv[5]); cz=atoi(argv[6]);
	nrs=atoi(argv[7]); ncs=atoi(argv[8]); nd=atoi(argv[9]);

	if(nrs<=0 || ncs<=0 || nd<=0 || cx<=0 || cy <=0 || cz<=0)
	{
		fprintf(stderr, "%s: Please give positive values for position and size of cropped zone\n", argv[0]);
		exit(1);
	}

	//Allocate space for ouput image
	output=allocimage(NULL, nrs, ncs, nd, type);
	if(output==NULL)
	{
		fprintf(stderr, "%s: Error, could not allocate output image into memory\n", argv[0]);
		exit(1);
	}

	if (crop_raw_on_disk(f, output, cx, cy, cz, nrs, ncs, nd, rs, cs, d, h, b, 1)==0)
	{
		fprintf(stderr, "%s: Error in crop_raw_on_disk\n", argv[0]);
		exit(1);
	}

	writeimage(output, argv[3]);

	freeimage(output);

	return(0);
}





