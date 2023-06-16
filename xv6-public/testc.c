#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"

int index=0;

void
create(uint filesize)
{
  int i;
  int fds[30];
  index++;

  char filename[] = "testFile .txt";
  filename[8] = (char)('0' + index);

  if((fds[index] = open(filename, O_CREATE | O_RDWR)) < 0) {
    printf(1, "error\n");
    exit();
  }

  const uint fs = filesize / 32;
  for(i = 0; i < fs; i++) {
    if(write(fds[index], "aaaaaaaaa\n", 16) != 16) {
      exit();
    }
    if(write(fds[index], "bbbbbbbbb\n", 16) != 16) {
      exit();
    }
  }

  printf(1, "Create file %d \n", index);
}

void
create_test(void)
{
  create(1 * 1024);
  create(16 * 1024);
  create(1 * 1024 * 1024);
  create(16 * 1024 * 1024);
}

int
main(int argc, char *argv[])
{
  create_test();
  exit();
}