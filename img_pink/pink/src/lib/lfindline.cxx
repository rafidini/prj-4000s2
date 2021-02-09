#  include <stdio.h>
#  include <stdint.h>
#  include <sys/stat.h>
#  include <fcntl.h>
#  include <stdlib.h>
#  include <math.h>
#  include <vector>
#  include <iostream>
#  include <fstream>
#  include <string>


#  include "mcimage.h"
#  include "mccodimage.h"
#  include "liarp.h"
#  include "liar_fseries.h"
#  include "larith.h"
#  include "lfindline.hxx"
#  include <lseuil.h>

using std::cout;
using std::endl;


/* =============================================================== */
    double mean2(uint16_t *Image, int colsize, int rowsize)
/* =============================================================== */
 {
  int i, l, q=0;
  double p=0;
  l=  colsize*rowsize;


  for (i=rowsize ; i<l; i++)
   {
       if (Image[i]>0)
       {
           p+=Image[i];
           q++;
       }

   }

   p=(double)p/q;
   cout << "moyenne de toute l'image "<< p << endl;
   return(p);
 }


/* =============================================================== */
    void control(uint16_t *Image, int colsize, int rowsize)
/* =============================================================== */
 {
  int i, l;
  double p=0;
  l=  colsize*rowsize;


  for (i=rowsize ; i<l; i++)
   {
       if ( (Image[i]>30) && (Image[i]<200) )
       {
           p+=1;
       }
   }

   p=(double)p;
   cout << p << endl;
 }


 /* =============================================================== */
    int32_t mean4(uint16_t *Image, int colsize, int rowsize)
