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


struct queue *queueA;
struct queue *queueB;
struct queue *queueC;
sem_t *mutexA,*mutexB,*mutexC;
sem_t *emptyA,*emptyB,*emptyC;
sem_t *fullA,*fullB,*fullC;
int segment_A,segment_B,segment_C;

int isEmpty(struct message msg);
struct message consumeMsg(struct message msg, char *input);
void chooseQueue(struct message msg, char new_queue_name);
char takeLetter(struct message msg);

int main(int argc,char *argv[]){
    int i,j;
    int is_empty;
    char *input;
    char q_name;
    struct message msg;

    segment_A = shmget(KEY_SM_A,shared_memory_size,0666);
    segment_B = shmget(KEY_SM_B,shared_memory_size,0666);
    segment_C = shmget(KEY_SM_C,shared_memory_size,0666);

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
        while(1){
          downSemaphore(fullA);
          downSemaphore(mutexA);
          queueA = (struct queue *)shmat(segment_A,(void *) 0, 0);
          msg = takeMsg(queueA);
          delFromQueue(queueA);
          shmdt(queueA);
          upSemaphore(mutexA);
          upSemaphore(emptyA);

          sleep(1);
          q_name = takeLetter(msg);
          if(msg.priority == 1){
            msg = consumeMsg(msg,input);
            chooseQueue(msg,q_name);
          };
        };
        break;

      case 'B':
      case 'b':
        while(1){
          downSemaphore(fullB);
          downSemaphore(mutexB);
          queueB = (struct queue *)shmat(segment_B,(void *) 0, 0);
          msg = takeMsg(queueB);
          delFromQueue(queueB);
          shmdt(queueB);
          upSemaphore(mutexB);
          upSemaphore(emptyB);

          sleep(1);
          q_name = takeLetter(msg);
          if(msg.priority == 1){
            msg = consumeMsg(msg,input);
            chooseQueue(msg,q_name);
          };
        };
        break;

      case 'C':
      case 'c':
        while(1){
          downSemaphore(fullC);
          downSemaphore(mutexC);
          queueC = (struct queue *)shmat(segment_C,(void *) 0, 0);
          msg = takeMsg(queueC);
          delFromQueue(queueC);
          shmdt(queueC);
          upSemaphore(mutexC);
          upSemaphore(emptyC);

          sleep(1);
          q_name = takeLetter(msg);
          if(msg.priority == 1){
            msg = consumeMsg(msg,input);
            chooseQueue(msg,q_name);
          };
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

int isEmpty(struct message msg){
  char first = msg.message_content[0][0];
  char second = msg.message_content[1][0];
  char third = msg.message_content[2][0];

  if (first == ' ' && second == ' ' && third == ' ') return -1;
  return 0;
};

struct message consumeMsg(struct message msg, char *input){
  int i,random;
  char choiceA[4][1] = {{"A"},{"B"},{"C"},{"X"}};
  char choiceB[4][1] = {{"X"},{"A"},{"B"},{"C"}};
  char choiceC[4][1] = {{"C"},{"X"},{"A"},{"B"}};

  for (i=0;i<2;i++){
    msg.message_content[i][0] = msg.message_content[i+1][0];
  };
  srand( time( NULL ));
  random = rand()%4;

  if (input == 'A' || input == 'a') msg.message_content[2][0] = NULL; msg.message_content[2][0] = choiceA[random][0];
  if (input == 'B' || input == 'b') msg.message_content[2][0] = NULL; msg.message_content[2][0] = choiceB[random][0];
  if (input == 'C' || input == 'c') msg.message_content[2][0] = NULL; msg.message_content[2][0] = choiceC[random][0];
  return msg;
};

void chooseQueue(struct message msg, char new_queue_name){
  switch(new_queue_name){
    case 'A':
      downSemaphore(emptyA);
      downSemaphore(mutexA);
      queueA = (struct queue *)shmat(segment_A,(void *) 0, 0);
      pushMsgBack(queueA,msg);
      printf("Consumer A:");
      showQueue(queueA);
      shmdt(queueA);
      upSemaphore(mutexA);
      upSemaphore(fullA);
      break;
    case 'B':
      downSemaphore(emptyB);
      downSemaphore(mutexB);
      queueB = (struct queue *)shmat(segment_B,(void *) 0, 0);
      pushMsgBack(queueB,msg);
      printf("Consumer B:");
      showQueue(queueB);
      shmdt(queueB);
      upSemaphore(mutexB);
      upSemaphore(fullB);
      break;
    case 'C':
      downSemaphore(emptyC);
      downSemaphore(mutexC);
      queueC = (struct queue *)shmat(segment_C,(void *) 0, 0);
      pushMsgBack(queueC,msg);
      printf("Consumer C:");
      showQueue(queueC);
      shmdt(queueC);
      upSemaphore(mutexC);
      upSemaphore(fullC);
      break;
    default:
      break;
  };
};

char takeLetter(struct message msg){
  return msg.message_content[0][0];
};
