#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"

#define MAX_NUM 100

void primeproc(int) __attribute__((noreturn));

void
primeproc(int readDp)
{
  int prime;
  if(read(readDp, &prime, 4) == 0){
    close(readDp);
    exit(0);
  }
  printf("prime %d\n", prime);

  int right[2];
  if(pipe(right) < 0){
    fprintf(2, "pipe error\n");
    exit(1);
  }

  int n[MAX_NUM];
  for(int i = 0; i< MAX_NUM; i++)
    n[i] = 1;
  int s = 0;
  while(read(readDp, &n[s], 4) > 0)
    s++;
  close(readDp);

  int pid = fork();

  if(pid < 0){
    fprintf(2, "fork error\n");
    exit(1);
  }

  if(pid == 0){
    close(right[1]);
    primeproc(right[0]);
  } else {
    close(right[0]);
    for(int i = 0; n[i] != 1; i++){
      if(n[i] % prime != 0)
        write(right[1], &n[i], 4);
    }
    close(right[1]);
    wait(0);
  }
  exit(0);
}

int
main(int argc, char *argv[])
{
  int p[2];
  if(pipe(p) < 0){
    fprintf(2, "pipe error\n");
    exit(1);
  }

  int pid = fork();

  if(pid < 0){
    fprintf(2, "fork error\n");
    exit(1);
  }

  if(pid == 0){
    close(p[1]);
    primeproc(p[0]);
  } else {
    close(p[0]);
    int n[MAX_NUM];
    for(int i = 0; i < MAX_NUM; i++){
      n[i] = i + 2;
      if(write(p[1], &n[i], 4) < 0){
        fprintf(2, "write error\n");
        exit(1);
      }
    }
    close(p[1]);
    wait(0);
  }
  
  exit(0);
}