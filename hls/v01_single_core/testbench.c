#include <stdio.h>
#include <stdlib.h>
#include "main.h"

/*
void intersectFPGA(
                int i_tNumber,          /// number of triangles
                volatile double *i_tData,        /// triangle information.    Size = i_tNumber * Triangle attribute number(12)
                volatile int *i_tIds,            /// triangle id information. Size = i_tNumber
                int i_rNumber,          /// number of rays
                volatile double *i_rData,        /// ray information                             Size = i_rNumber * Ray Attr number(6)
                volatile int *o_tIds,            /// min intersection triangle ids for each ray. Size = i_rNumber
                volatile double *o_tIntersects   /// min intersection t value for each ray       Size = i_rNumber
                );
*/

#define NUM_TRIS 1 /*2000*/
#define NUM_RAYS 1 /*40000*/
#define TRI_PAR 9
#define RAY_PAR 6

int main(){


	double tdata[] = {0, 0, 1, 1, 0, -1, -1, 0, -1};

	double rdata[] = {0, 1, 0, 0, -1, 0};

	int tids[] = {1};

	int outids[NUM_RAYS];
	double outdata[NUM_RAYS];

	/*
	double *tdata = (double*) malloc(sizeof(double) * NUM_TRIS * TRI_PAR);
	int *tids = (int*) malloc(sizeof(int)*NUM_TRIS);
	double *rdata = (double*) malloc(sizeof(double) * NUM_RAYS * RAY_PAR);

	int *outids = (int*) malloc(sizeof(int)*NUM_RAYS);
	double *outdata = (double*) malloc(sizeof(double) * NUM_RAYS);

	FILE *input, *output;

	input = fopen("input_2000_200_200.txt", "r");
	output = fopen("results_2000_200_200.txt", "r");
	int k;
	fscanf(input, "%d", &k);

	*/
    /*
	int i, j;
	for(i = 0; i < NUM_TRIS; i++)
	{
		int base = i * TRI_PAR;
		tids[i] = i;
		printf("--------------- ID = %d ----------------\n", tids[i]);
		for(j = 0; j < TRI_PAR; j++)
		{

			fscanf(input, "%lf", &(tdata[base + j]));
			printf("%lf ", tdata[base + j]);
		}
		printf("\n");

	}

	for(i = 0; i < NUM_RAYS; i++)
	{
		int base = i * RAY_PAR;

		//printf("--------------- RAY = %d ----------------\n", tids[i]);
		for(j = 0; j < RAY_PAR; j++)
		{

			fscanf(input, "%lf", &(rdata[base + j]));
			//printf("%lf ", tdata[base + j]);
		}
		//printf("\n");

	}*/

	intersectFPGA(NUM_TRIS, tdata, tids, NUM_RAYS, rdata, outids, outdata);

	//int tmpId;
	//double tmpT;
	int i, j;

	for(j = 0; j < NUM_RAYS; j++)
	{
		printf("--------------- RAY = %d ----------------\n", j);
		printf("current | saved\n");
		printf("%lf - ",outdata[j]);
		printf("%d\n",outids[j]);


		//fscanf(output, "%lf %d", &(tmpT), &(tmpId));


	}

	//fflush(stdout);
	//fclose(input);
	//fclose(output);

	//free(tdata);
	//free(rdata);
	//free(tids);

	//free(outids);
	//free(outdata);


	return 0;
}
