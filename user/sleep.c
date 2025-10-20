#include <kernel/types.h>
#include <user/user.h>
#include <kernel/stat.h>

int
main(int argc, char** argv)
{
  if(argc < 2)
  {
    printf("usage: sleep <ticks>\n");
  }
  sleep(atoi(argv[1]));
  exit(0);
}
