/*
Table for the bisector function it writes in a file r^2 y and x
printed and then read from a file (L, Rmax, and the number 27714 are changed 
when making bigger table)
*/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream.h>
#include <fstream.h>

typedef int * TabCTg;


void ComputeCTg (int L, TabCTg & CTg);


int main()
{
	int x,y, L1 = 540;
	FILE *filer;

	int Rmax=70000,a;
	int count=0;
	TabCTg CTg1 = new int[L1*L1];
	ComputeCTg(L1,CTg1);


	ofstream myFile("BigTableCTg.txt");
     if (! myFile) // Always test file open
    {
        cout << "Error opening output file" << endl;
        return -1;
    }

    myFile.close();


	filer = fopen ("BigTableCTg.txt", "w");
    if (filer == NULL)
		printf ("error while opening file\n\n");
    else
	{
	fprintf (filer,"27714\n\n");
		for (x=0;x<L1;x++)
		{
			for (y = 0; y <=x; y++)
				if (CTg1[x+y*L1]<=Rmax)
				{
				count=count+1;
				a=CTg1[x+y*L1];//+1
				fprintf(filer,"%d\t%d\t%d\t\n",a,y,x);
				}
		}
	fclose (filer);
	}

	printf("nb of CTg printed=%d\n",count);//this number must be put in the 
begining of the file BigTableCTg

return 0;
}



void ComputeCTg (int L, TabCTg & CTg)
{
	int x1,x2;

    for (x1 = 0; x1 < L; x1++)
	{
		for (x2 = 0; x2 <=x1 ; x2++)
		{
			CTg[ x2*L + x1 ] = pow(x1,2)+pow(x2,2);
		}
	}
}
