#include <kernel/types.h>
#include <user/user.h>
#include <kernel/stat.h>

void
primes(int* p) __attribute__((noreturn));
void
primes(int* p)
{
  close(p[1]);
  int a, b, n;
  int p1[2] = {0, 0};

  if(((n = read(p[0], &a, sizeof(a)))) < 4)
  {
    close(p[0]);
    wait(0);
    exit(0);
  }
  printf("prime %d\n", a);
  pipe(p1);
  if(fork() == 0)
  {
    close(p[0]);
    close(p1[1]);
    primes(p1);
    wait(0);
    exit(0);
  }
  else
  {
    close(p1[0]);
  }

  while(read(p[0], &b, sizeof(b)) == 4)
  {
    if(b % a != 0)
    {
      write(p1[1], &b, sizeof(b));
    }
  }
  write(p1[1], "a", 1);
  close(p1[1]);
  close(p[0]);
  wait(0);
  exit(0);
}

int
main(int argc, char* argv[])
{
  int p[2];
  pipe(p);

  if(fork() == 0)
  { // child
    primes(p);
  }
  else
  { // parent
    close(p[0]);
    for(int i = 2; i <= 280; i++)
    {
      write(p[1], &i, sizeof(i));
    }
    write(p[1], "a", 1);
    close(p[1]);
  }
  wait(0);
  exit(0);
}
