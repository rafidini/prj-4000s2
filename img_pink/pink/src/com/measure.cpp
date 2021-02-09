/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by applicable law.
  
  (C) UjoImro, 2010
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/

int main()
{
  return 0;  
}


//*******************************************************************************
//*******************************************************************************
//*******************************************************************************
//******************** THIS FILE NEEDS MINOR REVISION OR REMOVAL ****************
//*******************************************************************************
//*******************************************************************************
//*******************************************************************************


/*!
   \file measure.cpp

   \brief Normalized absolute measure calculated from the image.
   
   <B>Usage:</B> measure( (float_image)image) -> float_image
   
   <B>Description:</B> 

   <B>Types supported:</B> float 2d, float 3d
   
   <B>Category:</B> signal, development
   \ingroup signal, development

   <b>References:</b>
   <br>[1] <href url="http://www.gnu.org/software/gsl"> GNU Scientific Library </href>
   \author Laszlo Marak, 2009-2010

 */



// #include "pink.h"

// using namespace std;
// using namespace pink;

// #define epsilon 0.00001

// int main(int argc, char * argv[]){

//   if ( argc!=2 + 1 )
//   {
//     cerr << "\n\nNormalized absolute measure calculated from the image.\n";
//  //                        0           1                  2 
//     cerr << "usage: ./measure src(float_image) result(float_image)\n\n";
//     exit(1);
//   } /* if (argc!=2 + 1) */
  

  


//   PTR<float_image> src( new float_image(argv[1]) );

//   float min, max;
//   pair<float, float> mm;
  
  
//   mm = lminmaxval( *src );
//   min = mm.first;
//   max = mm.second;

  
//   if ( min == max )
//   {
//     pink_error("All pixels are equal in the image.");
//   } /* if */

//   PRINTIMAGE(src);
  
  
//   src = lnormalize( *src );

//   PTR<float_image> result = uiGradientAbs( *src );


//   PRINTIMAGE(result);


//   mm = lminmaxval( *result );
//   min = mm.first;
//   max = mm.second;

  
//   if ( min == max )
//   {
//     pink_error("All pixels are equal in the image.");
//   } /* if */


//   FOR(q, result->get_size().prod())
//   {
//     (*result)[q] =  1. / ( epsilon + (*result)[q] );
//   } /* FOR */

//   result = lnormalize( *result );

//   result->_writeimage(argv[2]);

// } /* main */









































//LuM end of file
