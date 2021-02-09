/*
figure9.cpp
from the article: " Exact Medial Axis With Euclidean Distance"
  algorithm figure 9

  Algoritm to compute the table and store it in the file table.txt

  parameter to adjust the table MAXLUTENTRY,Rtarget1 and L1  

  Note: the Columns are adjusted to put the right DT in face of the correct row of the table
  
  Written by: Rita Zrour
  created: 18/04/2004
  last modified: 29/04/2004
  

*/


#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream.h>
#include <fstream.h>


typedef long int * TabCTg;
typedef long int * TabDTg;
typedef int * Tabuff;
typedef long int * LookUpTable;


struct Weighting{
    int x, y, w,RR;
};

#define MAXWEIGHTING 100	//determine the numbers of vg (neighbors) that must be entered

struct MaskG {
    Weighting vg[MAXWEIGHTING];
    int ng;
};


/* Functions definitions*/
void ComputeCTg (int L, TabCTg & CTg); 
int AddWeighting (MaskG *M, int x, int y, int w);
void ComputeLutCol (long int * CTg, int L, Weighting *vg, int ivg, int Rmaxx, LookUpTable & Lutnew,long int * CTgNew);
void CompLutMask (long int * CTg,long int * DTg, int L, MaskG * MgL, LookUpTable & LutColumn,int Rknown, int Rtarget,long int * CTgNeww);
int IsMAg (int x, int y, MaskG *MgL, long int * LutColumn, long int * DTg, int L,int radiusmax);
void CompSEDT(TabDTg DTg1,int L,int M);
int RadiusMax(long int * gg,int LL,int MM);
int min(int x,int y);
void PrintLut (FILE *f, MaskG *MgL, LookUpTable & Lut,int L, int Rknown);
void PrintMgLut (FILE *f, MaskG *M);
void adjustCTg (int L, TabCTg & CTgg,TabCTg & CTgNew,int Rmaxx);


/* Main Function*/
int main()
{
	int L1=100;// here L1 is the length of the image
	int Rknown1=0;
	int Rtarget1=1370;
	TabCTg CTg1 = new long int[L1*L1];
	TabDTg DTg1 = new long int[L1*L1];
	LookUpTable LutColumn1=new long int[Rtarget1*MAXWEIGHTING];
	MaskG MgL1;
	MgL1.ng = 0;
    TabCTg CTgNew1 = new long int[Rtarget1];
	
	FILE *file1;
	
	ofstream myFile("table.txt"); 
     if (! myFile) // Always test file open
    {
        cout << "Error opening output file" << endl;
        return -1;
    }

    myFile.close();

    

	CompLutMask (CTg1, DTg1, L1, &MgL1, LutColumn1, Rknown1, Rtarget1,CTgNew1);
    Rknown1 = Rtarget1;

	file1 = fopen ("table.txt", "w");
    if (file1 == NULL) 
		printf ("error while opening file\n\n");
    else 
	{
        PrintMgLut (file1, &MgL1);
       // PrintLut (file1, &MgL1, LutColumn1,L1, Rknown1);
        printf ("\n\n");
        fclose (file1);
	}
	

return 0;
}	// end of main


	

/*	Function that compute CTgs*/
void ComputeCTg (int L, TabCTg & CTg) 
{
	int x1,x2;
    for (x1 = 0; x1 < L; x1++)
	{
		for (x2 = 0; x2 <= x1; x2++)
		{
			CTg[ x2*L + x1 ] =pow(x1,2)+pow(x2,2);
		}
	}
}


/*
 * Add a weigthing in the mask M.
 * Input:  M the generator of the mask, (x,y,w) the weighting.
 * Output: the number of the weighting in the mask.
*/

int AddWeighting (MaskG *M, int x, int y, int w,int RR)
{
    int i = M->ng;

    if (i >= MAXWEIGHTING) 
	{
        printf ("AddWeighting: weighting number limited to MAXWEIGHTING = %d\n",MAXWEIGHTING);
        return 0;
    }
    if (! (0 <= y && y <= x && 0 < x && 0 < w)) 
	{
        printf ("AddWeighting: (x = %d, y = %d, w = %d)\n",x, y, w);
        printf ("  does not respect 0 <= y <= x, 0 < x, 0 < w\n");
        return 0;
    }
    M->vg[i].x = x;
    M->vg[i].y = y;
    M->vg[i].w = w;
	M->vg[i].RR = RR;
    M->ng++;
    return i;
}


