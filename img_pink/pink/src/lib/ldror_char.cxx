#  include <stdio.h>
#  include <stdint.h>
#  include <sys/stat.h>
#  include <fcntl.h>
#  include <stdlib.h>
#  include <math.h>
#  include <vector>
#  include <iostream>

#  include "ldror_char.hxx"
#  include "lfindline.hxx"
#  include "lfindline_char.hxx"



using std::cout;
using std::endl;


/* =============================================================== */
    tree *Create(int val,tree *t1, tree *t2,tree *t3, tree *t4, int colsize, int rowsize)
/* =============================================================== */
{
	tree *res;

	res = (tree*)calloc(1,sizeof(tree));

	if( res == NULL )
	{
		fprintf(stderr,"Impossible d'allouer le noeud");
		return NULL;
	}

	res->value = val;
	res->high_left = t1;
	res->high_right = t2;
	res->low_left = t3;
	res->low_right = t4;
	res->colsize=colsize;
	res->rowsize=rowsize;

	return(res);
}


bool Isempty( tree *T)
{
	return T == NULL;
}



/* =============================================================== */
    void AddElt(tree *src,int position, int32_t elt)
/* =============================================================== */
{
//	if ( src == NULL )
//	{
//		src = Create(elt,NULL,NULL, NULL, NULL);
//	}
//	else if ( position==0 )
//	{
//		src->value = elt;
//	}
//	else if ( position==1 )
//	{
//		src->high_left = Create(elt,NULL,NULL, NULL, NULL);
//	}
//	else if ( position==2 )
//	{
//		src->high_right = Create(elt,NULL,NULL, NULL, NULL);
//	}
//	else if ( position==3 )
//	{
//		src->low_left = Create(elt,NULL,NULL, NULL, NULL);
//	}
//	else if ( position==4 )
//	{
//		src->low_right = Create(elt,NULL,NULL, NULL, NULL);
//	}

    src->value=elt;

}

/* =============================================================== */
void WideSearch(tree *T)
/* =============================================================== */
{

 //   cout << T->value << endl;
 //   cout <<T->high_left->value << " ; "<<T->high_right->value << " ; "<<T->low_left->value << " ; "<<T->low_right->value << endl;


//    cout <<T->high_left->value << " ; "<<T->high_right->value << " ; "<<T->low_left->value << " ; "<<T->low_right->value << endl;
//    if (T->high_left!=NULL)
//    {
//        WideSearch(T->high_left);
//        WideSearch(T->high_right);
//        WideSearch(T->low_left);
//        WideSearch(T->low_right);
//    }

    cout <<T->value << endl;
    if (Isempty(T->high_left)==false)
    {
        WideSearch(T->high_left);
    }

}


/* =============================================================== */
bool Isleave(tree *T)
/* =============================================================== */
{

    if (Isempty(T))
    {
        return(false);
    }
    else if ( Isempty(T->high_left) && Isempty(T->high_right) && Isempty(T->low_left) && Isempty(T->low_right) )
    {
        return(true);
    }
    else
    {
        return(false);
    }

}


/* =============================================================== */
void Iscorrect(tree *T)
/* =============================================================== */
{
    while (Isleave(T)==false)
    {
        if (T->value!=(T->high_left->value+T->high_right->value+T->low_left->value+T->low_right->value))
        {
            cout << "uncorrect tree "<< endl;
            break;
        }
        else
        {
            Iscorrect(T->high_left);
            Iscorrect(T->high_right);
            Iscorrect(T->low_left);
            Iscorrect(T->low_right);
        }
    }


}





/* =============================================================== */
void divide_image(uint8_t *I, uint8_t *I_high_left, uint8_t *I_high_right, uint8_t *I_low_left, uint8_t *I_low_right, int colsize, int rowsize)
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
int32_t query2(tree *T)
/* =============================================================== */
{
    int32_t p=0;
    if (!Isleave(T))
    {
        p=T->high_left->value
            + T->high_right->value
            + T->low_left->value
            + T->low_right->value;
        return(p);
    }
    else
    {
        cout <<"wrong query2"<< endl;
        return(0);
    }

}



