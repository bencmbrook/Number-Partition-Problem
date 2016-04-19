#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#include <time.h>

const int sizeA = 100;
const int maxIter = 25000;
const int numFiles = 50;
const int numAlgs = 7;

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

// Based on http://codingstreet.com/create-csv-file-in-c/#
void createCSV(char *filename,long long a[numFiles][numAlgs],int n,int m) {
  printf("\n Creating %s.csv file",filename);
  FILE *fp;
  int i,j;
  filename=strcat(filename,".csv");
  fp=fopen(filename,"w+");

  fprintf(fp,"trial, KK, RRStd, RRPtn, HCStd, HCPtn, SAStd, SAPtn");

  for(i=0;i<m;i++){
    fprintf(fp,"\n%d",i+1);
    for(j=0;j<n;j++) {
      fprintf(fp,",%lld ",a[i][j]);
    }
  }
  fclose(fp);
  printf("\n %sfile created",filename);
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
int * generateStdSolution() {
  static int S[sizeA];
  for (size_t i = 0; i < sizeA; i++) {
    if (rand() > RAND_MAX/2) {
      S[i] = 1;
    } else {
      S[i] = -1;
    }
  }
  return S;
}

int * generatePtnSolution() {
  static int S[sizeA];
  for (size_t i = 0; i < sizeA; i++) {
    S[i] = (rand() % (int)(100));
  }
  return S;
}

int * generateNeighbourStdSolution(int *S) {
  // With probability 1/2 we swap some i between sets
  int i = (rand() % (int)(100));
  if (rand() > RAND_MAX/2) {
    S[i]=-S[i];
  }
  // With probability 1/2 we swap some j between sets, where i!=j
  if (rand() > RAND_MAX/2) {
    int j = (rand() % (int)(100));
    while (j==i) {
      j = (rand() % (int)(100));
    }
    S[j]=-S[j];
  }
  return S;
}

int * generateNeighbourPtnSolution(int *S) {
  int i = (rand() % (int)(100));
  int j = (rand() % (int)(100));
  // Let P[i]=j where P[i]!=j
  while (S[i]==j) {
    j = (rand() % (int)(100));
  }
  S[i]=j;
  return S;
}

int getResidueStd(long long *A, int *S) {
  long long residue = 0;
  for (size_t i = 0; i < sizeA; i++) {
    residue+= A[i]*S[i];
  }
  return residue;
}

int getResiduePtn(long long *A, int *S) {
  // Initialize max heap for KK processing
  maxHeap hp = initMaxHeap(sizeA);

  // Initialize array of zeros
  long long Z[100];
  for (size_t i = 0; i < sizeA; i++) {
    Z[i]=0;
  }
  for (size_t i = 0; i < sizeA; i++) {
    Z[S[i]]+=A[i];
  }
  // Set up heap
  for (size_t i = 0; i < sizeA; i++) {
    insertNode(&hp, Z[i]);
  }

  // Run KK heuristic on prepartitioned values
  long long residue = KK(&hp);
  return residue;
}

// Repeated Random algorithm - Standard Solution
long long RRStd(long long *A) {
  int * S = generateStdSolution();
  long long residue = getResidueStd(A, S);
  long long tempResidue;
  // Run this maxIter times
  for (size_t i = 0; i < maxIter; i++) {
    int * tempS = generateStdSolution();
    tempResidue = getResidueStd(A, tempS);
    if (llabs(tempResidue)<llabs(residue)) {
      residue = tempResidue;
      S = tempS;
    }
  }
  return llabs(residue);
}

// Repeated Random algorithm - Prepartitioning Solution
long long RRPtn(long long *A) {
  int * S = generatePtnSolution();
  long long residue = getResiduePtn(A, S);
  long long tempResidue;
  // Run this maxIter times
  for (size_t i = 0; i < maxIter; i++) {
    int * tempS = generatePtnSolution();
    tempResidue = getResiduePtn(A, tempS);
    if (llabs(tempResidue)<llabs(residue)) {
      residue = tempResidue;
      S = tempS;
    }
  }
  return llabs(residue);
}

// Hill Climbing algorithm - Standard Solution
long long HCStd(long long *A) {
  int * S = generateStdSolution();
  long long residue = getResidueStd(A, S);
  long long tempResidue;
  // Run this maxIter times
  for (size_t i = 0; i < maxIter; i++) {
    int * tempS = generateNeighbourStdSolution(S);
    tempResidue = getResidueStd(A, tempS);
    if (llabs(tempResidue)<llabs(residue)) {
      residue = tempResidue;
      S = tempS;
    }
  }
  return llabs(residue);
}

// Hill Climbing algorithm - Prepartitioning Solution
long long HCPtn(long long *A) {
  int * S = generatePtnSolution();
  long long residue = getResiduePtn(A, S);
  long long tempResidue;
  // Run this maxIter times
  for (size_t i = 0; i < maxIter; i++) {
    int * tempS = generateNeighbourPtnSolution(S);
    tempResidue = getResiduePtn(A, tempS);
    if (llabs(tempResidue)<llabs(residue)) {
      residue = tempResidue;
      S = tempS;
    }
  }
  return llabs(residue);
}

// Simulated Annealing algorithm - Standard Solution
long long SAStd(long long *A) {
  int * S = generateStdSolution();
  int * temptempS = S;
  long long residue = getResidueStd(A, S);
  long long tempResidue;
  long long temptempResidue = residue;
  // Run this maxIter times
  for (size_t i = 0; i < maxIter; i++) {
    int * tempS = generateNeighbourStdSolution(S);
    tempResidue = getResidueStd(A, tempS);
    if (llabs(tempResidue)<llabs(residue)) {
      residue = tempResidue;
      S = tempS;
    }
    else {
      long double TIter = powl(10,10)*powl((long double)0.8,(long double)i/300.0);
      long double probability = exp(-((long double)llabs(tempResidue)-(long double)llabs(residue))/TIter);
      if ( rand() < probability*RAND_MAX ) {
        residue = tempResidue;
        S = tempS;
      }
    }
    if (llabs(residue)<llabs(temptempResidue)) {
      temptempResidue = residue;
      temptempS = S;
    }
  }
  return llabs(residue);
}

// Simulated Annealing algorithm - Prepartitioning Solution
long long SAPtn(long long *A) {
  int * S = generatePtnSolution();
  int * temptempS = S;
  long long residue = getResiduePtn(A, S);
  long long tempResidue;
  long long temptempResidue = residue;
  // Run this maxIter times
  for (size_t i = 0; i < maxIter; i++) {
    int * tempS = generateNeighbourPtnSolution(S);
    tempResidue = getResiduePtn(A, tempS);
    if (llabs(tempResidue)<llabs(residue)) {
      residue = tempResidue;
      S = tempS;
    }
    else {
      long double TIter = powl(10,10)*powl((long double)0.8,(long double)i/300.0);
      long double probability = exp(-((long double)llabs(tempResidue)-(long double)llabs(residue))/TIter);
      if ( rand() < probability*RAND_MAX ) {
        residue = tempResidue;
        S = tempS;
      }
    }
    if (llabs(residue)<llabs(temptempResidue)) {
      temptempResidue = residue;
      temptempS = S;
    }
  }
  return llabs(residue);
}

int main(int argc, char *argv[]) {
  srand(time(0));

  // Set up CSV files for recording data
  long long allResidues[numFiles][numAlgs];
  long long allTimes[numFiles][numAlgs];
  char residueCSV[20] = "residueCSV";
  char timeCSV[20] = "timeCSV";

  for (size_t fileSuffix = 0; fileSuffix < numFiles; fileSuffix++) {
    char filename[80];
    strcpy (filename,"./input/in");
    char numStr[15];
    sprintf(numStr, "%zu", fileSuffix);
    strcat (filename,numStr);
    strcat (filename,".txt");
    puts (filename);

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

    clock_t start = clock(), diff;
    // Get the Karmarkar-Karp residue
    long long residueKK = KK(&hp);
    diff = clock() - start;
    int msec = diff * 1000 / CLOCKS_PER_SEC;
    allResidues[fileSuffix][0] = residueKK;
    allTimes[fileSuffix][0] = (long long)msec;
    printf("KK residue is %lld\n", residueKK);
    printf("KK took %d seconds %d milliseconds\n\n", msec/1000, msec%1000);

    // Get the Repeated Random residue - Standard
    start = clock();
    long long residueRRStd = RRStd(A);
    diff = clock() - start;
    msec = diff * 1000 / CLOCKS_PER_SEC;
    allResidues[fileSuffix][1] = residueRRStd;
    allTimes[fileSuffix][1] = (long long)msec;
    printf("RRStd residue is %lld\n", residueRRStd);
    printf("RRStd took %d seconds %d milliseconds\n\n", msec/1000, msec%1000);

    // Get the Repeated Random residue - Prepartition
    start = clock();
    long long residueRRPtn = RRPtn(A);
    diff = clock() - start;
    msec = diff * 1000 / CLOCKS_PER_SEC;
    allResidues[fileSuffix][2] = residueRRPtn;
    allTimes[fileSuffix][2] = (long long)msec;
    printf("RRPtn residue is %lld\n", residueRRPtn);
    printf("RRPtn took %d seconds %d milliseconds\n\n", msec/1000, msec%1000);

    // Get the Hill Climbing residue - Standard
    start = clock();
    long long residueHCStd = HCStd(A);
    diff = clock() - start;
    msec = diff * 1000 / CLOCKS_PER_SEC;
    allResidues[fileSuffix][3] = residueHCStd;
    allTimes[fileSuffix][3] = (long long)msec;
    printf("HCStd residue is %lld\n", residueHCStd);
    printf("HCStd took %d seconds %d milliseconds\n\n", msec/1000, msec%1000);

    // Get the Hill Climbing residue - Prepartition
    start = clock();
    long long residueHCPtn = HCPtn(A);
    diff = clock() - start;
    msec = diff * 1000 / CLOCKS_PER_SEC;
    allResidues[fileSuffix][4] = residueHCPtn;
    allTimes[fileSuffix][4] = (long long)msec;
    printf("HCPtn residue is %lld\n", residueHCPtn);
    printf("HCPtn took %d seconds %d milliseconds\n\n", msec/1000, msec%1000);

    // Get the Simulated Annealing residue - Standard
    start = clock();
    long long residueSAStd = SAStd(A);
    diff = clock() - start;
    msec = diff * 1000 / CLOCKS_PER_SEC;
    allResidues[fileSuffix][5] = residueSAStd;
    allTimes[fileSuffix][5] = (long long)msec;
    printf("SAStd residue is %lld\n", residueSAStd);
    printf("SAStd took %d seconds %d milliseconds\n\n", msec/1000, msec%1000);

    // Get the Simulated Annealing residue - Prepartition
    start = clock();
    long long residueSAPtn = SAPtn(A);
    diff = clock() - start;
    msec = diff * 1000 / CLOCKS_PER_SEC;
    allResidues[fileSuffix][6] = residueSAPtn;
    allTimes[fileSuffix][6] = (long long)msec;
    printf("SAPtn residue is %lld\n", residueSAPtn);
    printf("SAPtn took %d seconds %d milliseconds\n\n", msec/1000, msec%1000);
  }
  createCSV(residueCSV,allResidues,numAlgs,numFiles);
  createCSV(timeCSV,allTimes,numAlgs,numFiles);
}
