/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by applicable law.
  
  (C) UjoImro, 2009-2010
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/
// This file is the bash front-end of the maxflow function


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
   \file maxflow.cpp

   \brief Calculates the optimal surface segmentation with the maximum
   flow algorithm [1].
   
   <B>Usage:</B> maxflow( (char_image)source and sink, (float_image)constraint image, (int)iterations, (float)tau [, (int)number of threads=0]) -> float_image
   
   <B>Description:</B> In the input, you specify the constraint image
   'g' and two subsets of the image 'S'==1 and 'P'==-1. The operator
   will return a binary flow 'result', which is 0 or 1 almost
   everywhere. The surface around the volume with value 1 will be the
   optimum in the sense, that it will contain 'S', not contain 'P' and
   and it's integral will be the smallest possible.

   Generally, we choose the inverse of the gradient for 'g'. Sometimes
   the inverse is weighted circularly, so gaps in the border are
   interpolated with circles. 

   Usually the values, that are known to be in the object (which you
   want to segment) are set to sink (value 1). You can set sink (value
   -1) the points, which you know to be outside the object, but
   generally the border is sufficient.
   
   <B>Types supported:</B> float 2d, float 3d, float 4d, ..., float xd
   
   <B>Category:</B> signal, development
   \ingroup  signal, development
   
   <b>References:</b> [1] Appleton, B. and Talbot, H. (2006). Globally minimal
   surfaces by continuous maximal ﬂows. IEEE Transactions on Pattern
   Analysis and Machine Intelligence, 28(1):106–118.

   \author Laszlo Marak, 2009-2010


 */

// #include <pink.h>

// #undef error
// #define error(msg) {stringstream fullmessage; fullmessage << "in maxflow.cpp: " << msg; call_error(fullmessage.str());}

// using namespace std;
// using namespace pink;


// template<class image_type>
// PTR<image_type> frame_it( 
//   PTR<image_type> src, 
//   typename image_type::pixel_type withval 
//   ){
    
//   if (src->get_size().size() != 2) // 3D
//   {
    
//     vint new_size(3);
        
//     // calculating the new sizes (one on each side)
//     new_size[0]=src->get_size()[0]+2;
//     new_size[1]=src->get_size()[1]+2;
//     new_size[2]=src->get_size()[2]+2;
    
//     PTR<image_type> result( new image_type(new_size) );
    
//     vint pos(3);
    
//   // filling up the borders with sinks
//     FOR(q, new_size[0]) {
//       FOR(w, new_size[1]) { 
// 	FOR(e, new_size[2]) {
	  
// 	  if ( 
// 	    // lower side
// 	    (q==0) or (w==0) or (e==0) or
// 	    // upper side
// 	    (q==new_size[0]-1) or (w==new_size[1]-1) or (e==new_size[2]-1) 
// 	    )
// 	  {
// 	    pos[0]=q;
// 	    pos[1]=w;
// 	    pos[2]=e;
// 	    (*result)[pos] = withval;
// 	  } /* if on the lower or the upper side */
// 	} /* FOR e */
//       } /* FOR w */
//     } /* FOR q */
    

//   vint shifted_pos(3);

//   // copying the rest of the image
//   FOR( q, src->get_size()[0])
//   {
//     FOR( w, src->get_size()[1])
//     {
//       FOR( e, src->get_size()[2])
//       {
// 	pos[0]=q;
// 	pos[1]=w;
// 	pos[2]=e;
	
// 	shifted_pos[0]=q + 1;
// 	shifted_pos[1]=w + 1;
// 	shifted_pos[2]=e + 1;
	
// 	(*result)[shifted_pos]=(*src)[pos];
//       } /* FOR e */
//     } /* FOR w */
//   } /* FOR q */
//   return result;
//   //error("This instance can only handle 2D images");
//   } 
//   else /* NOT src->get_size().size() != 2 */
//   {
//     vint new_size(2);
    
//     // calculating the new sizes (one on each side)
//     new_size[0]=src->get_size()[0]+2;
//     new_size[1]=src->get_size()[1]+2;
    
//     PTR<image_type> result( new image_type(new_size) );
    
