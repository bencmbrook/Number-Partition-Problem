#include <stdio.h>
#include <stdlib.h>
#include <time.h>

const int sizeA = 1000;

int * solution() {
  static int S[sizeA];
  for (size_t i = 0; i < sizeA; i++) {
    int random_variable = rand();
    if (random_variable > RAND_MAX/2) {
      S[i] = 1;
    } else {
      S[i] = -1;
    }
  }
  return S;
}

int KK(int* A) {
  for (size_t i = 0; i < sizeA; i++) {
    if *(A+i)>*(A+i+1) {
      *(A+i)+=*(A+i+1);
      *(A+i+1)=0;
    }
    else {
      *(A+i+1)+=*(A+i);
      *(A+i)=0;
    }
  }
}

int main() {
  srand(time(0));

  printf("%d\n", rand());
  printf("%d\n", RAND_MAX/2);

  int * S = solution();

  for (size_t i = 0; i < sizeA; i++) {
    printf("%d\n", *(S+i));
  }
}
