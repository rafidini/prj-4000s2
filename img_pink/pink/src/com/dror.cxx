/*
Copyright ESIEE (2009)

m.couprie@esiee.fr

This software is an image processing library whose purpose is to be
used primarily for research and teaching.

This software is governed by the CeCILL  license under French law and
abiding by the rules of distribution of free software. You can  use,
modify and/ or redistribute the software under the terms of the CeCILL
license as circulated by CEA, CNRS and INRIA at the following URL
"http://www.cecill.info".

As a counterpart to the access to the source code and  rights to copy,
modify and redistribute granted by the license, users are provided only
with a limited warranty  and the software's author,  the holder of the
economic rights,  and the successive licensors  have only  limited
liability.

In this respect, the user's attention is drawn to the risks associated
with loading,  using,  modifying and/or developing or reproducing the
software by the user in light of its specific status of free software,
that may mean  that it is complicated to manipulate,  and  that  also
therefore means  that it is reserved for developers  and  experienced
professionals having in-depth computer knowledge. Users are therefore
encouraged to load and test the software's suitability as regards their
requirements in conditions enabling the security of their systems and/or
data to be ensured and,  more generally, to use and operate it in the
same conditions as regards security.

The fact that you are presently reading this means that you have had
knowledge of the CeCILL license and that you accept its terms.
*/
/*! \file im2line.c

\brief find line into an image using bresenham definition

<B>Usage:</B>test4.pgm  testfind.pgm
test4.pgm  testfind.pgm
test4.pgm  testfind.pgm
 im2line in.fits {w}  out.pgmtest4.pgm  testfind.pgm


<B>Description:</B>
On commence pas netoyer l'image afin de retirer le bruit de fond et d'ameliorer le contraste.

On determine ensuite les points sur le bord de l'image qui correspondent potentiellement à une droite.

Parralellement, on crée un arbre...

On trace les droites de Bresenham entre les differents points des bords. Pour chacunes des droites on calcule
le nombre de points non nul present dans le plan sous la droite via l'arbre, puis le nombre de points non nuls pour une droite parralelle.
On soustrait ces 2 nombres, on obtient ainsi le nombre de pixel non nul contenu dans l'espace entre les 2 plans.
La droites contenant les plus de points non nul doit correspondre à la trace du satellite.

L'image finale est une image noire, avec une droite blanche correspondant à la trace du sattelite.

Parameters:
\li \b in.fits : source file in fits format


\author Juliette Charpentier
*/



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
#  include "lfindline_char.hxx"
#  include "ldror_char.hxx"
#  include "ldror.hxx"





using namespace std;




#define INT4_TYPE int


/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */

{
    clock_t start, end;
    double elapsed;
    start = clock();


    struct xvimage *image;
    struct xvimage *image2, *image3;
    char *filename;
    vector<int32_t> Bord1, Bord2, Bord3, Bord4;
    int32_t imheight, imwidth;
    int rowsize, colsize;
    int w;
    int b1, b2, b3, b4;
    tree *T;
    T=(tree*)calloc(1,sizeof(tree));


    int y3;

    if ((argc != 2) && (argc != 3))
    {
        fprintf(stderr, "usage: %s in.fits {w} out.pgm \n", argv[0]);
        exit(1);
    }

    if (argc = 2)
    {
        w=10;
    }
    else
    {
        w=atof(argv[2]);
    }

    filename=argv[1];
    //image=lfits2pgm(filename);
    image = readimage(argv[1]);
    if (image == NULL)
    {
        fprintf(stderr, "%s: readimage failed\n", argv[0]);
        exit(1);
    }

    rowsize=rowsize(image);
    colsize=colsize(image);
    image2=copyimage(image);
    image3=copyimage(image);

    uint16_t *I1, *I2, *I3;
    uint8_t *I;

    switch (image->data_storage_type) {
     case    VFF_TYP_2_BYTE:
        cerr << "Short image" << endl;
        I1=USHORTDATA(image);
        I2=USHORTDATA(image2);
        I3=USHORTDATA(image3);

//        b1= new_edge1(I1, colsize, rowsize);
//        b2= new_edge2(I1, colsize, rowsize);
//        b3= new_edge3(I1, colsize, rowsize);
//        b4= new_edge4(I1, colsize, rowsize);
//        b1= 10;
//        b2= 45;
//        b3= 35;
//        b4= 45;
//
//        end=clock();
//        elapsed=((double)end-start)/CLOCKS_PER_SEC;
//        printf("%.2f secondes pour lire l'image. \n",elapsed);
//        start = clock();
//
//        get_edge(I2,&Bord1,&Bord2,&Bord3,&Bord4, rowsize, colsize, b1, b2, b3, b4);
//
//        end=clock();
//        elapsed=((double)end-start)/CLOCKS_PER_SEC;
//        printf("%.2f secondes pour filtrer l'image et obtenir les points sur les bords \n",elapsed);
//        start = clock();
//
//        create_query_tree_short(I2,T,colsize,rowsize);
//        end=clock();
//        elapsed=((double)end-start)/CLOCKS_PER_SEC;
//        printf("%.2f secondes pour faire l'arbre. \n",elapsed);
//        start = clock();
//
//        //WideSearch(T);
//        //Iscorrect(T);
//        get_line_short(I1,T, Bord2, Bord3,Bord1,Bord4,rowsize,colsize, w);
//                end=clock();
//        elapsed=((double)end-start)/CLOCKS_PER_SEC;
//        printf("%.2f secondes pour compter le nb de point\n",elapsed);
//        start = clock();
//
//
//        break;


     case    VFF_TYP_1_BYTE		:
        cerr << "Byte image" << endl;
        I=UCHARDATA(image);

        clean1_char(I, colsize, rowsize);
        //clean2_char(I, colsize, rowsize);

        create_query_tree(I,T,colsize,rowsize);
        WideSearch(T);
        //Iscorrect(T);

        get_edge_char(I, &Bord1,&Bord2,&Bord3,&Bord4, rowsize, colsize);
        get_line(I,T, Bord2,Bord3,Bord1,Bord4,colsize, rowsize,w);
        break;

     default:
         std::cerr << "Unsupported pixel type " << image->data_storage_type <<std::endl;
         exit(1);
         break;
    }



    writeimage(image,argv[argc]);
    freeimage(image);

    end=clock();
    elapsed=((double)end-start)/CLOCKS_PER_SEC;
    printf("%.2f secondes pour fermer l'image. \n",elapsed);


    return(0);
}


