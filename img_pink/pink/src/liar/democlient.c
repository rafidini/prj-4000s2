/*------------------------------------------------------------------------
 *
 * Demo client for imview
 *
 * Hugues Talbot	 4 Jan 2001
 *      
 *-----------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include "liarp.h"
#include "liarwrap.h"
#include "imclient.h"

#define DFLTPORT 7600

const char *imview_cmd = "imview -debug -sync -server -fork";

int conn_id = -1;

static int democlient_upload_image(IMAGE *I, const char *lbl, const char *host, const ipctype method)
{
	const char *user;
	int         res;

	if (I != NULL) {
		user = getenv("USER");
		if (user != NULL) {
			imview_set_transfer_method(method);
			res = imviewlogin(user, host, DFLTPORT, &conn_id);
			if (res != 0)
				imexception("Login failed\n");
			res = imviewputimage(I, lbl, conn_id);
			if (res != 0)
				imexception("Put failed\n");
		}
	}

	return res;
}

static int democlient_load_image(const char *path, const char *host)
{
    const char *user;
    int         res;
    char  cmd[200];
    
    user = getenv("USER");
    if (user != NULL) {
        res = imviewlogin(user, host, DFLTPORT, &conn_id);
        if (res != 0)
            imexception("Login failed\n");
        sprintf(cmd, "load %s\r\n", path);
        if (imviewsendcommand(cmd, conn_id) == NULL) {
            imexception("Load failed\n");
            res = 1;
        }
    }

    return res;
}

static int democlient_zoombox_image(void)
{
    /* test of zooming to a box */
    char cmd[200];
    int  i, retval = 0;

    for (i = 0 ; i < 100 ; ++i) {
        sprintf(cmd, "zoom box %d %d 100 100\r\n", i, i);
        if (imviewsendcommand(cmd, conn_id) == NULL) {
            fprintf(stderr, "zoom box command failed\n");
            retval = 1;
            break;
        }
        fprintf(stderr, ".");
    }
    fprintf(stderr, "\n");

    return retval;
}

static int democlient_pan_image(void)
{
    /* test of image paning */
    char cmd[200];
    int  i, retval = 0;

    sprintf(cmd, "zoom factor 3\r\n");
    if (imviewsendcommand(cmd, conn_id) == NULL) {
        fprintf(stderr, "Zoom factor command failed\n");
        retval = 1;
    }  else {
        for (i = 0 ; i < 10 ; ++i) {
            sprintf(cmd, "pan %d %d\r\n", i, i);
            if (imviewsendcommand(cmd, conn_id) == NULL) {
                fprintf(stderr, "Pan command failed\n");
                retval = 1;
                break;
            }
            fprintf(stderr, ".");
        }
    }
    fprintf(stderr, "\n");

    return retval;
}

static void democlient_usage(const char *prog)
{
    fprintf(stderr,
            "Usage: %s <image.tif> [-d] [-ts/-tp/-tt] [-p]\n"
            "       Displays image in running imview server\n"
            "Options:\n"
            "      -d  : debugging output\n"
            "-t{s,p,t} : transfer using SYSV IPC, POSIX IPC or TCP/IP respectively\n"
            "      -p  : just give the path to the image to imview\n",
            prog);
    exit(2);
}

int democlient_main(int argc, char *argv[])
{
    IMAGE   *A;
    ipctype  transfer = SHM_NONE;
    int      res, narg, usepath = 0;
    const char *imenv;

    /* simple argument processing */
    narg = 1;
    while (narg < argc) {
        if (argv[narg][0] == '-') {
            if (argv[narg][1] == 't') {
                if(argv[narg][2] == 's')
                    transfer = SHM_SYSV; /* SYSV */
                else if (argv[narg][2] == 'p')
                    transfer = SHM_POSIX; /* POSIX */
                else
                    transfer = SHM_NONE;
            } else if (argv[narg][1] == 'd') {
                LIAREnableDebug();
            } else if (argv[narg][1] == 'p')
                usepath  = 1;
        }
        ++narg;
    }
    
    if (argc < 2) {
	democlient_usage(argv[0]);
    }


    imenv = getenv("IMVIEWCOMMAND");
    if (imenv == NULL)
        imenv = imview_cmd;

    system(imenv);

    if (!usepath) {
        A = imloadtiff(argv[argc -1]);
        res = democlient_upload_image(A, argv[argc-1], "localhost", transfer);
    } else {
        res = democlient_load_image(argv[argc-1], "localhost");
    }
    if (res == 0)
        res = democlient_pan_image();

    if (conn_id != -1)
	imviewlogout(conn_id);
    
    return res;
}