/* =============================================================== */
void create_query_tree(uint8_t *I,tree *T, int colsize, int rowsize)
/* =============================================================== */
{
    int p;
    int x, y;
    int a, b;
    uint8_t *I1,*I2,*I3,*I4;
    a=colsize%2;
    b=rowsize%2;

    if (min(rowsize/2, colsize/2)==1)
    {
        //cout << "On est dans une feuille" << endl;
        p = 0;
        for (y=0; y<colsize; y++)
        {
            for (x=0; x<rowsize; x++)
            {
                if (I[y*rowsize+x]!=0)
                {
                    p++;
                    //cout << "Ah, enfin quelquechose: " << p << " " ;
                }
            }
        }

        T->value=p;

    }
    else
    {

        T->high_left = (tree*)calloc(1,sizeof(tree));
        T->high_right= (tree*)calloc(1,sizeof(tree));
        T->low_left= (tree*)calloc(1,sizeof(tree));
        T->low_right= (tree*)calloc(1,sizeof(tree));

        T->high_left->rowsize=rowsize/2;
        T->high_right->rowsize=(rowsize+b)/2;
        T->low_left->rowsize=rowsize/2;
        T->low_right->rowsize=(rowsize+b)/2;

        T->high_left->colsize=colsize/2;
        T->high_right->colsize=colsize/2;
        T->low_left->colsize=(colsize+a)/2;
        T->low_right->colsize=(colsize+a)/2;

        I1=(uint8_t *)malloc(colsize*rowsize/4 * sizeof(uint8_t));
        I2=(uint8_t *)malloc(colsize*(rowsize+b)/4 * sizeof(uint8_t));
        I3=(uint8_t *)malloc((colsize+a)*rowsize/4 * sizeof(uint8_t)) ;
        I4=(uint8_t *)malloc((colsize+a)*(rowsize+b)/4 * sizeof(uint8_t));

        divide_image(I,I1,I2,I3,I4,colsize,rowsize);

        create_query_tree(I1,T->high_left,colsize/2,rowsize/2);
        create_query_tree(I2,T->high_right,colsize/2,(rowsize+b)/2);
        create_query_tree(I3,T->low_left,(colsize+a)/2,rowsize/2);
        create_query_tree(I4,T->low_right,(colsize+a)/2,(rowsize+b)/2);

        T->value=query2(T);
        //cout <<  query2(T) << " ; " << endl;
        //cout <<T->high_left->value << " ; "<<T->high_right->value << " ; "<<T->low_left->value << " ; "<<T->low_right->value << endl;

    }

}



/* =============================================================== */
void get_line(uint8_t *I,tree *T, vector<int32_t> Bord2, vector<int32_t> Bord3, vector<int32_t> Bord1,
               vector<int32_t> Bord4,int colsize,int rowsize, int w)