/* =============================================================== */
 {
  int i, l, m, n;
  int64_t p=0, q=0;
  l=  colsize*rowsize-34*rowsize;
  n= rowsize;

  for (i=n+1 ; i<l; i++)
   {
       if (Image[i]>0)
       {
           m=(Image[i-n-1]!=Image[i])+(Image[i-n]!=Image[i])+(Image[i-n+1]!=Image[i])+(Image[i-1]!=Image[i])+(Image[i+1]!=Image[i])
                +(Image[i+n-1]!=Image[i])+(Image[i+n]!=Image[i])+(Image[i+n+1]!=Image[i]);
           p+=abs(Image[i])*m;
           q+=m;
       }
   }

   p=p/q;
   return(p);
 }


 /* =============================================================== */
  void  clean3(uint16_t *Image, int colsize, int rowsize)
 /* =============================================================== */
 {
  int i;
  size_t l;
  int32_t p;
  p=mean4(Image, colsize, rowsize);
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
  void  clean4(uint16_t *Image, int colsize, int rowsize)
 /* =============================================================== */
 {
  int i, j;
  size_t l;
  int32_t y, p;
  double  m;

  // causal, non recursive averaging filter.
  for (i=0 ; i<colsize-1; i++)
  {
      for (j=0; j<rowsize-1; j++)
      {
          y= i*rowsize+j;
          if ( Image[y] > 0 )
          {
//              m=Image[y-rowsize-1]+Image[y-rowsize]+Image[y-rowsize+1]+Image[y]+Image[y+1]+Image[y-1]
//                +Image[y+rowsize-1]+Image[y+rowsize]+Image[y+rowsize+1];
             // we use only pixels that have not been filtered yet.
              m=Image[y]+Image[y+1]+Image[y+rowsize]+Image[y+rowsize+1];
              m/=4.0;
              Image[y]=(int32_t)m;
          }
          else
          {
              Image[y]=0;
          }

       }
  }


 }



/* =============================================================== */
    xvimage *tophat(xvimage *original, int w)
/* =============================================================== */
 {
    /**
    ARGUMENTS:
	uint16_t *Image:      Image à étudier
	int w:                largeur de la ligne

	DESCRIPTION
	Cette fonstion utilise des opérateurs de morphologie mathématique.
	Le but est de netoyer l'image, c'est à dire, de réduire un maximum
	le bruit de fond de l'image.
	**/


    xvimage *final;
    uint16_t *Original;
    uint16_t *Final;
    int b;
    index_t i, rs, cs, N;

    imfclose_rect(original, w, w , original); // On commence par faire une fermeture pour enlever le bruit de fond

    // maintenant on fais le tophat
    final=copyimage(original);
    imfopen_rect(final, 25, 25, final);

    Original=USHORTDATA(original);
    Final=USHORTDATA(final);
    rs=rowsize(original);
    cs=colsize(original);
    N=rs*cs;


    for (i=0 ; i<N; i++)
    {
        Final[i]=Original[i]-Final[i];
    }


    //imfdilat_rect(final, 11, 11, final);
    return(final);

}



 /* ============================================================================================== */
void get_edge(uint16_t *Image, vector<int32_t> *Bord1, vector<int32_t> *Bord2, vector<int32_t> *Bord3,
                vector<int32_t> *Bord4,int rowsize, int colsize, int b1, int b2, int b3, int b4)
  /* =========================================================================================== */
{
    int a, b, c, d;
    int m, n;
    n=rowsize;
    m=colsize;
    int32_t q, r ,s, p;
    int seuil=30;

    //clean1(Image, colsize, rowsize);
    clean3(Image, colsize, rowsize);
    clean4(Image, colsize, rowsize);

    // we look for a lit pixel on the first line
    for (a=n, s=0; a<n*2; a++,s++)
    {
        if ( (Image[a]>0) && (Image[a-1]>0) && (Image[a+1]>0) )
        {
            //cout << "point sur la premiere ligne "<< s << endl;
            Bord1->push_back(s);
            a+=3;
            s+=3;
        }
    }

    // we look for a lit pixel on the first column
    for (c=b2, q=0; c<m*n; c=c+n,q++)
     {
         if ( (Image[c]>seuil) && (Image[c-n]>seuil) && (Image[c+n]>seuil) && (Image[c-2*n]>seuil) && (Image[c+2*n]>seuil))

         {
             //cout << "point sur la  premiere colonne "<< q << endl;
             Bord2->push_back(q);
             c=c+4*n;
             q+=4;
         }
     }

    // we look for a lit pixel on the last colum
    for (d=n-b3-1, r=0; d<n*m; d=d+n,r++)
     {
         if ((Image[d]>seuil) && (Image[d-n]>seuil) && (Image[d+n]>seuil) && (Image[d-2*n]>seuil) && (Image[d+2*n]>seuil))

         {
             //cout << "point sur la derniere colonne "<< r << endl;
             Bord3->push_back(r);
             d=d+3*n;
             r+=3;
         }
     }


    // we look for a lit pixel on the last line
    for (b=m*n-1-b4*n,p=n-1; p>0 ;b--,p--)
     {
         if ( (Image[b]>seuil) && (Image[b-1]>seuil) && (Image[b+1]>seuil) )
         {
             //cout << "point sur la  derniere ligne "<< p << endl;
             Bord4->push_back(p);
             p-=3;
             b-=3;
         }
      }


}



/* =============================================================== */
void get_best_line(uint16_t *I, vector<int32_t> Bord2, vector<int32_t> Bord3, vector<int32_t> Bord1,
               vector<int32_t> Bord4,int rowsize, int colsize, int w, int b1, int b2, int b3, int b4, double seuil, double zero)
/* =============================================================== */
{

    /**
    ARGUMENTS:
	uint16_t *Image:        Image à étudier
	vector<int32_t> Bord:   vecteur contenant les points non nuls de chacun des bords
	int32_t x, y:           coordonnées du point sur lequel on se trouve
	int colsize:            hauteur de l'image
	int rowsize:            largeur of the image
	int bi:                 largeurs des bandes noires sur les bords de l'image

    DESCRIPTION:
    On parcours toutes les lignes entre les paires de points repérés par la fonction get_edge.
    Pour chaque ligne, on fait un compte du nombre de point sur la ligne digitale de largueur 2*w.

    Si ce compte est supérieur à 600, c'est que c'est une trace de satellite. On stock alors les
    coordonnées de depart et d'arrivée de la ligne.

    De plus, il peut y avoir 2 lignes sur la meme image. Il faut donc faire attention à
    ne pas considerer plusieurs fois la meme ligne.
    **/

    int i, j;
    int32_t y1, x1=b2,x2, y2;
    int32_t best_x1_ligne1=0, best_y1_ligne1=0, best_x2_ligne1=0, best_y2_ligne1=0; //coordonnées de la 1ere ligne
    int32_t best_x1_ligne2=0, best_y1_ligne2=0, best_x2_ligne2=0, best_y2_ligne2=0; // coordonnées de la 2eme ligne
    double nb=0, nb2=0, nb3=0;
    double max_ligne1=seuil, max_ligne2=seuil;



    // Pour chacun des points du bord 2, on test les lignes avec les points du bord 1, 3 et 4
     for (i=0; i<Bord2.size(); i++)
    {
        y1=Bord2[i];
        if (y1*rowsize+x1>rowsize*colsize)
                break; // on est plus dans l'image

        // On test les lignes avec le bord 1
        for (j=0; j<Bord1.size(); j++) //line between bord2 and bord1
        {
            x2=Bord1[j];
            y2=b1;
            nb=bresen_test(I,x1,y1,x2, y2, rowsize,colsize, w, zero);
            if ( ((abs(best_y1_ligne1-y1)>4*w) || (abs(best_x2_ligne1-x2)>4*w) )
                && (nb>max_ligne1) )
            {
                max_ligne1=nb;
                best_x1_ligne1=x1;
                best_y1_ligne1=y1;
                best_x2_ligne1=x2;
                best_y2_ligne1=y2;
            }
                 //else if ( (abs(best_y1_ligne1-y1)>4*w) || (abs(best_x2_ligne1-x2)>4*w) )
            else
            {
                if ( ((abs(best_y1_ligne1-y1)>4*w) || (abs(best_x2_ligne1-x2)>4*w))
                   && ((abs(best_y1_ligne2-y1)>4*w) || (abs(best_x2_ligne2-x2)>4*w))
                   && (nb>max_ligne2) )
                {
                    max_ligne2=nb;
                    best_x1_ligne2=x1;
                    best_y1_ligne2=y1;
                    best_x2_ligne2=x2;
                    best_y2_ligne2=y2;
                }
            }
        }


        for (j=0; j<Bord3.size(); j++) //line between bord2 and bord3
        {
             x2=rowsize-b3;
             y2=Bord3[j];

             if (y2*rowsize+x2>rowsize*colsize)
                break;  // on est plus dans l'image

             nb=bresen_test(I,x1,y1,x2, y2, rowsize,colsize, w, zero);
             if ( ((abs(best_y1_ligne1-y1)>4*w) || (abs(best_y2_ligne1-y2)>4*w) )
                && (nb>max_ligne1) )
             {
                max_ligne1=nb;
                best_x1_ligne1=x1;
                best_y1_ligne1=y1;
                best_x2_ligne1=x2;
                best_y2_ligne1=y2;
             }
                 //else if ( (abs(best_y1_ligne1-y1)>4*w) || (abs(best_x2_ligne1-x2)>4*w) )
             else
             {
                if ( ((abs(best_y1_ligne1-y1)>4*w) || (abs(best_y2_ligne1-y2)>4*w))
                   && ((abs(best_y1_ligne2-y1)>4*w) || (abs(best_y2_ligne2-y2)>4*w))
                   && (nb>max_ligne2) )
                {
                    max_ligne2=nb;
                    best_x1_ligne2=x1;
                    best_y1_ligne2=y1;
                    best_x2_ligne2=x2;
                    best_y2_ligne2=y2;
                }
            }
        }


        for (j=1; j<=Bord4.size(); j++) //line between bord2 and bord4
        {
            x2=Bord4[j];
            y2=colsize-b4;

            if (y2*rowsize+x2>rowsize*colsize)
                break;  // on est plus dans l'image

            nb=bresen_test(I,x1,y1,x2, y2, rowsize,colsize, w, zero);
            if ( ((abs(best_y1_ligne1-y1)>4*w) || (abs(best_x2_ligne1-x2)>4*w) )
                && (nb>max_ligne1) )
            {
                max_ligne1=nb;
                best_x1_ligne1=x1;
                best_y1_ligne1=y1;
                best_x2_ligne1=x2;
                best_y2_ligne1=y2;
            }
                 //else if ( (abs(best_y1_ligne1-y1)>4*w) || (abs(best_x2_ligne1-x2)>4*w) )
            else
            {
                if ( ((abs(best_y1_ligne1-y1)>4*w) || (abs(best_x2_ligne1-x2)>4*w))
                   && ((abs(best_y1_ligne2-y1)>4*w) || (abs(best_x2_ligne2-x2)>4*w))
                   && (nb>max_ligne2) )
                {
                    max_ligne2=nb;
                    best_x1_ligne2=x1;
                    best_y1_ligne2=y1;
                    best_x2_ligne2=x2;
                    best_y2_ligne2=y2;
                }
            }
        }


    }


    // on parcours maintenant les points du bord3 pour faire les lignes entre le bord 3 et les bors 1 et 4
    for (i=0; i<Bord3.size(); i++)
    {
        x1=rowsize-b3;
        y1=Bord3[i];

        if (y1*rowsize+x1>rowsize*colsize)
                break; // on est plus dans l'image

        for (j=0; j<Bord1.size(); j++) //line between bord3 and bord1
        {
            x2=Bord1[j];
            y2=b1;
            nb=bresen_test(I,x2, y2, x1,y1,rowsize,colsize, w, zero);
            if ( ((abs(best_y1_ligne1-y1)>4*w) || (abs(best_x2_ligne1-x2)>4*w) )
                && (nb>max_ligne1) )
            {
                max_ligne1=nb;
                best_x1_ligne1=x1;
                best_y1_ligne1=y1;
                best_x2_ligne1=x2;
                best_y2_ligne1=y2;
            }
                 //else if ( (abs(best_y1_ligne1-y1)>4*w) || (abs(best_x2_ligne1-x2)>4*w) )
            else
            {
                if ( ((abs(best_y1_ligne1-y1)>4*w) || (abs(best_x2_ligne1-x2)>4*w))
                   && ((abs(best_y1_ligne2-y1)>4*w) || (abs(best_x2_ligne2-x2)>4*w))
                   && (nb>max_ligne2) )
                {
                    max_ligne2=nb;
                    best_x1_ligne2=x1;
                    best_y1_ligne2=y1;
                    best_x2_ligne2=x2;
                    best_y2_ligne2=y2;
                }
            }
        }


        for (j=1; j<=Bord4.size(); j++) //line between bord3 and bord4
        {
            x2=Bord4[j];
            y2=colsize-b4;

            if (y2*rowsize+x2>rowsize*colsize)
                break;  // on est plus dans l'image

            nb=bresen_test(I,x2, y2, x1,y1,rowsize,colsize, w, zero);
            if ( ((abs(best_y1_ligne1-y1)>4*w) || (abs(best_x2_ligne1-x2)>4*w) )
                && (nb>max_ligne1) )
            {
                max_ligne1=nb;
                best_x1_ligne1=x1;
                best_y1_ligne1=y1;
                best_x2_ligne1=x2;
                best_y2_ligne1=y2;
            }
                 //else if ( (abs(best_y1_ligne1-y1)>4*w) || (abs(best_x2_ligne1-x2)>4*w) )
            else
            {
                if ( ((abs(best_y1_ligne1-y1)>4*w) || (abs(best_x2_ligne1-x2)>4*w))
                   && ((abs(best_y1_ligne2-y1)>4*w) || (abs(best_x2_ligne2-x2)>4*w))
                   && (nb>max_ligne2) )
                {
                    max_ligne2=nb;
                    best_x1_ligne2=x1;
                    best_y1_ligne2=y1;
                    best_x2_ligne2=x2;
                    best_y2_ligne2=y2;
                }
            }
        }


    }



    //cout << "nombre de point sur la droite "<< nb_max<< endl;
    //cout <<  max_ligne1<< endl;

     // Si best_y1 est différent de zéro, cela signifie que les coordonnées correspondent à une trace de satellite.
     if ((best_y1_ligne1!=0) && (abs(best_y1_ligne1-best_y2_ligne1)>4*w) && (abs(best_x1_ligne1-best_x2_ligne1)>4*w))
     {
        cout << "ligne entre le point ("<<best_x1_ligne1<<","<< best_y1_ligne1<< ") et le point ("<< best_x2_ligne1<<"," << best_y2_ligne1 << ")"<< endl;
        //bresen_final(I, best_x1_ligne1,best_y1_ligne1,best_x2_ligne1,best_y2_ligne1,rowsize, colsize, w);
     }
//
//     if (best_y1_ligne2!=0)
//     cout << "ligne entre le point ("<<best_x1_ligne2<<","<< best_y1_ligne2<< ") et le point ("<< best_x2_ligne2<<"," << best_y2_ligne2 << ")"<< endl;

     //bresen_final(I, best_x1_ligne1,best_y1_ligne1,best_x2_ligne1,best_y2_ligne1,rowsize, colsize, w);

}

/* ============================================================================================== */
int32_t query(uint16_t *Image,int32_t x,int32_t y,int w,int rowsize, int colsize)
/* ============================================================================================== */
{
/**
ARGUMENTS:
	uint16_t *Image:      Image à étudier
	int32_t x, y:         coordonnées du point sur lequel on se trouve
	int colsize:          hauteur de l'image
	int rowsize:          largeur of the image

DESCRIPTION:
Ce programme sert à compter les points le long de la droite pendant l'algo bresen_test.
Pour chaque pixel considere par l'algorithme, on regarde si la valeur de celui-ci.
Si la valeur est compris entre 30 et 2000, on ajoute la valeur du pixel au compte.

On ne prend pas en compte les pixel dont la valeur est inférieur à 30, car ceci correspond
au fond de ciel. Et on ne prend pas en compte ceux superieur à 2000, car ceci correspondent
à des étoiles.

De plus, comme on considere la droite digital de largeur 2*w, on regarde la valeur des w pixels
au dessus et au dessous du pixel de coordonnées (x,y)
**/

int32_t i;
int32_t nb=0;
long    index, nbpix;

nbpix = rowsize*colsize;
for (i=-w; i<=w; i++)
{
    index = (y+i)*rowsize + x;

    if ( (index < nbpix) && (index >=0)  )
    {
        if ( (Image[index]>30) && (Image[index]<1500))
        {
            nb += Image[index];
        }
    }
}

return(nb);
}



/* ============================================================================================== */
double bresen_test(uint16_t *Image,int32_t x1,int32_t y1,int32_t x2,int32_t y2,int rowsize, int colsize, int w, double zero)
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
        Cette fonction determine la ligne de bresenham entre les points (x1,y1) et
        (x2,y2).
        Pour chacun des pixels consideres comme sur la ligne, on applique la fonction
        query pour calculer le nombre de point sur la ligne.
        Pour finir on divise ce nombre par la taille de la ligne.

HISTORY:
        Written by Hugues Talbot/Juliette Charpentier	31 mars 2011
TESTS:
        The core of this program has been written in 1991.
        Many tests including purify.
**/

    int32_t       dx, dy;
    int32_t       i, e;
    double        dist=0, nb=0;
    int32_t       incx, incy, inc1, inc2;
    int32_t       x, y;  /* the actual positions */
    double        var=0;


    if ( ((y1*rowsize + x1) >colsize*rowsize) || ((y1*rowsize + x1) <0) || ((y2*rowsize + x2) >colsize*rowsize) || ((y2*rowsize + x2) <0)){
        return(0);}
    else {
    dx = abs(x1 - x2);
    dy = abs(y1 - y2);
    dist= sqrt(double(dx*dx+dy*dy)); //calcul la distance entre les deux points

    incx = 1;
    if (x2 < x1)
    {
        dx = abs(x1 - x2);
        dy = abs(y1 - y2);
        dist= sqrt(static_cast<double>(dx*dx+dy*dy)); //calcul la distance entre les deux points

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
            for (i = 0 ; i < dx ; i++)
            {
                if (e >= 0) {
                    if ( ((y*rowsize + x) <colsize*rowsize) && (Image[y*rowsize + x]==0)){
                            var++;}
                    nb+= query(Image,x,y,w,rowsize,colsize);
                    y += incy;
                    e += inc1;
                }
                else
                {
                    if ( ((y*rowsize + x) <colsize*rowsize) && (Image[y*rowsize + x]==0)){
                            var++;}
                    nb+= query(Image,x,y,w,rowsize,colsize);
                    e += inc2;
                }
                x += incx;

            }
        }
        else
        {
            e = 2*dx ;
            inc1 = 2*(dx-dy);
            inc2 = 2*dx;
            for (i = 0 ; i < dy ; i++)
            {
                if (e >= 0)
                {
                    if ( ((y*rowsize + x) <colsize*rowsize) && (Image[y*rowsize + x]==0)){
                            var++;}
                    nb+= query(Image,x,y,w,rowsize,colsize);
                    x += incx;
                    e += inc1;
                }
                else
                {
                    if ( ((y*rowsize + x) <colsize*rowsize) && (Image[y*rowsize + x]==0)){
                            var++;}
                    nb+= query(Image,x,y,w,rowsize,colsize);
                    e += inc2;
                }
                y += incy;

            }

        }

            //double seuil=1;
            var=var/dist;
            if (var*10>zero){
                return(0);}
            else {
                //cout << var<< endl;
                return((double)nb/dist);}
    }
    }

}



