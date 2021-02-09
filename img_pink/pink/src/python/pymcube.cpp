/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by applicable law.
  
  (C) UjoImro, 2010
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>

#include "larith.h"
#include "mcrbtp.h"
#include "mcmesh.h"
#include "mciomesh.h"
#include "lmcube.h"
#include "pink_python.h"

using namespace boost::python;
using namespace pink;

namespace pink {
  namespace python {
    
    template<class image_type>
    image_type inverse( const image_type & image )
    {
      image_type result;
      result = image.clone();
      
      if (!linverse(result.get_output()))
      {
        pink_error("function linverse failed");
      }
      
      return result;
    } /* inverse */

  
    std::string mcube(
      char_image image,
      int seuil,
      int nregul,
      int obj_id,
      int connex=26
      )
    {
      char_image tmp;      
      if (connex==6)
      {
        tmp = inverse(image);        
      }
      else /* NOT connex== 6 */
      {
        if (connex==26)
        {
          tmp = image;          
        }
        else /* NOT connex== 26 */
        {
          pink_error("bad connexity");
        } /* NOT connex== 26 */
      } /* NOT connex== 6 */
    
      InitLUT();
      InitMesh(1000); /* automatically reallocated if needed */
    
      std::stringstream result;
    
      if (seuil == 255)
      { // with fix-point preservation
        if (! lmarchingcubes2(tmp.get_output(), nregul, obj_id, reinterpret_cast<FILE*>(&result), T_VTK_PYTHON))
        {
          pink_error("function lmarchingcubes2 failed");
        }
      }
      else
      {
        if (! lmarchingcubes(tmp.get_output(), seuil, nregul, obj_id, reinterpret_cast<FILE*>(&result), T_VTK_PYTHON))
        {
          pink_error("function lmarchingcubes failed");
        }
      }
      
      TermineMesh();

      return result.str();      
    } /* mcube */
    
    

  } /* namespace python */
} /* namespace pink */

UI_EXPORT_ONE_FUNCTION(
  mcube,
  pink::python::mcube,
  (arg("image"), arg("seuil"), arg("nregul"), arg("obj_id"), arg("connex")=26),
  doc__mcube__c__
  // end of documentation
  );

















































// LuM end of file
