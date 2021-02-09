/*------------------------------------------------------------------------
 *
 * This file contains functions to connect to the Imview server.
 *
 * Hugues Talbot	 9 Mar 2000
 *      
 *-----------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// #include <unistd.h> in Microsoft Windows it does not exist, but we only need a subset of it
// time.h is on different place
#ifdef UNIXIO
#  include <unistd.h>
#  include <sys/time.h>
#else /* NOT UNIXIO */
#  include <stdlib.h>
#  include <io.h>
#  include <time.h>
#endif /* NOT UNIXIO */

#include <errno.h>
#include <sys/types.h>

#include "liarp.h"
#include "liarwrap.h"
#include "imclient.h"

#define ANSWER_MAX_SIZE 1024
// WHO DID THIS ?? #define ANSWER_MAX_SIZE 10
#define BIN_MAX_SIZE    8192
#define HNDSHK_SIZE     300
#define DEFAULTPORT     7600
#define DEFAULTHOST     "127.0.0.1"
#define EOC             "\r\n"
#define HANDSHAKE       "Imview >"
#define TIMEOUT_SEC     10
#define TIMEOUT_USEC    150000
#define HEADER_ID_SIZE  8
#define MAX_CONN_ARRAY  100

#define NET_PUT         "put"          /* using socket for data transfer */
#define SYSV_SHM_PUT    "putm"         /* using System V shared memory for data transfer */
#define PX_SHM_PUT      "putp"         /* using Posix shared memory for data transfer */
#define WAS_STATIC


WAS_STATIC int     force_socket = 0;

WAS_STATIC const char *putcmd[] = {
    NET_PUT,
    PX_SHM_PUT,
    SYSV_SHM_PUT
};

#define OVERLAY_MARKER  "<overlay>"

/* we all know it's a terrible macro */
#ifndef mymin
#   define mymin(X,Y) ((X) > (Y)) ? (Y) : (X)
#endif

#define SHM_SIZE 400000 /* valid for all shared memory models */

//WAS_STATIC ipctype use_shm_default = SHM_POSIX; /* by default we use POSIX IPC if we can */
WAS_STATIC ipctype use_shm_default = SHM_SYSV; /* by default we use SYSV IPC, as it is better tested */

/* data needed to describe a connection */
typedef struct connection_ {
    int       opened;
    int       fd;            /* not important: the file descriptor */
    char     *hostname;
    ipctype   use_shm;       /* bool: yes or no */
    char     *sync_filename; /* shared memory uploads */
    short     port;
} connection;

#ifdef WIN32
int winsock_started=0;
#endif


#ifndef WIN32
# include <sys/socket.h>
# include <netinet/in.h>
# include <netdb.h>
# define im_close close
# define USRID    "USER"
#else /* NOT WIN32 */
# include <windows.h>
# include <winsock.h>
struct timezone
{
    int tz_minuteswest;         /* Minutes west of GMT.  */
    int tz_dsttime;             /* Nonzero if DST is ever in effect.  */
}; 
/* For now redefine gettimeofday() as timerclear()
     ie. sets timeval to zero (see <winsock.h> and/or <sys/time.h>
     Else, define a new function for WIN32 :)
     extern int gettimeofday();
  */
# define gettimeofday(tvp, tzp) (timerclear(tvp))
# define im_close closesocket
# define USRID    "USERNAME"
#endif /* NOT WIN32 */


/* system V IPC */
#ifdef HAVE_SYSV_IPC
# include <sys/ipc.h>
# include <sys/sem.h>
# include <sys/shm.h>

/* not sure this is still needed, this is very weird
   Hugues Talbot	21 Dec 2010*/
# ifdef Linux
#   if defined(__GNU_LIBRARY__) && !defined(_SEM_SEMUN_UNDEFINED)
/* union semun is defined by including <sys/sem.h> */
#   else /* NOT GNU lib, etc */
/* according to X/OPEN we have to define it ourselves */
union semun {
    int val;                    /* value for SETVAL */
    struct semid_ds *buf;       /* buffer for IPC_STAT, IPC_SET */
    unsigned short int *array;  /* array for GETALL, SETALL */
    struct seminfo *__buf;      /* buffer for IPC_INFO */
};
#   endif /* NOT GNU lib, etc */
# endif /* Linux */

# define SEM_ACS  0
# define SEM_RD   1
# define SEM_WR   2

/* convenient globals */
int               semid, shmid;
WAS_STATIC  char *sysv_shm_data;


#endif /* HAVE_SYSV_IPC */

/* POSIX IPC */
#ifdef HAVE_POSIX_IPC
# include <semaphore.h>
# include <sys/mman.h>

// #include <unistd.h> in Microsoft Windows it does not exist, but we only need a subset of it
// time.h is on different place
# ifdef UNIXIO
#   include <unistd.h>
#   include <sys/time.h>
# else /* NOT UNIXIO */
#   include <stdlib.h>
#   include <io.h>
#   include <time.h>
# endif /* NOT UNIXIO */

# include <errno.h>
# include <fcntl.h>

# define STR_ACCESS   "_ACCESS"
# define STR_READ     "_READ"
# define STR_WRITE    "_WRITE"
# define STR_SHM      "_SHM"


# define SEM_ACCESS   0
# define SEM_READ     1
# define SEM_WRITE    2

// rw for creator only, must be Octal!
# define IPC_FILE_MODE 0600 

/* convenience WAS_STATIC global */
WAS_STATIC  char  *posix_shm_data;
WAS_STATIC  sem_t *imview_px_sem[3];
// WAS_STATIC  char  *ipc_base_path[ANSWER_MAX_SIZE]; // original definition

char              *px_templ_name;

WAS_STATIC const char  *px_resource_name[] = {
    STR_ACCESS,
    STR_READ,
    STR_WRITE
};

#endif /* HAVE_POSIX_IPC */

char  *ipc_base_path[ANSWER_MAX_SIZE];



WAS_STATIC connection conn_array[MAX_CONN_ARRAY];
WAS_STATIC int        conn_inited = 0; 

/* static functions prototypes */
static int imview_open_connection(const char *hostname,
				  short initial_port);

static void imview_close_connection(int s);
static int imview_watchForInput(int fd);
static int imview_watchForInput_withDelay(int fd, int sec, int usec);
static char *imview_command(int         fd,       
			    const char *command); 
static char *imview_general_command(int         fd,      
				    const char *command, 
				    int          expect_answer,
				    int          waitsec,
				    int          waitusec);

static char *reliable_imview_general_command(int         fd,       /* socket descriptor */
					     const char *command, /* the string command */
					     int          expect_answer,
					     int          waitsec,
					     int          waitusec);

static char *unlimited_imview_general_command(int         fd,       /* socket descriptor */
					      const char *command, /* the string command */
					      int          expect_answer,
					      int          waitsec,
					      int          waitusec);
static char *imview_command_noanswer(int fd,
				     const char *command);

#ifdef WIN32
static void init_winsock (void)
{
    WSADATA wd;
    WSAStartup (MAKEWORD (1,1), &wd);
    winsock_started = 1;
}
#endif


/* finds the first available port to which an connection is possible
   from a given port. It opens the connection and returns the file descriptor.
   Returns -1 if it fails.
*/
static int imview_open_connection(const char *hostname,
				  short initial_port)
{
    short              port;
    int                s, i, len, retval;
    struct hostent    *hp;
    struct sockaddr_in name;

#ifdef WIN32
    if (!winsock_started)
	init_winsock();
#endif
    
    if ((hp = gethostbyname(hostname)) == NULL) {
	imexception("imview_open_connection: unknown host %s\n", hostname);
	return -1;
    }

    retval = -1;
    for (i = 0, port = initial_port ; i < 100 ; i += 2, port += 2) {

	if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
	    imexception("imview_open_connection: cannot create socket %s\n", strerror(errno));
	    return -1;
	}
	/* we need to reset the whole `name' structure each time
	   we try to open a port. Just changing the port # is not
	   sufficient */
	memset(&name, 0, sizeof(struct sockaddr_in));
	name.sin_family = AF_INET;
	memcpy(&name.sin_addr, hp->h_addr_list[0], hp->h_length);
	len = sizeof(struct sockaddr_in);
	name.sin_port = htons(port);
	if (connect(s, (struct sockaddr *) &name, len) == 0) {
	    retval = s;
	    LIARdebug("Connected to port %d:", port);
	    break; /* success ! */
	} else {
	    LIARdebug("Cannot connect to port %d: %s", port, strerror(errno));
	    im_close(s);
	}
    }

    if (retval < 0) 
	imexception("imview_open_connection: cannot connect to any port from %d\n",
		    initial_port);

    return retval;
}

