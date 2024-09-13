#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fcntl.h"
#include "kernel/param.h"

int
main(int argc, char *argv[])
{
  if(argc > 1){
    int pid = fork();

    if(pid < 0){
      fprintf(2, "xargs: fork error\n");
      exit(1);
    }

    if(pid == 0){
      int xargc = argc - 1;
      int xc = xargc;

      if(xargc > MAXARG){
        fprintf(2, "xargs: argument number overflow\n");
        exit(1);
      }

      char *xargv[MAXARG];
      memset(xargv, 0, sizeof(xargv));
      memcpy(xargv, argv + 1, xargc * sizeof(char *));

      char p[64];
      int s = 0;
      while(read(0, &p[s], 1) > 0){
        if(p[s] == '\n'){
          xargc++;
          if(xargc > MAXARG){
            fprintf(2, "xargs: argument number overflow\n");
            exit(1);
          }
          p[s] = '\0';
          char *arg = malloc(strlen(p) + 1);
          strcpy(arg, p);
          xargv[xargc - 1] = arg;
          s = 0;
          xargv[xargc] = 0;

          int pid = fork();

          if(pid < 0){
            fprintf(2, "xargs: fork error\n");
            exit(1);
          }

          if(pid == 0){
            exec(argv[1], xargv);
            fprintf(2, "xargs: exec failed\n");
            exit(1);
          } else {
            wait(0);
          }

          xargc = xc;

        } else if (p[s] == ' '){
          xargc++;
          if(xargc > MAXARG){
            fprintf(2, "xargs: argument number overflow\n");
            exit(1);
          }
          p[s] = '\0';
          char *arg = malloc(strlen(p) + 1);
          strcpy(arg, p);
          xargv[xargc - 1] = arg;
          s = 0;
        } else {
          s++;
        }
      }

      
    } else {
      wait(0);
    }
  } else {
    fprintf(2, "xargs: no command\n");
    exit(1);
  }
  exit(0);
}