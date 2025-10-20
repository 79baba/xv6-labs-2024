#include <kernel/types.h>
#include <user/user.h>
#include <kernel/stat.h>

int
main(int argc, char* argv[])
{
  int p[2];
  int p1[2];
  char b[1];
  pipe(p);
  pipe(p1);

  if(fork() == 0)
  { // child
    if(read(p[0], b, 1) == 1)
    {
      fprintf(1, "%d: received ping\n", getpid());
      write(p1[1], b, 1);
    }
  }
  else
  { // parent
    write(p[1], "a", 1);
    if(read(p1[0], b, 1) == 1)
    {
      fprintf(1, "%d: received pong\n", getpid());
    }
  }
  exit(0);
}