/* this creates a binary connection to a given port. There is no port discovery */
static int imview_open_binconn(const char *hostname,
			       short given_port)
{
    int                s, len, retval;
    struct hostent    *hp;
    struct sockaddr_in name;

    if ((hp = gethostbyname(hostname)) == NULL) {
	imexception("imview_open_binconn: unknown host %s\n", hostname);
	return -1;
    }

    if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
	imexception("imview_open_binconn: cannot create socket %s\n", strerror(errno));
	return -1;
    }

    memset(&name, 0, sizeof(struct sockaddr_in));
    name.sin_family = AF_INET;
    memcpy(&name.sin_addr, hp->h_addr_list[0], hp->h_length);
    len = sizeof(struct sockaddr_in);

    retval = -1;
    name.sin_port = htons(given_port);
    if (connect(s, (struct sockaddr *) &name, len) == 0) {
	retval = s;
    }

    if (retval < 0) 
	imexception("imview_open_binconn: cannot connect to binary port %d, %s\n",
		    given_port, strerror(errno));

    return retval;
}

static void imview_close_connection(int s)
{
    int res;

    /* wait until everything in the pipe has been received */
    
    /* be nice and polite: send a QUIT request */
    imview_command_noanswer(s,"quit" EOC);
    /* then brutally close the socket! */
    res= im_close(s);
    if (res < 0)
	imexception("imview_close_connection: cannot close connection, %s.\n",
		    strerror(errno));
    return;
}

static void imview_close_binconn(int s)
{
    int res;

    /* brutally close the socket! */
    res= im_close(s);
    if (res < 0)
	imexception("imview_close_binconn: cannot close connection, %s.\n",
		    strerror(errno));
    return;
}

/* flushes the input channel. A timeout WILL be called. */
static void imview_flush_input(int fd)
{
    fd_set rfds;
    struct timeval tv;
    int retval, craprec;
    char crap[ANSWER_MAX_SIZE];

    LIARdebug("Flushing the input");
    /* Watch the given fd to see when it has input. */
    FD_ZERO(&rfds);
    FD_SET(fd, &rfds);

    /* Wait 10s at the most. */
    tv.tv_sec = TIMEOUT_SEC;
    tv.tv_usec = TIMEOUT_USEC;

    LIARdebug("Flushing: Waiting at most %d sec and %d usec", tv.tv_sec, tv.tv_usec);
    
    craprec = 0;
    while ((retval = select(FD_SETSIZE, &rfds, NULL, NULL, &tv)) > 0) {
	craprec += recv(fd, crap, ANSWER_MAX_SIZE, 0);
	tv.tv_sec = 0; /* we are expecting the second select to fail */
	tv.tv_usec = TIMEOUT_USEC; /* reset to 10ms */
	LIARdebug("Flushing: Waiting at most %d sec and %d usec", tv.tv_sec, tv.tv_usec);
    }

    LIARdebug("received %d crappy bytes we didn't want...", craprec);

    return;
}

/* to be used only when we KNOW we've had an answer and we want to get it */
static int imview_get_delayed_response(int fd, char *resp)
{
    return(recv(fd, resp, ANSWER_MAX_SIZE, 0)); /* this will get one TCP packet back */
}

static int imview_watchForInput(int fd)
{
    return imview_watchForInput_withDelay(fd, 0, TIMEOUT_USEC); /* default wait is only 10ms */
}

static int imview_watchForInput_withDelay(int fd, int sec, int usec)
{
    fd_set rfds;
    struct timeval tv;
    int retval;

    /* Watch the given fd to see when it has input. */
    FD_ZERO(&rfds);
    FD_SET(fd, &rfds);

    /* Wait up to some time */
    LIARdebug("Waiting at most for %d sec and %d usec", sec, usec);
    tv.tv_sec = sec;
    tv.tv_usec = usec;

    retval = select(FD_SETSIZE, &rfds, NULL, NULL, &tv);

    if (retval == -1)
	imexception("imview_watchForInput: select returned error: %s\n", strerror(errno));
    
    /* if retval = 0, nothing to read */
    if (retval == 0)
	LIARdebug("imview_watchForInput: Timeout expired, there was nothing to read");
    return retval;
}

/* command for which no answer is expected (typically: quit) */
static char *imview_command_noanswer(int fd,
				     const char *command)
{
    /* expect no answer, wait only 10 ms */
    return imview_general_command(fd, command, 0, 0, TIMEOUT_USEC); /* can retun NULL, test for it! */
}

/* normal commands (answer is returned) */
static char *imview_command(int fd,
			    const char *command)
{
    return imview_general_command(fd, command, 1, 0, TIMEOUT_USEC);
}

static char *imview_general_command(int         fd,       /* socket descriptor */
				    const char *command, /* the string command */
				    int          expect_answer,
				    int          waitsec,
				    int          waitusec)
{
    int          res, len, tmp_rec;
    static char  answer_buf[ANSWER_MAX_SIZE + 1];
    char        *answer;

    len = strlen(command);
    answer = NULL;
    if (len > 0) {
	/* send the bloody command in one go */
	res = send(fd, command, len, 0);
	memset(answer_buf, 0, ANSWER_MAX_SIZE+1);
	if ((res == len) && expect_answer) {
	    /* receive the freaking result in one go as well */
	    tmp_rec = recv(fd, answer_buf, ANSWER_MAX_SIZE, 0);
	    if (tmp_rec < 0) {
		imexception("Receive failed: %s\n", strerror(errno));
		return NULL;
	    } else if (tmp_rec == 0)
		LIARdebug("Nothing received: socket was closed or missing EOC");
	    answer = answer_buf;
	} else {
	    if (res < 0) {
		imexception("Send failed: Command wasn't sent properly, %s\n", strerror(errno));
		return NULL;
	    } else if (res < len) {
		imexception("Send failed: Command only partially sent\n");
		return NULL;
	    }
	}
    } else {
	imexception("Send failed: NULL command\n");
	return NULL;
    }
    return answer_buf;
}

/* look for `end of command' */
#define BACKLINES 3 /* go back only 3 lines at most */
#define CMD_OK        1
#define CMD_FAILED    2
#define CMD_UNKNOWN   3

static int imview_EOC(char *message)
{
    int   i, j, reason;
    char *p[BACKLINES];
    /* we look backwards through the lines */
    for (i = 0 ; i < BACKLINES ; i++) {
	p[i] = strrchr(message, '\n');
	reason = 0;
	if (p[i] != NULL) {
	    if (((strncmp(p[i]+1, "000 OK", 6) == 0) && ((reason = CMD_OK) > 0))  /* I do mean = here, not == */
	        || ((strncmp(p[i]+1, "500 Command failed", 18) == 0) && ((reason = CMD_FAILED) > 0))
	        || ((strncmp(p[i]+1, "501 Unknown command", 19) == 0) && ((reason = CMD_UNKNOWN) > 0))) {
		LIARdebug("End of command detected %d lines from the end\n", i);
		/* make the command stop just before this message */
		*(p[i]+1) = '\0'; /* we keep the last \n */
		return reason; /* end of command, unknown */
	    } else {
		*(p[i]) = '\0'; /* remove the \n, shorten the string */
	    }
	} else {
	    /* Maybe the whole command is the return */
	    if (((strncmp(message, "000 OK", 6) == 0) && ((reason = CMD_OK) > 0))  /* I do mean = here, not == */
	        || ((strncmp(message, "500 Command failed", 18) == 0) && ((reason = CMD_FAILED) > 0))
	        || ((strncmp(message, "501 Unknown command", 19) == 0) && ((reason = CMD_UNKNOWN) > 0))) {
		LIARdebug("End of command detected\n", i);
		/* message[0] = '\0'; no need to keep the returned command */
		/* make the command stop just before this message */
		return reason; /* end of command, unknown, etc */
	    }
	    break; /* the end */
	}
    }
    for (j = 0 ; j < i ; j++)
	*(p[j]) = '\n';  /* put back what we removed, we don't want to disturb the result */
    return 0; /* not found */
}

/* expects an unlimited answer */
/* the answer will be put in a static buffer that grows as needed. Subsequent calls to
   this function erase the previous content, obviously. */
