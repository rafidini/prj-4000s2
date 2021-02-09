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
#include "ldir.h"
#include "lccv.h"
#include "lhtkern.h"
#include "lconvol3.h"
#include "legalise.h"
#include "lcontours.h"
#include "lhtkern3d.h"
#include "lhistscal.h"
#include "ldirections.h"
#include "ldetectcercles.h"

#include <boost/preprocessor/slot/counter.hpp>

using namespace pink;
using boost::python::arg;
using boost::python::def;

namespace arithmetic
{
  UI_WRAP_FUNCTION(
    "ccv",
    lccv,
    (arg("image"), arg("elem")),
    doc__ccv__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
    "contours",
    lcontours,
    (arg("image"), arg("higher threshold"), arg("lower threshold")),
    doc__contours__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
    "convol3",
    lconvol3,
    (arg("image"), arg("mask"), arg("")),
    doc__convol3__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
    "detectcercles",
    ldetectcercles,
    (arg("image"), arg("radius")),
    doc__detectcercles__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

//NOTE: consider directionalfilter because of the converfloat

  UI_WRAP_FUNCTION(
    "dir",
    ldir,
    (arg("image"), arg("dir")),
    doc__dir__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
    "directions",
    ldirections,
    (arg("image"), arg("connex"), arg("nbpoints"), arg("normale")),
    doc__directions__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
    "dynrecons",
    ldynrecons,
    (arg("image"), arg("cond image"), arg("connexity")),
    doc__dynrecons__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
    "egalise",
    legalise,
    (arg("image"), arg("n")),
    doc__egalise__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

// UI_WRAP_FUNCTION(
//   "fermetrous3d",
//   lfermetrous3dbin,
//   (arg("image"), arg("connex"), arg("hole size")),
//   doc__fermetrous3d__c__
//   );
// # include BOOST_PP_UPDATE_COUNTER()


// NOTE: fft up for discussion
// NOTE: gettree return value type

// UI_WRAP_FUNCTION(
//   "gradill",
//   lgradill,
//   ( arg("image"), arg("alpha") ),
//   doc__gradill__c__
//   );
// # include BOOST_PP_UPDATE_COUNTER()

// UI_WRAP_FUNCTION(
//   "gradinf",
//   lgradinf,
//   (arg("image")),
// /*  doc__gradinf__c__*/ "WRITE ME!!!"
//   );
// # include BOOST_PP_UPDATE_COUNTER()

// UI_WRAP_FUNCTION(
//   "gradinf3d",
//   lgradinf3d,
//   (arg("image")),
//   /* doc__gradinf3d__c__ */ "WRITE ME!!!"
//   );
// # include BOOST_PP_UPDATE_COUNTER()

// UI_WRAP_FUNCTION(
//   "gradsup",
//   lgradsup,
//   (arg("image")),
//   /*doc__gradsup__c__*/ "WRITE ME!!!"
//   );
// # include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
    "histscal",
    lhistscal3,
    (arg("image"), arg("a"), arg("A"), arg("b"), arg("B"), arg("c"), arg("C")),
    doc__histscal__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
    "histscal",
    lhistscal,
    (arg("image"), arg("a"), arg("A"), arg("b"), arg("B")),
    doc__histscal__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
  "surfacerestoration",
  lsurfacerestoration,
  (arg("image"), arg("number of maximal iterations"), arg("connexity")),
  doc__surfacerestoration__c__
  );
# include BOOST_PP_UPDATE_COUNTER()


  
} /* namespace points */

using namespace arithmetic;

void pyarithmetic()
{
  CALL_EXPORTED_FUNCTIONS(BOOST_PP_COUNTER);
}



// LuM end of file
