/*
  This software is licensed under
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but
  without any warranty to the extent permitted by applicable law.

  (C) UjoImro, 2011
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/
// The pink python wrapper core file

#include "pink_python.h"
#include "lseltopo.h"
#include "lgeodesic.h"
#include "lfermetrous3d.h"

#include <boost/preprocessor/slot/counter.hpp>

using namespace pink;
using boost::python::arg;
using boost::python::def;

namespace points
{
  UI_WRAP_FUNCTION(
    "ptisolated",
    lptisolated,
    ( arg("image"), arg("connexity") ),
    doc__ptisolated__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
    "ptend",
    lptend,
    ( arg("image"), arg("connexity") ),
    doc__ptend__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
    "ptcurve",
    lptcurve,
    ( arg("image"), arg("connexity") ),
    doc__ptcurve__c__
    );
# include BOOST_PP_UPDATE_COUNTER()


  UI_WRAP_FUNCTION(
    "ptjunction",
    lptjunction,
    ( arg("image"), arg("connexity") ),
    doc__ptjunction__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
    "selectcomp",
    lselectcomp,
    ( arg("image"), arg("connexity"), arg("x"), arg("y"), arg("z") ),
    doc__selectcomp__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
    "holeclosing",
    lfermetrous3dbin,
    ( arg("image"), arg("connexity"), arg("holesize") ),
    doc__holeclosing__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
    "holeclosing",
    lfermetrous3dbin2,
    ( arg("image"), arg("guiding image") ,arg("connexity"), arg("holesize") ),
    doc__holeclosing__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

} /* namespace points */

using namespace points;

void pypoints()
{
  CALL_EXPORTED_FUNCTIONS(BOOST_PP_COUNTER);
}



// LuM end of file




// /*
//   This software is licensed under
//   CeCILL FREE SOFTWARE LICENSE AGREEMENT

//   This software comes in hope that it will be useful but
//   without any warranty to the extent permitted by applicable law.

//   (C) UjoImro, 2011
//   Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
//   ujoimro@gmail.com
// */
// // The pink python wrapper core file

// # include "llpemeyer.h"
// //# include "llpemeyer.h"
// # include "lwshedtopo.h"
// # include "pink_python.h"
// # include "ldynamique_grimaud.h"

// # include <boost/preprocessor/slot/counter.hpp>

// using namespace pink;
// using boost::python::arg;
// using boost::python::def;

// namespace points
// {

// } /* namespace points */

// using namespace points;

// void pypoints()
// {
//   CALL_EXPORTED_FUNCTIONS(BOOST_PP_COUNTER);
// }



// // LuM end of file
