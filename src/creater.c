#include <stdio.h>

int main() {
  for (int i = 0; i < 128; i++) {
    printf("%d\t", i);
    if (i < 30 || i == 127)
      printf("[]");
    else
      printf("[%c]", (char)i);
    printf("\t");
    if ((i + 1) % 4 == 0) printf("\n");
  }
  return 0;
}
