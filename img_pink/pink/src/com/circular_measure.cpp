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
//******************** THIS FILE NEEDS MAJOR REVISION OR REMOVAL ****************
//*******************************************************************************
//*******************************************************************************
//*******************************************************************************

// This file is the bash front-end of the lmeasure function

// #include <pink.h>

// #undef error
// #define error(msg) {stringstream fullmessage; fullmessage << "in circular_measure.cpp: " << msg; call_error(fullmessage.str());}

// using namespace std;
// using namespace pink;

// #define epsilon 0.00001

// int main(int argc, char * argv[]){

//   if ( argc!=4 + 1 )
//   {
//     cerr << "\n\nNormalized absolute measure calculated from the image and weighted with a circular balance\n";
//  //                          0                 1             2       3            4
//     cerr << "usage: ./circular_measure src(float_image)  center_x center_y result(float_image)\n\n";
//     exit(1);
//   } /* if (argc!=2 + 1) */
  

//   PTR<float_image> src( new float_image(argv[1]) );

//   if ( src->get_size().size() != 2 )
//   {
//     error("Prepared only for 2D images.");    
//   } /* src->size.size() != 2 */

//   int center_x = lexical_cast<int>(argv[2]);
//   int center_y = lexical_cast<int>(argv[3]);
  
//   float min, max;

//   pair<float, float> mm;
  
//   mm = lminmaxval( *src );
//   min = mm.first;
//   max = mm.second;
  

//   if ( min == max )
//   {
//     error("All pixels are equal in the image.");
//   } /* if */

//   src = lnormalize( *src );

//   PTR<float_image> result = uiGradientAbs( *src );


//   PRINTIMAGE(result);


//   mm = lminmaxval( *result );
//   min = mm.first;
//   max = mm.second;
  


//   if ( min == max )
//   {
//     error("All pixels are equal in the image.");
//   } /* if */

//   cout << "ici\n";

//   vint curr(2);  
  
//   FOR(q, result->get_size()[0])
//   {    
//     FOR(w, result->get_size()[1])
//     {
//       curr[0]=q;
//       curr[1]=w;      

//       if ( pow( q - center_x, 2 ) + pow( w - center_y, 2 ) > 3. ) // ( not ( ( q==center_x ) and ( w==center_y ) ) )
//       {        
//         (*result)[curr] = 1. / ( ( epsilon + (*result)[curr] ) * ( pow( q - center_x, 2 ) + pow( w - center_y, 2 ) ) );
//       } /* if */
      
//     } /* FOR w */
//   } /* FOR q */

//   _PRINTIMAGE( result );
  
  
//   result = lnormalize( *result );

//   result->_writeimage(argv[4]);

// } /* main */









































//LuM end of file
