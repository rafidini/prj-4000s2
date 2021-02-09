/*------------------------------------------------------------------------
 *
 * This simple client program starts up an imview, where the portfile
 * is in fact a named pipe. This is trickier than with a file but it
 * works better if you want to start several imviews, as it forces
 * a measure of synchronization.
 *
 * Hugues Talbot	14 Mar 2001
 *      
 *-----------------------------------------------------------------------*/


#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>

#define PROG "startim"
#define PIPESTR "/tmp/toto"
#define IMVIEWCMD  "imview -server -fork -portfile " PIPESTR

#define FAIL do {perror(PROG); unlink(PIPESTR); return(1);} while (0)

int startim_main()
{

#ifndef _WINDOWS

  char *execstring = IMVIEWCMD;
  char buffer[5];
  int   portnum, filenum, res;

  unlink(PIPESTR); /* might fail, don't care */

  res = mkfifo(PIPESTR,S_IRUSR|S_IWUSR);
  if (res != 0)
    FAIL;

  /* open the pipe for reading, notice the NO DELAY  */
  filenum = open(PIPESTR, O_RDONLY | O_NDELAY, 0);
  if (filenum < 0)
    FAIL;
    
  /* then exec imview */
  printf("Imview away...\n");
  system(execstring);

  /* then read the pipe */
  memset(buffer, '\0', 5);
  if (read(filenum, buffer, 4) <= 0)
    FAIL;


  printf("port number is %s\n", buffer);

  close(filenum);
  unlink(PIPESTR); 

#endif /* _WINDOWS */

  return 0;
}
