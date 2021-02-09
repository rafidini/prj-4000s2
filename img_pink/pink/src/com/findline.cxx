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
/*! \file findline.cxx

\brief find line into an image using bresenham definition
w is the digital line size.

<B>Usage:</B> findline in.fits {w}  out.pgm

<B>Description:</B>
On commence pas netoyer l'image afin de retirer le bruit de fond et d'ameliorer le contraste.

On determine ensuite les points sur le bord de l'image qui correspondent potentiellement à une droite.

On trace les droites de Bresenham entre les differents points des bords. Pour chacunes des droites on calcule
le nombre de points non nul present sur la droite. La droites contenant les plus de points non nul doit correspondre
à la trace du satellite.

L'image finale est une image noire, avec une droite blanche correspondant à la trace du sattelite.


Parameters:
\li \b in.fits : source file in fits format


\author Juliette Charpentier
*/

/* Juliette Charpentier, mai 2011 */

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
#  include "liarp.h"
#  include "liar_fseries.h"
#  include "larith.h"
#  include "lfits2pgm.h"
#  include "lfindline.hxx"
#  include "lfindline_char.hxx"




using namespace std;




#define INT4_TYPE int


/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */

{
//    clock_t start, end;
//    double elapsed;
//    start = clock();


    struct xvimage *image, *image2;
    char *filename;
    vector<int32_t> Bord1, Bord2, Bord3, Bord4;
    int32_t rowsize, colsize;
    int w=3;
    int b1, b2, b3, b4;
    double seuil;
    double zero;


    if ((argc != 2) && (argc != 3))
    {
        fprintf(stderr, "usage: %s in.fits nb zero\n", argv[0]);
        exit(1);
    }

//    seuil=atof(argv[2]);
//    zero=atof(argv[3]);
    seuil=600;
    zero=1;
    //std::cerr << seuil<<' '<< zero <<std::endl;

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


    uint16_t *I1, *I2, *I3;
    uint8_t *I;

    switch (image->data_storage_type) {
     case    VFF_TYP_2_BYTE		:
        //std::cerr << "short image"  <<std::endl;

        image2= tophat(image, w);
        I1=USHORTDATA(image2);
//        b1= new_edge1(I1, colsize, rowsize);
//        b2= new_edge2(I1, colsize, rowsize);
//        b3= new_edge3(I1, colsize, rowsize);
//        b4= new_edge4(I1, colsize, rowsize);
//        b1= 10;
//        b2= 45;
//        b3= 35;
//        b4= 55;
        b1= 400;
        b2= 100;
        b3= 100;
        b4= 200;


//        end=clock();
//        elapsed=((double)end-start)/CLOCKS_PER_SEC;
//        printf("%.2f secondes pour lire l'image et faire le tophat. \n",elapsed);
//        start = clock();

        get_edge(I1,&Bord1,&Bord2,&Bord3,&Bord4, rowsize, colsize, b1, b2, b3, b4);

//        end=clock();
//        elapsed=((double)end-start)/CLOCKS_PER_SEC;
//        printf("%.2f secondes pour filtrer l'image et obtenir les points sur les bords. \n",elapsed);
//        start = clock();

        get_best_line(I1, Bord2, Bord3,Bord1,Bord4,rowsize,colsize, w, b1, b2, b3, b4, seuil, zero);

//        end=clock();
//        elapsed=((double)end-start)/CLOCKS_PER_SEC;
//        printf("%.2f secondes pour bresenham. \n",elapsed);
//        start = clock();
        break;

     case    VFF_TYP_1_BYTE		:
        std::cerr << "Char image "  <<std::endl;
        I=UCHARDATA(image);
        //clean1_char(I, colsize, rowsize);
        //clean4_char(I, colsize, rowsize);

        get_edge_char(I, &Bord1,&Bord2,&Bord3,&Bord4, rowsize, colsize);
        get_best_line_char(I, Bord2, Bord3,Bord1,Bord4,rowsize,colsize, w);
        break;

     default:
         std::cerr << "Unsupported pixel type " << image->data_storage_type <<std::endl;
         exit(1);
         break;
    }



    writeimage(image2,argv[2]);
    freeimage(image);
    freeimage(image2);
//
//    end=clock();
//    elapsed=((double)end-start)/CLOCKS_PER_SEC;
//    printf("%.2f secondes pour fermer l'image. \n",elapsed);

    return(0);

    }


