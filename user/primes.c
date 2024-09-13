#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"

#define MAX_NUM 270

void primes(int) __attribute__((noreturn));

void
primes(int readDp)
{
  short prime;
  if(read(readDp, &prime, 2) == 0){
    close(readDp);
    exit(0);
  }
  printf("prime %d\n", prime);

  int right[2];
  if(pipe(right) < 0){
    fprintf(2, "pipe error\n");
    exit(1);
  }

  short n;
  while(read(readDp, &n, 2) > 0){
    if(n % prime != 0)
      write(right[1], &n, 2);
  }
    
  close(readDp);
  close(right[1]);

  int pid = fork();

  if(pid < 0){
    fprintf(2, "fork error\n");
    exit(1);
  }

  if(pid == 0){
    primes(right[0]);
    close(right[0]);
  } else {
    close(right[0]);
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
    primes(p[0]);
  } else {
    close(p[0]);
    short n[MAX_NUM];
    for(int i = 0; i < MAX_NUM; i++){
      n[i] = i + 2;
      if(write(p[1], &n[i], 2) < 0){
        fprintf(2, "write error\n");
        exit(1);
      }
    }
    close(p[1]);
    wait(0);
  }
  
  exit(0);
}