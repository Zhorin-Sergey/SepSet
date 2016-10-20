#include "mpi.h"
#include "stdio.h"
int main(int argc, char* argv[]) {
  MPI_Status status;
  int i = 0;
  int thread_count, rank;
  int *duff;
  int m = 100; //int(argv[1]);
  int n = 110;// int(argv[2]);
  int *a = 0;
  MPI_Datatype anyStructType;
  int* len = new int[m];
  MPI_Aint* pos = new MPI_Aint[m];
  MPI_Datatype* typ = new MPI_Datatype[m];
  while (i<3) {
 //   printf("%c ", ((argv[i])));
    i++;
  }
  i = 0;
  int segmentSize, bufferSize;

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
 /* int* sendcounts;
  int* displs;
  sendcounts = new int[thread_count];
  displs = new int[thread_count];

  while (i < thread_count) {
    sendcounts[i] = segmentSize;
    displs[i] = i*segmentSize*sizeof(int);
    i++;
  }*/
  i = 0;
  if (rank == 0) {
    a = new int[n*m];
    i = 0;
    duff = new int[n*m];
    while (i<n*m) {
      a[i] = i;
      duff[i] = 0;
      i++;
    }
    i = 1;

   /* MPI_Scatterv(a, sendcounts, displs,
      anyStructType, duff, segmentSize,
      anyStructType, 0, MPI_COMM_WORLD);*/
   /* MPI_Scatter(a, segmentSize, anyStructType,
      duff, segmentSize, anyStructType,
      0, MPI_COMM_WORLD);*/
    while (i < thread_count) {
      MPI_Send(a + (i)*segmentSize, 1, anyStructType, i, 0, MPI_COMM_WORLD);
      i++;
    }
    i = 0;
  }
  else {
    duff = new int[n*m];
    i = 0;
    while (i<n*m) {
      duff[i] = 0;
      i++;
    }
    i = 0;
   /* MPI_Scatterv(0, sendcounts, displs,
      anyStructType, duff, segmentSize,
      anyStructType, 0, MPI_COMM_WORLD);*/
   /* MPI_Scatter(0, 0, anyStructType,
      duff, segmentSize, anyStructType,
      0, MPI_COMM_WORLD);*/
    MPI_Recv(duff, 1, anyStructType, 0, 0, MPI_COMM_WORLD, &status);
  } 

  // if (rank == 0) {
   // for (i = 1; i < thread_count; i++) {
    /*
      MPI_Scatterv(a, sendcounts, displs,
        anyStructType, duff, segmentSize,
        anyStructType, 0, MPI_COMM_WORLD); */
      // int count = segmentSize;
     /* MPI_Scatter(a, segmentSize, anyStructType,
        duff, segmentSize, anyStructType,
      0, MPI_COMM_WORLD);*/
     // MPI_Send(vector + i*segmentSize, count, MPI_INT, i, 0, MPI_COMM_WORLD);
   // }
 // }
 // else {
   /* MPI_Scatter(0, 0, anyStructType,
      duff, segmentSize, anyStructType,
      0, MPI_COMM_WORLD);*/
    //MPI_Recv(vector, bufferSize, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    //MPI_Scatterv(0, sendcounts, displs,
      //anyStructType, duff, segmentSize,
     // anyStructType, 0, MPI_COMM_WORLD);
 // } 
  i = 0;
  while (i<n*m) {
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
      while (j < m*n) {
        sum += duff[j];
        j = j + n;
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
    i = 0;
    while (i<n) {
     printf("%i ", b[i]);
      i++;
    }
  }

  MPI_Finalize();
  return 0;
}
