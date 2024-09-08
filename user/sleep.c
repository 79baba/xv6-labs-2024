#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  if(argc <= 1){
    fprintf(2, "No time argument.\n");
    exit(1);
  } else {
    sleep(atoi(argv[1]));
    exit(0);
  }
}