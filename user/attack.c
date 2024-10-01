#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"
#include "kernel/riscv.h"

int
main(int argc, char *argv[])
{
  // your code here.  you should write the secret to fd 2 using write
  // (e.g., write(2, secret, 8)
  if(argc != 1){
    printf("Usage: secret the-secret\n");
    exit(1);
  }
  char *end = sbrk(PGSIZE*32);
  end = end + 8 * PGSIZE;
  fprintf(2, end+16, 8);
  exit(1);
}