/* =============================================================== */
{
    int i, j;
    int32_t y1, x1=0;
    int32_t best_y1=0, best_x2=0, best_y2=0;
    int32_t nb1=0,nb2=0,n;
    int32_t nb_max=0;
    int32_t a, b,c,d;



     for (i=0; i<Bord2.size(); i++)
    {
        y1=Bord2[i];
        if (y1-w>0)
            a=y1-w;
        else
            a=y1;

        if (y1+w<colsize)
            c=y1+w;
        else
            c=y1;

//        for (j=0; j<Bord1.size(); j++) //line between bord2 and bord1
//        {
//             nb1=count21(T,y1-w,Bord1[j]-w,colsize,rowsize);
//             nb2=count21(T,y1+w,Bord1[j]+w,colsize,rowsize);
//             if (abs(nb1-nb2)> nb_max)
//                    {
//                        nb_max=abs(nb1-nb2);
//                        best_y1=y1;
//                        best_x2=Bord1[j];
//                        best_y2= 0;
//                    }
//        }


        for (j=0; j<Bord3.size(); j++) //line between bord2 and bord3
        {
             if (Bord3[j]-w>0)
                 b=Bord3[j]-w;
             else
                 b=Bord3[j];

             if (Bord3[j]+w<colsize)
                 d=Bord3[j]+w;
             else
                 d=Bord3[j];

             nb1=count23(T,a,b,colsize,rowsize);
             nb2=count23(T,c,d,colsize,rowsize);
             if (abs(nb1-nb2)> nb_max)
                    {
                        nb_max=abs(nb1-nb2);
                        best_y1=y1;
                        best_x2=rowsize-1;
                        best_y2= Bord3[j];
                    }
        }

//        for (j=1; j<=Bord4.size(); j++) //line between bord2 and bord4
//        {
//             nb1=count24(T,y1-w,Bord4[j]+w,colsize,rowsize);
//             nb2=count24(T,y1+w,Bord4[j]-w,colsize,rowsize);
//             if (abs(nb1-nb2)> nb_max)
//                    {
//                        nb_max=abs(nb1-nb2);
//                        best_y1=y1;
//                        best_x2=Bord4[j];
//                        best_y2= colsize-1;
//                    }
//        }
    }


    // Draw the bresenham best line in the image
    cout << "ligne entre le point (0,"<< best_y1<< ") et le point ("<< best_x2<<"," << best_y2 << ")"<< endl;
    bresen_final_char(I, x1, best_y1, best_x2, best_y2, rowsize, w);

}


/* =============================================================== */
uint32_t find_x3(int x1, int y1,int x2, int y2, int rowsize,int colsize)
/* =============================================================== */
{
  double a=0, b=0;
  int x3;



  if (x2-x1!=0)
  {
      a=(double)(y2-y1)/(double)(x2-x1);
      b=(double)y2-(double)a*x2;
      if (a!=0)
      {
          double hinter = (colsize/2 - b) / a;
          if (hinter < 0)
          {
            //cout << "Negative hinter : " << x1<<"," << y1<<"," << x2<<"," << y2<<"," << rowsize<< ","<<colsize<<endl;
            return(0);
          }

          if (floor(hinter)<=rowsize)
          {
              return(floor(hinter));
          }
          else
          {
              //cout << "Wrong find_x3 : " << x1<<"," << y1<<"," << x2<<"," << y2<<"," << rowsize<< ","<<colsize<<endl;
                return(rowsize);
                //exit(1);
          }
      }
      else
      {
          //x3=rowsize/2;
          return (-1);
      }

  }
  else
  {
      return(x2);
  }

}

/* =============================================================== */
uint32_t find_y3(int x1, int y1,int x2, int y2, int rowsize,int colsize)
/* =============================================================== */
{
  double a=0, b=0;
  int y3;


    if (x2-x1!=0)
    {
      a=(double)(y2-y1)/(double)(x2-x1);
      b=(double)y2-(double)a*x2;

      double vinter = a * (rowsize/2) + b;

      if (vinter < 0)
      {
        //cout << "Negative vinter " << x1<<"," << y1<<"," << x2<<"," << y2<<"," << rowsize<< ","<<colsize<<endl;
        return(0);
      }

      if (floor(vinter) <= colsize)
      {
          return(floor(vinter));
      }
//      else if (a==0)
//      {
//          return((double)abs(y2-y1)+min(y1,y2));
//      }
      else
      {
          //cout << "Wrong find_y3 : " << x1<<"," << y1<<"," << x2<<"," << y2<<"," << rowsize<< ","<<colsize<<endl;
          return(colsize);

      }
    }
    else
    {
          return(colsize/2);

    }



}


