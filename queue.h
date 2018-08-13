#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <stdbool.h>

#define KEY_SM_A (1597)
#define KEY_SM_B (1598)
#define KEY_SM_C (1599)

struct message{
    char message_content[3][1];
    int priority; //pryorytet zwykly, 0-wazny
};

struct queue{
  struct message message_queue[20];
  int count;
  bool empty;
  bool full;
};

const int shared_memory_size = 110;

int pushMsgBack(struct queue *queue,struct message msg){
  if (queue->count == 20){
      queue->full = true;
      queue->empty = false;
      return -1;
  };
  queue->message_queue[queue->count] = msg;
  queue->count++;
  return 0;
};

void showQueue(struct queue *queue){
  int i,j;
  struct message msg;
  for(i=0;i < queue->count;i++){
    msg = queue->message_queue[i];
    printf("(");
    for(j=0;j<3;j++){
      printf("%c",msg.message_content[j][0]);
    };
    printf(")");
  };
  printf("\n");
};

struct message takeMsg(struct queue *queue){
  struct message msg;
  msg = queue->message_queue[0];
  return msg;
};

void delFromQueue(struct queue *queue){
  int i;
  int count;
  count = queue->count;
  for(i = 0; i < count-1; i++){
      queue->message_queue[i] = queue->message_queue[i+1];
  };
  queue->count = count - 1;
};
