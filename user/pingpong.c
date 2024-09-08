#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int p1[2], p2[2];
  char ball = 'b';
  
  pipe(p1);
  pipe(p2);

  if(fork() == 0){
    close(p1[1]);
    close(p2[0]);
    read(p1[0], &ball, 1);
    fprintf(1, "%d: received ping\n", getpid());
    write(p2[1], &ball, 1);
  } else {
    close(p1[0]);
    close(p2[1]);
    write(p1[1], &ball, 1);
    read(p2[0], &ball, 1);
    fprintf(1, "%d: received pong\n", getpid());
  }
  exit(0);
}