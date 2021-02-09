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

#include "lskelpar3d.h"
#include "l2dcollapse.h"
#include "l3dcollapse.h"
#include "lmedialaxis.h"
#include "lbarycentre.h"
#include "l2dkhalimsky.h"
#include "l3dkhalimsky.h"

#include "ui_extractplane.hpp"

#include <boost/preprocessor/slot/counter.hpp>

using namespace pink;
using boost::python::arg;
using boost::python::def;

namespace connect
{
  UI_WRAP_CPP(
    "extractplane",
    pink::extractplane,
    ( arg("image"), arg("number of the plane"), arg("mode") ),
    doc__extractplane__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

// UI_WRAP_VALUE(
//   "l2dinvariants",
//   l2dinvariants,
//   ( arg("image") ),
//   doc__2dinvariants__c__
//   );
// # include BOOST_PP_UPDATE_COUNTER()


// UI_WRAP_VALUE(
//   "countvalues",
//   lcountvalues,
//   ( arg("image"), arg("make") ),
//   "WRITE ME!!"//doc__countvalues__c__
//   );
// # include BOOST_PP_UPDATE_COUNTER()

// UI_WRAP_VALUE(
//   "invariants3d",
//   l3dinvariants,
//   ( arg("image"), arg("make") ),
//   "WRITE ME!!"//doc__countvalues__c__
//   );
// # include BOOST_PP_UPDATE_COUNTER()


  // THIS FUNCTION HAS BEEN REPLACED BY INVERSE
//   UI_WRAP_FUNCTION(
//     "invert",
//     linvert,
//     (arg("image")),
//     doc__inverse__c__
//     );
// # include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
    "barycentre",
    lbarycentre,
    (arg("image"), arg("connvex")),
    doc__barycentre__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_RESULT(
    float_image,
    "lmedialaxis_lbisector",
    lmedialaxis_lbisector,
    (arg("image"), arg("mask")),
    doc__bisector__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_RESULT(
    float_image,
    "lambdamedialaxis",
    lmedialaxis_lambdamedialaxis,
    (arg("image")),
    doc__lambdamedialaxis__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
    "border2d",
    l2dborder,
    (arg("image") ),
    doc__2dborder__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
    "color2d",
    l2dcolor,
    (arg("image") ),
    doc__2dcolor__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
    "color2d",
    l2dcolor,
    (arg("image") ),
    doc__2dcolor__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
    "makecomplex2d",
    l2dmakecomplex,
    (arg("image") ),
    doc__2dmakecomplex__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
    "closebeta2d",
    l2dclosebeta,
    (arg("image") ),
    doc__2dmakecomplex__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
    "pardircollapse2d",
    l2dpardircollapse,
    (arg("image"), arg("nsteps"), arg("inhibit") ),
    doc__2dpardircollapse__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
    "pardircollapse2d",
    l2dpardircollapse_short,
    (arg("image"), arg("nsteps") ),
    doc__2dpardircollapse__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
    "seltype2d",
    l2dseltype,
    (arg("image"), arg("d-"), arg("d+"),arg("a-"),arg("a+"),arg("b-"),arg("b+")),
    doc__2dseltopo__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
    "thin2d",
    l2dthin,
    (arg("image") ),
    doc__2dthin__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
    "alpha3d",
    l3dalpha,
    (arg("image") ),
    doc__3dalpha__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
    "beta3d",
    l3dbeta,
    (arg("image") ),
    doc__3dbeta__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
    "color3d",
    l3dcolor,
    (arg("image") ),
    doc__3dcolor__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
    "isthmus3d",
    l3disthmus,
    (arg("image") ),
    doc__3disthmus__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
    "makecomplex3d",
    l3dmakecomplex,
    (arg("image") ),
    doc__3dmakecomplex__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
    "pardircollapse3d",
    l3dpardircollapse,
    (arg("image"), arg("nsteps"), arg("inhibit") ),
    doc__3dpardircollapse__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
    "pardircollapse3d",
    l3dpardircollapse_short,
    (arg("image"), arg("nsteps") ),
    doc__3dpardircollapse__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
    "plane3d",
    l3dplane,
    ( arg("image"), arg("a"), arg("b"), arg("c"), arg("d") ),
    doc__3dplane__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
    "seltype3d",
    l3dseltype,
    (arg("image"), arg("d-"), arg("d+"),arg("a-"),arg("a+"),arg("b-"),arg("b+")),
    doc__3dseltopo__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
    "skelsurf3d",
    l3dskelsurf,
    (arg("image"), arg("nsteps") ),
    doc__3dskelsurf__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
    "sphere3d",
    l3dsphere,
    (arg("image"), arg("x0"), arg("y0"), arg("z0"), arg("radius")),
    doc__3dsphere__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
    "thin3d",
    l3dthin,
    (arg("image"), arg("nsteps") ),
    doc__3dthin__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
    "skel_ACK3",
    lskelACK3,
    (arg("image"), arg("nsteps"), arg("inhibit")),
    doc__skel_ACK3__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  

} /* namespace points */

using namespace connect;

void pyconnect()
{
  CALL_EXPORTED_FUNCTIONS(BOOST_PP_COUNTER);
}



// LuM end of file
