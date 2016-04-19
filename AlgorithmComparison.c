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
void heapify(maxHeap *hp, long long i) {
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
  // allocating space
  if(hp->size) {
      hp->elem = realloc(hp->elem, (hp->size + 1) * sizeof(node)) ;
  } else {
      hp->elem = malloc(sizeof(node)) ;
  }

  node nd;
  nd.data = data;

  // printf("Inserting %lld\n", data);

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
    // printf("Deleting node %lld\n", hp->elem[0].data);
    hp->elem[0] = hp->elem[--(hp->size)];
    hp->elem = realloc(hp->elem, hp->size * sizeof(node));
    heapify(hp, 0);
    return data;
  } else {
    // printf("\nMax Heap is empty!\n");
    free(hp->elem);
    return 0;
  }
}

// Karmarkar Karp Algorithm Using Max Heap
long long KK(maxHeap *hp) {
  // Reinsert the difference between the max elt and second-max elt
  while (hp->size>1) {
    // printf("Heap Size: %d\n", hp->size);
    long long one = deleteNode(hp);
    long long two = deleteNode(hp);
    insertNode(hp, one-two);
    // printf("\n");
  }
  // Return the last elt
  return deleteNode(hp); // Residue
}

// Helper functions for the next 3 algorithms
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

// int * generateNeighbourStdSolution(int *S) {
//   static int tempS[sizeA];
//   return tempS;
// }

int getResidue(long long *A, int *S) {
  long long residue = 0;
  for (size_t i = 0; i < sizeA; i++) {
    residue+= A[i]*S[i];
  }
  return residue;
}

// Repeated Random algorithm
long long RR(long long *A) {
  int * S = generateSolution();
  long long residue = getResidue(A, S);
  long long tempResidue;
  // Run this maxIter times
  for (size_t i = 0; i < maxIter; i++) {
    int * tempS = generateNeighbourSolution(S);
    tempResidue = getResidue(A, tempS);
    if (llabs(tempResidue)<llabs(residue)) {
      residue = tempResidue;
      S = tempS;
    }
  }
  return llabs(residue);
}

// Hill Climbing algorithm
long long HC(long long *A) {
  int * S = generateSolution();
  long long residue = getResidue(A, S);
  long long tempResidue;
  // Run this maxIter times
  for (size_t i = 0; i < maxIter; i++) {
    int * tempS = generateSolution(); // this part changes TODO
    tempResidue = getResidue(A, tempS);
    if (llabs(tempResidue)<llabs(residue)) {
      residue = tempResidue;
      S = tempS;
    }
  }
  return llabs(residue);
}

int main(int argc, char *argv[]) {
  srand(time(0));

  // Check for inputfile argument, otherwise run my generated input file
  char* filename;
  if (argc > 1) {
    filename = argv[1];
  }
  else {
    printf("No input argument specified. Using input/in1.txt\n");
    filename = "./input/in1.txt";
  }

  FILE* fp = fopen(filename, "r");
  if(!fp) {
    perror("File opening failed");
    return EXIT_FAILURE;
  }

  // Set up max heap
  maxHeap hp = initMaxHeap(sizeA);

  // Read values out of file and insert into heap
  long long A[sizeA];
  int counterArray = 0;
  int c; // note: int, not char, required to handle EOF
  long long data;
  char dataString[14];
  int counterString = 0;
  while ((c = fgetc(fp)) != EOF) { // standard C I/O file reading loop
    if (c != '\n') {
      dataString[counterString] = c;
      counterString++;
    }
    else {
      counterString++;
      dataString[counterString] = '\0';
      data = atoll(dataString);
      // Put in heap
      insertNode(&hp, data);
      // Also put in array for other algorithms
      A[counterArray] = data;
      counterString = 0;
      counterArray++;
    }
  }

  if (ferror(fp))
  puts("I/O error when reading");
  // else if (feof(fp))
  // puts("End of file reached successfully");
  fclose(fp);

  // Get the Karmarkar-Karp residue
  long long residueKK = KK(&hp);
  printf("KK residue is %lld\n", residueKK);

  // Get the Repeated Random residue
  long long residueRR = RR(A);
  printf("RR residue is %lld\n", residueRR);

  // Get the Hill Climbing residue
  long long residueHC = HC(A);
  printf("HC residue is %lld\n", residueHC);

  // Get the Simulated Annealing residue
  // long long residueSA = SA(A);
  // printf("SA residue is %lld\n", residueSA);

  // int * S = generateSolution();
}
