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

#include <pink.h>

namespace pink {

  int file_size( const std::string & filename )
  {
    int result;
    std::ifstream file;

    file.open( filename.c_str(), std::ios::binary );
    file.seekg( 0, std::ios::end );
    
    result = file.tellg();

    file.seekg( 0, std::ios::beg );
    file.close(); 
    
    return result;
  } /* file_size */

} /* namespace pink */








#endif /*__cplusplus*/
/* LuM end of file */
