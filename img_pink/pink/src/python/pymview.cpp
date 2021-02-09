/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by applicable law.
  
  (C) Hugues Talbot, 2009-2011 Université Paris-Est, Laboratoire
  d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy
  le Grand CEDEX

  hugues.talbot@gmail.com

*/

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#ifdef _WINDOWS
# include <process.h>
#endif /* _WINDOWS */

#include "liarp.h"
#include "liarwrap.h"
#include "imclient.h"
#include "pink_python.h"

// 'error' replaced with global macro 'pink_error'

using namespace boost::python;
using namespace pink;

namespace pink {
    namespace python {


        // globals
        std::string imviewcmd("imview -server -fork -portfile ");
        int             imview_window = 0;
        int             imview_connexion = -1;

        int Pimview (void)
        {
#           ifndef _WINDOWS
            const char *directory = getenv("TMPDIR"); // should work on most unices
            if (directory == NULL) {
                directory = "/tmp"; // but just in case
            }

            // calling command
            std::ostringstream command ;
            command << imviewcmd ;
            std::ostringstream portfilename;
            // build a deterministic unique file name
            portfilename << directory << "/" << imview_window << "_" << getpid();

            // append port filename to command
            command << portfilename.str();

            // show command
            std::ostringstream debugmsg;
            debugmsg << command.str() << std::endl;
            LIARdebug(debugmsg.str().c_str());
            debugmsg.str(""); // reset the string
            
            // remove FIFO
            unlink(portfilename.str().c_str()); // this may fail but doesn't matter.
            
            // create portfile
            int res = mkfifo(portfilename.str().c_str(), S_IRUSR | S_IWUSR);
            if (res != 0) {
                std::cerr << "Failed to create FIFO " << portfilename.str() << std::endl ;
                return(-1);
            }

            /* open the pipe for reading, notice the NO DELAY  */
            int filenum = -1;
            filenum = open(portfilename.str().c_str(), O_RDONLY | O_NDELAY, 0);
            if (filenum < 0) {
                std::cerr << "Failed to open FIFO" <<  portfilename.str() << std::endl ;
                return(-2);
            }

            // call imview !
            system(command.str().c_str());

            char buffer[5] = {0};

            if (read(filenum, buffer, 4) <= 0) {
                std::cerr << "Failed to read the portnumber" << std::endl;
                return(-3);
            }
            close(filenum);
            unlink(portfilename.str().c_str());
            
            debugmsg << "This will show an image eventually, using port " << buffer << std::endl;
            LIARdebug(debugmsg.str().c_str());
            
            // fill the port pointer as an integer
            return(atoi(buffer));
#           else /* NOT _WINDOWS */
	    pink_error("The Windows(tm) Imview named pipes are not yet implemented. Nag the developer.");
#           endif /* NOT _WINDOWS */
        }

        // login onto the image server.
        // if the imview server has died, this is the fcnt that wil first report an error.
        int Pimviewlogin(char *user, char *hostname, int port)
        {
            int connid;

            //imview_force_socket();
            
            int retval = imviewlogin(user, hostname, port, &connid);
            if (retval == 0)
                return connid; // connexion will remain open as long a the imview server is running
            else {
                std::cerr << "Login failed" << std::endl;
                return -1;
            }
        }

