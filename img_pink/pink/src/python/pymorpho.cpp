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
#include "lstat.h"
#include "ldist.h"
#include "lasft.h"
#include "larith.h"
#include "uiImage.h"
#include "lderiche.h"
#include "ldistgeo.h"
#include "lerosplan.h"
#include "uiFrame.hpp"
#include "lminmax.hpp"
#include "uiInsert.hpp"
#include "lattribvol.h"
#include "lattribute.h"
#include "ldilateros.h"
#include "ldilatbin3d.h"
#include "llambdakern.h"
#include "lattribarea.h"
#include "ldilateros3d.h"
#include "lfiltrestopo.h"
#include "lfiltreordre.h"
#include "lattribheight.h"
#include "llabelextrema.h"


#include <boost/preprocessor/slot/counter.hpp>

using namespace pink;
using boost::python::arg;
using boost::python::def;

namespace morpho
{
  UI_WRAP_FUNCTION(
    "lambdaskel",
    llambdakern,
    ( arg("image"), arg("imcond"), arg("connexity"), arg("lambda") ),
    doc__lambdaskel__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
    "lambdaskel",
    llambdakern_short,
    ( arg("image"), arg("connexity"), arg("lambda") ),
    doc__lambdaskel__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
    "rankfilter",
    lfiltreordre,
    ( arg("image"), arg("structuring element"), arg("center_x"), arg("center_y"), arg("r") ),
    doc__rankfilter__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
    "rankfilter",
    lfiltreordre3d,
    ( arg("image"), arg("structuring element"), arg("center_x"), arg("center_y"), arg("center_z"), arg("r") ),
    doc__rankfilter__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
    "tuf",
    ltuf,
    ( arg("image"), arg("min connex"), arg("radius") ),
    doc__tuf__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
    "taf",
    ltaflambda,
    ( arg("image"), arg("min connex"), arg("radius"), arg("lambdapics"), arg("lambdapuits") ),
    doc__taf__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
    "labelextrema",
    llabelextrema,
    ( arg("image"), arg("connex"), arg("function"), arg("result"), arg("nblabels") ),
    doc__label__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_RESULT(
    int_image,
    "labelfgd",
    llabelfgd,
    ( arg("image"), arg("connex") ),
    doc__labelfgd__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
    "heightmaxima",
    lheightmaxima,
    ( arg("image"), arg("connexity"), arg("height") ),
    doc__heightmaxima__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
    "heightminima",
    lheightminima,
    ( arg("image"), arg("connexity"), arg("height") ),
    doc__heightminima__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

// functions added by Michel

  UI_WRAP_FUNCTION(
    "segmentheight",
    lsegmentheight,
    ( arg("image"), arg("connexity"), arg("height"), arg("mode") ),
    doc__segmentheight__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
    "segmentarea",
    lsegmentarea,
    ( arg("image"), arg("connexity"), arg("area"), arg("mode") ),
    doc__segmentarea__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
    "segmentvol",
    lsegmentvol,
    ( arg("image"), arg("connexity"), arg("vol"), arg("mode") ),
    doc__segmentvol__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
    "heightselnb",
    lheightselnb,
    ( arg("image"), arg("connexity"), arg("height") ),
    doc__heightselnb__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
    "areaselnb",
    lareaselnb,
    ( arg("image"), arg("connexity"), arg("area") ),
    doc__areaselnb__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
    "volselnb",
    lvolselnb,
    ( arg("image"), arg("connexity"), arg("area") ),
    doc__volselnb__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_RESULT(
    int_image,
    "distgeo",
    ldistgeo,
    ( arg("image"), arg("mask"), arg("mode") ),
    doc__distgeo__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_RESULT(
    int_image,
    "attribute",
    lattribute,
    ( arg("image"), arg("connex"), arg("typregion"), arg("attrib"), arg("seuil") ),
    doc__attribute__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_RESULT(
    int_image,
    "planarity",
    lplanarity,
    ( arg("image"), arg("connex") ),
    doc__planarity__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

// // NOTE: affine's using copyimage
// UI_WRAP_RESULT(
//   "affine",
//   laffinetransformation,
//   ( arg("image"), arg("hx"), arg("hy"), arg("theta"), arg("tx"), arg("ty")),
//   doc__affine__c__
//   );
// # include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
    "asf",
    ldilateros_lasf,
    (arg("image"), arg("radius max"), arg("radius min")=1),
    doc__asf__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
    "asft",
    lasft_2D3D,
    ( arg("image"), arg("constraint image"), arg("complementary constraint image"), arg("connexity"), arg("radius max") ),
    doc__asft__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
    "asft",
    lasft_2D3D_null,
    ( arg("image"), arg("connexity"), arg("radius max") ),
    doc__asft__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
    "gaussianfilter",
    lgaussianfilter,
    ( arg("image"), arg("alpha") ),
    doc__gaussianfilter__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
    "gaussianfilter",
    lgaussianfilter,
    ( arg("image"), arg("alpha") ),
    doc__gaussianfilter__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
    "gradientcd",
    lgradientcd,
    ( arg("image"), arg("alpha") ),
    doc__gradientcd__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
    "exp",
    lexp,
    (arg("image")),
    doc__exp__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_CPP(
    "print_image",
    print_image,
    (arg("image")),
    "Prints the values in a table. Usefull for debugging small 2D images."
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_CPP(
    "insert_image",
    insert_image,
    ( arg("big_image"), arg("small_image"), arg("shift_vector") ),
    "this function inserts an image to another one. The "
    "shift vector indicates the position of the lower corner "
    "of the new image. If the image doesn't fit into the big one, "
    "it is cropped."
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_CPP(
    "merge_max_image",
    merge_max_image,
    ( arg("big_image"), arg("small_image"), arg("shift_vector") ),
    "this function inserts an image to another one. The "
    "shift vector indicates the position of the lower corner "
    "of the new image. If the image doesn't fit into the big one, "
    "it is cropped. During the insertion the pixels are compared "
    "and the bigger value is conserved"
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_CPP(
    "frame",
    frame,
    ( arg("image"), arg("with value") ),
    "This function takes an image, and in the result it sets it's most outer rectangle to the given value."
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_CPP(
    "frame_around",
    frame_around,
    ( arg("image"), arg("with value") ),
    "This function takes an image, and in the result and adds a frame around it, with"
    " a given value. THIS FUNCTION CHANGES THE RESULTING IMAGE'S SIZE."
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_CPP(
    "frame_remove",
    frame_remove,
    ( arg("image") ),
    "This function takes an image, and in the result and removes the most outer rectangle from the result. "
    "THIS FUNCTION CHANGES THE RESULTING IMAGE'S SIZE."
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_CPP(
    "max",
    immap_max,
    ( arg("image1"), arg("image2") ),
    "Generates an image result[i]:=max(image1[i],image2[i])"
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_CPP(
    "min",
    immap_min,
    ( arg("image1"), arg("image2") ),
    "Generates an image result[i]:=min(image1[i],image2[i])"
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
    "segmentvol",
    lsegmentvol,
    (arg("image"), arg("connexity"), arg("param"), arg("m")),
    doc__segmentvol__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
    "segmentarea",
    lsegmentarea,
    (arg("image"), arg("connexity"), arg("area"), arg("m")),
    doc__segmentarea__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
    "gammacor",
    lgammacor,
    (arg("image"), arg("gamma")),
    "WRITE ME!!! doc__gammacor__c__"
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
    "pow",
    lpow,
    (arg("image"), arg("p")),
    doc__pow__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
    "log",
    llog,
    (arg("image")),
    doc__log__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
    "inverse",
    linverse,
    (arg("image")),
    doc__inverse__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_VALUE(
    "area",
    larea,
    ( arg("image") ),
    doc__area__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_VALUE(
    "average",
    laverage1,
    ( arg("image") ),
    doc__average1__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_VALUE(
    "average",
    laverage2,
    ( arg("image"), arg("mask") ),
    doc__average1__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
    "grayskel",
    lgrayskel,
    (arg("image"), arg("cond image"), arg("connex"), arg("lambda")),
    doc__grayskel__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
    "grayskel",
    lgrayskel_short,
    (arg("image"), arg("connex"), arg("lambda")),
    doc__grayskel__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
    "erosnum",
    lerosnum,
    (arg("image"), arg("elem"), arg("x"), arg("y")),
    doc__erosnum__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
    "erosplan",
    lerosplan,
    (arg("image"), arg("elem"), arg("x"), arg("y")),
    doc__erosplan__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
    "dilatbin",
    ldilateros_ldilatbin,
    (arg("image"), arg("elem"), arg("x"), arg("y")),
    doc__dilatbin__c__
    );
# include BOOST_PP_UPDATE_COUNTER()


  UI_WRAP_FUNCTION(
    "dilatbin",
    ldilatbin3d,
    (arg("image"), arg("elem"), arg("x"), arg("y"), arg("z")),
    doc__dilatbin__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  
  UI_WRAP_FUNCTION(
    "dilatnum",
    ldilatnum,
    (arg("image"), arg("elem"), arg("x"), arg("y")),
    doc__dilatnum__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
    "lerosbin",
    ldilateros_lerosbin,
    (arg("image"), arg("elem"), arg("x"), arg("y")),
    doc__erosbin__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
    "lerosbin",
    ldilateros3d_lerosbin3d,
    (arg("image"), arg("elem"), arg("x"), arg("y"), arg("z")),
    doc__erosbin3d__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
    "ecarttype",
    lecarttype,
    (arg("image"), arg("elem")),
    doc__ecarttype__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

// NOTE: lderiche the question of l parameter
  UI_WRAP_FUNCTION(
    "deriche",
    lderiche,
    (arg("image"), arg("alpha"), arg("function"), arg("l") ),
    doc__deriche__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
    "shencastan",
    lshencastan,
    (arg("image"), arg("beta")),
    doc__shencastan__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
    "asftmed",
    lasftmed_2D3D,
    (arg("image"),arg("connexity"), arg("max radius")),
    doc__asftmed__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
    "areaopening",
    lareaopening,
    (arg("image"),arg("connexity"), arg("area")),
    doc__areaopening__c__
    );
# include BOOST_PP_UPDATE_COUNTER()


  UI_WRAP_FUNCTION(
    "areaclosing",
    lareaclosing,
    (arg("image"),arg("connexity"), arg("area")),
    doc__areaclosing__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
    "openball",
    lopenball,
    (arg("image"), arg("radius"), arg("mode")=0),
    doc__openball__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
    "closeball",
    lcloseball,
    (arg("image"), arg("radius"), arg("mode")=0),
    doc__closeball__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
    "dilatball",
    ldilatball,
    (arg("image"), arg("radius"), arg("mode")=0),
    doc__dilatball__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
    "erosball",
    lerosball,
    (arg("image"), arg("radius"), arg("mode")=0),
    doc__erosball__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
    "xor",
    lxor,
    (arg("image1"), arg("image2")),
    doc__xor__c__
    );
# include BOOST_PP_UPDATE_COUNTER()
  
  UI_WRAP_FUNCTION(
    "volselnb",
    lvolselnb,
    (arg("image"), arg("connex"), arg("param")),
    doc__volselnb__c__
    );
# include BOOST_PP_UPDATE_COUNTER()
  
  UI_WRAP_FUNCTION(
    "volmaxima",
    lvolmaxima,
    (arg("image"), arg("connex"), arg("param")),
    doc__volmaxima__c__
    );
# include BOOST_PP_UPDATE_COUNTER()
  
  UI_WRAP_VALUE(
    "variance",
    lvariance1,
    (arg("image")),
    doc__variance1__c__
    );
# include BOOST_PP_UPDATE_COUNTER()
  
  UI_WRAP_VALUE(
    "variance",
    lvariance2,
    (arg("image"), arg("mask")),
    doc__variance1__c__
    );
# include BOOST_PP_UPDATE_COUNTER()
  
  
  
  
} /* namespace points */

using namespace morpho;

void pymorpho()
{
  CALL_EXPORTED_FUNCTIONS(BOOST_PP_COUNTER);
}



// LuM end of file
