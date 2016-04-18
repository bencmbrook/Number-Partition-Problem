#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>

const int sizeA = 100;
const int maxIter = 25000;

// Max Heap based on http://robin-thomas.github.io/max-heap/
#define LCHILD(x) 2 * x + 1
#define RCHILD(x) 2 * x + 2
#define PARENT(x) (x - 1) / 2

typedef struct node {
    int64_t data;
} node;

typedef struct maxHeap {
    int size;
    node *elem;
} maxHeap;

/*
    Function to initialize the max heap with size = 0
*/
maxHeap initMaxHeap(int size) {
    maxHeap hp;
    hp.size = 0;
    hp.elem = malloc(size * sizeof(node));
    return hp;
}

/*
    Function to swap data within two nodes of the max heap using pointers
*/
void swap(node *n1, node *n2) {
    node temp = *n1;
    *n1 = *n2;
    *n2 = temp;
}

/*
    Heapify function is used to make sure that the heap property is never violated
    In case of deletion of a node, or creating a max heap from an array, heap property
    may be violated. In such cases, heapify function can be called to make sure that
    heap property is never violated
*/
void heapify(maxHeap *hp, int i) {
    int largest = (LCHILD(i) < hp->size && hp->elem[LCHILD(i)].data > hp->elem[i].data) ? LCHILD(i) : i;
    if(RCHILD(i) < hp->size && hp->elem[RCHILD(i)].data > hp->elem[largest].data) {
        largest = RCHILD(i);
    }
    if(largest != i) {
        swap(&(hp->elem[i]), &(hp->elem[largest]));
        heapify(hp, largest);
    }
}


/*
    Build a Max Heap given an array of numbers
    Instead of using insertNode() function n times for total complexity of O(nlogn),
    we can use the buildMaxHeap() function to build the heap in O(n) time
*/
void buildMaxHeap(maxHeap *hp, int *arr, int size) {
    int i;

    // Insertion into the heap without violating the shape property
    for(i = 0; i < size; i++) {
        if(hp->size) {
            hp->elem = realloc(hp->elem, (hp->size + 1) * sizeof(node));
        } else {
            hp->elem = malloc(sizeof(node));
        }
        node nd;
        nd.data = arr[i];
        hp->elem[(hp->size)++] = nd;
    }

    // Making sure that heap property is also satisfied
    for(i = (hp->size - 1) / 2; i >= 0; i--) {
        heapify(hp, i);
    }
}


/*
    Function to insert a node into the max heap, by allocating space for that node in the
    heap and also making sure that the heap property and shape propety are never violated.
*/
void insertNode(maxHeap *hp, int data) {
    node nd;
    nd.data = data;

    int i = (hp->size)++;
    while(i && nd.data > hp->elem[PARENT(i)].data) {
        hp->elem[i] = hp->elem[PARENT(i)];
        i = PARENT(i);
    }
    hp->elem[i] = nd;
}


/*
    Function to delete a node from the max heap
    It shall remove the root node, and place the last node in its place
    and then call heapify function to make sure that the heap property
    is never violated
*/
void deleteNode(maxHeap *hp) {
    if(hp->size) {
        printf("Deleting node %lld\n\n", hp->elem[0].data);
        hp->elem[0] = hp->elem[--(hp->size)];
        hp->elem = realloc(hp->elem, hp->size * sizeof(node));
        heapify(hp, 0);
    } else {
        printf("\nMax Heap is empty!\n");
        free(hp->elem);
    }
}

int compare_ints(const void* a, const void* b)
{
  int arg1 = *(const int*)a;
  int arg2 = *(const int*)b;

  if (arg1 < arg2) return -1;
  if (arg1 > arg2) return 1;
  return 0;
}

int * generateSolution() {
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

// Returns the residue
int KK(int* A) {
  // qsort(A, sizeA, sizeof(int), compare_ints);
  for (size_t i = 0; i < sizeA; i+=2) {
    if (*(A+i)>*(A+i+1)) {
      *(A+i)+=*(A+i+1);
      *(A+i+1)=0;
    }
    else {
      *(A+i+1)+=*(A+i);
      *(A+i)=0;
    }
  }
  return 0; // should return residue
}

int main(int argc, char *argv[]) {
  srand(time(0));

  // Check for inputfile argument, otherwise run my generated input file
  FILE* fp = NULL;
  if (argc > 1) {
    FILE* fp = fopen(argv[0], "r");
    if(!fp) {
        perror("File opening failed");
        return EXIT_FAILURE;
    }
  }
  else {
    FILE* fp = fopen("inputfile.txt", "r");
    if(!fp) {
        perror("File opening failed");
        return EXIT_FAILURE;
    }
  }

  // Set up max heap
  maxHeap hp = initMaxHeap(sizeA);

  // Read values out of file and insert into heap
  char c;
  while((c = fgetc(fp)) != EOF) {
    if (c != '\n') {
      int64_t data = atoi(&c);
      printf("%lld\n", data);
    }
  }

  int * S = generateSolution();

  // TODO generate A randomly
  // TODO KK on A
  fclose(fp);
}
