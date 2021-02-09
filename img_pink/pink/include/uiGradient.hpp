/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by applicable law.
  
  (C) UjoImro, 2006
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/

#ifndef UIGRADIENT_H_
#define UIGRADIENT_H_

/** \file uiGradient.h
 * \brief Header file for uiGradient.cpp
 *
 * Using interpolation capabilities of GSL. It calculates derivates of the diskret function in every point where the function is defined the step is considered to be 1 so far.
 */

/* using interpolation capabilities of GSL */
/* returns derivates of the diskrete function 
   in every point where the function is defined
   the step is considered to be 1 so far*/

 //PTR<vector<uiVal_type> > uiDerivate( vector<uiVal_type> & values );
 //PTR<uiScalarField> uiGradientAbs( PTR<uiScalarField> & f );
 //PTR<uiScalarField> uiDirectionalGrad( PTR<uiScalarField> & f );
 //PTR<uiScalarField> uiDirGradSpecial( PTR<uiScalarField> & f );

#include "ujimage.hpp"

namespace pink {

  float_image uiGradientAbs( float_image f );

};


#endif /*UIGRADIENT_H_*/
/* LuM end of file */
