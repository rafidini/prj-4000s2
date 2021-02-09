/*
  This software is licensed under
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but
  without any warranty to the extent permitted by applicable law.

  (C) UjoImro, 2009-2010
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/
#include "liar_line.h"
#include "pink_python.h"

using namespace boost::python;
using namespace pink;


namespace pink {
    namespace python {
        template <class image_t>
        image_t liaropenbun
        (
            const image_t & src,
            const int radius,
            const int nblines,
            const double angle,
            const double anglerange,
            const double rank
            )
        {
            // inefficient but don't know what else to do because of const madness
            image_t newsrc, result;
            newsrc = src.clone();
            result = src.clone();
          
            // The low-level function compute_openbun_rankmax etc return 0 to indicate success
            // HT 20120302

            if ( src.get_size().size()==2) // the image is 2D
            {
                const struct xvimage *myxvin = newsrc.get_output();
                struct xvimage *myxvimage = result.get_output();
                if (imopenbun( myxvin, radius, nblines, angle, anglerange, rank, myxvimage ) )
                {
                    pink_error("function imopenbun failed");
                }
            }
            else  // NOT the image is 2D
            {
                pink_error("functions imopenbun work only on 2D images");
            } // NOT the image is 2D

            return result;
        } /* liaropenbun */
      
        template <class image_t>
        image_t liarclosebin
        (
            const image_t & src,
            const int radius,
            const int nblines,
            const double angle,
            const double anglerange,
            const double rank
            )
        {
            // inefficient but don't know what else to do because of const madness
            image_t newsrc, result;
            newsrc = src.clone();
            result = src.clone();

            // The low-level function compute_openbun_rankmax etc return 0 to indicate success
            // HT 20120302

            if ( src.get_size().size()==2) // the image is 2D
            {
                const struct xvimage *myxvin = newsrc.get_output();
                struct xvimage *myxvimage = result.get_output();
                if (imclosebin( myxvin, radius, nblines, angle, anglerange, rank, myxvimage ) )
                {
                    pink_error("function imclosebin failed");
                }
            }
            else  // NOT the image is 2D
            {
                pink_error("functions imopenbun work only on 2D images");
            } // NOT the image is 2D

            return result;
        } /* liarclosebin */
 
    } /* namespace python */
} /* namespace pink */


UI_EXPORT_FUNCTION(
    openbun,
    pink::python::liaropenbun,
    ( arg("src"), arg("radius"), arg("nblines"),arg("angle")=0.0, arg("anglerange")=180.0,arg("rank")=0.0),
    "Opening by a union of <nblines> lines, of radius <radius>. An angle range can be specified with"
    "the angle and anglerange parameters. A rank can be given, between 0 and 1. If > 0, this"
    "corresponds to a proportion of noisy pixels that are ignored"
    // end of the documentation
    );


UI_EXPORT_FUNCTION(
    closebin, 
    pink::python::liarclosebin,
    ( arg("src"), arg("radius"), arg("nblines"),arg("angle")=0.0, arg("anglerange")=180.0,arg("rank")=0.0),
    "Closing by intersection of <nblines> lines, of radius <radius>. An angle range can be specified with"
    "the angle and anglerange parameters. A rank can be given, between 0 and 1. If > 0, this"
    "corresponds to a proportion of noisy pixels that are ignored"
    // end of the documentation
    );




















































// LuM end of file
