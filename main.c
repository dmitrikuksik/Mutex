#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include "queue.h"
#include "sem.h"


int main(){
  int segment_id;

  segment_id = shmget(KEY_SM_A,shared_memory_size, 0666 | IPC_CREAT);
  if(segment_id < 0){
    perror("Problem with sharing memmory");
    exit(1);
  };

  segment_id = shmget(KEY_SM_B,shared_memory_size,0666 | IPC_CREAT);
  if(segment_id < 0){
    perror("Problem with sharing memmory");
    exit(1);
  };

  segment_id = shmget(KEY_SM_C,shared_memory_size,0666 | IPC_CREAT);
  if(segment_id < 0){
    perror("Problem with sharing memmory");
    exit(1);
  };

  initializeSemaphore(MUTEX_A,1);
  initializeSemaphore(MUTEX_B,1);
  initializeSemaphore(MUTEX_C,1);

  initializeSemaphore(EMPTY_A,SIZE);
  initializeSemaphore(EMPTY_B,SIZE);
  initializeSemaphore(EMPTY_C,SIZE);

  initializeSemaphore(FULL_A,0);
  initializeSemaphore(FULL_B,0);
  initializeSemaphore(FULL_C,0);

  return 0;
};
