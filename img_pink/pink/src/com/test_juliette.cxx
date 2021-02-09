
#  include <stdio.h>
#  include <stdint.h>
#  include <sys/stat.h>
#  include <fcntl.h>
#  include <stdlib.h>
#  include <math.h>
#  include <vector>
#  include <iostream>
#  include <time.h>


#  include "mcimage.h"
#  include "mccodimage.h"
#  include "lfits2pgm.h"
#  include "lfindline.hxx"
#  include "ldilateros.h"
# include <liarp.h>


using namespace std;




#define INT4_TYPE int




/* =============================================================== */
    xvimage *tophat(xvimage *original,xvimage *elem, index_t x, index_t y, xvimage *elem2, index_t x2, index_t y2)
/* =============================================================== */
 {
     // on transforme l'image de uint16_t a unint32_t pour pouvoir appliquer les opération de morphologie mathématique
    xvimage *im;
    int32_t *Original;
    xvimage *ouvert;
    int32_t *Ouvert;
    int b;
    index_t i, rs, cs, N;

    //short2long(&original);
    b=ldilatlong(original, elem, x, y);
    b=leroslong(original, elem, x, y);
    ouvert=copyimage(original);
    b=leroslong(ouvert, elem2, x2, y2);
    b=ldilatlong(ouvert, elem2, x2, y2);

    Original=SLONGDATA(original);
    Ouvert=SLONGDATA(ouvert);
    rs=rowsize(original);
    cs=colsize(original);
    N=rs*cs;


    for (i=0 ; i<N; i++)
    {
        Ouvert[i]=Original[i]-Ouvert[i];
    }

     b=ldilatlong(ouvert, elem, x, y);
     return(ouvert);

 }



/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */

{
    clock_t start, end;
    double elapsed;
    start = clock();

    struct xvimage *image, *image2;
    int dimx=0, dimy=0;
    char *filename;
//    int rowsize, colsize;
//    struct xvimage * elem;
//    index_t x, y, z;
//    struct xvimage * elem2;
//    index_t x2, y2, z2;
//    int32_t b;
//    vector<int32_t> Bord1, Bord2, Bord3, Bord4;
//    int w=2;

    filename=argv[1];
    image = readimage(argv[1]);

    if (image == NULL)
    {
        fprintf(stderr, "%s: readimage failed\n", argv[0]);
        exit(1);
    }

//    elem = readse(argv[2], &x, &y, &z);
//    elem2 = readse(argv[3], &x2, &y2, &z2);
//
//    if ( (elem == NULL) || (elem2 == NULL) )
//    {
//    fprintf(stderr, "%s: readse failed\n", argv[0]);
//    exit(1);
//    }

    dimx=atof(argv[2]);
    dimy=atof(argv[3]);
    if ( (dimx==0) ||(dimy==0) )
    {
        fprintf(stderr, "%s wrong dimension for the rectangle \n", argv[0]);
        exit(1);
    }

    uint16_t *I1;
    int32_t *I;

    switch (image->data_storage_type) {
     case    VFF_TYP_2_BYTE:
        cerr << "Short image" << endl;
        //short2long(&image);
        //I1=USHORTDATA(image);
        //binning(&image);
        //binning(&image);
        //short2long(&image);
        //imferode3D_rect(image, dimx, dimy,1, image);
//        image2=copyimage(image);
//        b=leroslong(image, elem, x, y);
//        b=ldilatlong(image, elem, x, y);
//        image2=tophat(image,elem, x, y, elem2, x2, y2);
//        end=clock();
//        elapsed=((double)end-start)/CLOCKS_PER_SEC;
//        printf("%.2f secondes pour le tophat. \n",elapsed);
//        start = clock();

        /* solution en restant en long: */
//        I= SLONGDATA(image2);
//        rowsize=rowsize(image2);
//        colsize=colsize(image2);
////        clean_long(I,colsize,rowsize);
////        clean_long2(I,colsize,rowsize);
////        clean_long2(I,colsize,rowsize);
//        get_edge_long(I, &Bord1,&Bord2, &Bord3, &Bord4,rowsize, colsize);
//        get_best_line_long(I, Bord2, Bord3, Bord1,Bord4, rowsize, colsize, w);


        /* solution en repassant en short: */
        //long2short(&image2);
//        I1=USHORTDATA(image);
//        clean1(I1, colsize, rowsize);
//        I1=get_edge(I1,I1, &Bord1,&Bord2,&Bord3,&Bord4, rowsize, colsize);
//        get_best_line(I1,I1, Bord2, Bord3,Bord1,Bord4,rowsize,colsize, w);




        break;

        case    VFF_TYP_1_BYTE		:
        std::cerr << "char type " <<std::endl;
        break;

     default:
         std::cerr << "Unsupported pixel type " << image->data_storage_type <<std::endl;
         exit(1);
         break;
    }


    writeimage(image2,argv[4]);
    freeimage(image2);


    end=clock();
    elapsed=((double)end-start)/CLOCKS_PER_SEC;
    printf("%.2f secondes pour fermer l'image. \n",elapsed);

    return(0);

}