/* ============================================================================================== */
void black_line(uint16_t *Image,int32_t x,int32_t y,int w,int rowsize, int colsize)
/* ============================================================================================== */
{
int i;

for (i=-w; i<=w; i++)
{
    // (y*rowsize+x+i)<sizeof(Image)
    if  ((y+i)*rowsize+x<rowsize*colsize)
    {
        Image[(y+i)*rowsize+x]=0;
    }
}

}






/* ============================================================================================== */
void bresen_final(uint16_t *Image,int32_t x1,int32_t y1,int32_t x2,int32_t y2,int rowsize,int colsize, int w)
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
    double        dist,moy=0, var=0;


    if ( ((y1*rowsize + x1) <colsize*rowsize) || ((y1*rowsize + x1) >0) || ((y2*rowsize + x2) <colsize*rowsize) || ((y2*rowsize + x2) >0))
    {
        dx = abs(x1 - x2);
        dy = abs(y1 - y2);
        dist= sqrt(static_cast<double>(dx*dx+dy*dy)); //calcul la distance entre les deux points

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
            for (i = 0 ; i < dx ; i++)
            {
                if (e >= 0) {
                    if ( ((y*rowsize + x) <colsize*rowsize) && (Image[y*rowsize + x]==0)){
                            var++;}

                    y += incy;
                    e += inc1;
                }
                else
                {
                    if ( ((y*rowsize + x) <colsize*rowsize) && (Image[y*rowsize + x]==0)){
                            var++;}

                    e += inc2;
                }
                x += incx;

            }
        }
        else
        {
            e = 2*dx ;
            inc1 = 2*(dx-dy);
            inc2 = 2*dx;
            for (i = 0 ; i < dy ; i++)
            {
                if (e >= 0)
                {
                    if ( ((y*rowsize + x) <colsize*rowsize) && (Image[y*rowsize + x]==0)){
                            var++;}

                    x += incx;
                    e += inc1;
                }
                else
                {
                    if ( ((y*rowsize + x) <colsize*rowsize) && (Image[y*rowsize + x]==0)){
                            var++;}

                    e += inc2;
                }
                y += incy;

            }

        }

            var=var/dist;
            cout << var<< endl;

    }

}