static char *unlimited_imview_general_command(int         fd,       /* socket descriptor */
					      const char *command, /* the string command */
					      int          expect_answer,
					      int          waitsec,
					      int          waitusec)
{
    int          res, len, tmp_rec, rec_so_far, has_more, eoc;
    static char  tmp_buf[ANSWER_MAX_SIZE+1];
    static char *answer_buf = NULL;
    static int   answer_len;
    char        *answer;

    if (!answer_buf) {
	answer_buf = (char *)malloc(ANSWER_MAX_SIZE +1);
	answer_len = ANSWER_MAX_SIZE +1;
    }
    
    len = strlen(command);
    answer = NULL;
    if (len > 0) {
	/* send the bloody command in one go */
	res = send(fd, command, len, 0);
	memset(answer_buf, 0, answer_len);
	has_more = 0;
	if ((res == len) && expect_answer) {
	    /* keep calling recv until we meet an EOF */
	    rec_so_far = 0;
	    do {
		memset(tmp_buf, 0, ANSWER_MAX_SIZE+1);
		tmp_rec = recv(fd, tmp_buf, ANSWER_MAX_SIZE, 0);
		if (tmp_rec < 0)
		    imexception("Receive failed: %s\n", strerror(errno));
		else if (tmp_rec == 0)
		    LIARdebug("Nothing received: socket was closed or missing EOC");
		else {
		    LIARdebug("Received %d bytes", tmp_rec);
		    if ((rec_so_far + tmp_rec) > answer_len) {
			answer_buf = realloc(answer_buf, 2*answer_len);
			memset(answer_buf + answer_len, 0, answer_len);
			answer_len *= 2;
		    }
		    memcpy(answer_buf+rec_so_far, tmp_buf, tmp_rec);
		    rec_so_far += tmp_rec; 
		    answer = answer_buf;
		    /*watch for the marker of the `end-of-command'
		     * a line starting with 3 digits followed by a message.
		     */
		    if ((eoc=imview_EOC(answer_buf)) > 0) {
			if (eoc == CMD_FAILED) {
			    imexception("The command that was sent failed, imview returned:\n%s", answer_buf);
			} else if (eoc == CMD_UNKNOWN) {
			    imexception("The command that was sent is unknown to imview\n");
			} else {
			    LIARdebug("The command ran OK");
			}
			/* in any case */
			break;
		    }
		    /* wait for next packet to arrive, if any */ 
		    has_more = imview_watchForInput_withDelay(fd, 10, waitusec);
		} 
	    } while ((tmp_rec > 0) && (has_more > 0));
	} else { 
 	    if (res < 0)
	 	imexception("Send failed: Command wasn't sent properly, %s\n", strerror(errno));
	    else if (res < len)
 		imexception("Send failed: Command only partially sent\n");
 	}
    } else {
        imexception("Send failed: NULL command\n");
    } 
    
    return answer; 
} 

static char *reliable_imview_general_command(int         fd,       /* socket descriptor */
					     const char *command, /* the string command */
					     int          expect_answer,
					     int          waitsec,
					     int          waitusec)
		     
{
/** Send an arbitrary command to imview and return its result

    RETURN VALUE:	char *

    DESCRIPTION:
    Sends an arbitrary command to imview on a reputed hostname+port.
    returns the answer. The answer is in a static buffer so doesn't
    need to be freed, but only one command can be sent at a time.

    The port that will be opened may not be the one specified, but
    ports are tried from the one given.

    HISTORY:

    TESTS:

    REFERENCES:

    KEYWORDS:

**/
    int          res, len, received, tmp_rec;
    static char answer_buf[BIN_MAX_SIZE], tmp_buf[ANSWER_MAX_SIZE+1];
    char        *answer;

    answer = NULL; /* pessimistic scenario */
    
    /* send the commmand */
    len = strlen(command);
    if (len > 0) {
	res = send(fd, command, len, 0);
	if (res < 0) {
	    imexception("imview_command: send failed, %s\n",
			strerror(errno));
	} else if (expect_answer) {
	    /* wait for reply! */
	    tmp_rec = received = 0;
	    /* clear receive buffer */
	    memset(tmp_buf, 0, ANSWER_MAX_SIZE);
	    memset(answer_buf, 0, BIN_MAX_SIZE);
	    while ((imview_watchForInput_withDelay(fd, waitsec, waitusec) != 0) && (tmp_rec >= 0)) {	
		tmp_rec = recv(fd, tmp_buf, ANSWER_MAX_SIZE, 0);
		if (tmp_rec > 0) {
		    if (strncmp(tmp_buf, HANDSHAKE, strlen(HANDSHAKE)) == 0) {
			LIARdebug("Handshake received");
			break;
		    }
		    received += tmp_rec;
		    strcat(answer_buf, tmp_buf);
		    memset(tmp_buf, 0, tmp_rec+1); /* +1 for the \0 */
		}
	    }
	    if (tmp_rec < 0) {
		imexception("imview_command: received failed\n");
	    }
	    LIARdebug("Received: %d bytes", received);
	    answer = answer_buf;
	} else { /* no answer expected */
	    received = 0;
	    /* still watch for input, this allows the command to go through */
	    imview_watchForInput(fd);
	    /* and then just return after timeout */
	}
    } else {
	imexception("imview_command: NULL command\n");
    }
    
    return answer;
}


static wsize_t imview_send_data(int           bfd,       /* socket descriptor */
				char         *data,      /* the raw data */
				wsize_t        len,        /* its length */
				int           eot)       /* end of transmission */
		     
{
/** Send arbitrary binary data to imview

    RETURN VALUE:	int

    DESCRIPTION:
    Sends an arbitrary  buffer through a socket connection.
    The data is divided up in smallish chunks and sent packet
    by packet as a byte stream.

    HISTORY:

    TESTS:

    REFERENCES:

    KEYWORDS:

**/
    wsize_t         res,  sent, thislen; /* slct_res, */
    static char    sbuf[BIN_MAX_SIZE];
    static char    rbuf[HNDSHK_SIZE]; /* big enough for hanshake */
    /* fd_set         rfds; */
    /* struct timeval tv;  */

    res = 0;
    sent = 0;
    while (sent < len) {
	thislen = mymin(len-sent, BIN_MAX_SIZE);
	memcpy(sbuf, data+sent, thislen);
	res = send(bfd, sbuf, thislen, 0);
	if (res != thislen) {
	    imexception("imview_send_data: send failed, only %d bytes were sent, wanted to send %d\n"
			"reason: %s\n",
			sent, len, strerror(errno));
	    break;
	}
	sent += res;
	/* wait for handshake */
	memset(rbuf, 0, HNDSHK_SIZE);
	/* tv.tv_sec = TIMEOUT_SEC;
	** tv.tv_usec = TIMEOUT_USEC;
	** FD_ZERO(&rfds);
	** FD_SET(bfd, &rfds);
	** slct_res = select(FD_SETSIZE, &rfds, NULL, NULL, &tv);
	** if (slct_res == 0)
	**    imexception("Timeout waiting for binary handshake\n");
	**else { */
	res = recv(bfd, rbuf, HNDSHK_SIZE, 0);
	/*    if (strncmp(rbuf, "OK\n", 3) != 0)
	**	imexception("Send handshake failed, received: (%s)\n", rbuf);
	** } */
	if (res == 0)
	    break; /* connection closed */
    }

    if (eot && (len > HEADER_ID_SIZE))
	while (res != 0) {
	    /* wait for `end of transmission: read of 0' */
	    res = recv(bfd, rbuf, HNDSHK_SIZE, 0);
	}
    
    return sent;
}


void imview_set_transfer_method(const ipctype method)
{
    /* this just sets the default, no guarantee that
       this will be the method actually used -- depends
       if it's available for a start... --
    */
    use_shm_default = method;
}

void imview_force_socket(void)
{
    /* use sockets even if shared memory is available */
    force_socket = 1;
}

#ifdef HAVE_SYSV_IPC
int imview_sysv_ipc_setup(char *sync_filename)
{
    key_t         key;
    
    struct sembuf sb = {0, -1, 0};  /* set to allocate resource */


    LIARdebug("Connecting to the ipc facilities setup by imview");
    if ((key = ftok(sync_filename, 'A')) == -1) {
        imexception("imview_ipc_setup, ftok sem: %s\n", strerror(errno));
        return(1);
    }
    /* grab the semaphore set created by imview: */
    if ((semid = semget(key, 3, 0)) == -1) {
        imexception("imview_ipc_setup, semget: %s\n", strerror(errno));
        return(1);
    }
    
    LIARdebug("We are now connected");
    /* now try to access the shared memory buffer */
    sb.sem_num = SEM_ACS;
    sb.sem_op = -1; /* down */
    if (semop(semid, &sb, 1) == -1) {
	imexception("imview_ipc_setup, semop_access: %s\n", strerror(errno));
	return(1);
    }
    
    LIARdebug("Access to shared memory granted, connecting to it");
    if ((key = ftok(sync_filename, 'B')) == -1) {
	imexception("imview_ipc_setup, ftok shm: %s\n", strerror(errno));
        return(2);
    }
    LIARdebug("Reference filename: %s, key: %p", sync_filename, key);
    if ((shmid = shmget(key, SHM_SIZE, 0)) == -1) {
	imexception("imview_ipc_setup, shmget: %s\n", strerror(errno));
        return(2);
    }

    /* attach to the segment to get a pointer to it: */
    sysv_shm_data = shmat(shmid, (void *)0, 0);
    if (sysv_shm_data == (char *)(-1)) {
	imexception("imview_ipc_setup, shmat: %s\n", strerror(errno));
        return(2);
    }
    
    LIARdebug("OK, shared memory is ready to be used");
    
    return 0;
}

