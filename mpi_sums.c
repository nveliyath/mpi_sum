#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
 #define N 1000000
 int my_id, numprocs;
 MPI_Init (&argc, &argv);
 MPI_Comm_size (MPI_COMM_WORLD, &numprocs);
 MPI_Comm_rank (MPI_COMM_WORLD, &my_id);

 long array[N];
 long brray[numprocs];
 int i;
 long total = 0;
 int roll;
 time_t t;
 int master = 0;
 MPI_Status status;
 long selfTotal = 0;
 double totalTime = 0;

 srand((unsigned) time(&t));
 //clock_t begin = clock();

 for (int j = 0; j < N; j++)
 {
  array[j] = (rand() % 10 + 1);
 }

 MPI_Bcast (array, N, MPI_DOUBLE, master, MPI_COMM_WORLD);
 
 clock_t begin = clock();
 double self = ((double) my_id)/((double) numprocs);
 double selfie = (((double) my_id) + 1)/((double) numprocs);
 self = self * N;
 selfie = selfie * N;
 int self2 = (int) self;
 int selfie2 = (int) selfie;
 
 for (i = self2; i < selfie2; i++)
 {
  selfTotal = selfTotal + array[self2];
  //brray[(int)my_id] = brray[(int)my_id] + array[(int)self];
 }

 clock_t end = clock();
 printf ("\n" );
 printf ("Process %d:\n", my_id );
 //printf ("My sum is %ld\n", brray[(int)my_id]);
 printf ("My sum is %ld\n", selfTotal);
 double time = (double)(end - begin) / CLOCKS_PER_SEC;
 printf("\nTime taken is: %f secs", time);
 
 if ( my_id != master ) 
 {
  //MPI_Send (&brray[(int)my_id], 1, MPI_LONG, master, 1, MPI_COMM_WORLD );
  MPI_Send (&selfTotal, 1, MPI_LONG, master, 1, MPI_COMM_WORLD );
  MPI_Send (&time, 1, MPI_DOUBLE, master, 1, MPI_COMM_WORLD );
 }
 else 
 {
  total = selfTotal;
  totalTime = time;
  for (i = 1; i < numprocs; i++ ) 
  {
    //MPI_Recv (&brray[(int)my_id], 1, MPI_LONG, MPI_ANY_SOURCE, 1, 
      //MPI_COMM_WORLD, &status);
    MPI_Recv (&selfTotal, 1, MPI_LONG, MPI_ANY_SOURCE, 1, 
      MPI_COMM_WORLD, &status);
    MPI_Recv (&time, 1, MPI_DOUBLE, MPI_ANY_SOURCE, 1, 
      MPI_COMM_WORLD, &status);
    total = total + selfTotal;
    totalTime = totalTime + time;
  }    
     //total = total + brray[(int)my_id];
 }

 if ( my_id == master )
  {
    printf ("\n");         
    printf ("Master process:\n");         
    printf ("Total sum is %.16ld\n", total);
    printf ("Total time is: %f secs\n", totalTime);
  }
 
 MPI_Finalize ( );
 //clock_t end = clock();
 //double time = (double)(end - begin) / CLOCKS_PER_SEC;
 //printf("\nTime taken is: %f secs", time);
}