/* =============================================================== */
int32_t count23(tree *T,int y1,int y2, int rowsize, int colsize)
/* =============================================================== */
{
    int32_t p=0, x3, y3;
    int a, b;
    a=(colsize%2+colsize)/2;
    b=(rowsize%2+rowsize)/2;

    if ( (T->value==0) || (Isleave(T)==true) )
    {
        return(T->value);
    }
    else if (Isleave(T->low_right))
    {
        return(T->value);
    }
    else
    {
        if (y1<colsize/2)
        {
            if (y2<colsize/2)
            {
                y3=find_y3(0,y1,rowsize-1,y2,rowsize,colsize); // point de rencontre avec ligne verticale
                p= T->low_left->value+ T->low_right->value
                    + count23(T->high_left,y1,y3,T->high_left->rowsize,T->high_left->colsize)
                    +count23(T->high_right,y3,y2,T->high_right->rowsize,T->high_right->colsize);
                return(p);
            }
            else
            {
                y3=find_y3(0,y1,rowsize-1,y2,rowsize,colsize); // point de rencontre avec ligne verticale
                x3=find_x3(0,y1,rowsize-1,y2,rowsize,colsize); // point de rencontre avec ligne horizontale
                if (y3< colsize/2)
                {
                    p= count23(T->high_left,y1,y3,T->high_left->rowsize,T->high_left->colsize)
                        +count24(T->high_right,y3,x3-b,T->high_right->rowsize,T->high_right->colsize)
                        +count13(T->low_right,x3-b,y2-a,T->low_right->rowsize,T->low_right->colsize);
                    return(p);
                }
                else
                {
                    p= count24(T->high_left,y1,x3,T->high_left->rowsize,T->high_left->colsize)
                        +count13(T->low_left,x3,y3-a,T->low_left->rowsize,T->low_left->colsize)
                        +count23(T->low_right,y3-a,y2-a,T->low_right->rowsize,T->low_right->colsize);
                    return(p);
                }
            }
        }
        else if (y1>colsize/2)
        {
            if (y2<colsize/2)
            {
                x3=find_x3(0,y1,rowsize-1,y2,rowsize,colsize); // point de rencontre avec ligne horizontale
                y3=find_y3(0,y1,rowsize-1,y2,rowsize,colsize);// point de rencontre avec ligne verticale
                if (y3> colsize/2)
                {
                    p= count23(T->low_left,y1-a,y3-a,T->low_left->rowsize,T->low_left->colsize)
                        +count21(T->low_right,y3-a,x3-b,T->low_right->rowsize,T->low_right->colsize)
                        +count43(T->high_right,x3-b,y2,T->high_right->rowsize,T->high_right->colsize);
                    return(p);
                }
                else
                {
                    p= count21(T->low_left,y1-a,x3,T->low_left->rowsize,T->low_left->colsize)
                        +count43(T->high_left,x3,y3,T->high_left->rowsize,T->high_left->colsize)
                        +count23(T->high_right,y3,y2,T->high_right->rowsize,T->high_right->colsize);
                    return(p);
                }
            }
            else
            {
                y3=find_y3(0,y1,rowsize-1,y2,rowsize,colsize);// point de rencontre avec ligne verticale
                p= count23(T->low_left,y1-a,y3-a,T->low_left->rowsize,T->low_left->colsize)
                        +count23(T->low_right,y3-a,y2-a,T->low_right->rowsize,T->low_right->colsize);
                return(p);
            }
        }
        else
        {
           p= T->low_left->value+ T->low_right->value;
           return(p);
        }

        //return(p);
    }

}




