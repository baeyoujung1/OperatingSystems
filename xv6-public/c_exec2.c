#include "types.h"
#include "user.h"
#include "stat.h"

int main(void) {
  char *argv[10];
  argv[0]="mlfq_test";
  argv[1]=0;
  int result = exec2(argv[0],argv,5);

  if (result < 0) {
    printf(1, "exec2 failed\n");
  } else {
    printf(1, "exec2 succeeded\n");
  }

  exit();
}
