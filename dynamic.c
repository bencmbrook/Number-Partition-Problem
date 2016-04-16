#include <stdio.h>

int compare_ints(const void* a, const void* b)
{
    int arg1 = *(const int*)a;
    int arg2 = *(const int*)b;

    if (arg1 < arg2) return -1;
    if (arg1 > arg2) return 1;
    return 0;

}

int main() {
  int a[] = {1,2,3,4,5,6,7,4,9,10};
  int size = sizeof(a) / sizeof(*a);

  qsort(a, size, sizeof(int), compare_ints);

  for (size_t i = 0; i < size; i++) {
    printf("%d\n", a[i]);
  }
}