/* =============================================================== */
int32_t count21(tree *T,int y1,int x2, int rowsize, int colsize)
/* =============================================================== */
{
    int32_t p=0, x3, y3;
    int a, b;
    a=(colsize%2+colsize)/2;
    b=(rowsize%2+rowsize)/2;

//    y3=find_y3(0,y1,x2,0,colsize,rowsize); // point de rencontre avec ligne verticale
//    x3=find_x3(0,y1,x2,0,colsize,rowsize); // point de rencontre avec ligne horizontale

    if ( (T->value==0) || (Isleave(T)==true) )
    {
        return(T->value);
    }
    else if (Isleave(T->low_right))
    {
        return(T->value);
    }
    else
    {

        if (y1<=colsize/2)
        {
            if (x2<=rowsize/2)
            {
                p= T->high_right->value+T->low_left->value + T->low_right->value
                    + count21(T->high_left,y1,x2,T->high_left->rowsize,T->high_left->colsize);
                return(p);
            }
            else
            {
                y3=find_y3(0,y1,x2,0,rowsize,colsize);
                p=T->low_left->value +T->low_left->value
                    +count23(T->high_left,y1,y3,T->high_left->rowsize,T->high_left->colsize)
                    +count21(T->high_right,y3,x2-b,T->high_right->rowsize,T->high_right->colsize);
                return(p);
            }
        }
        else
        {
            if (x2<rowsize/2)
            {
                x3=find_x3(0,y1,x2,0,rowsize,colsize);
                p= T->high_right->value+T->low_right->value
                    + count21(T->low_left,y1-a,x3,T->low_left->rowsize,T->low_left->colsize)
                    +count14(T->high_left,x2,x3,T->high_left->rowsize,T->high_left->colsize);
                return(p);
            }
            else
            {
                x3=find_x3(0,y1,x2,0,rowsize,colsize);
                y3=find_y3(0,y1,x2,0,rowsize,colsize);
                p= T->low_right->value+count21(T->low_left,y1-a,x3,T->low_left->rowsize,T->low_left->colsize)
                    +count43(T->high_left,x3,y3,T->high_left->rowsize,T->high_left->colsize)
                    +count21(T->high_right,y3,x2-b,T->high_right->rowsize,T->high_right->colsize);
                return(p);
            }
        }
       //return(p);
    }

}



/* =============================================================== */
int32_t count24(tree *T,int y1,int x2, int rowsize, int colsize)
/* =============================================================== */
{
    int32_t p=0, x3, y3;
    int a, b;
    a=(colsize%2+colsize)/2;
    b=(rowsize%2+rowsize)/2;

//    y3=find_y3(0,y1,x2,colsize,colsize,rowsize); // point de rencontre avec ligne verticale
//    x3=find_x3(0,y1,x2,colsize,colsize,rowsize); // point de rencontre avec ligne horizontale

    if ( (T->value==0) || (Isleave(T)==true) )
    {
        return(T->value);
    }
    else if (Isleave(T->low_right))
    {
        return(T->value);
    }
    else
    {

        if ( y1>= colsize/2 )
        {
            if (x2<=rowsize/2)
            {
                p= count21(T->low_left,y1-colsize/2,x2,rowsize/2,colsize/2);
                return(p);
            }
            else
            {
                y3=find_y3(0,y1,x2,colsize-1,rowsize,colsize);
                p=count23(T->low_left,y1-a,y3-a,T->low_left->rowsize,T->low_left->colsize)
                    +count24(T->low_right,y3-a,x2-b,T->low_right->rowsize,T->low_right->colsize);
                return(p);
            }
        }
        else
        {
            if (x2<rowsize/2)
            {
                x3=find_x3(0,y1,x2,colsize-1,rowsize,colsize);
                p= count24(T->high_left,y1,x3,T->high_left->rowsize,T->high_left->colsize)
                    +count14(T->low_left,x3,x2,T->low_left->rowsize,T->low_left->colsize);
                return(p);
            }
            else
            {
                x3=find_x3(0,y1,x2,colsize-1,rowsize,colsize);
                y3=find_y3(0,y1,x2,colsize-1,rowsize,colsize);
                p=count24(T->high_left,y1,x3,T->high_left->rowsize,T->high_left->colsize)
                    +count13(T->low_left,x3,y3-a,T->low_left->rowsize,T->low_left->colsize)
                    +count24(T->low_right,y3-a,x2-b,T->low_right->rowsize,T->low_right->colsize);
                return(p);
            }
        }

        //return(p);
    }
}


