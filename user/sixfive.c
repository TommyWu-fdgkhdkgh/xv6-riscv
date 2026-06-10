#include "kernel/fcntl.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

char separator[] = " -\r\t\n./,";

int is_number(char *buf) {
  int bufi = 0;
  if (buf[0] == 0) {
    return 0;
  }

  while (buf[bufi]) {
    if (buf[bufi] < '0' || buf[bufi] > '9') {
      return 0;
    }
    bufi++;
  }
  return 1;
}

void sixfive(int fd) {
  char buf[512];
  int bufi = 0;
  char tmp;

  buf[0] = 0;
  while(read(fd, &tmp, sizeof(tmp)) == sizeof(tmp)) {
    if (strchr(separator, tmp)) {
      buf[bufi] = 0;

      if (is_number(buf)) {
        int num = atoi(buf);
	if (num % 5 == 0 || num % 6 == 0) {
          printf("%d\n", num);
	}
      }

      bufi = 0;
      buf[0] = 0;
    } else {
      buf[bufi++] = tmp;
    }
  }

  if (is_number(buf)) {
    int num = atoi(buf);
    if (num % 5 == 0 || num % 6 == 0) {
      printf("%d\n", num);
    }
  }
}

int
main(int argc, char *argv[])
{
  int fd;
  struct stat st;
  if (argc < 2) {
    fprintf(2, "usage: sixfive xxx.txt\n");
  }

  for (int i = 1; i < argc; i++) {
    if((fd = open(argv[i], O_RDONLY)) < 0){
      fprintf(2, "ls: cannot open %s\n", argv[i]);
      exit(1);
    }

    if(fstat(fd, &st) < 0){
      fprintf(2, "ls: cannot stat %s\n", argv[i]);
      close(fd);
      exit(1);
    }

    switch(st.type) {
    case T_DEVICE:
    case T_DIR:
      break;
    case T_FILE:
      sixfive(fd);
      break;
    }
    close(fd);
  }

  exit(0);
}
