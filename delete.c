#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include "queue.h"
#include "sem.h"


int main(){
  deleteSemaphore(MUTEX_A);
  deleteSemaphore(MUTEX_B);
  deleteSemaphore(MUTEX_C);

  deleteSemaphore(EMPTY_A);
  deleteSemaphore(EMPTY_B);
  deleteSemaphore(EMPTY_C);

  deleteSemaphore(FULL_A);
  deleteSemaphore(FULL_B);
  deleteSemaphore(FULL_C);

  system("ipcrm -M 0x0000063d");
  system("ipcrm -M 0x0000063e");
  system("ipcrm -M 0x0000063f");
  return 0;
};
