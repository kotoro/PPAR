#include <stdio.h>
#include <mpi.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <time.h>

#define N 4000
#define OFFSET 97

int main(int argc, char ** argv)
{
	int rank,n,i,j;
	int occ[26] = {0};
	int occResult[26] = {0};
	char str[N];
	char recv1[1000];

	srand (time(NULL));

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &n);
	
	if (rank == 0)
	{	
		srand (time(NULL));
		for(j = 0; j < N; j++)
		{
			// Valeurs ASCII de ESPACE a Z
			str[j] = rand() % 26 + OFFSET;
		}
		printf("=>%d\n", strlen(str));
		printf("%d = %d / %d\n", strlen(str)/n,strlen(str),n);
	} 


	MPI_Scatter(str, 1000, MPI_CHAR, recv1, 1000, MPI_CHAR, 0, MPI_COMM_WORLD);

	printf("rank: %d\n", rank);

	for(i = 0; i < strlen(recv1); i++)
	{
		occ[recv1[i] - 97]++;
	}

	//MPI_Alltoall(recv1,1,MPI_INT,recv2,1,MPI_INT, MPI_COMM_WORLD);


	//MPI_Gather(recv2, N*N/n, MPI_INT, a, N*N/n, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Reduce(occ,occResult,26,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);

	if (rank == 0)
	{
		printf("\nResultat :\n");
		// Affiche la matrice modifié
		for(i = 0; i < 26; i++)
		{
			printf("%c:%d\n",i+97, occResult[i]);
		}
	} 

	MPI_Finalize();
	return 0;
}