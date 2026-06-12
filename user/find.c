#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"


void traverse(char *path, char *target, int argc, char **argv) {
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;
  int pid;

  if((fd = open(path, O_RDONLY)) < 0){
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }

  switch(st.type){
  case T_DEVICE:
  case T_FILE:
    break;
  case T_DIR:
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf("find: path too long\n");
      break;
    }
    strcpy(buf, path);

    p = buf + strlen(buf) - 1;
    if (*p != '/') {
      p++;
      *p = '/';
    }
    p++;

    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0)
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;

      if (strcmp(de.name, target) == 0) {
        if (argc == 0) {
          // normal find
          printf("%s\n", buf);
	} else {
          // initialize the argv
          char **exec_argv = (char **) malloc(sizeof(char *) * (argc - 2));
          memset(exec_argv, 0, sizeof(char *) * (argc - 2));
          for (int i = 0; i < argc - 4; i++) {
            exec_argv[i] = argv[i + 4];
          }
          exec_argv[argc - 4] = buf;

          // exec find
          pid = fork();
          if (pid == 0) {
            // child-process
            exec(exec_argv[0], exec_argv);
          }
          // parent-process
          wait(0);

          free(exec_argv);
        }
      }

      if (strcmp(de.name, ".") == 0 ||
          strcmp(de.name, "..") == 0) {
        continue;
      }
      traverse(buf, target, argc, argv);
    }
    break;
  }
  close(fd);

}

int
main(int argc, char *argv[])
{
  if (argc < 3) {
    fprintf(2, "usage: find path target\n");
    exit(1);
  } else if (argc == 3) {
    // normal find
    traverse(argv[1], argv[2], 0, 0);
  } else if (argc >= 5 && strcmp(argv[3], "-exec") == 0) {
    // exec find
    traverse(argv[1], argv[2], argc, argv);
  }

  exit(0);
}
