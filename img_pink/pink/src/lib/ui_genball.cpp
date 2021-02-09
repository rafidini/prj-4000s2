/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by applicable law.
  
  (C) UjoImro, 2010
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/

#ifdef __cplusplus

#include "ujimage.hpp"

// you shouldn't use one character macros
#undef N

using namespace std;



namespace pink {

  char_image genball( double radius, int dim )
  {
    char_image result;
    
    if (dim==2) 
    {
      pink::types::vint size(2), curr(2), tmp(2);
      size << 2*radius+1, 2*radius+1;
      
      result=char_image(size);
      result.get_center() << radius, radius;      

      FOR(q, result.get_size()[0])
      {    
        FOR(w, result.get_size()[1])
        {
          curr << radius-q, radius-w;          
          if (curr.fabs() <= radius)
          {
            tmp << q,w;            
            result[tmp]=255;            
          }
        } /* FOR */
      } /* FOR */      
    }
    else /* NOT dim == 2 */
    {
      if (dim==3)
      {               
        pink::types::vint size(3), curr(3), tmp(3);
        size << 2*radius+1, 2*radius+1, 2*radius+1;
      
        result=char_image(size);
        result.get_center() << radius, radius, radius;        

        FOR(q, result.get_size()[0])
        {    
          FOR(w, result.get_size()[1])
          {
            FOR(e, result.get_size()[2])
            {
              curr << radius-q, radius-w, radius-e;
              if (curr.fabs()<=radius)
              {
                tmp << q,w,e;            
                result[tmp]=255;            
              }
            } /* FOR */            
          } /* FOR */
        } /* FOR */      
        
      }
      else /* NOT dim == 3 */
      {
        pink_error("only 2D and 3D supported in this moment.");           
      } /* NOT dim == 3 */      
    } /* NOT dim == 2 */

    
      return result;  
    
  } /* genball */
  




  
} /* namespace pink */








#endif /*__cplusplus*/
/* LuM end of file */
