#  include <stdio.h>
#  include <stdint.h>
#  include <sys/stat.h>
#  include <fcntl.h>
#  include <stdlib.h>
#  include <math.h>
#  include <vector>
#  include <iostream>


#  include "lfindline_char.hxx"

using std::cout;
using std::endl;

/* =============================================================== */
    int32_t mean2_char(uint8_t *Image, int colsize, int rowsize)
/* =============================================================== */
 {
     // calcul la moyenne euclidienne de l'image
     int i, l;
     int32_t p=0, n;
     l=  colsize*rowsize;
     //l=4904951;

     for (i=0 ; i<l; i++)
     {
           p+=Image[i];
     }

     p=p/l;
     return(p);
 }



///* =============================================================== */
//    int new_edge1(uint16_t *Image, int colsize, int rowsize)
///* =============================================================== */
//{
//    int i, l, n=colsize;
//    int b1=0, b2=0, b3=0, b4=0;
//    l=  colsize*rowsize;
//
////    for (i=rowsize/2 ; i<l; i=i+rowsize, b1++)
////    {
////       if (Image[i]>0)
////       {
////           break;
////       }
////    }
//
//
//    for (i=l-rowsize/2 ; i>0; i-=rowsize, b4++)
//    {
//       if (Image[i]>0)
//       {
//           break;
//       }
//    }
//
//    n=b4-b1;
//    return(n);
//
//}
//
//
///* =============================================================== */
//    int new_edge2(uint16_t *Image, int colsize, int rowsize)
///* =============================================================== */
//{
//    int i, l, n=rowsize;
//    int b1=0, b2=0, b3=0, b4=0;
//    l=  colsize*rowsize;
//
//
//    for (i=(colsize/2)*rowsize ; i<l; i++, b2++)
//    {
//       if (Image[i]>0)
//       {
//           break;
//       }
//    }
//
//    for (i=(colsize/2)*rowsize ; i>0; i--, b2++)
//    {
//       if (Image[i]>0)
//       {
//           break;
//       }
//    }
//
//    n=b3-b2;
//    return(n);
//
//}




/* =============================================================== */
    void clean1_char(uint8_t *Image, int colsize, int rowsize)
/* =============================================================== */
 {
     // Fonction qui soustrait la moyenne de l'image à chaque pixel.
     //Si la valeur du pixel moins la moyenne est inferieur à 0, on met le pixel à zero
      int i;
      size_t l;
      int32_t p;
      //p=19;
      p=mean2_char(Image, colsize, rowsize);
      l=colsize*rowsize;

        for (i=0 ; i<l; i++)
        {
            if (Image[i]-p>=0)
            {
                Image[i]=Image[i]-p;
            }
            else
            {
                Image[i]=0;
            }
        }


 }


 /* =============================================================== */
    void clean2_char(uint8_t *Image, int colsize, int rowsize)
 /* =============================================================== */
{
    // Fonction qui met à zero les pixels entourés de plus de 4 pixels nuls.
    int i,j, l, p, q;
    int m, n;
    l=colsize*rowsize;
    m=colsize;
    n=rowsize;

    for (i=m+1 ; i<l-m-1;i++)
       {
            if (Image[i]>0)
            {
                p=(Image[i-m-1]==0)+(Image[i-m]==0)+(Image[i-m+1]==0)+(Image[i-1]==0)
                    +(Image[i+1]==0)+(Image[i+m-1]==0)+(Image[i+m]==0)+(Image[i+m+1]==0);
                if (p>=4)
                {
                    Image[i]=0;
                }
            }
        }
}


/* =============================================================== */
  void  clean4_char(uint8_t *Image, int colsize, int rowsize)
 /* =============================================================== */
 {
  int i, j;
  size_t l;
  int32_t m, y;


  for (i=0 ; i<colsize-1; i++)
  {
      for (j=0; j<rowsize-1; j++)
      {
          y= i*rowsize+j;
          if ( (y<colsize*rowsize) && Image[y]>0)
          {
//              m=Image[y-rowsize-1]+Image[y-rowsize]+Image[y-rowsize+1]+Image[y]+Image[y+1]+Image[y-1]
//                +Image[y+rowsize-1]+Image[y+rowsize]+Image[y+rowsize+1];
              m=Image[y]+Image[y+1]+Image[y+rowsize]+Image[y+rowsize+1];
              m=m/9;
              Image[y]=m;
          }
          else
          {
              Image[y]=0;
          }

       }
  }


 }

 /* ============================================================================================== */
