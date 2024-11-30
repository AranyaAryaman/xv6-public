#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

#define BUFFER_SIZE 4096
#define DEFAULT_LINES 10

char buffer[BUFFER_SIZE];
char *lines[BUFFER_SIZE];

void tail(int fd, int n) {
  int i, len, line_count = 0;
  int start = 0, end = 0;
  
  while ((len = read(fd, buffer + end, sizeof(buffer) - end)) > 0) {
    end += len;
    for (i = 0; i < len; i++) {
      if (buffer[end - len + i] == '\n') {
        line_count++;
        if (line_count > n) {
          while (start < end - len + i && buffer[start] != '\n') {
            start++;
          }
          start++;
        }
      }
    }
    
    if (end == sizeof(buffer)) {
      memmove(buffer, buffer + start, end - start);
      end -= start;
      start = 0;
    }
  }
  
  for (i = start; i < end; i++) {
    printf(1, "%c", buffer[i]);
  }
  if (buffer[end-1] != '\n') {
    printf(1, "\n");
  }
}

int main(int argc, char *argv[]) {
  int fd = 0;  // Default to stdin
  int n = DEFAULT_LINES;
  char *filename = 0;

  if (argc > 1 && argv[1][0] == '-') {
    n = atoi(&argv[1][1]);
    if (n <= 0) n = DEFAULT_LINES;
    if (argc > 2) {
      filename = argv[2];
    }
  } else if (argc > 1) {
    filename = argv[1];
  }

  if (filename) {
    fd = open(filename, O_RDONLY);
    if (fd < 0) {
      printf(2, "tail: cannot open %s\n", filename);
      exit();
    }
  }

  tail(fd, n);

  if (fd != 0) {
    close(fd);
  }
  exit();
}