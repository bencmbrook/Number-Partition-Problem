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
  long long data;
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
  long long largest = (LCHILD(i) < hp->size && hp->elem[LCHILD(i)].data > hp->elem[i].data) ? LCHILD(i) : i;
  if(RCHILD(i) < hp->size && hp->elem[RCHILD(i)].data > hp->elem[largest].data) {
    largest = RCHILD(i);
  }
  if(largest != i) {
    swap(&(hp->elem[i]), &(hp->elem[largest]));
    heapify(hp, largest);
  }
}

/*
Function to insert a node into the max heap, by allocating space for that node in the
heap and also making sure that the heap property and shape propety are never violated.
*/
void insertNode(maxHeap *hp, long long data) {
  node nd;
  nd.data = data;

  printf("Inserting %lld\n", data);

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
long long deleteNode(maxHeap *hp) {
  if(hp->size) {
    long long data = hp->elem[0].data;
    printf("Deleting node %lld\n", hp->elem[0].data);
    hp->elem[0] = hp->elem[--(hp->size)];
    hp->elem = realloc(hp->elem, hp->size * sizeof(node));
    heapify(hp, 0);
    return data;
  } else {
    printf("\nMax Heap is empty!\n");
    free(hp->elem);
    return 0;
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

// Karmarkar Karp Algorithm Using Max Heap
long long KK(maxHeap *hp) {
  // Reinsert the difference between the max elt and second-max elt
  while (hp->size>1) {
    printf("Heap Size: %d\n", hp->size);
    long long one = deleteNode(hp);
    long long two = deleteNode(hp);
    insertNode(hp, one-two);
    printf("\n");
  }
  // Return the last elt
  return deleteNode(hp); // Residue
}

int main(int argc, char *argv[]) {
  srand(time(0));

  // Check for inputfile argument, otherwise run my generated input file
  char* filename;
  if (argc > 1) {
    filename = argv[1];
  }
  else {
    filename = "inputfile.txt";
  }

  FILE* fp = fopen("inputfile.txt", "r");
  if(!fp) {
    perror("File opening failed");
    return EXIT_FAILURE;
  }

  // Set up max heap
  maxHeap hp = initMaxHeap(sizeA);

  // Read values out of file and insert into heap
  int c; // note: int, not char, required to handle EOF
  long long data;
  char dataString[14];
  int counter = 0;
  while ((c = fgetc(fp)) != EOF) { // standard C I/O file reading loop
    if (c != '\n') {
      dataString[counter] = c;
      counter++;
    }
    else {
      counter++;
      dataString[counter] = '\0';
      data = atoll(dataString);
      // Put in heap
      insertNode(&hp, data);
      counter = 0;
    }
  }

  if (ferror(fp))
  puts("I/O error when reading");
  else if (feof(fp))
  puts("End of file reached successfully");

  fclose(fp);

  long long residue = KK(&hp);
  printf("%lld\n", residue);

  int * S = generateSolution();

  // TODO generate A randomly
  // TODO KK on A

}
