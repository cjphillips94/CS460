#include "include/type.h"

PROC proc[NPROC], *running, *freeList, *readyQueue, *sleepList;
int procSisze = sizeof(PROC);
int nproc = 0;

int body();

char *pname[] = {
  "Sun", "Mercury", "Venus", "Earth",  "Mars", "Jupiter",
  "Saturn", "Uranus", "Neptune"
};

int color;

#include "sys_util.c"
//#include "wait.c"
//#include "kernel.c"
#include "int.c"

int init()
{
  PROC *p;
  int i;

  color = 0x0C;

  printf("Initializing processes ... ");

  for(i = 0; i < NPROC; i++)
  {
    p = &proc[i];
    p->pid = i;
    p->status = FREE;
    p->priority = 0;
    strcpy(proc[i].name, pname[i]);
    p->next = &proc[i + 1];
  }

  freeList = &proc[0];
  proc[NPROC - 1].next = 0;
  readyQueue = sleepList = 0;

  /* Set Proc 0 as the running process: */
  p = get_proc(&freeList);
  p->status = RUNNING;
  p->ppid = 0;
  p->parent = p;
  running = p;
  nproc = 1;

  printf("Complete.\n");
}

int scheduler()
{
  if (running->status == READY)
  {
    enqueue(&readyQueue, running);
  }

  running = dequeue(&readyQueue);
  color = running->pid + 0x0A;
}

int int80h();

int set_vector(u16 vector, u16 handler)
{
  // Usage: put_word(word, segment, offset)

  put_word(handler, 0, vector << 2);
  put_word(0x1000, 0, (vector << 2) + 2);
}

main()
{
  printf("MTX starting in main()\n\n");
  init();
  set_vector(80, int80h);

  kfork("/bin/user1"); // Fork to process 1

  while (1)
  {
    printf("[P0 running]\n");
    while(!readyQueue)
    {
      printf("[Switching process away from P0]\n");
      tswitch();
    }
  }
}