/*	Function that compute one Column of lut*/
void ComputeLutCol (long int * CTg, int L, Weighting *vg, int ivg, int Rmaxx, LookUpTable & Lutnew,long int * CTgNew)
{
	int x, y, r1, r2, ra, rb,indice;
	int count=0;		//this counter is used to put correct CTg in rox of the column
	LookUpTable Lut=new long int[Rmaxx];

	//Initializes Lut[ivg] and lut column to 0
    for (int r = 0; r < Rmaxx; r++)   
	{
		Lut[r] = 0;
		
	}

	for (x = 0; x < L - vg->x; x++)
	{
		for (y = 0; y <= x; y++)
		{
			//Radius of the ball where p1 is located 
			r1 = CTg[ y*L + x ]+1 ;
			// Same for p2 
			r2 = CTg[ (y+vg->y)*L + x+vg->x ] +1;
			if ((r1 < Rmaxx) && (r2 > Lut[r1])) 
				{
				Lut[r1] = r2;
				}
		}
	}
	
	rb = 0;
    for (ra = 0; ra < Rmaxx; ra++)
    {
		if (Lut[ra]!=0)
		{
        if (Lut[ra] > rb)
             rb = Lut[ra];
        else 
		Lut[ra] = rb;
		}
	}


	for (r = 0; r < Rmaxx; r++)    
	{
		Lutnew[ivg*Rmaxx+r] = 0;
	}

	for (r = 0; r < Rmaxx; r++)
	{
		if (Lut[r]!=0)
		{
			count=count+1;
			indice=CTgNew[count];
			Lutnew[ivg*Rmaxx+indice]=Lut[r];
		}
	}
}



/*
 * Full Lut Computation Algorithm with determination of MgLut.
 *
 * Input:  CTg and DTg two images, L the side length
 *			MgL the generator of the Lut mask, LutColumn the look-up
 *         table, Rknown the last verified radius, Rtarget the maximum radius
 *         to be verified and CTgNew used to put the values of the lut column in the correct
 place to be read using distance transform Image
 * Output: MgL and LutColumn are filled with the correct values.
 *
 * To compute MgL and Lut from beginning to the maximum radius allowed with L:
 */


void CompLutMask (long int * CTg,long int * DTg, int L, MaskG * MgL, LookUpTable & LutColumn,int Rknown, int Rtarget,long int * CTgNeww)
{
    int x, y, p, R, i;

    ComputeCTg (L, CTg);
	adjustCTg (L, CTg,CTgNeww,Rtarget);

    for (R = Rknown+1; R < Rtarget; R++)
    {
        printf ("R = %d / %d\n", R, Rtarget);

        
        for (x = 0; x < L; x++)
        for (y = 0; y <= x; y++)
        {
            p = y*L + x;
            if (CTg[p] <= R) 
				DTg[p] = 1; 
			else 
				DTg[p] = 0;
        }
		
		// distance map
        CompSEDT (DTg,L,L);

        for (x = 1; x < L; x++)
        for (y = 0; y <= x; y++)
        {
            p = y*L + x;
            if ( DTg[p] > 0 && IsMAg (x, y, MgL, LutColumn, DTg, L,Rtarget) )
            {
                printf ("(%2d, %2d, %3d) added for R = %d\n",x, y, CTg[p], R);

                /* Add a new weighting to MgL */
                i = AddWeighting (MgL, x, y, CTg[p],R);

                /* New column in Lut */
                ComputeLutCol (CTg, L, MgL->vg+i, i, Rtarget, LutColumn,CTgNeww);
				
                if (IsMAg (x, y, MgL, LutColumn, DTg, L,Rtarget))
                {
                    printf ("\nComputeAndVerifyLut: ERROR for R = %d\n", R);
                    return;
                }
            }
        }
    }
}


/*This functions require DTg and LutColumn values; x et y are the coordinate of p*/

