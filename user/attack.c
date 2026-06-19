#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"
#include "kernel/riscv.h"

#define PGNUM (8)

char data[PGNUM][PGSIZE];

int
main(int argc, char *argv[])
{
  // Your code here.
  for (int i = 0; i < PGNUM; i++) {
    //printf("str : %s\n", data[i]);
    if (strcmp(data[i], "This may help.") == 0) {
      printf("%s\n", &data[i][16]);
      break;
    }
  }

  exit(1);
}