/* =============================================================== */
int32_t count14(tree *T,int x1,int x2, int rowsize, int colsize)
/* =============================================================== */
{
//    int p=0, x3, y3;
//
//    y3=find_y3(x1,0,x2,colsize,colsize,rowsize); // point de rencontre avec ligne verticale
//    x3=find_x3(x1,0,x2,colsize,colsize,rowsize); // point de rencontre avec ligne horizontale
//
//    if ( (T->value==0) || (Isleave(T)) )
//    {
//        return(0);
//    }
//    else if (Isleave(T->low_left))
//    {
//        return(T->value);
//    }
//    else
//    {
//        if (x1<=rowsize/2)
//        {
//            if (x2<=rowsize/2)
//            {
//                //x3= // point de rencontre avec ligne horizontale
//                p= count14(T->high_left,x1,x3,colsize/2,rowsize/2)
//                    + count14(T->low_left,x3,x2,colsize/2,rowsize/2);
//            }
//            else
//            {
//                //x3= // point de rencontre avec ligne horizontale
//                //y3=  // point de rencontre avec ligne verticale
//                if (y3<colsize/2)
//                {
//                    p= T->low_left->value+count13(T->high_left,x1,y3,colsize/2,rowsize/2)+count24(T->high_right,y3,x3,colsize/2,rowsize/2)
//                        + count14(T->low_right,x3,x2,colsize/2,rowsize/2);
//                }
//                else
//                {
//                    p= count14(T->high_left,x1,x3,colsize/2,rowsize/2)+count13(T->low_left,x3,y3,colsize/2,rowsize/2)
//                        + count24(T->low_right,y3,x2,colsize/2,rowsize/2);
//                }
//             }
//        }
//        else
//        {
//            if (x2<=rowsize/2)
//            {
//                //x3= // point de rencontre avec ligne horizontale
//                //y3=  // point de rencontre avec ligne verticale
//                if (y3<colsize/2)
//                {
//                    p= T->low_right->value+ count21(T->high_right,y3,x1,colsize/2,rowsize/2)
//                        +count43(T->high_left,x3,y3,colsize/2,rowsize/2) +count14(T->low_left,x2,x3,colsize/2,rowsize/2);
//                }
//                else
//                {
//                    p= count14(T->high_right,x1,x3,colsize/2,rowsize/2)+count21(T->low_right,y3,x3,colsize/2,rowsize/2)
//                        + count43(T->low_left,x2,y3,colsize/2,rowsize/2);
//                }
//            }
//            else
//            {
//                //x3=   // point de rencontre avec ligne horizontale
//                p= T->high_left->value+ T->low_left->value + count14(T->high_left,x1,x3,colsize/2,rowsize/2)
//                    + count14(T->low_left,x3,x2,colsize/2,rowsize/2);
//            }
//        }
//
//         return(p);
//    }
  return 0;  
}




