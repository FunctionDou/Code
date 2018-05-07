#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char* argv[]) 
{
  if (argc < 2) 
  {
    printf("Usage: %s <fifoname>\n", argv[0]);
    return 1;
  }
  char buf[64];
  int n;
  int fd = open(argv[1], O_WRONLY);
  if (fd < 0) 
  {
    perror("open pipe");
    return 1;
  }

  int max;
  max  =fd;
  // max = STDIN_FILENO > fd ? STDIN_FILENO + 1 : fd + 1;

  fd_set FD;
  FD_ZERO(&FD);
  // FD_SET(STDIN_FILENO, &FD);
  FD_SET(fd, &FD);

  while(1) 
  {
    select(max + 1, NULL, &FD, NULL, NULL);
    n = read(STDIN_FILENO, buf, 64);
    if(n == 0)
    {
      printf("duang\n");
    }
    write(fd, buf, n); 
  }
  return 0;
}
