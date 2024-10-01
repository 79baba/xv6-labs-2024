#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"
#include "kernel/riscv.h"

char secret[8];
char output[64];

// from FreeBSD.
int
do_rand(unsigned long *ctx)
{
/*
 * Compute x = (7^5 * x) mod (2^31 - 1)
 * without overflowing 31 bits:
 *      (2^31 - 1) = 127773 * (7^5) + 2836
 * From "Random number generators: good ones are hard to find",
 * Park and Miller, Communications of the ACM, vol. 31, no. 10,
 * October 1988, p. 1195.
 */
    long hi, lo, x;

    /* Transform to [1, 0x7ffffffe] range. */
    x = (*ctx % 0x7ffffffe) + 1;
    hi = x / 127773;
    lo = x % 127773;
    x = 16807 * lo - 2836 * hi;
    if (x < 0)
        x += 0x7fffffff;
    /* Transform to [0, 0x7ffffffd] range. */
    x--;
    *ctx = x;
    return (x);
}

unsigned long rand_next = 1;

int
rand(void)
{
    return (do_rand(&rand_next));
}

// generate a random string of the indicated length.
char *
randstring(char *buf, int n)
{
  for(int i = 0; i < n-1; i++) {
    buf[i] = "./abcdef"[(rand() >> 7) % 8];
  }
  if(n > 0)
    buf[n-1] = '\0';
  return buf;
}

int
main(int argc, char *argv[])
{
  int pid;
  int fds[2];

  // an insecure way of generating a random string, because xv6
  // doesn't have good source of randomness.
  rand_next = uptime();
  randstring(secret, 8);
  
  if((pid = fork()) < 0) {
    printf("fork failed\n");
    exit(1);   
  }
  if(pid == 0) {
    char *newargv[] = { "secret", secret, 0 };
    exec(newargv[0], newargv);
    printf("exec %s failed\n", newargv[0]);
    exit(1);
  } else {
    wait(0);  // wait for secret to exit
    if(pipe(fds) < 0) {
      printf("pipe failed\n");
      exit(1);   
    }
    if((pid = fork()) < 0) {
      printf("fork failed\n");
      exit(1);
    }
    if(pid == 0) {
      close(fds[0]);
      close(2);
      dup(fds[1]);
      char *newargv[] = { "attack", 0 };
      exec(newargv[0], newargv);
      printf("exec %s failed\n", newargv[0]);
      exit(1);
    } else {
      close(fds[1]);
      if(read(fds[0], output, 64) < 0) {
        printf("FAIL; read failed; no secret\n");
        exit(1);
      }
      if(strcmp(secret, output) == 0) {
        printf("OK: secret is %s\n", output);
      } else {
        printf("FAIL: no/incorrect secret\n");
      }
    }
  }
}
