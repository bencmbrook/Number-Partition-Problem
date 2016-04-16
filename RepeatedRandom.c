#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
  srand(time(0)); //use current time as seed for random generator
  for (size_t i = 0; i < 100; i++) {
    int x;
    int random_variable = rand();
    if (random_variable > RAND_MAX/2) {
      x = 1;
    } else {x = -1;}
    printf("%d\n", x);
  }
}
