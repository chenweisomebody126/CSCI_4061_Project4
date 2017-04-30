/* csci4061 S2016 Assignment 4
* section: one_digit_number
* date: mm/dd/yy
* names: Name of each member of the team (for partners)
* UMN Internet ID, Student ID (xxxxxxxx, 4444444), (for partners)
*/

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "util.h"

#define MAX_THREADS 100
#define MAX_QUEUE_SIZE 100
#define MAX_REQUEST_LENGTH 1024

//Structure for a single request.
typedef struct request
{
  int m_socket;
  char  m_szRequest[MAX_REQUEST_LENGTH];
} request_t;

/* The mutex lock */
pthread_mutex_t request_queue_access= PTHREAD_MUTEX_INITIALIZER;;

/* the condiitonal variables */
pthread_cond_t some_request = PTHREAD_COND_INITIALIZER;
pthread_cond_t free_slot = PTHREAD_COND_INITIALIZER;

/* the bounded buffer to store request*/
request_t request_queue[MAX_QUEUE_SIZE];

/* request counter and
in for insert position and
out for push position
*/
int count=0;
int in=0;
int out=0;

void insert_queue(request_t* request){
  pthread_mutex_lock(&request_queue_access);
  while (count == MAX_QUEUE_SIZE){
    pthread_cond_wait(&free_slot, &request_queue_access);
    request_queue[in] =request;
    in = (in+1)%MAX_QUEUE_SIZE;
    count++;
  }
  pthread_cond_signal(&some_request);
  pthread_mutex_unlock(&request_queue_access);
}

void push_queue(request_t* request){
  pthread_mutex_lock(&request_queue_access);
  while(count == 0){
    pthread_cond_wait(&some_request, &request_queue_access);
    request = request_queue[out];
    out = (out-1)%MAX_QUEUE_SIZE;
    count--;
  }
  pthread_cond_signal(&free_slot);
  pthread_mutex_unlock(&request_queue_access);
}
/*repeatedly accept an incoming connection,
read the request from the connection,
insert the request in the request queue
*/
void * dispatch(void * arg)
{
  //assigne ONE file discriptor to each thread to accept the connection
  int fd;
  char filename[1024];
  request_t * request;
  while (((fd =accept_connection())>=0){
    //repeatedly read the request from the connection
    if (get_request(fd, filename)!=0){
      //when get faulty request, do NOT exit, but recover it by "continue"
      printf("get_request failure due to faulty requests");
      continue;
    }
    request->m_socket =fd;
    strcpy(request->m_szRequest,filename);
    insert_queue(request);
  }
  return NULL;
}

void * worker(void * arg)
{
  request_t* request;
  char* content_type;

  worker_t* worker_struct;
  memcpy(worker_struct, (worker_t *) arg, sizeof(worker_t));
  //repeatedly monitor the request queue and pick up the request from it
  while (true){
    push_queue(request);
    /*serve the request back to the client by using return_result()
    if there was any problem with accessing the file, use return_error() instead
    */
    //read the file from fd

    //open the file to obtain file pointer

    //obtain the actual size of file

    //read from file num_bytes_read

    //set content_type according to the suffix

    if(return_result(request->m_socket, content_type,request->m_szRequest, 1024)!=0){
      return_error(request->m_socket, request->m_szRequest);
    }
  }

  return NULL;
}

int main(int argc, char **argv)
{
        //Error check first.
  if(argc != 6 && argc != 7)
  {
    printf("usage: %s port path num_dispatcher num_workers queue_length [cache_size]\n", argv[0]);
    return -1;
  }

  printf("Call init() first and make a dispather and worker threads\n");
//initializes the connection once in the main thread
  int port = atoi(argv[1]);
  init(port);
  /*
create an array of dispathers and an array of workers,
each consists of threadId, regNum, path
  */
  int i;
  int error;
  pthread_t dispatchers[MAX_THREADS], workers[MAX_THREADS];
  num_dispatchers = atoi(argv[3]);
  num_workers = atoi(argv[4]);
  //loop through each dispatcher to create dispatch thread
  for (i=0; i<num _dispatchers; i++){
    if ((error = pthread_create(&dispatchers[i], NULL, dispatch, NULL))!=0){
      fprintf(stderr, "fail to create dispatch thread %d: %s", i+1, strerror(error));
      return -1;
    }
  }
  //loog through each worker to create each worker thread
  for (i=0; i<num_workers ; i++){
    if((error=pthread_create(&workers[i], NULL, worker, NULL))!=0){
      fprintf(stderr, )
    }
  }




  return 0;
}
