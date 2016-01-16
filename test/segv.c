#include <stdio.h>

#include <debugme.h>

int main() {
  volatile int *p = 0;
  *p = 0;

  return 0;
}
