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


#define USAGE "<input_image_file> <raw|pgm> <ouput_pgm_file> <shrink_factor> <for raw input only: width height depth header_size type(uint_8|uint_16|uint_32|float|double)>"

#define SIDE 300

int32_t main(int argc, char* argv[])
{
	FILE *f;
	uint64_t rs, cs, d, nrs, ncs, nd, pixel, avct, h, b, i, j, k, size;
	struct xvimage *output, *crop, *input_header;
	double factor, factor_rs, factor_cs, factor_d, factor_x, factor_y, factor_z, fx1, fx2, fy1, fy2, fz1, fz2, v, somme_facteur;
	uint64_t sid, i_d, j_d, k_d, i_a, j_a, k_a, i_a_arrivee, j_a_arrivee, k_a_arrivee, sidsquare;
	char mode, type;
	struct stat st;

	sid=SIDE;
	sidsquare=sid*sid;

	if(argc!=5 && argc!=10)
	{
		fprintf(stderr, "usage: %s %s\n", argv[0], USAGE);
		exit(1);
	}

	f=fopen(argv[1], "rb");
	if(f==NULL)
	{
		fprintf(stderr, "%s: Could not open file %s\n", argv[0], argv[1]);
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
		if(argc!=10)
		{
			fprintf(stderr, "usage: %s %s\n", argv[0], USAGE);
			exit(1);
		}

		rs=atoi(argv[5]); cs=atoi(argv[6]); d=atoi(argv[7]);
		h=atoi(argv[8]);

		if(strcmp(argv[9],"uint_8")==0)
		{
			b=sizeof(uint8_t);
			type=VFF_TYP_1_BYTE;
		}
		else if(strcmp(argv[9],"uint_16")==0)
		{
			b=sizeof(uint16_t);
			type=VFF_TYP_2_BYTE;
		}
		else if(strcmp(argv[9],"uint_32")==0)
		{
			b=sizeof(uint32_t);
			type=VFF_TYP_4_BYTE;
		}
		else if(strcmp(argv[9],"float")==0)
		{
			b=sizeof(float);
			type=VFF_TYP_FLOAT;
		}
		else if(strcmp(argv[9],"double")==0)
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

		freeimage(input_header);
	}



	factor=strtod(argv[4], NULL);
	if ((factor <= 0.0) || (factor > 1.0))
	{
		fprintf(stderr, "%s: Error: shrink factor must be between 0 and 1\n", argv[0]);
		exit(1);
	}

	factor = 1.0 / factor;

	nrs=ceil((double)rs/factor); ncs=ceil((double)cs/factor); nd=ceil((double)d/factor);

	factor_rs=(double)rs/(double)nrs; factor_cs=(double)cs/(double)ncs; factor_d=(double)d/(double)nd;

	//Allocate space for ouput image
	output=allocimage(NULL, nrs, ncs, nd, type);
	if(output==NULL)
	{
		fprintf(stderr, "%s: Error, could not allocate output image into memory\n", argv[0]);
		exit(1);
	}

	crop=allocimage(NULL, sid, sid, sid, type);


	avct=nrs*ncs*nd;
	pixel=0;

	fprintf(stdout, "Processing image...     ");
	fflush(stdout);

	//Now, we finished all the input check, we are ready for the main algorithm
	i_d=0; //Index in the original image
	j_d=0; //Index in the original image
	k_d=0;
	while(1) //we will break out of the loop
	{
		fprintf(stdout, "\b\b\b\b");
		fprintf(stdout, "%3u%c", (u_int32_t)((double)pixel/avct*100.0), '%');
		fflush(stdout);

		if(crop_raw_on_disk(f, crop, i_d, j_d, k_d, sid, sid, sid, rs, cs, d, h, b, 0)==0)
		{
			fprintf(stderr, "%s: Error in crop_on_disk (%lld %lld %lld)\n", argv[0], i_d, j_d, k_d);
			exit(1);
		}


		i_a_arrivee = (((((i_d+sid-1)/factor_rs)-1) < (nrs - 1)) ? (((i_d+sid-1)/factor_rs)-1) : (nrs - 1));
		j_a_arrivee = (((((j_d+sid-1)/factor_cs)-1) < (ncs - 1)) ? (((j_d+sid-1)/factor_cs)-1) : (ncs - 1));
		k_a_arrivee = (((((k_d+sid-1)/factor_d)-1) < (nd - 1)) ? (((k_d+sid-1)/factor_d)-1) : (nd - 1));

		for(k_a=ceil(((double)k_d/factor_d)); k_a<=k_a_arrivee; k_a++)
			for(j_a=ceil(((double)j_d/factor_cs)); j_a<=j_a_arrivee; j_a++)
				for(i_a=ceil(((double)i_d/factor_rs)); i_a<=i_a_arrivee; i_a++)
				{
					v=0.0;
					somme_facteur=0.0;

					for(k=(k_a*factor_d); k<=ceil((double)(k_a+1)*factor_d)-1; k++)
					{
						fz1=fabs((double)k+1.0 - (double)k_a*factor_d);
						fz2=fabs((double)k - (double)(k_a+1)*factor_d);
						factor_z = ((fz1 < fz2) ? fz1 : fz2);
						if(factor_z > 1.0) factor_z=1.0;
						for(j=(j_a*factor_cs); j<=ceil((double)(j_a+1)*factor_cs)-1; j++)
						{
							fy1=fabs((double)j+1.0 - (double)j_a*factor_cs);
							fy2=fabs((double)j - (double)(j_a+1)*factor_cs);
							factor_y = ((fy1 < fy2) ? fy1 : fy2);
							if(factor_y > 1.0) factor_y=1.0;
							for(i=(i_a*factor_rs); i<=ceil((double)(i_a+1)*factor_rs)-1; i++)
							{
								fx1=fabs((double)i+1.0 - (double)i_a*factor_rs);
								fx2=fabs((double)i - (double)(i_a+1)*factor_rs);
								factor_x = ((fx1 < fx2) ? fx1 : fx2);
								if(factor_x > 1.0) factor_x=1.0;

								switch(type)
								{
									case(VFF_TYP_1_BYTE):
										v+=(double)UCHARDATA(crop)[(k-k_d)*sidsquare+(j-j_d)*sid+(i-i_d)]*factor_x*factor_y*factor_z;
										break;

									case(VFF_TYP_2_BYTE):
										v+=(double)USHORTDATA(crop)[(k-k_d)*sidsquare+(j-j_d)*sid+(i-i_d)]*factor_x*factor_y*factor_z;
										break;

									case(VFF_TYP_4_BYTE):
										v+=(double)ULONGDATA(crop)[(k-k_d)*sidsquare+(j-j_d)*sid+(i-i_d)]*factor_x*factor_y*factor_z;
										break;

									case(VFF_TYP_FLOAT):
										v+=(double)FLOATDATA(crop)[(k-k_d)*sidsquare+(j-j_d)*sid+(i-i_d)]*factor_x*factor_y*factor_z;
										break;

									case(VFF_TYP_DOUBLE):
										v+=(double)DOUBLEDATA(crop)[(k-k_d)*sidsquare+(j-j_d)*sid+(i-i_d)]*factor_x*factor_y*factor_z;
										break;
								}

								somme_facteur+=factor_x*factor_y*factor_z;
							}
						}
					}

					switch(type)
					{
						case(VFF_TYP_1_BYTE):
							UCHARDATA(output)[k_a*ncs*nrs+j_a*nrs+i_a]=(uint8_t)(v/somme_facteur +0.5);
							break;

						case(VFF_TYP_2_BYTE):
							USHORTDATA(output)[k_a*ncs*nrs+j_a*nrs+i_a]=(uint16_t)(v/somme_facteur +0.5);
							break;

						case(VFF_TYP_4_BYTE):
							ULONGDATA(output)[k_a*ncs*nrs+j_a*nrs+i_a]=(uint32_t)(v/somme_facteur +0.5);
							break;

						case(VFF_TYP_FLOAT):
							FLOATDATA(output)[k_a*ncs*nrs+j_a*nrs+i_a]=(float)(v/somme_facteur);
							break;

						case(VFF_TYP_DOUBLE):
							DOUBLEDATA(output)[k_a*ncs*nrs+j_a*nrs+i_a]=(v/somme_facteur);
							break;
					}
					pixel++;
				}

		if(i_a_arrivee < (nrs -1))
		{
			i_d= (i_a_arrivee +1)*factor_rs;
		}
		else
		{
			i_d=0;
			if(j_a_arrivee < (ncs -1))
			{
				j_d= (j_a_arrivee +1)*factor_cs;
			}
			else
			{
				j_d=0;
				if(k_a_arrivee < (nd -1))
				{
					k_d= (k_a_arrivee +1)*factor_d;
				}
				else
					break;
			}
		}
	}

	fprintf(stdout, "\b\b\b\bdone\n");
	fflush(stdout);

	writeimage(output, argv[3]);

	freeimage(output);

	return(0);
}