/* send data via shm */
int imview_sysv_shm_sendata(char         *in_data,      /* the raw data */
		       wsize_t        len)       /* amount of data to be sent */
{
    int            total, this_one;
    int            tbs;
    struct sembuf  sb = {0, -1, 0};  /* set to allocate resource */
    
    total = 0;
    tbs = len;

    do {
	/* attempt to write (should block if not OK) */
	sb.sem_num = SEM_WR;
	sb.sem_op = -1;
	if (semop(semid, &sb, 1) == -1) {
	    imexception("imview_shm_sendata, semop_W: %s\n", strerror(errno));
	    return(-1);
	}
	this_one = mymin((SHM_SIZE-sizeof(int)), tbs);
	tbs -= this_one;
	memcpy(sysv_shm_data, &this_one, sizeof(int));
	memcpy(sysv_shm_data+sizeof(int), in_data+total, this_one);
	/* OK, now imview can read this data */
	sb.sem_num = SEM_RD;
	sb.sem_op = 1; /* up */
	if (semop(semid, &sb, 1) == -1) {
	    imexception("imview_shm_sendata, semop_R: %s\n", strerror(errno));
	    return(-1);
	}
	total += this_one;
    } while (tbs > 0);

    return total;
}

int imview_sysv_ipc_wind_down(void)
{
    struct sembuf sb = {0, -1, 0};  /* set to allocate resource */
    /* releases the  access to the shared memory buffer */
    sb.sem_num = SEM_ACS;
    sb.sem_op = 1; /* up */
    if (semop(semid, &sb, 1) == -1) {
	imexception("imview_ipc_wind_down, semop_access: %s\n", strerror(errno));
	return(1);
    }
    /* detach from the memory segment */
    if (shmdt(sysv_shm_data) == -1) {
        imexception("imview_ipc_wind_down, shmdet: %s\n", strerror(errno));
	return(1);
    }
    return 0;
}

#else
/* stubs */
int imview_sysv_ipc_setup(const char *sync_filename)
{
    imexception("imview_sysv_ipc_setup should not have been called, this is just a stub.\n");
    return 1;
}
int imview_sysv_shm_sendata(char         *in_data,      /* the raw data */
		       wsize_t        len)       /* amount of data to be sent */
{
    imexception("imview_sysv_shm_sendata should not have been called, this is just a stub.\n");
    return -1;
}
int imview_sysv_ipc_wind_down(void)
{
    imexception("imview_sysv_ipc_wind_down should not have been called, this is just a stub.\n");
    return 1;
}
#endif /* HAVE_SYSV_IPC */


#ifdef HAVE_POSIX_IPC

static char *imview_px_ipc_name(const char *name)
{
    char	*dir, *dst;
    const char  *slash;
    
    if ( (dst = (char *)malloc(ANSWER_MAX_SIZE * sizeof(char))) == NULL)
        return(NULL);

    /* can override default directory with environment variable */
    if ( (dir = getenv("PX_IPC_NAME")) == NULL) {
# ifdef	POSIX_IPC_PREFIX
        dir = POSIX_IPC_PREFIX;		/* from "config.h" */
# else // NOT POSIX_IPC_PREFIX
        dir = "/tmp/";				/* default */
# endif // NOT POSIX_IPC_PREFIX
    }
		/* dir must end in a slash */
    slash = (dir[strlen(dir) - 1] == '/') ? "" : "/";
    snprintf(dst, ANSWER_MAX_SIZE, "%s%s%s", dir, slash, name);
    
    return(dst);			/* caller can free() this pointer */
}

int imview_px_ipc_setup(const char *sync_filename)
{
    int           l, shmfd, i;
    char         *tmprid;
    char          tmppathbuf[ANSWER_MAX_SIZE];

    /* template name is platform-dependent */

    tmprid = strstr(sync_filename + 1, "/") + 1; /* get rid of '/tmp/' */
    px_templ_name = imview_px_ipc_name(tmprid); /* will need to be freed */
    
    /* then connect to all the semaphores */
    l = strlen(px_templ_name);
    
    for (i = 0 ; i < 3 ; ++i) {
        strcpy(tmppathbuf, px_templ_name);
        strncat(tmppathbuf, px_resource_name[i], ANSWER_MAX_SIZE-l);
        imview_px_sem[i] = sem_open(tmppathbuf, 0,  IPC_FILE_MODE, 0); /* semaphore is NOT created, value is ignored */
        if ( imview_px_sem[i] == SEM_FAILED) {
            imexception("imview_px_ipc_setup, sem_open(%s) failed: %s\n", tmppathbuf, strerror(errno));
            return(1);
        }
    }
    
    LIARdebug("We are now connected");
    /* now try to access the shared memory buffer */
    if (sem_wait(imview_px_sem[SEM_ACCESS]) != 0) {
	imexception("imview_px_ipc_setup, sem_wait failed on ACCESS: %s\n", strerror(errno));
	return(1);
    }
    LIARdebug("Access to shared memory granted, connecting to it");

    /* construct path to shared memory */
    strcpy(tmppathbuf, px_templ_name);
    strncat(tmppathbuf, STR_SHM, ANSWER_MAX_SIZE-l);
    /* file descriptor */
    shmfd = shm_open(tmppathbuf, O_RDWR, IPC_FILE_MODE); /* should have been created by server */

    if (shmfd < 0) {
        imexception("imview_px_ipc_setup, shm_open(%s) failed: %s\n", tmppathbuf, strerror(errno));
        return(2);
    }

    /* get pointer */
    if ((posix_shm_data = mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shmfd, 0)) == MAP_FAILED) {
        imexception("imview_px_ipc_setup, mmap failed: %s\n", strerror(errno));
        return(3);
    }

    /* the shm file can be closed now */
    close(shmfd);

    LIARdebug("OK, shared memory is ready to be used");
    
    return 0;
}

/* send data via shm */
int imview_px_shm_sendata(char         *in_data,      /* the raw data */
                          wsize_t        len)       /* amount of data to be sent */
{
    int            total, this_one;
    int            tbs;

    total = 0;
    tbs = len;

    LIARdebug("Sending data over shared memory");
    
    do {
	/* attempt to write (should block if not OK) */
        if (sem_wait(imview_px_sem[SEM_WRITE]) != 0) {
            imexception("imview_px_shm_sendata, sem_wait failed on WRITE: %s\n", strerror(errno));
            return(1);
        }

	this_one = mymin((SHM_SIZE-sizeof(int)), tbs);
	tbs -= this_one;
	memcpy(posix_shm_data, &this_one, sizeof(int));
	memcpy(posix_shm_data+sizeof(int), in_data+total, this_one);
	/* OK, now imview can read this data */
        if (sem_post(imview_px_sem[SEM_READ]) != 0) {
            imexception("imview_ipc_shm_sendata, sem_post failed on READ: %s\n", strerror(errno));
            return(1);
        }

	total += this_one;
    } while (tbs > 0);

    LIARdebug("Finished sending %d bytes", total);
    
    return total;
}

int imview_px_ipc_wind_down(void)
{
    int   i, l;
    char  tmppathbuf[ANSWER_MAX_SIZE];
    
    /* detach from all semaphores */
    l = strlen(px_templ_name);
    for (i = 0 ; i < 3 ; ++i) {
        if (sem_close(imview_px_sem[i]) != 0)
            imexception("Ooops: sem_close[%s] failed\n", px_resource_name[i]); /* nothing else we can do really */
        strcpy(tmppathbuf, px_templ_name);
        strncat(tmppathbuf, px_resource_name[i], ANSWER_MAX_SIZE-l);
        if (sem_unlink(tmppathbuf) != 0)
            imexception("Ooops: sem_unlink[%s] failed\n", tmppathbuf); /* same here */
    }
    
    /* detach from the memory segment */
    if (posix_shm_data != NULL)
        munmap(posix_shm_data, SHM_SIZE);

    strcpy(tmppathbuf, px_templ_name);
    strncat(tmppathbuf, STR_SHM, ANSWER_MAX_SIZE-l);
    if (shm_unlink(tmppathbuf) != 0) {
        imexception("Ooops: shm_unlink[%s] failed\n", tmppathbuf); /* same here */
    }

    free(px_templ_name);
    px_templ_name = NULL;
    
    return 0;
}

#else

/* stubs */
int imview_px_ipc_setup(const char *sync_filename)
{
    imexception("imview_px_ipc_setup should not have been called, this is just a stub.\n");
    return 1;
}
int imview_px_shm_sendata(char         *in_data,      /* the raw data */
		       wsize_t        len)       /* amount of data to be sent */
{
    imexception("imview_px_shm_sendata should not have been called, this is just a stub.\n");
    return -1;
}
int imview_px_ipc_wind_down(void)
{
    imexception("imview_px_ipc_wind_down should not have been called, this is just a stub.\n");
    return 1;
}

#endif


