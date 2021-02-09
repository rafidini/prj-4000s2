/*
  This software is licensed under
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but
  without any warranty to the extent permitted by applicable law.

  (C) UjoImro, 2009-2010
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/

// Wrapper file for ex-LIAR stuff.
// pypink.cpp was beginning to be too big

#include "pink_python.h"
#include "liar_fseries.h"
#include <boost/preprocessor/slot/counter.hpp>

using namespace pink;
using namespace boost::python;

// using "modern" wrapping

// here we use namespaces, so the export_functions
// would not interfere with each other.
// c++ is a cool language, ain't



// HT: these function are wrapped correctly but return a zero-filled imageg.
// not sure why
// 2012-03-02

namespace liar
{

#if 0 // these functions do not work
  UI_WRAP_RESULT(
    char_image,
    "ferode3d_rect",
    imferode3D_rect,
    (arg("input"), arg("SEnx"), arg("SEny"), arg("SEnz")),
    "This performs a fast erosion by a parallelepiped."
    );
# include BOOST_PP_UPDATE_COUNTER()

    UI_WRAP_RESULT(
    char_image,
    "imferoderect",
    imferode_rect,
    (arg("input"), arg("SEnx"), arg("SEny")),
    "This performs a fast erosion by a rectangle."
        );

   # include BOOST_PP_UPDATE_COUNTER()

    UI_WRAP_RESULT(
    char_image,
    "imfopenrect",
    imfopen_rect,
    (arg("input"), arg("SEnx"), arg("SEny")),
    "This performs a fast erosion by a rectangle."
        );

    # include BOOST_PP_UPDATE_COUNTER()

    UI_WRAP_RESULT(
    char_image,
    "imfdilaterect",
    imfdilat_rect,
    (arg("input"), arg("SEnx"), arg("SEny")),
    "This performs a fast dilation by a rectangle."
        );


    # include BOOST_PP_UPDATE_COUNTER()

    UI_WRAP_RESULT(
    char_image,
    "imfcloserect",
    imfclose_rect,
    (arg("input"), arg("SEnx"), arg("SEny")),
    "This performs a fast erosion by a rectangle."
        );


# include BOOST_PP_UPDATE_COUNTER()


  UI_WRAP_RESULT(
    char_image,
    "imferode3drect",
    imferode3D_rect,
    (arg("input"), arg("SEnx"), arg("SEny"), arg("SEnz")),
    "This performs a fast erosion by a parallelepiped."
      );
# include BOOST_PP_UPDATE_COUNTER()

    UI_WRAP_RESULT(
    char_image,
    "imfdilate3drect",
    imfdilat3D_rect,
    (arg("input"), arg("SEnx"), arg("SEny"), arg("SEnz")),
    "This performs a fast dilation by a parallelepiped."
          );

# include BOOST_PP_UPDATE_COUNTER()

    UI_WRAP_RESULT(
    char_image,
    "imfopen3drect",
    imfopen3D_rect,
    (arg("input"), arg("SEnx"), arg("SEny"), arg("SEnz")),
    "This performs a fast opening by a parallelepiped."
        );

# include BOOST_PP_UPDATE_COUNTER()

    UI_WRAP_RESULT(
    char_image,
    "imfclose3drect",
    imfclose3D_rect,
    (arg("input"), arg("SEnx"), arg("SEny"), arg("SEnz")),
    "This performs a fast closing by a parallelepiped."
      );
# include BOOST_PP_UPDATE_COUNTER()

#endif // 0

} /* namespace liar */

using namespace liar;

void pyliar()
{
  CALL_EXPORTED_FUNCTIONS(BOOST_PP_COUNTER);
}

