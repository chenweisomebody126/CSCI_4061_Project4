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
  //repeatedly monitor the request queue and pick up the request from it
  while (true){
    push_queue(request);
  }
  //serve the request back to the client

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
  init(argv[1]);





  return 0;
}