void get_edge_char(uint8_t *Image, vector<int32_t> *Bord1, vector<int32_t> *Bord2, vector<int32_t> *Bord3,
                vector<int32_t> *Bord4,int rowsize, int colsize)
  /* =========================================================================================== */
{
    // Cette fonction repere les pixels non nul sur les bords de l'image. Ces bords vont servir afin de reperer les lignes de l'image
    int a, b, c, d;
    int m, n;
    n=rowsize;
    m=colsize;
    int32_t q, r ,s, p;

//    // we look for a lit pixel on the first line
//    for (a=0, s=0; a<n; a++,s++)
//    {
//        if ( (Image[a]>0) && (Image[a-1]>0) && (Image[a+1]>0) && (Image[a-2]>0) && (Image[a+2]>0))
//        //if (Image[a]>0)
//        {
//            cout << "point sur la  premiere ligne "<< a << endl;
//            Bord1->push_back(s);
//        }
//    }

    // we look for a lit pixel on the last line
//    for (b=(m*n)-1,p=n-1; b>(m-1)*n;b--,p--)
//     {
//         if ( (Image[b]>0) && (Image[b-1]>0) && (Image[b+1]>0) && (Image[b-2]>0) && (Image[b+2]>0))
//         //if (Image[b]>0)
//         {
//             cout << "point sur la  derniere ligne "<< p << endl;
//             Bord4->push_back(p);
//         }
//    }

    // we look for a lit pixel on the first column
    for (c=0, q=0; c<m*n; c=c+n,q++)
     {
         if ( (Image[c]>0) /*&& (Image[c-n]>0) && (Image[c+n]>0) && (Image[c-2*n]>0) && (Image[c+2*n]>0)*/)
         //if (Image[c]>0)
         {
             cout << "point sur la  premiere colonne "<< q << endl;
             Bord2->push_back(q);
         }
     }

    // we look for a lit pixel on the last colum
    for (d=n-1, r=0; d<n*m; d=d+n,r++)
     {
         if ( (Image[d]>0) /*&& (Image[d-n]>0) && (Image[d+n]>0) && (Image[d-2*n]>0) && (Image[d+2*n]>0)*/ )
         //if (Image[d]>0)
         {
             cout << "point sur la derniere colonne "<< r << endl;
             Bord3->push_back(r);
         }
     }

//  if ((Bord1[0] == NULL) || (Bord2[0] == NULL) || (Bord3[0]== NULL) || (Bord4[0] == NULL))
//    {
//        fprintf(stderr, "probleme au niveau des bords\n");
//        exit(1);
//    }

}



/* =============================================================== */
void get_best_line_char(uint8_t *I, vector<int32_t> Bord2, vector<int32_t> Bord3, vector<int32_t> Bord1,
               vector<int32_t> Bord4,int rowsize, int colsize, int w)
/* =============================================================== */
{
    // On test les lignes entres les points non nul de la 1ere colonne et ceux des autres bords de l'image.
    // On calcule pour chacunes des lignes le nombres de points non nul sur la droites.
    // On garde la droite qui contient le plus de points non nul.

    int i, j;
    int32_t y1, x1=0;;
    int32_t best_y1=0, best_x2=0, best_y2=0;
    double nb=0;
    double nb_max=0;



     for (i=0; i<Bord2.size(); i++)
    {
        y1=Bord2[i];

//        for (j=0; j<Bord1.size(); j++) //line between bord2 and bord1
//        {
//            nb=bresen_test_char(I,x1,y1,Bord1[j], 0, rowsize, colsize,w);
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
             nb=bresen_test_char(I,x1,y1,rowsize, Bord3[j], rowsize, colsize,w);
             if (nb> nb_max)
                    {
                        nb_max=nb;
                        best_y1=y1;
                        best_x2=rowsize;
                        best_y2= Bord3[j];
                    }
        }

//        for (j=1; j<=Bord4.size(); j++) //line between bord2 and bord4
//        {
//            nb=bresen_test_char(I,x1,y1,Bord4[j], rowsize, rowsize,colsize, w);
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
     bresen_final_char(I, x1, best_y1, best_x2, best_y2, rowsize, w);

}

/* ============================================================================================== */
int32_t query_char(uint8_t *Image,int32_t x,int32_t y,int w,int rowsize, int colsize)
/* ============================================================================================== */
{
    // Cette fonction est utilisée dans la fonction bresen_test_char.
    // Elle permet de compter le nombre de points non nuls sur la droite digital de largueur w.

    int i;
    int32_t nb=0;

    for (i=-w; i<=w; i++)
    {
       if ((y+i)*rowsize+x<rowsize*colsize)
        {
            if (Image[(y+i)*rowsize+x]>0)
            {
                nb=nb+1;
            }
       }
    }

    return(nb);
}