void imview_init_conn_array(void)
{
    if (!conn_inited) {
	memset(conn_array, 0, sizeof(connection) * MAX_CONN_ARRAY);
	conn_inited = 1;
    }

    return;
}

int imview_fill_conn_array(int         fd,
			   const char *hostname, /* can be NULL */
			   ipctype     use_shm,
			   const char *sync_filename, /* can be NULL */
			   short       port)
{
    int i;

    for (i = 0 ; i < MAX_CONN_ARRAY ; i++) {
	if (!conn_array[i].opened)
	    break;
    }

    if (i >= MAX_CONN_ARRAY)
	return -1; /* no structure left! */

    conn_array[i].opened = 1;
    conn_array[i].fd = fd;
    if (hostname != NULL)
	conn_array[i].hostname = strdup(hostname);
    else
	conn_array[i].hostname = NULL;
    
    conn_array[i].use_shm = use_shm;
    if (sync_filename != NULL)
	conn_array[i].sync_filename = strdup(sync_filename);
    else
	conn_array[i].sync_filename = NULL;
    
    conn_array[i].port = port;

    return i;
}

void imview_release_conn_array(int i)
{
    if (i >= 0) {
	if (conn_array[i].hostname != NULL)
	    free(conn_array[i].hostname);
	if (conn_array[i].sync_filename != NULL)
	    free(conn_array[i].sync_filename);

	conn_array[i].opened = 0;
    }

    return;
}

int imviewlogin(const char *user,        /* user name (default NULL: current user) */
		const char *hn,          /* hostname (default localhost if NULL) */
		int         port,        /* connection port (suitable default if 0) */
		int        *conn_id)     /* return a index in the conn_array */
{
/** log to an imview server

    RETURN VALUE:	int 
    O if success
    Positive value if login is not achieved

    DESCRIPTION:
    This function sets up a TCP connection between the caller and a running
    imview image display server. Set up and login require a hostname, a port
    number and a user name. Sensible default are used if these arguments
    are all 0 or NULL. The returned value is an error code, but the conn_id
    argument is essential for sending further commands to the server and
    for eventually logging out.

    HISTORY:
    Written by Hugues Talbot	 5 Apr 2000

    TESTS:
    Lots and many.

    REFERENCES:

    KEYWORDS:

**/

    char           username[40], *u, *hostname;
    char           login_command[50],login_result[50], sync_filename[50];
    char           FnName[512];
    char          *login_answer; /* result of the login command */
    int            nbscanned;
    int            fd;
    ipctype        use_shm;
    

    sprintf(FnName,"imview_login(%s,%s,%d)", user, hn, port);

    
    if ((hn == 0) || (hn[0] == '\0'))
	hostname = DEFAULTHOST;
    else
	hostname = (char *)hn;

    if (!force_socket && (strcmp(hostname, "localhost") == 0)) {
	use_shm = use_shm_default;
    } else {
	use_shm = SHM_NONE;
    }
    
    /* radical solution for now if sysv IPC are not available */
#if !defined(HAVE_SYSV_IPC) && !defined(HAVE_POSIX_IPC)
    use_shm = SHM_NONE;
#endif
    
    
    /* get the user name */
    username[0] = '\0'; /* empty user field */
    if ((user == NULL) || (user[0] == '\0')) {
	if ((u = getenv(USRID)) != NULL) {
	    strcpy(username, u);
	}
    } else {
	strcpy(username, user);
    }

    if (username[0] == '\0') {
	imexception("%s: no user name specified and no USER environment variable\n");
	return 5;
    }

    LIARdebug("Step 1: open socket");
    if (port == 0) {
	port = DEFAULTPORT;
    }

    fd = imview_open_connection(hostname, port);
    if (fd < 0) {
	imexception("%s: cannot open connection, %s\n",
		    FnName, strerror(errno));
	return 6;
    }
    
    sprintf(login_command, "user %s", username);
    strcat(login_command, EOC);
    
    /* send the login command */
    login_answer = imview_command(fd, login_command); 

    if (login_answer == NULL) {
	imexception("%s: login failed, NULL response\n", FnName);
	imview_close_connection(fd);
	return 7;
    }

    /* analyse the result */
    LIARdebug("Login command result: %s", login_answer);
    nbscanned = sscanf(login_answer, "Welcome, %s %s", login_result, sync_filename);
    if ((nbscanned != 2) || (strcmp(login_result, username) != 0)) {
	imexception("%s: login failed, incorrect user name or unexpected response from server\n", FnName);
	imview_close_connection(fd);
	return 8;
    }

    if (strncmp(sync_filename, "no_shm", 6) == 0) {
	/* NO shared memory! */
	use_shm = SHM_NONE;
					      
    }
    
    /* if we are here, we can fill a new connection structure */
    imview_init_conn_array(); /* safe to call all the time */
    *conn_id = imview_fill_conn_array(fd, hostname, use_shm, sync_filename, port);
    
    return 0;
}

/* CAUTION: this function will only work for command that return something */
/* on the other hand, the something can be as long as desired, assuming the connection is
   fast enough (otherwise lengthen the TIMEOUT parameters, keeping in mind that you will
   wait at least as long at the end)

   The response comes in a static buffer. It will be overwritten next time this function
   is called!

   Hugues Talbot	14 Jul 2000
*/
char *imviewsendcommand(const char *cmd,
			int         conn_id)
{

    return unlimited_imview_general_command(conn_array[conn_id].fd, cmd, 1, 0, TIMEOUT_USEC);
}

int imviewputoverlay(IMAGE     *I, /* the overlay we want to apply on the current image */
		     const char *name, /* name of the overlay */
		     int          conn_id) /* current connection */
{
    char newName[ANSWER_MAX_SIZE];

    strcpy(newName, OVERLAY_MARKER);
    strcat(newName, name);
    /* relay the call */
    return imviewputimage(I, newName, conn_id);
}

