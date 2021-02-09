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

#include "ldist.h"
#include "lcrop.h"
#include "lhtkern.h"
#include "lmedialaxis.h"
#include "lbarycentre.h"
#include "l2dcollapse.h"
#include "l3dcollapse.h"
#include "l2dkhalimsky.h"
#include "l3dkhalimsky.h"
#include "lfiltrestopo.h"
#include "ldetectcercles.h"

#include <boost/preprocessor/slot/counter.hpp>

using namespace pink;
using boost::python::arg;
using boost::python::def;

namespace sept
{
  UI_WRAP_MALLOC(
    "crop",
    lcrop,
    (arg("image"), arg("x"), arg("y"), arg("w"), arg("h")),
    doc__crop__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_MALLOC(
    "crop",
    lcrop3d,
    (arg("image"), arg("x"), arg("y"), arg("z"), arg("w"), arg("h"), arg("d")),
    doc__crop__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_MALLOC(
    "autocrop",
    lautocrop,
    (arg("image"), arg("threshold")),
    doc__autocrop__c__
    );
# include BOOST_PP_UPDATE_COUNTER()
  
  UI_WRAP_RESULT(
    float_image,
    "flowskeleton2d",
    l2dflowskeleton,
    (arg("image"), arg("mode"), arg("level")),
    doc__2dflowskeleton__c__
    )
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_RESULT(
    float_image,
    "flowskeleton3d",
    l3dflowskeleton,
    (arg("image"), arg("mode"), arg("level")),
    doc__3dflowskeleton__c__
    )
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_RESULT(
    char_image,
    "label3d",
    l3dlabel,
    (arg("image")),
    doc__3dlabel__c__
    )
# include BOOST_PP_UPDATE_COUNTER()
  
  UI_WRAP_RESULT(
    char_image,
    "surfacecollapse3d",
    l3dsurfacecollapse,
    (arg("image"), arg("nsteps")),
    doc__3dsurfacecollapse__c__
    )
# include BOOST_PP_UPDATE_COUNTER()
  
  UI_WRAP_RESULT(
    float_image,
    "bisector",
    lmedialaxis_lbisector,
    (arg("image"), arg("mask")),
    doc__bisector__c__
    )
# include BOOST_PP_UPDATE_COUNTER()
  
  UI_WRAP_RESULT(
    float_image,
    "bisector_talbot",
    lmedialaxis_lbisector_talbot,
    (arg("image")),
    doc__bisector_talbot__c__
    )
# include BOOST_PP_UPDATE_COUNTER()
  
  UI_WRAP_FUNCTION(
    "disthmus3d",
    l3disthmus,
    (arg("image")),
    doc__3disthmus__c__
    )
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
    "barycentrelab",
    lbarycentrelab,
    (arg("image")),
    doc__barycentrelab__c__
    )
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
    "crestrestoration",
    lcrestrestoration,
    (arg("image"), arg("imagecond"), arg("nitermax"), arg("connex")),
    doc__crestrestoration__c__
    )
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
    "despics2d",
    ldespics,
    (arg("image"), arg("mask"), arg("connexmin")),
    doc__despics__c__
    )
# include BOOST_PP_UPDATE_COUNTER()
  
  UI_WRAP_FUNCTION(
    "despics3d",
    ldespics3d,
    (arg("image"), arg("mask"), arg("connexmin")),
    doc__despics3d__c__
    )
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
    "despuits",
    ldespuits,
    (arg("image"), arg("mask"), arg("connexmin")),
    doc__despuits__c__
    )
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
    "despuits3d",
    ldespuits3d,
    (arg("image"), arg("mask"), arg("connexmin")),
    doc__despuits3d__c__
    )
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
    "detectcircles",
    ldetectcercles,
    (arg("image"), arg("radius")),
    doc__detectcercles__c__
    )
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_VALUE(
    "distsets",
    ldistsets,
    (arg("image1"), arg("image2"), arg("mode"), arg("cut")=0.),
    doc__distsets__c__
    )
# include BOOST_PP_UPDATE_COUNTER()
  
  
} /* namespace topo */

using namespace sept;

void pysept()
{
  CALL_EXPORTED_FUNCTIONS(BOOST_PP_COUNTER);
}



// LuM end of file
