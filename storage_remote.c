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
  printf("sent init\n");
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
    if (reader != init->len_message)
    {
      fprintf(stderr, "Couldn't get file\n");
      exit(-1);
    }
    printf("got file\n");   
    s->fd_to_storage = fd_in;
    s->fd_from_storage = fd_out; 
  }
  ///

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
  HEADER * read_req = malloc(sizeof(HEADER));

  read_req->type = READ_REQUEST;
  read_req->len_message = 0;
  read_req->location = location;
  read_req->len_buffer = len;

  //send READ message
  int written = write(storage->fd_to_storage, read_req, sizeof(HEADER));
  if (written != sizeof(HEADER))
  {
    fprintf(stderr, "Couldn't send READ_REQUEST\n");
    exit(-1);
  }
  printf("READ_REQUEST sent\n");
  ///

  //get DATA header
  int reader = read(storage->fd_from_storage, read_req, sizeof(HEADER));
  if (reader != sizeof(HEADER))
  {
    fprintf(stderr, "Couldn't get DATA message\n");
    exit(-1);
  }
  printf("Recieved DATA\n");
  ///

  //get the data from the server
  reader = read(storage->fd_from_storage, buf, read_req->len_message);
  if (reader != read_req->len_message)
  {
    fprintf(stderr, "Couldn't read from file\n");
    exit(-1);
  }
  printf("Data read\n");
  ///

  // Success
  return(len);
};


/**
   Write bytes to the storae

   Send the write request message + the data
 */
int put_bytes(STORAGE *storage, unsigned char *buf, int location, int len)
{
  HEADER * write_req = malloc(sizeof(HEADER));

  write_req->type = WRITE_REQUEST;
  write_req->len_message = 128;
  write_req->location = location;
  write_req->len_buffer = len;

  //send WRITE_REQUEST to server
  int written = write(storage->fd_to_storage, write_req, sizeof(HEADER));
  if (written != sizeof(HEADER))
  {
    fprintf(stderr, "Couldn't send WRITE_REQUEST\n");
    exit(-1);
  }
  printf("sent WRITE\n");
  ///

  //send data to be written to server
  written = write(storage->fd_to_storage, buf, write_req->len_message);
  if (written != write_req->len_message)
  {
    fprintf(stderr, "Couldn't send data\n");
    exit(-1);
  }
  printf("sent data\n");
  ///

  //get ACK from server
  int reader = read(storage->fd_from_storage, write_req, sizeof(HEADER));
  if (write_req->type != ACKNOWLEDGE)
  {
    fprintf(stderr, "Couldn't get ACK\n");
    exit(-1);
  }
  printf("Got ACK\n");
  ///

  // Success
  return(len);
};

