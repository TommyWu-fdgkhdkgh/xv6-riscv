#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  printf("xv6-riscv : sleep\n");
  if (argc == 1) {
    pause(1);
  } else {
    pause(atoi(argv[1]));
  }
  exit(0);
}