//     vint pos(2);
    
//     // filling up the X borders with sinks
//     FOR(q, new_size[0]){
//       // lower side
//       pos[0]=q;
//       pos[1]=0;
//       (*result)[pos] = withval;
      
//       // upper side
//       pos[0]=q;
//       pos[1]=new_size[1] - 1;;
//       (*result)[pos] = withval;
      
//     } /* FOR */
    
    
//     // filling up the Y borders with sinks
//     FOR(w, new_size[1]){
//       // lower side
//       pos[0]=0;
//       pos[1]=w;
//       (*result)[pos] = withval;
      
//       // upper side
//       pos[0]=new_size[0] - 1;
//       pos[1]=w;
//       (*result)[pos] = withval;     
      
//     } /* FOR */
    
    
//     vint shifted_pos(2);
    
//     // copying the rest of the image
//     FOR( q, src->get_size()[0])
//     {
//       FOR( w, src->get_size()[1])
//       {
// 	pos[0]=q;
// 	pos[1]=w;
	
// 	shifted_pos[0]=q + 1;
// 	shifted_pos[1]=w + 1;
	
// 	(*result)[shifted_pos]=(*src)[pos];
//       } /* FOR w*/
//     } /* FOR q */
//     return result;
//   } /* NOT src->get_size().size() != 2 */ 
  
// } /* frame_it */


// template<class image_type>
// PTR<image_type> crop_the_frame( PTR<image_type> src ){
  
//   if (src->get_size().size() != 2) //3D case
//   {
//     vint new_size(3);

  
//     // calculating the new sizes (one on each side)
//     new_size[0]=src->get_size()[0]-2;
//     new_size[1]=src->get_size()[1]-2;
//     new_size[2]=src->get_size()[2]-2;

//     PTR<image_type> result( new image_type(new_size) );
  
//     vint pos(3);
//     vint shifted_pos(3);
    
//     // copying the middle of the image
//     FOR( q, new_size[0])
//     {
//       FOR( w, new_size[1])
//       {
// 	FOR( r, new_size[2])
// 	{
// 	  pos[0]=q;
// 	  pos[1]=w;
// 	  pos[2]=r;
	  
// 	  shifted_pos[0]=q + 1;
// 	  shifted_pos[1]=w + 1;
// 	  shifted_pos[2]=r + 1;
	  
// 	  (*result)[pos]=(*src)[shifted_pos];
// 	}/* FOR r*/
//       } /* FOR w*/
//     } /* FOR q */
    
  
//     return result;
//   } 
//   else /* NOT src->get_size().size() != 2 */
//   {
//     vint new_size(2);
    
    
//     // calculating the new sizes (one on each side)
//     new_size[0]=src->get_size()[0]-2;
//     new_size[1]=src->get_size()[1]-2;
    
//     PTR<image_type> result( new image_type(new_size) );
    
//     vint pos(2);
//     vint shifted_pos(2);
    
//     // copying the middle of the image
//     FOR( q, new_size[0])
//     {
//       FOR( w, new_size[1])
//       {
// 	pos[0]=q;
// 	pos[1]=w;
	
// 	shifted_pos[0]=q + 1;
// 	shifted_pos[1]=w + 1;
	
// 	(*result)[pos]=(*src)[shifted_pos];
//       } /* FOR w*/
//     } /* FOR q */
    
//     return result;
//   } /* NOT src->get_size().size() != 2 */
// } /* crop_the_frame */


// int main(int argc, char * argv[]){

//   if ( (argc!=6) and (argc!=7) ){
//     //                       0              1           2     3       4           5                6
//     cerr << "\n\nusage: ./maxflow number_of_iterations tau G_image src_sink stable_image [ number_of_threads ]\n\n";
//     exit(1);
//   }; /* if (argc!=5) */
  

//   // reading the measure field

//   PTR<deep_xvimage> _gg;
//   PTR<float_image> gg;

//   try {

//     cout << "reading '" << argv[3] << "'\n";
//     _gg = py_readimage( argv[3] );
//     gg.reset( new float_image( *_gg ) );
    