int IsMAg (int x, int y, MaskG *MgL, long int * Lut, long int * DTg, int L,int radiusmax)
{
    int xx, yy, val, i;
	val = DTg[ y*L + x ];

    for (i = 0; i < MgL->ng ; i++)
    {
        xx = x-MgL->vg[i].x;
        yy = y-MgL->vg[i].y;

        if (0 <= yy && yy <= xx)
        {
            if ( DTg[ yy*L + xx ] >= Lut[i*radiusmax+val] )
                return 0;
        }
    }

    return 1;
}


/* Function that find the minimum*/
int min(int x,int y)
{
if (x<y)
return x;
else 
return y;
}


/*Function that returns the maximum radius in the distance image*/
int RadiusMax(long int * gg,int LL,int MM)  //LL=width of image  MM=height of image
{
	int i,j;
	int max=0;
	for (j=0;j<MM;j++)
	{
		for (i=0;i<LL;i++)
		{
		if (gg[j*LL+i]>max)
		max=gg[j*LL+i];
		}
	}
	return max;
}



/*Function for the distance map*/
void CompSEDT(TabDTg DTg1,int L,int M)
{
	int i,j,df,db,n;
	Tabuff buff = new int[L];
	int w,d,rMax,rStart,rEnd;

	//Forward scan
	for (j=0;j<M;j++)
	{
	df=L;
		for (i=0;i<L;i++)
		{
			if (DTg1[j*L+i]!=0)
			df=df+1;
			else 
			df=0;
		DTg1[j*L+i]=df*df;
		}
	}


	//Backward scan
	for (j=0;j<M;j++)
	{
	db=L;
		for (i=L-1;i>=0;i--)
		{
			if (DTg1[j*L+i]!=0)
			db=db+1;
			else 
			db=0;
		DTg1[j*L+i]=min(DTg1[j*L+i],db*db);

		}
	}

	//step 2
	for (i=0;i<L;i++)
	{
		for (j=0;j<M;j++)
		{
		buff[j]=DTg1[i+j*L];
		}
	
		for (j=0;j<M;j++)
		{
			d=buff[j];
			if(d!=0)
			{
			rMax=int(sqrt(d))+1;
			rStart=min(rMax,j);
			rEnd=min(rMax,(M-1-j));
			
				for (n=-rStart;n<=rEnd;n++)
				{
				w=buff[j+n]+n*n;
				if (w<d)
				d=w;
				}
			}
		DTg1[i+j*L]=d;
		}
	}
	free(buff);
}


/*Print a mask M to a file.
 * Input:  f a file, M the generator of a mask.
 * Output: to file f*/
void PrintMgLut (FILE *f, MaskG *M)
{
    int i;
	fprintf (f, "Weighting MgL1[%d]= ",M->ng);
	fprintf (f, "{\n");
    for (i = 0; i < M->ng; i++)
        fprintf (f, "{%d,%d,%d},\n",M->vg[i].x, M->vg[i].y, M->vg[i].RR);
    fprintf (f, "\n");
	fprintf (f, "};\n");
}



/*Print the LUT to a file*/
void PrintLut (FILE *f, MaskG *MgL, LookUpTable & Lut,int L, int Rknown)
{
    int i,ra;
	fprintf (f, "int LutColumn[%d][%d]={\n",Rknown,MgL->ng);
	// Loop to print lut
	for (i = 0; i < MgL->ng; i++)
	{
		for (ra = 0; ra <Rknown; ra++)
		fprintf(f,"%5d\n",Lut[i*Rknown+ra]);
		fprintf (f, "\n\n\n");
	}
	fprintf (f, "};\n");

}



/* Function that puts CTgs values in an increasing order letting each CTg appear just once */
void adjustCTg (int LL, TabCTg & CTgg,TabCTg & CTgNew,int Rmaxx)
{
	int x1,x2,i,a;
	int count =-1;
	TabCTg Compare = new long int[Rmaxx];

	for (i = 0; i <= Rmaxx; i++)
	{
	Compare[i]=i;
	}
	
    for (i = 0; i <= Rmaxx; i++)
	{
		for (x1 = 0; x1 < LL; x1++)
		{
			for (x2 = 0; x2 <= x1; x2++)
			{
				if (CTgg[ x2*LL + x1 ] ==Compare[i] )
				{
					count=count+1;
					CTgNew[count]=Compare[i];
					goto stoploops;
				}
			}
		
		}
	stoploops:
	a=1;
	}

}

