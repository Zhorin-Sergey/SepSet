#include "mpi.h"
#include "stdio.h"
#include "stdlib.h"
int main(int argc, char* argv[]) {
  MPI_Status status;
  int i = 0;
  int thread_count, rank;
  int *duff;
  int m = atoi(argv[1]);
  int n = atoi(argv[2]);
  int seed = atoi(argv[3]);
  int *a = 0;
  int* b2 = 0;
  MPI_Datatype anyStructType;
  int* len = new int[m];
  MPI_Aint* pos = new MPI_Aint[m];
  MPI_Datatype* typ = new MPI_Datatype[m];

   //printf("%i ", m);


  i = 0;
  int segmentSize, bufferSize;
  double starttime, endtime;
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &thread_count);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  segmentSize = n / thread_count;
  bufferSize = segmentSize;
  int* buff = new int[2 * segmentSize];
  while (i<m) {
    len[i] = segmentSize;
    pos[i] = i*n*sizeof(int);
    typ[i] = MPI_INT;
    i++;
  }
  i = 0;
  MPI_Type_struct(m, len, pos, typ, &anyStructType);
  MPI_Type_commit(&anyStructType);
  i = 0;
  if (rank == 0) {
    int k = 0;
    a = new int[n*m];
    b2 = new int[n];
    srand(seed);
	int l = 1;
    while (k<m*n) {
      a[k] = rand() % 10;
	  if(n*m < 25) {
        printf("%i ", a[k]);
		if(k == (n*l) -1) {
			printf(" \n");
			l++;
		}
	  }
	k++;
	}
	printf(" \n");
    k = 0;
    while (k<n) {
      b2[k] = 0;
      k++;
    }
    k = 0;
    int j = 0;
    starttime = MPI_Wtime();
    while (k < n) {
      j = k;
      while (j < n*m){
        (b2[(k)]) += a[j];
        j = j + n;
      }
      k = k + 1;
    }
    endtime = MPI_Wtime();
    k = 0;
    while (k<n) {
     // printf("%i ", b2[k]);
      k++;
    }
    printf("%f\n", endtime - starttime);
  }
  if (rank == 0) {
   
    i = 0;
    duff = new int[segmentSize*m];
    while (i<segmentSize*m) {
     
      duff[i] = 0;
      i++;
    }
    i = 1;
    starttime = MPI_Wtime();
    while (i < thread_count) {
      MPI_Send(a + (i)*segmentSize, 1, anyStructType, i, 0, MPI_COMM_WORLD);
      i++;
    }
    i = 0;
  }
  else {
    duff = new int[segmentSize*m];
    i = 0;
    while (i<segmentSize*m) {
      duff[i] = 0;
      i++;
    }
    i = 0;
    MPI_Recv(duff, segmentSize*m, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
  } 
  i = 0;
  while (i<segmentSize*m) {
  // printf("%i ", duff[i]);
    i++;
  }
 //printf("rank %i \n", rank);
  
  if (rank > 0) {
    i = 0;
    int x = 0;
    while (i < 2*segmentSize) {
      int sum = 0;
      int j = x;
      while (j < segmentSize*m) {
        sum += duff[j];
        j = j + segmentSize;
      }
     // printf("sum = %i \n", sum);
      buff[i] = rank*segmentSize + x;
      buff[i + 1] = sum;
      i= i+2;
      x++;
    }
    i = 0;
    MPI_Send(buff, 2 * segmentSize, MPI_INT, 0, 0, MPI_COMM_WORLD);
  }
  if (rank == 0) {
    int* b = new int[n];
    i = 0;
    while (i<n) {
      b[i] = 0;
      i++;
    }
    i = 0;
    while (i < segmentSize) {
      int j = i;
      while (j < m*n) {
        b[i] += a[j];
        j = j + n;
      }
      i++;
    }
    i = 0;
   // printf("%i ", b[0]);
    
    for (i = 1; i < thread_count; i++) {
      MPI_Recv(buff, 2 * segmentSize, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
      for (int j = 0; j < 2*segmentSize; j = j+2)
        b[buff[j]] = buff[j + 1];
    }
    i = 0;

    if (thread_count*segmentSize < n){
      i = thread_count*segmentSize;
      while (i < n) {
        int j = i;
        while (j < m*n) {
          b[i] += a[j];
          j = j + n;
        }
        i++;
      }
      
    }
	endtime = MPI_Wtime();
    i = 0;
    int flag = 1;
	if(n*m < 25)
      while (i<n) {
        printf("%i ", b[i]);
		i++;
	  }
	      printf(" \n");
	  i = 0;
    while (i<n) {
      if (b[i] != b2[i])
        flag = 0;
      i++;
    }
    printf("%i \n", flag);
    printf("%f\n", endtime - starttime);
  }
  
  MPI_Finalize();
  return 0;
}
