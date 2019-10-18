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
  int fd_in;
  int fd_out;

  init->type = INIT_CONNECTION;
  init->len_message = strlen(name);
  init->location = -1;
  init->len_buffer = -1;

  //opening writing pipe
  fd_in = open(PIPE_NAME_TO_STORAGE, O_WRONLY);
  if (fd_in < 0)
  {
    fprintf(stderr, "Couldn't open pipe\n");
    exit(-1);
  }
  printf("opened write pipe | fd_in:%d\n", fd_in);
  sleep(1);
  ///

  //opening reading pipe
  fd_out = open(PIPE_NAME_FROM_STORAGE, O_RDONLY);
  if (fd_out < 0)
  {
    fprintf(stderr, "Couldn't open pipe\n");
    exit(-1);
  }
  printf("opened read pipe | fd_out:%d\n", fd_out);
  sleep(1);
  ///

  //sending init message and then the name of the file
  int written = write(fd_in, init, sizeof(HEADER));
  if (written != sizeof(HEADER))
  {
    fprintf(stderr, "Couldn't send header\n");
    exit(-1);
  }
  printf("send init\n");
  ///

  //give server file name
  written = write(fd_in, name, strlen(name));
  if (written != strlen(name))
  {
  fprintf(stderr, "Couldn't send file name\n");
  exit(-1);
  }
  printf("name written\n");
  ///

  //get ack
  int reader = read(fd_out, init, sizeof(HEADER));  //error check
  if (init->type == ACKNOWLEDGE)
  {
    printf("got ACKNOWLEDGE\n");
    //get file
    reader = read(fd_out, s, init->len_message); //error check
    printf("%d number of bytes read\n", reader);
    printf("%d init message length\n",init->len_message);
    if (reader != init->len_message) //STORAGE different
    {
      fprintf(stderr, "Couldn't get file\n");
      exit(-1);
    }
    printf("got file\n");    
  }
  ///

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