int imviewputimage(IMAGE       *I,       /* the image we want to display */
		   const char  *name,    /* imview descriptor (name by which the file is known) */
		   int          conn_id) /* ID of the connection */
{
/** Sends an image to an imview server after a communication has been established.

    RETURN VALUE:	int 
    Error code = 0 if everything went all right.

    DESCRIPTION:
    This functions takes an arbitrary image and uploads it for display to a running
    imview server. It is assumed that a communication has been established using the
    imview_login() function, within the same process, and that therefore the
    connection ID is known.

    See the implementation of imviewput for an example of using this function.


    NOTE:
    At the moment only single and multispectral images can be uploaded. Arbitrary multi
    component images (with different components having different size, pixel type, etc)
    are not uploaded in their entirety: only the first component will be uploaded in this
    fashion. This is due to an incompatibility with the internal Imview format.

    HISTORY:
    Written by Hugues Talbot	 5 Apr 2000

    TESTS:

    REFERENCES:

    KEYWORDS:

    SEE ALSO:
    imview_login
    imviewput

**/

    int            res;   /* result of the call of this function */
    int            c, nbc, nx, ny, nz, ox, oy, oz;
    const char    *imgt, *pixt; /* named version of these parameters */
    int            nb_da; /* nb of bytes received at the delayed answer */
    short          binport; /* the binary data port */
    unsigned       magic_number; /* necessary for socket binary data send */
    char           magic[HEADER_ID_SIZE];
    int            bfd;
    char           FnName[100]; /* for debugging message */
    char           final_name[ANSWER_MAX_SIZE];
    char           subcommand[ANSWER_MAX_SIZE], *cmdres;
    char           put_command[8*ANSWER_MAX_SIZE];
    char           delayed_answer[ANSWER_MAX_SIZE];
    void          *buff;   /* arbitrary component data buffer */
    wsize_t         buflen, rec, tbs = 0;
    pixtype        imp;
    struct timeval start_prog, start_put, start_upload, end_upload, end_prog;
    struct timezone dummy;
    double         time0, time1, time2, time3, time4;
    double         dt0, dt1, dt2, dt3, dt4, dt5;
    

    res = 0;
    
    gettimeofday(&start_prog, &dummy);
    sprintf(FnName,"imviewput(%p,%s,%d)", I, name, conn_id);
    
    if (!I || !name) {
	imexception("%s: requires an image and a name\n", FnName);
	return 1;
    }

    LIARdebug("Basic checks on the image");
     
    /* Make sure we are saving a valid image */
    if (imgetimgtype(I) == IM_BADIMAGE) {
	imexception("%s: bad image\n", FnName);
	return 2;
    }

    /* get the image basic information */
    nbc = imgetnumcomp(I);
    nx = imgetnx(I, 0);
    ny = imgetny(I, 0);
    nz = imgetnz(I, 0);
    ox = imgetfirstx(I, 0);
    oy = imgetfirsty(I, 0);
    oz = imgetfirstz(I, 0);
    imp = imgetpixtype(I, 0);
    imgt = imgetimgtypestr(I);
    pixt = imgetpixtypestr(I,0);

    /* some people don't set IM_MULTI correctly */
    if (imgetimgtype(I) == IM_MULTI) {
	if (nbc > 1) {
	    /* check whether the image is a true multicomponent image */
	    for (c = 1 ; c < nbc ; c ++) {
		if (   (imgetnx(I,c) != nx)
		       || (imgetny(I,c) != ny)
		       || (imgetnz(I,c) != nz)
		       || (imgetfirstx(I,c) != ox)
		       || (imgetfirsty(I,c) != oy)
		       || (imgetfirstz(I,c) != oz)
		       || (imgetpixtype(I, c) != imp)) {
		    if (c > 2) {
			imexception(
			    "%s: warning, cannot cope with true multi-component images yet.\n"
			    "    Only the first %d components will be sent\n",
			    FnName, c+1);
			nbc = c+1;
			break;
		    } else {
			imexception(
			    "%s: warning, cannot cope with true multi-component images yet.\n"
			    "    Only the first component will be sent\n",
			    FnName);
			nbc = 1;
			break;
		    }
		}
	    }
	    if (c >= nbc)
		imexception("%s: warning, IM_MULTI should be reserved for true multi-component images\n",
			    FnName);
	} else
	    imexception("%s: warning, IM_MULTI should be reserved for images with more than one component\n",
			FnName);
    }

    /* 4D images not yet supported either */
    if (imgetnt(I,0) != 1) {
	imexception("%s: imview can't cope with 4D images yet\n", FnName);
	return 4;
    }   

    LIARdebug("Sending Put command");

    final_name[0] = '"';
    strncpy(final_name+1, name, ANSWER_MAX_SIZE-100);
    strcat(final_name, " <memory>\""); /* to identify the data coming from the client */
    sprintf(put_command,
	    "%s %s %d ",
            putcmd[conn_array[conn_id].use_shm],
	    final_name,
	    1);

    /* for the time being we only support SPECTRUM-type image (all nx, etc the same) */
    
    sprintf(subcommand,"%d %d %d %d %d %d %s %s %d ",
	    nx, ny, nz,
	    ox, oy, oz,
	    imgt, pixt, nbc);
    strcat(put_command, subcommand);

    strcat(put_command, EOC); /* end of command */
    LIARdebug("Command: %s\n", put_command);
    
    /* send the actual command */
    LIARdebug("Shared memory type: %s", putcmd[conn_array[conn_id].use_shm] );

    /* priority is SYSV > POSIX > SOCKET */
    /* at this stage POSIX IPC is not well-tested */
    if (conn_array[conn_id].use_shm == SHM_SYSV) { /* using Shared Memory */
	gettimeofday(&start_put, &dummy);
	if ((imview_command_noanswer(conn_array[conn_id].fd, put_command) != NULL) 
	    && (imview_sysv_ipc_setup(conn_array[conn_id].sync_filename) == 0)) {
	    gettimeofday(&start_upload, &dummy);
	    for (c = 0 ; c < nbc ; c++) {
		buff = imgetbuff(I, c);
		buflen = imgetnx(I,c) * imgetny(I,c) * imgetnz(I,c) * imgetnt(I,c);
		tbs = buflen * imgetpixsize(I,c);
		rec = imview_sysv_shm_sendata(buff, tbs);
		if (rec != tbs ) {
		    imexception("Incomplete transfer, sent: %d, wanted to send: %d\n", rec, tbs);
		    res = 7;
		    break;
		}
	    }
	    gettimeofday(&end_upload, &dummy);
	    /* release access to shared memory */
	    imview_sysv_ipc_wind_down();
	    nb_da=imview_get_delayed_response(conn_array[conn_id].fd, delayed_answer); /* I don't know that this is necessary */
	    if (nb_da < 0)
		imexception("Transfer probably failed\n");
	    else
		LIARdebug("Transfer response: %s", delayed_answer);
	} else {
	    imexception("SYSV IPC setup failed, closing connection\n");
	    imview_release_conn_array(conn_id);
	    res = 9;
	}
    } else if (conn_array[conn_id].use_shm == SHM_POSIX) { /* using POSIX IPC */
        char *ppath_answer = NULL; 
        
        gettimeofday(&start_put, &dummy);
        /* find the path to the resources */
        ppath_answer = imview_command(conn_array[conn_id].fd, "ppath" EOC);
        sscanf(ppath_answer,"%s ", ipc_base_path);
        LIARdebug("ipc_base_path = (%s)", ipc_base_path);
	if ((imview_command_noanswer(conn_array[conn_id].fd, put_command) != NULL) 
	    && (imview_px_ipc_setup(ipc_base_path) == 0)) {
	    gettimeofday(&start_upload, &dummy);
	    for (c = 0 ; c < nbc ; c++) {
		buff = imgetbuff(I, c);
		buflen = imgetnx(I,c) * imgetny(I,c) * imgetnz(I,c) * imgetnt(I,c);
		tbs = buflen * imgetpixsize(I,c);
		rec = imview_px_shm_sendata(buff, tbs);
		if (rec != tbs ) {
		    imexception("Incomplete transfer, sent: %d, wanted to send: %d\n", rec, tbs);
		    res = 7;
		    break;
		}
	    }
	    gettimeofday(&end_upload, &dummy);
	    /* release access to shared memory */
	    imview_px_ipc_wind_down();
	    nb_da=imview_get_delayed_response(conn_array[conn_id].fd, delayed_answer); /* I don't know that this is necessary */
	    if (nb_da < 0)
		imexception("Transfer probably failed\n");
	    else
		LIARdebug("Transfer response: %s", delayed_answer);
	} else {
	    imexception("POSIX IPC setup failed, closing connection\n");
	    imview_release_conn_array(conn_id);
	    res = 9;
	}
    } else { /* upload via the TCP/IP connection */
	gettimeofday(&start_put, &dummy);
	cmdres = imview_command(conn_array[conn_id].fd, put_command); 
	if (cmdres == NULL) {
	    imexception("imview_put: sent command didn't work\n");
	    /* imview_close_connection(conn_array[conn_id].fd); */
	    imview_release_conn_array(conn_id);
	    return 8;
	}

	LIARdebug("Got result: %s", cmdres);
	/* check the result. It should contain a port number and a magic number */
	sscanf(cmdres, "%hd %u", &binport, &magic_number); /* match a shord and an unsigned */
	memset(magic, 0, HEADER_ID_SIZE);
	memcpy(magic, &magic_number, sizeof(unsigned int)); /* the rest of the header is padded with 0s */

	if (binport > 0 && magic > 0) {
	    /* step 3: everything is peachy, send the binary data now */
	    LIARdebug("OK, sending binary data now");
	    /* 3a: open binary port connection (same host) */
	    if ((bfd = imview_open_binconn(conn_array[conn_id].hostname, binport)) > 0) {
		gettimeofday(&start_upload, &dummy);
		/* 3b: send back the magic number */
		if (imview_send_data(bfd, (char *)(&magic), HEADER_ID_SIZE, 0) == HEADER_ID_SIZE) {
		    /* 3c: send each buffer in turn */
		    for (c = 0 ; c < nbc ; c++) {
			LIARdebug("Sending buffer #%d", c);
			buff = imgetbuff(I, c);
			buflen = imgetnx(I,c) * imgetny(I,c) * imgetnz(I,c) * imgetnt(I,c);
			tbs = buflen * imgetpixsize(I,c);
			rec = imview_send_data(bfd,
					       (char *)buff,
					       tbs,
					       (c == (nbc-1))); /* check for end of transmission */
			if (rec != tbs ) {
			    imexception("Incomplete transfer, sent: %d, wanted to send: %d\n", rec, tbs);
			    res = 7;
			    break;
			}
		    }
		    gettimeofday(&end_upload, &dummy);
		    /* close the binary connection */
		    LIARdebug("Closing binary connection");
		    imview_close_binconn(bfd);
		}
	    }
	}
    }
    gettimeofday(&end_prog,&dummy);

    time0 = start_prog.tv_sec + start_prog.tv_usec/1e6;
    time1 = start_put.tv_sec + start_put.tv_usec/1e6;
    dt0 = time1 - time0;
    time2 = start_upload.tv_sec + start_upload.tv_usec/1e6;
    dt1 = time2 - time1;
    time3 = end_upload.tv_sec + end_upload.tv_usec/1e6;
    dt2 = time3 - time2;
    time4 = end_prog.tv_sec + end_prog.tv_usec/1e6;
    dt3 = time4-time3;
    dt4 = time3-time1; 
    dt5 = time4-time0;

    LIARdebug("\n"
	      "Timings: start to put                : %6.4f sec\n"
	      "         put to beginning of upload  : %6.4f sec\n"
	      "         begining of upload to end   : %6.4f sec\n"
	      "         close conn to exit          : %6.4f sec\n"
	      "-------------------------------------           \n"
	      "         put to close conn           : %6.4f sec\n"
	      "         start to exit               : %6.4f sec\n",
	      dt0, dt1, dt2, dt3, dt4, dt5);

    if ((dt1>0) && (dt2> 0) && (dt4 > 0) && (dt5 > 0))
	LIARdebug("\n"
		  "Throughput:             upload only : %10.3f kB/s\n"
		  "                       steady state : %10.3f kB/s\n"
		  "                     whole process  : %10.3f kB/s\n",
		  (tbs*nbc)/(dt2*1024), (tbs*nbc)/(dt4*1024), (tbs*nbc)/(dt5*1024));
    
    return res;
}

