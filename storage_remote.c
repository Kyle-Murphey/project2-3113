/**
   API for remote storage.  Identical API as provided by the local storage implementation 
 */
#include <string.h>
#include "storage_remote.h"
#include "storage_common.h"
#include "comm.h"

/**
   initialize the storage

   Open the two fds (in and out), wait for an init message, initialize the local storage
 */
STORAGE * init_storage(char * name)
{
  // Create space for the STORAGE object
  STORAGE *s = malloc(sizeof(STORAGE));
  HEADER *init = malloc(sizeof(HEADER));
  char *buffer = malloc(sizeof(HEADER));
  int fd_in = -1;
  int fd_out;

  init->type = INIT_CONNECTION;
  init->len_message = 0;
  init->location = -1;
  init->len_buffer = -1;

  fd_in = open(PIPE_NAME_TO_STORAGE, O_WRONLY);

  if (fd_in < 0)
  {
    fprintf(stderr, "Couldn't open pipe\n");
    exit(-1);
  }
  printf("opened write pipe | fd_in:%d\n", fd_in);
  sleep(1);

  int written = write(fd_in, init, sizeof(HEADER));
  if ( written != sizeof(HEADER))
  {
    printf("lmao");
    fprintf(stderr, "Couldn't send message\n");
    exit(-1);
  }
  printf("%d\n", written);
  printf("connected to pipe\n");
  sleep(1);

  // All okay 
  return s;
};

/**
   Shut down the connection

   Tell the server to shut down
 */
int close_storage(STORAGE *storage)
{
  // Create the shutdown message
  HEADER header;



  
  // Free the storage struction
  free(storage);

  // Done
  return(0);
}

/**
   read bytes from the storage
 */
int get_bytes(STORAGE *storage, unsigned char *buf, int location, int len)
{



  // Success
  return(len);
};


/**
   Write bytes to the storae

   Send the write request message + the data
 */
int put_bytes(STORAGE *storage, unsigned char *buf, int location, int len)
{



  // Success
  return(len);
};