//   } 
//   catch (...)
//   {
//     error("cannot read file");
//   }


//   // Normalization of the image

// //   FOR(q, gg->get_size().prod()){    
// //     (*gg)[q]=  (*gg)[q]/ 255.0;
// // //    (*gg)[q]=1.;
// //       //(*gg)[q]= 1- (*gg)[q]/ 255.0 + 0.01;

// //   } /* FOR */

// //  gg->_write_amira("test.am");

//   // reading the src and the sink

//   PTR<deep_xvimage> _srcsink;
//   PTR<char_image> srcsink;

//   try {

//     cout << "reading '" << argv[4] << "'\n";
//     _srcsink = py_readimage( argv[4] );
//     srcsink.reset( new char_image( *_srcsink, "srcsink" ) );
    
//   }
//   catch (...)
//   {
//     error("cannot read file");
//   }

//   FOR(q, srcsink->get_size().prod()){
//     if ((*srcsink)[q] == 255)
//     {
//       (*srcsink)[q]=1;
//     } /* (*srcsink)[q] == 255 */
//   } /* FOR */

//   // FOR(q, srcsink->get_size().prod()){

//   //   int current_pix = (*srcsink)[q];

//   //   if ( current_pix < 100 )
//   //   {
//   //     (*srcsink)[q] = 1;
//   //   } 
//   //   else /* NOT (*srcsink)[q] < 100 */
//   //   {
//   //     if ( current_pix > 180 )
//   //     {
//   // 	(*srcsink)[q] = -1;
//   //     } 
//   //     else /* NOT (*srcsink)[q] > 145 */
//   //     {
//   // 	(*srcsink)[q] = 0;
//   //     } /* NOT (*srcsink)[q] > 145 */	  
//   //   } /* NOT (*srcsink)[q] < 100 */ 
    
//   // } /* FOR */
    

// //  cout << minval(*gg) << " < gg < " << maxval(*gg) << "\n";

// //  srcsink->_writeimage("srcsink_converted.pgm");
// //  gg->_writeimage("g_converted.pgm");


//   // converting tau and the number of iterations to proper formats

//   float tau = lexical_cast<float>(argv[2]);

//   int iterations = lexical_cast<int>(argv[1]);

//   int number_of_threads = 0;

//   if (argc==7)
//   {
//     number_of_threads = lexical_cast<int>(argv[6]);
//   } /* (argc==7) */


//   // adding the frames to the images
//   PTR<char_image> framed_srcsink;
//   PTR<float_image> framed_gg;

//   framed_srcsink = frame_it( srcsink, -1 );
//   framed_gg = frame_it( gg, 0 );

// //  crop_the_frame(framed_srcsink)->_writeimage("unframed_srcsink.pgm");
// //  cout << minval(*framed_gg) << " < framed_gg < " << maxval(*framed_gg) << "\n";
// //  crop_the_frame(framed_gg)->_writeimage("unframed_gg.pgm");

// //  framed_srcsink->_writeimage("framed_srcsink.pgm");
// //  framed_gg->_writeimage("framed_gg.pgm");

//   framed_gg->_writeimage("framed_gg.pgm");
//   framed_srcsink->_writeimage("framed_srcsink.pgm");

//   PTR<float_image> result = maxflow_float( *framed_srcsink, *framed_gg, iterations, tau, number_of_threads );

// //  result->_writeimage("pure_result.pgm");

//   //removing the frame from the result
//   PTR<float_image> unframed_result;
//   unframed_result = crop_the_frame( result );

//   unframed_result->_writeimage(argv[5]);

// //  cout << "writing the result in 'stab.am'\n";
// //  unframed_result->_write_amira("stab.am");

// //  result->_writeimage(argv[5]);

// //   PTR<deep_xvimage> _gg  = py_readimage("/home/ujoimro/tmp/uigradient/g.pgm");
// //   PTR<float_image> gg(new float_image(*_gg));

// //   PTR<float_image> result = uiGradientAbs( *gg );
// //   result->_writeimage("/home/ujoimro/tmp/uigradient/g_der.pgm");


// } /* main */









































//LuM end of file