void imviewlogout(int conn_id) /* ID of the connection */
{
/** terminates a connection to an existing imview server

    RETURN VALUE:	void 

    DESCRIPTION:
    This function logs the user out of an existing imview connection.
    It is essential to do this in order not to run out of connections.

    HISTORY:
    Written by Hugues Talbot	 5 Apr 2000

    TESTS:

    REFERENCES:

    KEYWORDS:

**/

    /* close the connection */
    imview_close_connection(conn_array[conn_id].fd);
    imview_release_conn_array(conn_id);

    return;
}

int imviewput(IMAGE          *I,         /* input image */
	      const char     *name,      /* identifier */
	      const char     *user,      /* optional user name (NULL default: current user) */
	      const char     *hostname,  /* hostname (default localhost if NULL) */
	      short           port)      /* connection port (suitable default if 0) */
{
/** Puts an image to the imview server for display

    RETURN VALUE:
    0 if success. Error code > 0 if not.

    DESCRIPTION:
    This function puts an arbitrary image to imview. It uses
    a TCP/IP protocol to connect and upload to an existing imview
    server.

    NOTE:
    The upload will be known to imview through its name. If
    two uploads are made with the same name, the later one
    to complete will overwrite the earlier one. Uploads
    with different names are retained (of course they can
    be deleted manually by the imview user).
   
    HISTORY:
    Written by Hugues Talbot	 9 Mar 2000
   
    TESTS:

    REFERENCES:

    KEYWORDS:
    display, socket, tcp/ip.
**/
   
    int            cmdres;      /* what various command return */
    int            conn_id;     /* connection ID */
    char           FnName[512]; /* Function name (used all the time) */
    
    sprintf(FnName,"imviewput(%s,%s,%d)", name, hostname, port);
    

    /* Step 1: establish connextion and log in */
    cmdres = imviewlogin(user, hostname, port, &conn_id);
    if (cmdres != 0) {
	imexception("%s: login failed\n", FnName);
	return 1;
    }
	
    /* Step 2: put command */
    cmdres = imviewputimage(I, name, conn_id);
    if (cmdres != 0) 
	imexception("%s: put command failed\n", FnName); 

    /* don't return, we need to close the connection */    
    /* Step 3: close connection */
    LIARdebug("Closing command connection");
    imviewlogout(conn_id);

    return 0;
}

