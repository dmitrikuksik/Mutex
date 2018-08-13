#ifndef sem
#define sem

#include <sys/types.h>
#include <sys/ipc.h>
#include <fcntl.h>
#include <time.h>
#include <semaphore.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>

#define MUTEX_A "mutex_a"
#define MUTEX_B "mutex_b"
#define MUTEX_C "mutex_c"

#define EMPTY_A "empty_a"
#define FULL_A  "full_a"

#define EMPTY_B "emty_b"
#define FULL_B  "full_b"

#define EMPTY_C "empty_c"
#define FULL_C  "full_c"
#define SIZE    20


sem_t* getSemaphore(const char* name){
  sem_t *semp;
  semp = sem_open(name, O_RDWR);
  if(semp == SEM_FAILED){
    perror("ge_sem");
    exit(1);
  }
  return semp;
};

void closeSemaphore(sem_t *semp){
  if(sem_close(semp) < 0){
      perror("closeSemphore");
      exit(1);
  };
};

void initializeSemaphore(const char* name,unsigned int init_value){
  sem_t *semp;
  semp = sem_open(name, O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, init_value);
  sem_close(semp);
};

void deleteSemaphore(const char* name){
  if(sem_unlink(name) < 0){
    perror("deleteSemaphore");
    exit(1);
  };
};

void downSemaphore(sem_t *semp){
  if (sem_wait(semp) < 0){
    perror("downSemaphore");
    exit(1);
  }
};

void upSemaphore(sem_t *semp){
  if (sem_post(semp) < 0){
    perror("upSemaphore");
    exit(1);
  }
};

#endif
