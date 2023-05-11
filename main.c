#include "file.c"
#include <stdio.h>
#include "processes.c"

int main(int argc, char *argv[]) {

  parse(argc, argv);
  process();
  return 0;
  
}