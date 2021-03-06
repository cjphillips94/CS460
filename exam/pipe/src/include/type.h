#ifndef TYPE_H
#define TYPE_H

typedef unsigned char   u8;
typedef unsigned short u16;
typedef unsigned long  u32;

#define NPROC       9
#define SSIZE       1024

/******* PROC status ********/
#define FREE        0
#define READY       1
#define RUNNING     2
#define STOPPED     3
#define SLEEP       4
#define ZOMBIE      5

#define NAMESIZE    32

#define NPIPE       10
#define NOFT        20
#define NFD         10
#define PSIZE       10

#define FILE_LENGTH 64

#define READ_PIPE   20
#define WRITE_PIPE  30

/* Error codes */
#define BROKEN_PIPE 100

typedef struct pipe
{
  char buf[PSIZE];
  int  head, tail;
  int  data, room;
  int  nReader, nWriter;
  int busy;
} PIPE;

typedef struct oft
{
  int  mode;
  int  refCount;
  PIPE *ppipe;
} OFT;

typedef struct proc
{
    struct proc *next;
    int    *ksp;               // at offset 2

    int    uss, usp;           // at offsets 4,6
    int    inkmode;            // at offset 8

    int    pid;                // add pid for identify the proc
    int    status;             // status = FREE|READY|RUNNING|SLEEP|ZOMBIE
    int    ppid;               // parent pid
    struct proc *parent;
    int    priority;
    int    event;
    int    exitCode;
    char   name[NAMESIZE];           // name string of PROC

    OFT    *fd[NFD];

    int    kstack[SSIZE];      // per proc stack area
}PROC;


PROC proc[NPROC], *running, *freeList, *readyQueue, *sleepList;
int procSize = sizeof(PROC);
int nproc = 0;

OFT   oft[NOFT];
PIPE  pipe[NPIPE];

char *pname[] = {
  "Sun", "Mercury", "Venus", "Earth",  "Mars", "Jupiter",
  "Saturn", "Uranus", "Neptune"
};

int color;

#endif /* TYPE_H */
