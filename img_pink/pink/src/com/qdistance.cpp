/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by applicable law.
  
  (C) UjoImro, 2010
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/
// This file is the bash front-end of the ungerflow function

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


// #include <pink.h>

// #undef error
// #define error(msg) {stringstream fullmessage; fullmessage << "in qdistance.cpp: " << msg; call_error(fullmessage.str());}

// using namespace std;
// using namespace pink;


// template <class image_type>
// void evaluate( const deep_xvimage & image_A, 
// 	       const deep_xvimage & image_B 
//   )
// {
//   PTR<image_type> ujimage_A( new image_type( image_A ) );
//   PTR<image_type> ujimage_B( new image_type( image_B ) );
//   cout << "The QDistance is: " << lQDistance( *ujimage_A, *ujimage_B ) << endl;
// } /* evaluate */


// int main(int argc, char * argv[])
// {
  
//   if ( argc != 2 + 1 )
//   {
//     //                        0       1         2    
//     cerr << "\n\nusage: ./qdistance image_A image_B\n";    
//     exit(1);
//   } /* if (argc != 2 + 1) */


//   PTR<deep_xvimage> image_A = py_readimage( argv[1] );
//   PTR<deep_xvimage> image_B = py_readimage( argv[2] ); 

//   switch (image_A->data_storage_type)
//   {
    
// //   case VFF_TYP_1_BYTE:
// //     evaluate<char_image>( *image_A, *image_B );
// //     break;
    
// //   case VFF_TYP_2_BYTE:
// //     evaluate<short_image>( *image_A, *image_B );
// //     break;
    
//   case VFF_TYP_4_BYTE:
//     evaluate<int_image>( *image_A, *image_B );
//     break;
    
//   case VFF_TYP_FLOAT:
//     evaluate<float_image>( *image_A, *image_B );
//     break;
    
// //   case VFF_TYP_DOUBLE:
// //     evaluate<double_image>( *image_A, *image_B );
// //     break;    
    
//   default:
//     cerr << "Non of the supported imagetypes could be recognized.\n";
//     error("supported types PGM char, short, int, float, double");
//   } /* switch */



//   return 0;  

// } /* main */









































//LuM end of file
