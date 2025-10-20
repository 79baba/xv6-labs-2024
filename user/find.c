#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"

void
find(char* path, char* dest)
{
  int fd;
  char buf[512];
  char* p;
  struct dirent de;
  struct stat st;

  if((fd = open(path, O_RDONLY)) < 0)
  {
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0)
  {
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }

  if(st.type != T_DIR)
  {
    fprintf(2, "find: starting path %s is not a directory\n", path);
    close(fd);
    return;
  }

  strcpy(buf, path);
  p = buf + strlen(buf);
  *p++ = '/';

  while(read(fd, &de, sizeof(de)) == sizeof(de))
  {
    if(de.inum == 0 || strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
      continue;
    memmove(p, de.name, DIRSIZ);
    p[DIRSIZ] = 0;
    if(stat(buf, &st) < 0)
    {
      printf("find: cannot stat %s\n", buf);
      continue;
    }
    switch(st.type)
    {
    case T_DEVICE:
    case T_FILE:
      if(strcmp(de.name, dest) == 0)
      {
        printf("%s\n", buf);
      }
      break;

    case T_DIR:
      find(buf, dest);
      break;
    }
  }
  close(fd);
}

int
main(int argc, char* argv[])
{
  find(argv[1], argv[2]);
}
