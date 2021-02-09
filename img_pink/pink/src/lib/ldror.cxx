#  include <stdio.h>
#  include <stdint.h>
#  include <sys/stat.h>
#  include <fcntl.h>
#  include <stdlib.h>
#  include <math.h>
#  include <vector>
#  include <iostream>

#  include "ldror_char.hxx"
#  include "ldror.hxx"
#  include "lfindline.hxx"
#  include "lfindline_char.hxx"



using std::cout;
using std::endl;



/* =============================================================== */
void divide_image_short(uint16_t *I, uint16_t *I_high_left, uint16_t *I_high_right, uint16_t *I_low_left, uint16_t *I_low_right, int colsize, int rowsize)
/* =============================================================== */
{
    int x, y, a=0, b=0, c=0, d=0;

    for (y=0; y<(colsize/2); y++)
    {
        for (x=0 ; x<(rowsize/2); x++,a++)
        {
            I_high_left[a]=I[y*rowsize+x];
        }
        for ( /* x = rowsize/2 */ ; x<rowsize; x++, b++)
        {
            I_high_right[b]=I[y*rowsize+x];
        }
    }

    for (y=colsize/2; y<colsize; y++)
    {
        for (x=0; x<(rowsize/2); x++,c++)
        {
            I_low_left[c]=I[y*rowsize+x];
        }
        for ( /* x = rowsize/2 */ ; x<rowsize; x++,d++)
        {
            I_low_right[d]=I[y*rowsize+x];
        }
    }

}



/* =============================================================== */
void get_line_short(uint16_t *I,tree *T, vector<int32_t> Bord2, vector<int32_t> Bord3, vector<int32_t> Bord1,
               vector<int32_t> Bord4,int rowsize, int colsize, int w)
/* =============================================================== */
{
    int i, j;
    int32_t y1, x1=0;
    int32_t best_y1=0, best_x2=0, best_y2=0;
    int32_t nb1=0,nb2=0,n;
    int32_t nb_max=0;




     for (i=0; i<Bord2.size(); i++)
    {
        y1=Bord2[i];

//        for (j=0; j<Bord1.size(); j++) //line between bord2 and bord1
//        {
//            nb=bresen(I,x1,y1,Bord1[j], 0, rowsize, w);
//            if (nb>nb_max)
//                {
//                    nb_max=nb;
//                    best_y1=y1;
//                    best_x2=Bord1[j];
//                    best_y2=rowsize;
//                }
//        }


        for (j=0; j<Bord3.size(); j++) //line between bord2 and bord3
        {

             nb1=count23(T,y1-w,Bord3[j]-w,colsize,rowsize);
             nb2=count23(T,y1+w,Bord3[j]+w,colsize,rowsize);
             if (abs(nb1-nb2)> nb_max)
                    {
                        nb_max=abs(nb1-nb2);
                        best_y1=y1;
                        best_x2=rowsize;
                        best_y2= Bord3[j];
                    }
        }

//        for (j=1; j<=Bord4.size(); j++) //line between bord2 and bord4
//        {
//            nb=bresen(I,x1,y1,Bord4[j], rowsize, rowsize, w);
//            if (nb>nb_max)
//                    {
//                        nb_max=nb;
//                        best_y1=y1;
//                        best_x2=Bord4[j];
//                        best_y2=rowsize;
//                    }
//        }
    }


    // Draw the bresenham best line in the image
    cout << "ligne entre le point (0,"<< best_y1<< ") et le point ("<< best_x2<<"," << best_y2 << ")"<< endl;
    //bresen_final(I, x1, best_y1, best_x2, best_y2, rowsize,colsize, w);
    mask_line(I, x1, best_y1, best_x2, best_y2, rowsize,colsize, w);

}



