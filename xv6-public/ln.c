#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char *argv[])
{
  
 
  if (argc == 4 && ((strcmp(argv[1], "-h"))==0))
  {
    if (link(argv[2], argv[3]) < 0)
      printf(2, "link %s %s: failed\n", argv[1], argv[2]);
  }
  else if ((strcmp(argv[1], "-s")==0))
  {
    if (symlink(argv[2], argv[3]) < 0)
      printf(2, "symlink %s %s: failed\n", argv[2], argv[3]);
  }
  else
  {
    printf(2,"%d", argc);
    printf(2, "Usage: ln old new\n");
    printf(2, "or\n");
    printf(2, "Usage for symbolic link: ln -s old new\n");
    exit();
  }
  exit();
}