        // this translates the image_type into an IMAGE *
        // and displays it.
        template <class image_type> int Pimviewputimage(image_type &image,
                                                        const char *name,
                                                        int conn_id)
        {
            xvimage *realdata = image.get_output(); // get the basic data
            IMAGE *tbu = new(IMAGE); // TBU = To Be Uploaded

            // hand copy...

            // this could change
            tbu->ox = tbu->oy = tbu->oz = tbu->ot = 0;
            // straightforward I hope
            tbu->nx = realdata->row_size;
            tbu->ny = realdata->col_size;
            tbu->nz = realdata->depth_size;
            tbu->nt = realdata->time_size;
            tbu->nc = realdata->num_data_bands;

            // not very precise
            if ((tbu->nc == 2) || (tbu->nc > 3))
                tbu->it = IM_SPECTRUM;
            else if (tbu->nc == 3)
                tbu->it = IM_RGB;
            else if (tbu->nc == 1)
                tbu->it = IM_SINGLE;
            else
                tbu->it = IM_UNSPEC; // means "we don't know" in other words...

            // pixel type (no one-to-one correspondance)
            int datasize = 0;
            switch (realdata->data_storage_type) {
            case VFF_TYP_BIT :
                tbu->pt = IM_BINARY; // this is incorrect actually
                std::cerr << "Incorrect binary type" << std::endl;
                break;
            case VFF_TYP_1_BYTE :
                tbu->pt = IM_UINT1;
                datasize = 1;
                break;
            case VFF_TYP_2_BYTE :
                tbu->pt = IM_UINT2;
                datasize = 2;
                break;
            case VFF_TYP_4_BYTE :
                tbu->pt = IM_INT4;
                datasize = 4;
                break;
            case VFF_TYP_FLOAT:
                tbu->pt = IM_FLOAT;
                datasize = 4;
                break;
            case VFF_TYP_DOUBLE:
                tbu->pt = IM_DOUBLE;
                datasize = 8;
                break;
            default:
                std::cerr << "Unknown input data type: " << realdata->data_storage_type << std::endl;
                break;
            }

            if (datasize > 0) {
                // at any rate...
                tbu->buff = new void *[tbu->nc];
                for (int i = 0 ; i < tbu->nc ; ++i) {
                    // is this ugly or what ?
                    tbu->buff[i] = ((uint8_t*)(realdata->image_data) + (datasize*tbu->nx*tbu->ny*tbu->nz*tbu->nt)*i);
                }
            }

            std::cerr 
                << "Uloading data, nx=" << tbu->nx 
                << ",ny = " << tbu->ny 
                << ", nz = " << tbu->nz
                << ", nt = " << tbu->nt
                << ", nc = " << tbu->nc
                << ", datasize = " << datasize << std::endl;

            // away we go
            //imview_force_socket();
            int res = imviewputimage(tbu, name, conn_id);
            
            return res;
        }

        int Pimview_setdebug(bool debug)
        {
            int retval = 0;
            if (debug) {
                LIAREnableDebug();
                retval = 1;
            }  else
                LIARDisableDebug();

            return retval;
        }

        int Pimview_sendcommand(char *command, int connid)
        {
            int retval = 0;
            char *result = 0;
            if ((result=imviewsendcommand(command, connid)) == NULL) {
                std::cerr << "Sending command: '" << command << "' failed" << std::endl;
                retval = 1;
            } else {
                LIARdebug("Result of command = '%s' was '%s'", command, result);
            }
            return retval;
        }

    } /* namespace python */
} /* namespace pink */

// template <class image_type> void Rimview (const image_type & image)

// UI_EXPORT_FUNCTION(
//     Rimview,
//     pink::python::Rimview,
//     (arg("src")
//     (
//         ),
//     "Runs the imview viewer in server mode and returns its TCP/IP port"
//     );

// manual export function
// reverse-engineering the UJ-Way
// Hugues Talbot	20 Dec 2010
void Pimview_export()
{
    def("Pimview",
        pink::python::Pimview,
        "Starts an imview instance and returns the port number");
}

void Pimviewlogin_export()
{
    def("Pimviewlogin",
        pink::python::Pimviewlogin,
        (arg("user"),arg("hostname"),arg("port")),
        "Logs onto a running imview");
}

UI_EXPORT_FUNCTION(
    Pimviewputimage,
    pink::python::Pimviewputimage,
    (arg("image"),arg("name"),arg("connid")),
    "Upload an image to an imview server via TCP or shared memory"
    );

void PLiarEnableDebug_export()
{
    def("Pimview_setdebug",
        pink::python::Pimview_setdebug,
        (arg("debug")),
        "Starts/stop LIAR library debugging messages");
}

void Pimview_force_socket_export()
{
    def("Pimview_force_socket",
        imview_force_socket,
        "Disable used of shared memory when using imview");
}

void Pimview_sendcommand_export()
{
    def("Pimview_sendcommand",
        pink::python::Pimview_sendcommand,
        (arg("command"), arg("connid")),
        "Sends a command to a running imview server");
}