/* ============================================================================================== */
double bresen_test_char(uint8_t *Image,int32_t x1,int32_t y1,int32_t x2,int32_t y2,int rowsize, int colsize, int w)
/* ============================================================================================== */
{
/**
ARGUMENTS:
	int x1:      x starting coordinate
	int y1:      y starting corrdinate
	int x1:      x last coordinate
	int y1:      y last corrdinate
	int colsize:  width of the image
	int rowsize: height of the image

DISCRIPTION:
        This function makes a Bresenham line between two point.

HISTORY:
        Written by Hugues Talbot/Juliette Charpentier	31 mars 2011
TESTS:
        The core of this program has been written in 1991.
        Many tests including purify.
**/

    int32_t       dx, dy;
    int32_t       i, e, nb=0;
    double        dist=0;
    int32_t       incx, incy, inc1, inc2;
    int32_t       x, y;  /* the actual positions */

    dx = abs(x1 - x2);
    dy = abs(y1 - y2);
    dist= sqrt(double(dx*dx+dy*dy));

    incx = 1;
    if (x2 < x1) incx = -1;
    incy = 1;
    if (y2 < y1) incy = -1;

    /* starting position */
    if(x1 > x2)
      {
            x = x2;
            y = y2;
      }
    else
      {
            x = x1;
            y = y1;
      }




    if (dx > dy)
    {
        e = 2*dy -dx;
        inc1 = 2*(dy-dx);
        inc2 = 2*dy;
        for (i = 0 ; i < dx ; i++) {
            if (e >= 0) {
                nb+= query_char(Image,x,y,w,rowsize,colsize);
                y += incy;
                e += inc1;
            }
            else
            {
                nb+= query_char(Image,x,y,w,rowsize,colsize);
                e += inc2;

            }
            x += incx;

        }
    }
    else
    {
        e = 2*dx-dy ;
        inc1 = 2*(dx-dy);
        inc2 = 2*dx;
        for (i = 0 ; i < dy ; i++)
        {
            if (e >= 0)
            {
                nb+= query_char(Image,x,y,w,rowsize,colsize);
                x += incx;
                e += inc1;
            }
            else
            {
                nb+= query_char(Image,x,y,w,rowsize,colsize);
                e += inc2;
            }
                y += incy;

        }

	}

    return((double)nb/dist);

    }



/* ============================================================================================== */
void black_line_char(uint8_t *Image,int32_t x,int32_t y,int w,int rowsize)
/* ============================================================================================== */
{
    // cette fonction met les points de la droite digitale de largueur w à zéros
    int i;

    for (i=-w; i<=w; i++)
    {
        // (y*rowsize+x+i)<sizeof(Image)
        if  ((x+i)<rowsize)
        {
            Image[(y+i)*rowsize+x]=0;
        }
    }

}






/* ============================================================================================== */
void bresen_final_char(uint8_t *Image,int32_t x1,int32_t y1,int32_t x2,int32_t y2,int rowsize, int w)
/* ============================================================================================== */
{
/**
ARGUMENTS:
	int x1:      x starting coordinate
	int y1:      y starting corrdinate
	int x1:      x last coordinate
	int y1:      y last corrdinate
	int colsize:  width of the image
	int rowsize: height of the image

DISCRIPTION:
        This function makes a Bresenham line between two point.

HISTORY:
        Written by Hugues Talbot/Juliette Charpentier	31 mars 2011
TESTS:
        The core of this program has been written in 1991.
        Many tests including purify.
**/

    int32_t       dx, dy;
    int32_t       i, e, nb=0;
    int32_t       incx, incy, inc1, inc2;
    int32_t       x, y;  /* the actual positions */

    dx = abs(x1 - x2);
    dy = abs(y1 - y2);

    incx = 1;
    if (x2 < x1) incx = -1;
    incy = 1;
    if (y2 < y1) incy = -1;

    /* starting position */
    if(x1 > x2)
      {
            x = x2;
            y = y2;
      }
    else
      {
            x = x1;
            y = y1;
      }




    if (dx > dy)
    {
        e = 2*dy ;
        inc1 = 2*(dy-dx);
        inc2 = 2*dy;
        for (i = 0 ; i < dx ; i++) {
            if (e >= 0)
            {
                black_line_char(Image,x,y,w,rowsize);
                y += incy;
                e += inc1;
            }
            else
            {
                black_line_char(Image,x,y,w,rowsize);
                e += inc2;
            }
            x += incx;
        }
    }
    else
    {
        e = 2*dx;
        inc1 = 2*(dx-dy);
        inc2 = 2*dx;
        for (i = 0 ; i < dy ; i++)
        {
            if (e >= 0)
            {
                black_line_char(Image,x,y,w,rowsize);
                x += incx;
                e += inc1;
            }
            else
            {
                black_line_char(Image,x,y,w,rowsize);
                e += inc2;
            }
            y += incy;
        }

	}



    }

