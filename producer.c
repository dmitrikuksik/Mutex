#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>
#include "queue.h"
#include "sem.h"


struct queue *queueA = {0, 0, true,false};
struct queue *queueB = {0, 0, true,false};
struct queue *queueC = {0, 0, true,false};

struct message produce_spec(char *input);

int main(int argc,char *argv[]){
  char *input;
  int i=0, j, segment_id, status;
  struct message msg;
  sem_t *mutexA,*mutexB,*mutexC;
  sem_t *emptyA,*emptyB,*emptyC;
  sem_t *fullA,*fullB,*fullC;

  mutexA = getSemaphore(MUTEX_A);
  mutexB = getSemaphore(MUTEX_B);
  mutexC = getSemaphore(MUTEX_C);

  emptyA = getSemaphore(EMPTY_A);
  emptyB = getSemaphore(EMPTY_B);
  emptyC = getSemaphore(EMPTY_C);

  fullA = getSemaphore(FULL_A);
  fullB = getSemaphore(FULL_B);
  fullC = getSemaphore(FULL_C);

  if(argc == 1){
    printf("No arguments\n");
    return 0;
  }

  input = argv[1];

  switch(*input){
    case 'A':
    case 'a':

      segment_id = shmget(KEY_SM_A,shared_memory_size,0666);
      if(segment_id < 0){
        perror("Problem with sharing memmory");
        exit(1);
      };
      while(1){
        msg = produce_spec(*input);
        downSemaphore(emptyA);
        downSemaphore(mutexA);
        queueA = (struct queue *)shmat(segment_id, (void *)0, 0);
        pushMsgBack(queueA,msg);
        printf("Producer A:");
        showQueue(queueA);
        shmdt(queueA);
        upSemaphore(mutexA);
        upSemaphore(fullA);
        sleep(2);
      };
      break;

    case 'B':
    case 'b':
      segment_id = shmget(KEY_SM_B,shared_memory_size,0666);
      if(segment_id < 0){
        perror("Problem with sharing memmory");
        exit(1);
      };
      while(1){
        msg = produce_spec(*input);
        downSemaphore(emptyB);
        downSemaphore(mutexB);
        queueB =(struct queue *) shmat(segment_id, (void *)0, 0);
        pushMsgBack(queueB,msg);
        printf("Producer B:");
        showQueue(queueB);
        shmdt(queueB);
        upSemaphore(mutexB);
        upSemaphore(fullB);
        sleep(2);
      };

    break;

    case 'C':
    case 'c':
      segment_id = shmget(KEY_SM_C,shared_memory_size,0666);
      if(segment_id < 0){
        perror("Problem with sharing memmory");
        exit(1);
      };
      while(1){
        msg = produce_spec(*input);
        downSemaphore(emptyC);
        downSemaphore(mutexC);
        queueC =(struct queue *) shmat(segment_id, (void *)0, 0);
        pushMsgBack(queueC,msg);
        printf("Producer C:");
        showQueue(queueC);
        shmdt(queueC);
        upSemaphore(mutexC);
        upSemaphore(fullC);
        sleep(2);
      };
    break;
    default:
      printf("Not correct argument.\n");
      break;
  };
  closeSemaphore(mutexA);
  closeSemaphore(mutexB);
  closeSemaphore(mutexC);

  closeSemaphore(emptyA);
  closeSemaphore(emptyB);
  closeSemaphore(emptyC);

  closeSemaphore(fullA);
  closeSemaphore(fullB);
  closeSemaphore(fullC);
  return 0;
};

struct message produce_spec(char *input){
  struct message msg;
  int random;
  int i;
  char choiceA[3][1] = {{"A"},{"B"},{"C"}};
  char choiceB[3][1] = {{"B"},{"C"},{"A"}};
  char choiceC[3][1] = {{"C"},{"A"},{"B"}};
  srand( time( NULL ));

  for(i=0;i<3;i++){
    random = rand()%3;
    if (input == 'A' || input == 'a') msg.message_content[i][0] = choiceA[random][0];
    if (input == 'B' || input == 'b') msg.message_content[i][0] = choiceB[random][0];
    if (input == 'C' || input == 'c') msg.message_content[i][0] = choiceC[random][0];
  };
  msg.priority = 1;
  return msg;
};