/* =============================================================== */
int32_t count43(tree *T,int x1,int y2, int rowsize, int colsize)
/* =============================================================== */
{
    int32_t p=0, x3, y3;
    int a, b;
    a=(colsize%2+colsize)/2;
    b=(rowsize%2+rowsize)/2;

//    y3=find_y3(x1, colsize,rowsize,y2,colsize,rowsize); // point de rencontre avec ligne verticale
//    x3=find_x3(x1, colsize,rowsize,y2,colsize,rowsize); // point de rencontre avec ligne horizontale

    if ( (T->value==0) || (Isleave(T)==true) )
    {
        return(T->value);
    }
    else if (Isleave(T->low_right))
    {
        return(T->value);
    }
    else
    {
        if (x1<rowsize/2)
        {
            if (y2<colsize/2)
            {
                x3=find_x3(x1, colsize-1,rowsize-1,y2,rowsize,colsize);
                y3=find_y3(x1, colsize-1,rowsize-1,y2,rowsize,colsize);
                p=count43(T->low_left,x1,y3-a,T->low_left->rowsize,T->low_left->colsize)
                    +count21(T->low_right,y3-a,x3-b,T->low_right->rowsize,T->low_right->colsize)
                    +count43(T->high_right,x3-b,y2,T->high_right->rowsize,T->high_right->colsize);
                return(p);
            }
            else
            {
                y3=find_y3(x1, colsize-1,rowsize-1,y2,rowsize,colsize);
                p= count43(T->low_left,x1,y3-a,T->low_left->rowsize,T->low_left->colsize)
                    +count23(T->low_right,y3-a,y2-a,T->low_right->rowsize,T->low_right->colsize);
                return(p);
            }
        }
        else
        {
            if (y2<colsize/2)
            {
                x3=find_x3(x1, colsize-1,rowsize-1,y2,rowsize,colsize);
                p= count14(T->low_right,x1-b,x3-b,T->low_right->rowsize,T->low_right->colsize)
                    +count43(T->high_right,x3-b,y2,T->high_right->rowsize,T->high_right->colsize);
                return(p);
            }
            else
            {
                p=count43(T->low_right,x1-b,y2-a,T->low_right->rowsize,T->low_right->colsize);
                return(p);
            }
        }

        //return(p);
    }

}



/* =============================================================== */
int count13(tree *T,int x1,int y2,int rowsize, int colsize)
/* =============================================================== */
{
    int32_t p=0, x3, y3;
    int a, b;
    a=(colsize%2+colsize)/2;
    b=(rowsize%2+rowsize)/2;

//    y3=find_y3(x1,0,rowsize,y2,colsize,rowsize); // point de rencontre avec ligne verticale
//    x3=find_x3(x1,0,rowsize,y2,colsize,rowsize); // point de rencontre avec ligne horizontale

    if ( (T->value==0) || (Isleave(T)==true) )
    {
        return(T->value);
    }
    else if (Isleave(T->low_right))
    {
        return(T->value);
    }
    else
    {
        if (x1<rowsize/2)
        {
            if (y2<colsize/2)
            {
                y3=find_y3(x1,0,rowsize-1,y2,rowsize,colsize);
                p=T->low_left->value+T->low_right->value
                    + count13(T->high_left,x1,y3,T->high_left->rowsize,T->high_left->colsize)
                    +count23(T->high_right,y3,y2,T->high_right->rowsize,T->high_right->colsize);
                return(p);
            }
            else
            {
                x3=find_x3(x1,0,rowsize-1,y2,rowsize,colsize);
                y3=find_y3(x1,0,rowsize-1,y2,rowsize,colsize);
                p=T->low_left->value+ count13(T->high_left,x1,y3,T->high_left->rowsize,T->high_left->colsize)
                    +count24(T->high_right,y3,x3-b,T->high_right->rowsize,T->high_right->colsize)
                    +count13(T->low_right,x3-b,y2-a,T->low_right->rowsize,T->low_right->colsize);
                return(p);
            }
        }
        else
        {
            if (y2<=colsize/2)
            {
                p= T->high_left->value+T->low_left->value + T->low_right->value
                    +count13(T->high_right,x1-b,y2,T->high_right->rowsize,T->high_right->colsize);
                return(p);
            }
            else
            {
                x3=find_x3(x1,0,rowsize-1,y2,rowsize,colsize);
                p=count14(T->high_right,x1-b,x3-b,T->high_right->rowsize,T->high_right->colsize)
                    +count13(T->low_right,x3-b,y2-a,T->low_right->rowsize,T->low_right->colsize);
                return(p);
            }
        }
        //return(p);
    }
}

