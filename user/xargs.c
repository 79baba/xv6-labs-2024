#include "kernel/types.h"
#include "user/user.h"
#include "kernel/stat.h"
#include "kernel/param.h"

void
run(char* cmd, char** args)
{
  if(fork() == 0)
  {
    exec(cmd, args);
    exit(0);
  }
  return;
}

void
xargs(int argc, char** argv)
{
  char buf[2048];
  int n = argc;
  int i = 0, j = 0;
  while(read(0, &buf[i], 1) > 0)
  {
    if(buf[i] == ' ')
    {
      buf[i] = '\0';
      argv[n++] = &buf[j];
      j = i + 1;
    }
    if(buf[i] == '\n')
    {
      buf[i] = '\0';
      argv[n++] = &buf[j];
      j = i + 1;
      argv[n] = 0;

      run(argv[1], &argv[1]);
      n = argc;
    }
    i++;
  }
  if(n != argc)
  {
    buf[i] = '\0';
    argv[n++] = &buf[j];
    argv[n] = 0;
    run(argv[1], &argv[1]);
  }
}
int
main(int argc, char** argv)
{
  char* args[MAXARG * 4];
  memmove(args, argv, argc * sizeof(char*));
  xargs(argc, args);

  while(wait(0) != -1)
  {
  }
  exit(0);
}