int imviewput2(IMAGE          *I,         /* input image */
	       const char     *name,      /* identifier */
	       const char     *user,      /* optional user name (NULL default: current user) */
	       const char     *hostname,  /* hostname (default localhost if NULL) */
	       short           port)      /* connection port (suitable default if 0) */
{
/**
   Puts an image to the imview server for display                    
                                                                      
   RETURN VALUE:                                                     
   0 if success. Error code > 0 if not.                              
                                                                      
   DESCRIPTION:                                                      
   This function puts an arbitrary image to imview. It uses          
   a TCP/IP protocol to connect and upload to an existing imview     
   server.                                                           
                                                                      
   NOTE:                                                             
   The upload will be known to imview through its name. If           
   two uploads are made with the same name, the later one            
   to complete will overwrite the earlier one. Uploads               
   with different names are retained (of course they can             
   be deleted manually by the imview user).                          
                                                                      
   HISTORY:                                                          
   Written by Hugues Talbot         9 Mar 2000                       
                                                                      
   TESTS:                                                            
                                                                      
   REFERENCES:                                                       
                                                                      
   KEYWORDS:                                                         
   display, socket, tcp/ip.                                          
**/
    void          *buff;
    wsize_t         buflen, rec, tbs = 0;
    int            c, fd, bfd, res;
    char           FnName[100];
    short          binport;
    unsigned       magic_number;
    char           magic[HEADER_ID_SIZE];
    int            nbc;
    char           final_name[ANSWER_MAX_SIZE];
    char           put_command[8*ANSWER_MAX_SIZE];
    char           subcommand[ANSWER_MAX_SIZE], *cmdres;
    char           username[40], *u;
    char           login_command[50], login_result[50], sync_filename[50];
    char           delayed_answer[ANSWER_MAX_SIZE];
    int            nbscanned;
    ipctype        use_shm;
    int            nb_da;
    int            nx, ny, nz, ox, oy, oz;
    pixtype        imp;
    const char    *imgt, *pixt;
    struct timeval start_prog, start_opensocket, start_login; 
    struct timeval start_put, start_upload, end_upload, before_close, end_prog;
    struct timezone dummy;
    double         time0, time1, time2, time3, time4, time5, time6, time7;
    double         dt0, dt1, dt2, dt3, dt4, dt5, dt6, dt7, dt8;

    gettimeofday(&start_prog, &dummy);
    sprintf(FnName,"imviewput2(%s,%s,%d)", name, hostname, port);
    res = 0; /* all peachy */
    
    if (!I || !name) {
	imexception("%s: requires an image and a name\n", FnName);
	return 1;
    }
    /* NOTES:
    ** The server decides on the integer identifier
    ** names decide on the removal policy
    */
    
    /* Make sure we are saving a valid image */
    if (imgetimgtype(I) == IM_BADIMAGE) {
	imexception("%s: bad image\n", FnName);
	return 2;
    }

    nbc = imgetnumcomp(I);
    
    /* some people don't set IM_MULTI correctly */
    if (imgetimgtype(I) == IM_MULTI) {
	if (nbc > 1) {
	    /* check whether the image is a true multicomponent image */
	    nx = imgetnx(I, 0);
	    ny = imgetny(I, 0);
	    nz = imgetnz(I, 0);
	    ox = imgetfirstx(I, 0);
	    oy = imgetfirsty(I, 0);
	    oz = imgetfirstz(I, 0);
	    imp = imgetpixtype(I, 0);
	    for (c = 1 ; c < nbc ; c ++) {
		if (   (imgetnx(I,c) != nx)
		       || (imgetny(I,c) != ny)
		       || (imgetnz(I,c) != nz)
		       || (imgetfirstx(I,c) != ox)
		       || (imgetfirsty(I,c) != oy)
		       || (imgetfirstz(I,c) != oz)
		       || (imgetpixtype(I, c) != imp)) {
		    if (c > 2) {
			imexception(
			    "%s: warning, cannot cope with true multi-component images yet.\n"
			    "    Only the first %d components will be sent\n", c+1);
			nbc = c+1;
			break;
		    } else {
			imexception(
			    "%s: warning, cannot cope with true multi-component images yet.\n"
			    "    Only the first component will be sent\n");
			nbc = 1;
			break;
		    }
		}
	    }
	    if (c>=nbc)
		imexception("%s: warning, IM_MULTI should be reserved for true multi-component images\n");
		
	} else
	    imexception("%s: warning, IM_MULTI should be reserved for images with more than one component\n");
    }

    /* 4D images not yet supported either */
    if (imgetnt(I,0) != 1) {
	imexception("%s: imview can't cope with 4D images yet\n", FnName);
	return 4;
    }

    /* get the user name */
    username[0] = '\0'; /* empty user field */
    if ((user == NULL) || (user[0] == '\0')) {
	if ((u = getenv(USRID)) != NULL) {
	    strcpy(username, u);
	}
    } else {
	strcpy(username, user);
    }

    if (username[0] == '\0') {
	imexception("%s: no user name specified and no USER environment variable\n");
	return 5;
    }

    gettimeofday(&start_opensocket, &dummy);
    LIARdebug("Step 1: open socket");
    /* step 1: open a socket to the imview command server */
    if (port == 0) {
	port = DEFAULTPORT;
    }

    if ((hostname == 0) || (hostname[0] == '\0'))
	hostname = DEFAULTHOST;

    if (!force_socket && (strcmp(hostname, "localhost") == 0)) {
	use_shm = use_shm_default;
    } else {
	use_shm = SHM_NONE;
    }
    
    /* radical solution for now */
#if !defined(HAVE_SYSV_IPC) && !defined(HAVE_POSIX_IPC)
    use_shm = SHM_NONE;
#endif
    
    fd = imview_open_connection(hostname, port);
    if (fd < 0) {
	imexception("%s: cannot open connection, %s\n",
		    FnName, strerror(errno));
	return 6;
    }

    gettimeofday(&start_login, &dummy);
    LIARdebug("Step 2: log in");
    /* step 2: log in */
    sprintf(login_command, "user %s", username);
    strcat(login_command, EOC);
    
    /* flush the input from any previous answer. They would confuse the issue */
    /* imview_flush_input(fd); */
    cmdres = imview_command(fd, login_command); 

    if (cmdres == NULL) {
	imexception("%s: login command didn't work\n", FnName);
	imview_close_connection(fd);
	return 7;
    }

    LIARdebug("Login command result: %s", cmdres);
    nbscanned = sscanf(cmdres, "Welcome, %s %s", login_result, sync_filename);
    if ((nbscanned != 2) || (strcmp(login_result, username) != 0)) {
	imexception("%s: login failed\n", FnName);
	imview_close_connection(fd);
	return 8;
    }


    /* step 2: send a PUT command */
    gettimeofday(&start_put, &dummy);
    LIARdebug("Sending Put command");
    final_name[0] = '"';
    strncpy(final_name+1, name, ANSWER_MAX_SIZE-100);
    strcat(final_name, " <memory>\""); /* to identify the data coming from the client */
    sprintf(put_command,
	    "%s %s %d ",
            putcmd[use_shm],
	    final_name,
	    1);

    /* for the time being we only support SPECTRUM-type image (all nx, etc the same) */
    c = 0;
    nx = imgetnx(I,c);
    ny = imgetny(I,c);
    nz = imgetnz(I,c);
    ox = imgetfirstx(I,c);
    oy = imgetfirsty(I,c);
    oz = imgetfirstz(I,c);
    imgt = imgetimgtypestr(I);
    pixt = imgetpixtypestr(I,c);
    
    sprintf(subcommand,"%d %d %d %d %d %d %s %s %d ",
	    nx, ny, nz,
	    ox, oy, oz,
	    imgt, pixt, nbc);
    strcat(put_command, subcommand);

    strcat(put_command, EOC); /* end of command */

    /* flush the input from any previous answer. They would confuse the issue */
    /* imview_flush_input(fd); no need I think*/

    /* send the actual command */
    if (use_shm == SHM_SYSV) {
	imview_command_noanswer(fd, put_command);
	if (imview_sysv_ipc_setup(sync_filename) == 0) {
	    gettimeofday(&start_upload, &dummy);
	    for (c = 0 ; c < nbc ; c++) {
		buff = imgetbuff(I, c);
		buflen = imgetnx(I,c) * imgetny(I,c) * imgetnz(I,c) * imgetnt(I,c);
		tbs = buflen * imgetpixsize(I,c);
		rec = imview_sysv_shm_sendata(buff, tbs);
		if (rec != tbs ) {
		    imexception("Incomplete transfer, sent: %d, wanted to send: %d\n", rec, tbs);
		    res = 7;
		    break;
		}
	    }
	    gettimeofday(&end_upload, &dummy);
	    /* release access to shared memory */
	    imview_sysv_ipc_wind_down();
	    nb_da=imview_get_delayed_response(fd, delayed_answer); /* I don't know that this is necessary */
	    if (nb_da < 0)
		imexception("Transfer probably failed\n");
	    else
		LIARdebug("Transfer response: %s", delayed_answer);
	} else {
	    imexception("SYSV IPC setup failed, closing connection\n");
	}
    } else if (use_shm == SHM_POSIX) {
        char *ppath_answer = imview_command(fd, "ppath" EOC);
        /* find the path to the resources */
        sscanf(ppath_answer, "%s ", ipc_base_path);
        LIARdebug("ipc_base_path = (%s)", ipc_base_path);
        
        imview_command_noanswer(fd, put_command);
        
	if (imview_px_ipc_setup(ipc_base_path) == 0) {
	    gettimeofday(&start_upload, &dummy);
	    for (c = 0 ; c < nbc ; c++) {
		buff = imgetbuff(I, c);
		buflen = imgetnx(I,c) * imgetny(I,c) * imgetnz(I,c) * imgetnt(I,c);
		tbs = buflen * imgetpixsize(I,c);
		rec = imview_px_shm_sendata(buff, tbs);
		if (rec != tbs ) {
		    imexception("Incomplete transfer, sent: %d, wanted to send: %d\n", rec, tbs);
		    res = 7;
		    break;
		}
	    }
	    gettimeofday(&end_upload, &dummy);
	    /* release access to shared memory */
	    imview_px_ipc_wind_down();
	    nb_da=imview_get_delayed_response(fd, delayed_answer); /* I don't know that this is necessary */
	    if (nb_da < 0)
		imexception("Transfer probably failed\n");
	    else
		LIARdebug("Transfer response: %s", delayed_answer);
	} else {
	    imexception("POSIX IPC setup failed, closing connection\n");
	}
    } else {
	cmdres = imview_command(fd, put_command); 
	
	if (cmdres == NULL) {
	    imexception("%s: sent command didn't work\n", FnName);
	    imview_close_connection(fd);
	    return 8;
	}

	LIARdebug("Got result: %s", cmdres);
	/* check the result. It should contain a port number and a magic number */
	sscanf(cmdres, "%hd %u", &binport, &magic_number); /* match a shord and an unsigned */
	memset(magic, 0, HEADER_ID_SIZE);
	memcpy(magic, &magic_number, sizeof(unsigned int)); /* the rest of the header is padded with 0s */

	if (binport > 0 && magic > 0) {
	    /* step 3: everything is peachy, send the binary data now */
	    LIARdebug("OK, sending binary data now");
	    /* 3a: open binary port connection (same host) */
	    if ((bfd = imview_open_binconn(hostname, binport)) > 0) {
		/* 3b: send back the magic number */
		gettimeofday(&start_upload, &dummy);
		if (imview_send_data(bfd, (char *)(&magic), HEADER_ID_SIZE, 0) == HEADER_ID_SIZE) {
		    /* 3c: send each buffer in turn */
		    for (c = 0 ; c < nbc ; c++) {
			LIARdebug("Sending buffer #%d", c);
			buff = imgetbuff(I, c);
			buflen = imgetnx(I,c) * imgetny(I,c) * imgetnz(I,c) * imgetnt(I,c);
			tbs = buflen * imgetpixsize(I,c);
			rec = imview_send_data(bfd,
					       (char *)buff,
					       tbs,
					       (c == (nbc-1))); /* check for end of transmission */
			if (rec != tbs ) {
			    imexception("Incomplete transfer, sent: %d, wanted to send: %d\n", rec, tbs);
			    res = 7;
			    break;
			}
		    }
		    gettimeofday(&end_upload, &dummy);
		    /* close the binary connection */
		    LIARdebug("Closing binary connection");
		    imview_close_binconn(bfd);
		}
	    }
	}
    }
    gettimeofday(&before_close,&dummy);
    /* close connection gracefully */
    LIARdebug("Closing command connection");
    imview_close_connection(fd);
    gettimeofday(&end_prog,&dummy);

    /* compute timings */
    time0 = start_prog.tv_sec + start_prog.tv_usec/1e6;
    time1 = start_opensocket.tv_sec + start_opensocket.tv_usec/1e6;
    dt0 = time1 - time0;
    time2 = start_login.tv_sec + start_login.tv_usec/1e6;
    dt1 = time2 - time1;
    time3 = start_put.tv_sec + start_put.tv_usec/1e6;
    dt2 = time3 - time2;
    time4 = start_upload.tv_sec + start_upload.tv_usec/1e6;
    dt3 = time4 - time3;
    time5 = end_upload.tv_sec + end_upload.tv_usec/1e6;
    dt4 = time5 - time4;
    time6 = before_close.tv_sec + before_close.tv_usec/1e6;
    dt5 = time6-time5;
    time7 = end_prog.tv_sec + end_prog.tv_usec/1e6;
    dt6 = time7-time6;
    dt7 = time6-time3; 
    dt8 = time7-time0;

    
    LIARdebug("\n"
	      "Timings: start to opening of socket  : %6.4f sec\n"
	      "         opening of socket to login  : %6.4f sec\n"
	      "         login to put                : %6.4f sec\n"
	      "         put to beginning of upload  : %6.4f sec\n"
	      "         begining of upload to end   : %6.4f sec\n"
	      "         end of upload to close conn : %6.4f sec\n"
	      "         close conn to exit          : %6.4f sec\n"
	      "-------------------------------------           \n"
	      "         put to close conn           : %6.4f sec\n"
	      "         start to exit               : %6.4f sec\n",
	      dt0, dt1, dt2, dt3, dt4, dt5, dt6, dt7, dt8);

    if ((dt2> 0) && (dt1 > 0))
	LIARdebug("\n"
		  "Throughput:             upload only : %10.3f kB/s\n"
		  "                       steady state : %10.3f kB/s\n"
		  "                     whole process  : %10.3f kB/s\n",
		  (tbs*nbc)/(dt4*1024), (tbs*nbc)/(dt7*1024), (tbs*nbc)/(dt8*1024));

    return res;
}