/* =============================================================== */
void mask_line(uint16_t *Image,int32_t x1,int32_t y1,int32_t x2,int32_t y2,int rowsize,int colsize, int w)
/* =============================================================== */
{
    double a, b, c;
    int x, y;
    a=(double)(y2-y1)/(double)(x2-x1);
    b=(double)y2-(double)a*x2;


    for (y=0; y<colsize; y++)
    {
        for (x=0; x<rowsize; x++)
        {
            if  ( (y<a*x+b+w) && (y>a*x+b-w) )
            {
                Image[y*rowsize+x]=10000;
            }
            else
            {
                Image[y*rowsize+x]=0;
            }
        }
    }
}



/* =============================================================== */
    int new_edge1(uint16_t *Image,int colsize, int rowsize)
/* =============================================================== */
{
    int i, l, n=colsize;
    int b1=0;
    l=  colsize*rowsize;

    for (i=rowsize/2 ; i<l; i=i+rowsize, b1++)
    {
       if (Image[i]>99)
       {
           break;
       }
    }

    //cout << "bord1= "<< b1+1<< endl;
    return(b1+1);

}


/* =============================================================== */
    int new_edge4(uint16_t *Image,int colsize, int rowsize)
/* =============================================================== */
{
    int i, l, n=colsize;
    int b4=0;
    l=  colsize*rowsize;

    for (i=l-rowsize/2 ; i>0; i-=rowsize, b4++)
    {
       if (Image[i]>99)
       {
           break;
       }
    }

    //cout << "bord4= "<< b4+1<< endl;
    return(b4+1);

}



/* =============================================================== */
    int new_edge2(uint16_t *Image, int colsize, int rowsize)
/* =============================================================== */
{
    int i, j, l, n=rowsize;
    int b2=0;
    l=  colsize*rowsize;


    for (i=colsize*rowsize/4 ; i<l; i++, b2++)
    {
       if (Image[i]>99)
       {
           break;
       }
    }

    //cout << "bord2= "<< b2+1<< endl;
    return(b2+1);

}



/* =============================================================== */
    int new_edge3(uint16_t *Image, int colsize, int rowsize)
/* =============================================================== */
{
    int i, j, l, n=rowsize;
    int b3=0;
    l=  colsize*rowsize;


    for (i=(colsize/2)*rowsize ; i>0; i--, b3++)
    {
       if (Image[i]>99)
       {
           break;
       }
    }

    //cout << "bord3= "<< b3+1<< endl;
    return(b3+1);

}
