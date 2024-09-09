#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"

void
primeproc(int readDp)
{
  int prime;
  if(read(readDp, &prime, 4) == 0){
    close(readDp);
    return;
  }
  fprintf(1, "prime %d\n", prime);

  int right[2];
  if(pipe(right) < 0){
    fprintf(1, "pipe error\n");
    exit(1);
  }

  if(fork() == 0){
    close(right[1]);
    primeproc(right[0]);
    close(right[0]);
    exit(0);
  } else {
    close(right[0]);
    int n;
    while(read(readDp, &n, 4)){
      if(n % prime != 0){
        write(right[1], &n, 4);
      }
    }
    close(right[1]);
    wait(0);
  }
}

int
main(int argc, char *argv[])
{
  int p[2];
  pipe(p);

  if(fork() == 0){
    close(p[1]);
    primeproc(p[0]);
    close(p[0]);
    exit(0);
  } else {
    close(p[0]);
    for(int i = 0; i < 276; i++){
      int n = i + 2;
      write(p[1], &n, 4);
    }
    close(p[1]);
    wait(0);
  }
  
  exit(0);
}