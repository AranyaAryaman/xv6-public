#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

#define BUFFER_SIZE 1024
#define MAX_LINES 10

char buffer[BUFFER_SIZE];
char *lines[MAX_LINES];

void tail(int fd, int n) {
  int i, len, line_count = 0;
  int start = 0, end = 0;
  
  // Read the entire file
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
    
    // If buffer is full, shift content
    if (end == sizeof(buffer)) {
      memmove(buffer, buffer + start, end - start);
      end -= start;
      start = 0;
    }
  }
  
  // Print the last n lines
  for (i = start; i < end; i++) {
    printf(1, "%c", buffer[i]);
  }
}

int main(int argc, char *argv[]) {
  int fd, n = MAX_LINES;

  if (argc < 2) {
    printf(2, "Usage: tail [-n] [file]\n");
    exit();
  }

  if (argv[1][0] == '-') {
    n = atoi(&argv[1][1]);
    if (n <= 0) n = MAX_LINES;
    if (argc == 2) {
      tail(0, n);
      exit();
    }
    fd = open(argv[2], O_RDONLY);
  } else {
    fd = open(argv[1], O_RDONLY);
  }

  if (fd < 0) {
    printf(2, "tail: cannot open %s\n", argv[1]);
    exit();
  }

  tail(fd, n);
  close(fd);
  exit();
}