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
#include "lgeo.h"
#include "lkern.h"
#include "ldraw.h"
#include "ljones.h"
#include "lnbvois.h"
#include "lselrect.h"
#include "lseltopo.h"
#include "lhthiniso.h"
#include "llevialdi.h"
#include "lgeodesic.h"
#include "lrotations.h"
#include "ltopotypes.h"
#include "lremspnoise.h"
#include "lmaxdiameter.h"
#include "lhoughcercles.h"
#include "lsegmentlignes.h"
#include "lreconsplateaux.h"


#include <boost/preprocessor/slot/counter.hpp>

using namespace pink;
using boost::python::arg;
using boost::python::def;

namespace topo
{
  UI_WRAP_RESULT(
    char_image,
    "houghcircles",
    lhoughcercles,
    (arg("image"), arg("min radius"), arg("pas radius"), arg("nb pas")),
    doc__houghcercles__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
    "hthiniso",
    lhthiniso,
    (arg("image"), arg("dmax"), arg("connex"), arg("pixwhratio")),
    doc__hthiniso__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
    "jones",
    ljones,
    (arg("image"), arg("connex")),
    doc__jones__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
    "kern",
    lkern,
    (arg("image"), arg("connex")),
    doc__kern__c__
    );
# include BOOST_PP_UPDATE_COUNTER()


// optionally lrao instead of lleviandi
  UI_WRAP_FUNCTION(
    "levialdi",
    llevialdi,
    (arg("image"), arg("connex"), arg("n")),
    doc__levialdi__c__
    );
# include BOOST_PP_UPDATE_COUNTER()


  UI_WRAP_FUNCTION(
    "line",
    ldrawline2,
    (arg("image")),
    doc__line__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

// // NOTE: localextrema's allocating the same type
// UI_WRAP_RESULT(
//   "localextrema",
//   llocalextrema,
//   (arg("image"), arg("connex"), arg("minim")),
//   doc__localextrema__c__
//   );
// # include BOOST_PP_UPDATE_COUNTER()


  UI_WRAP_FUNCTION(
    "matchellipse",
    lmatchellipse,
    (arg("image"), arg("connex")),
    doc__matchellipse__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
    "matchrect",
    lmatchrect,
    (arg("image"), arg("connex"), arg("mode")),
    doc__matchrect__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
    "maxdiameter",
    lmaxdiameter,
    (arg("image"), arg("connex")),
    doc__maxdiameter__c__
    );
# include BOOST_PP_UPDATE_COUNTER()


  UI_WRAP_FUNCTION(
    "nbvois",
    lnbvois,
    (arg("image"), arg("connex")),
    doc__nbvois__c__
    );
# include BOOST_PP_UPDATE_COUNTER()


  UI_WRAP_FUNCTION(
    "pthseparatinggray",
    lpthseparatinggray,
    (arg("image"), arg("connex"), arg("h")),
    doc__pthseparatinggray__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
    "ptinterior",
    lptinterior,
    (arg("image"), arg("connex")),
    doc__ptinterior__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
    "ptjunction",
    lptjunction,
    (arg("image"), arg("connex")),
    doc__ptjunction__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
    "ptmultiple",
    lptmultiple,
    (arg("image"), arg("connex")),
    doc__ptmultiple__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
    "ptseparating",
    lptseparating,
    (arg("image"), arg("connex")),
    doc__ptseparating__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
    "ptseparatinggray",
    lptseparatinggray,
    (arg("image"), arg("connex")),
    doc__ptseparatinggray__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
    "ptsimple",
    lptsimple,
    (arg("image"), arg("connex")),
    doc__ptsimple__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
    "quasishear",
    lquasishear,
    (arg("image"), arg("theta"), arg("x"), arg("y")),
    doc__quasishear__c__
    );
# include BOOST_PP_UPDATE_COUNTER()


// NOTE: this function returns a pointer
// UI_WRAP_FUNCTION(
//   "quasishear",
//   lquasishear2,
//   (arg("image"), arg("theta")),
//   doc__quasishear__c__
//   );
// # include BOOST_PP_UPDATE_COUNTER()


// NOTE: recalagerigide returns a list

  UI_WRAP_FUNCTION(
    "reconsplateaux",
    lreconsplateaux,
    (arg("image1"), arg("image2"), arg("connexity")),
    doc__reconsplateaux__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
    "remspnoise",
    lremspnoise,
    (arg("image"), arg("g"), arg("k")),
    doc__remspnoise__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

// NOTE: think about image resizing
// UI_WRAP_FUNCTION(
//   "rotate",
//   lrotationInner1,
//   (arg("image"), arg("theta")),
//   doc__rotate__c__
//   );
// # include BOOST_PP_UPDATE_COUNTER()

// UI_WRAP_FUNCTION(
//   "rotate",
//   lrotationInner2,
//   (arg("image"), arg("theta"), arg("x"), arg("y")),
//   doc__rotate__c__
//   );
// # include BOOST_PP_UPDATE_COUNTER()


  UI_WRAP_FUNCTION(
    "segmentlines",
    lsegmentlignes,
    (arg("image"), arg("connexity"), arg("lower threshold"), arg("higher threshold")),
    doc__segmentlignes__c__
    );
# include BOOST_PP_UPDATE_COUNTER()


  UI_WRAP_FUNCTION(
    "selectcomp",
    lselectcomp,
    (arg("image"), arg("connex"), arg("x"), arg("y"), arg("z")),
    doc__selectcomp__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
    "selrect",
    lselrect,
    (arg("image"), arg("x"), arg("y"), arg("w"), arg("h")),
    doc__selrect__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
    "seltopo",
    lseltopo,
    (arg("image"), arg("connexity"), arg("t-"), arg("t+"), arg("tb-"), arg("tb+")),
    doc__seltopo__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
    "alpha",
    lalpha,
    ( arg("image"), arg("connexity"), arg("sign") ),
    doc__alpha__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
    "amont",
    lamont,
    ( arg("image"), arg("connexity"), arg("sign") ),
    doc__amont__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
    "drawline",
    ldrawline,
    (arg("image"), arg("x1"), arg("y1"), arg("x2"), arg("y2")),
    doc__drawline__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
    "drawline",
    ldrawline3d,
    (arg("image"), arg("x1"), arg("y1"), arg("z1"), arg("x2"), arg("y2"), arg("z2")),
    doc__drawline__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
    "ellipticite",
    lellipticite,
    (arg("image"), arg("connex")),
    doc__ellipticite__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
    "drawtorus",
    ldrawtorus,
    (arg("image"), arg("r1"), arg("r2"), arg("xc"), arg("yc"), arg("zc")),
    doc__drawtorus__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
    "drawball",
    ldrawball,
    (arg("image"), arg("radius"), arg("xc"), arg("yc"), arg("zc")),
    doc__drawball__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
    "deletecomp",
    ldeletecomp,
    (arg("image"), arg("connex"), arg("x"), arg("y"), arg("z")),
    doc__deletecomp__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
    "boxmin",
    lboxmin,
    (arg("image")),
    doc__boxmin__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
    "surrect",
    lrectangleincluant,
    (arg("image"), arg("connexity")),
    doc__surrect__c__
    );
# include BOOST_PP_UPDATE_COUNTER()


  
} /* namespace topo */

using namespace topo;

void pytopo()
{
  CALL_EXPORTED_FUNCTIONS(BOOST_PP_COUNTER);
}



// LuM end of file
