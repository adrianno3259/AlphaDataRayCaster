#include <stdio.h>
#include "admxrc3.h"
#include <time.h>
#include <sys/time.h>

#define XVADD_LITE1_ADDR_AP_CTRL     0x00
#define XVADD_LITE1_ADDR_GIE         0x04
#define XVADD_LITE1_ADDR_IER         0x08
#define XVADD_LITE1_ADDR_ISR         0x0c
#define XVADD_LITE1_ADDR_A_DATA      0x10
#define XVADD_LITE1_BITS_A_DATA      32
#define XVADD_LITE1_ADDR_B_DATA      0x18
#define XVADD_LITE1_BITS_B_DATA      32
#define XVADD_LITE1_ADDR_C_DATA      0x20
#define XVADD_LITE1_BITS_C_DATA      32
#define XVADD_LITE1_ADDR_N_ELEM_DATA 0x28
#define XVADD_LITE1_BITS_N_ELEM_DATA 32

#define TRUE (0==0)
#define FALSE (0==1)

#define VSIZE 10000

#define XINTERSECTFPGA_CONTROL_ADDR_AP_CTRL            0x00
#define XINTERSECTFPGA_CONTROL_ADDR_GIE                0x04
#define XINTERSECTFPGA_CONTROL_ADDR_IER                0x08
#define XINTERSECTFPGA_CONTROL_ADDR_ISR                0x0c
#define XINTERSECTFPGA_CONTROL_ADDR_I_TNUMBER_DATA     0x10
#define XINTERSECTFPGA_CONTROL_BITS_I_TNUMBER_DATA     32
#define XINTERSECTFPGA_CONTROL_ADDR_I_TDATA_DATA       0x18
#define XINTERSECTFPGA_CONTROL_BITS_I_TDATA_DATA       32
#define XINTERSECTFPGA_CONTROL_ADDR_I_TIDS_DATA        0x20
#define XINTERSECTFPGA_CONTROL_BITS_I_TIDS_DATA        32
#define XINTERSECTFPGA_CONTROL_ADDR_I_RNUMBER_DATA     0x28
#define XINTERSECTFPGA_CONTROL_BITS_I_RNUMBER_DATA     32
#define XINTERSECTFPGA_CONTROL_ADDR_I_RDATA_DATA       0x30
#define XINTERSECTFPGA_CONTROL_BITS_I_RDATA_DATA       32
#define XINTERSECTFPGA_CONTROL_ADDR_O_TIDS_DATA        0x38
#define XINTERSECTFPGA_CONTROL_BITS_O_TIDS_DATA        32
#define XINTERSECTFPGA_CONTROL_ADDR_O_TINTERSECTS_DATA 0x40
#define XINTERSECTFPGA_CONTROL_BITS_O_TINTERSECTS_DATA 32


int main(int argc, char **argv)
{
	unsigned long index = 0;
	ADMXRC3_STATUS status;
    ADMXRC3_HANDLE hCard = ADMXRC3_HANDLE_INVALID_VALUE;

	uint64_t NUM_RAYS = 1;
	uint64_t RAY_SIZE = 6;
	uint64_t MAX_RAYS = 40000;

	uint64_t NUM_TRIS = 1;
	uint64_t TRI_SIZE = 9;
	uint64_t MAX_TRIS = 50000;

    float tData[NUM_TRIS * TRI_SIZE];
    int idData[NUM_TRIS];
    float rData[NUM_RAYS * RAY_SIZE];
    int outIds[NUM_RAYS];
    float outInter[NUM_RAYS];

	//float vA[VSIZE];
	//float vB[VSIZE];
	//float vC[VSIZE];

	int i;
	uint64_t ctrl;

	//uint64_t n_elem;
	//uint64_t addr_a;
	//uint64_t addr_b;
	//uint64_t addr_c;
	//uint64_t aux;

	ctrl = 0;
    /*
    n_elem = VSIZE;
	addr_a = 0;
	addr_b = 40000;
	addr_c = 80000;

	for(i = 0 ; i < VSIZE ; i++)
	{
		vA[i] = (float) i;
		vB[i] = 1.0;
		vC[i] = -1.0;
	}*/

	printf("%d - %d - %d - %d\n", sizeof(uint64_t), sizeof(uint32_t), sizeof(int*), sizeof(float*));

	////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////

	uint64_t addr_tris = 0;
	uint64_t addr_ids = sizeof(float) * TRI_SIZE * MAX_TRIS;
	uint64_t addr_rays = addr_ids + (sizeof(int) * MAX_TRIS);
	uint64_t addr_outInter = addr_rays + (sizeof(float) * MAX_RAYS * RAY_SIZE);
	uint64_t addr_outIds = addr_outInter + (sizeof(float) * MAX_RAYS);

	tData[0] = 0; tData[1] = 0; tData[2] = 1;
	tData[3] = 1; tData[4] =  0; tData[5] = -1;
	tData[6] = -1; tData[7] =  0; tData[8] =  -1;

	idData[0] = 1;

	rData[0] = 0; rData[1] = 1; rData[2] = 0;
	rData[3] = 0; rData[4] = -1; rData[5] = 0;


	for(i = 0 ; i < NUM_RAYS * RAY_SIZE; i++)
	{
		printf(" rData[%d] = %f \n", i, rData[i]);
	}

	for(i = 0 ; i < NUM_TRIS * TRI_SIZE; i++)
	{
		printf(" tData[%d] = %f \n", i, tData[i]);
	}


	status = ADMXRC3_Open(0, &hCard);

	if (status != ADMXRC3_SUCCESS) {
		fprintf(stderr,"Failed to open card with index %lu: %s\n", (unsigned long)index, ADMXRC3_GetStatusString(status, TRUE));
		return -1;
	}

	printf("IP Status:\n");
	ADMXRC3_Read(hCard, 0, 0, XINTERSECTFPGA_CONTROL_ADDR_AP_CTRL, sizeof(uint64_t), &ctrl);
	printf("status 0x%lx\n",ctrl);

	ADMXRC3_Write(hCard, 0, 0, XINTERSECTFPGA_CONTROL_ADDR_AP_CTRL, sizeof(uint64_t), &ctrl);

	ADMXRC3_Write(hCard, 0, 0, XINTERSECTFPGA_CONTROL_ADDR_I_TNUMBER_DATA, sizeof(uint64_t), &NUM_TRIS);
	ADMXRC3_Write(hCard, 0, 0, XINTERSECTFPGA_CONTROL_ADDR_I_TIDS_DATA, sizeof(uint64_t), &addr_ids);
	ADMXRC3_Write(hCard, 0, 0, XINTERSECTFPGA_CONTROL_ADDR_I_TDATA_DATA, sizeof(uint64_t), &addr_tris);

	ADMXRC3_Write(hCard, 0, 0, XINTERSECTFPGA_CONTROL_ADDR_I_RNUMBER_DATA, sizeof(uint64_t), &NUM_RAYS);
	ADMXRC3_Write(hCard, 0, 0, XINTERSECTFPGA_CONTROL_ADDR_I_RDATA_DATA, sizeof(uint64_t), &addr_rays);

	ADMXRC3_Write(hCard, 0, 0, XINTERSECTFPGA_CONTROL_ADDR_O_TIDS_DATA, sizeof(uint64_t), &addr_outIds);
	ADMXRC3_Write(hCard, 0, 0, XINTERSECTFPGA_CONTROL_ADDR_O_TINTERSECTS_DATA, sizeof(uint64_t), &addr_outInter);


	//ADMXRC3_Write(hCard, 0, 0, XVADD_LITE1_ADDR_A_DATA, sizeof(uint64_t), &addr_a);
	//ADMXRC3_Write(hCard, 0, 0, XVADD_LITE1_ADDR_B_DATA, sizeof(uint64_t), &addr_b);
	//ADMXRC3_Write(hCard, 0, 0, XVADD_LITE1_ADDR_C_DATA, sizeof(uint64_t), &addr_c);
	//ADMXRC3_Write(hCard, 0, 0, XVADD_LITE1_ADDR_N_ELEM_DATA, sizeof(uint64_t), &n_elem);


	//---------------------------------------------------
	//---------------------dma writes--------------------
	//---------------------------------------------------

	status = ADMXRC3_WriteDMA(hCard, 0, 0, tData, NUM_TRIS * TRI_SIZE * sizeof(float), addr_tris);
	if (status != ADMXRC3_SUCCESS) {
		fprintf(stderr,"Triangle data write DMA transfer failed: %s\n", ADMXRC3_GetStatusString(status, TRUE));
		return -1;
	}

	status = ADMXRC3_WriteDMA(hCard, 0, 0, idData, NUM_TRIS * sizeof(int), addr_ids);
	if (status != ADMXRC3_SUCCESS) {
		fprintf(stderr,"Id data write DMA transfer failed: %s\n", ADMXRC3_GetStatusString(status, TRUE));
		return -1;
	}

	status = ADMXRC3_WriteDMA(hCard, 0, 0, rData, NUM_RAYS * RAY_SIZE * sizeof(float), addr_rays);
	if (status != ADMXRC3_SUCCESS) {
		fprintf(stderr,"Triangle data write DMA transfer failed: %s\n", ADMXRC3_GetStatusString(status, TRUE));
		return -1;
	}


	/*
	status = ADMXRC3_WriteDMA(hCard,0,0,vA,VSIZE*sizeof(int),addr_a);
	if (status != ADMXRC3_SUCCESS) {
		fprintf(stderr,"vA write DMA transfer failed: %s\n", ADMXRC3_GetStatusString(status, TRUE));
		return -1;
	}

	status = ADMXRC3_WriteDMA(hCard,0,0,vB,VSIZE*sizeof(int),addr_b);
	if (status != ADMXRC3_SUCCESS) {
		fprintf(stderr,"vB write DMA transfer failed: %s\n", ADMXRC3_GetStatusString(status, TRUE));
		return -1;
	}*/
	printf("write dma done\n");
	//---------------------------------------------------
	//---------------------------------------------------
	//---------------------------------------------------

	ADMXRC3_Read(hCard, 0, 0, XINTERSECTFPGA_CONTROL_ADDR_AP_CTRL, sizeof(uint64_t), &ctrl);

    if(ctrl == 4)
    {
            printf("core is idle, ctrl = %ld\n",ctrl);
    }

    //start
    ctrl = 1;
	ADMXRC3_Write(hCard, 0, 0, XINTERSECTFPGA_CONTROL_ADDR_AP_CTRL, sizeof(uint64_t), &ctrl);
	ADMXRC3_Read(hCard, 0, 0, XINTERSECTFPGA_CONTROL_ADDR_AP_CTRL, sizeof(uint64_t), &ctrl);
	printf("core started\n");

	//espera resultado
	ADMXRC3_Read(hCard, 0, 0, XINTERSECTFPGA_CONTROL_ADDR_AP_CTRL, sizeof(uint64_t), &ctrl);
	//while(ctrl != 4)
	while(ctrl != 4)
	{
		ADMXRC3_Read(hCard, 0, 0, XINTERSECTFPGA_CONTROL_ADDR_AP_CTRL, sizeof(uint64_t), &ctrl);
		//printf("waiting (0x%lx)\n",ctrl);
	}
	printf("done!\n");

	//---------------------------------------------------
	//---------------------dma read----------------------
	//---------------------------------------------------
	status = ADMXRC3_ReadDMA(hCard, 0, 0, outIds, NUM_RAYS * sizeof(int), addr_outIds);
	if (status != ADMXRC3_SUCCESS) {
		fprintf(stderr,"outIds read DMA transfer failed: %s\n", ADMXRC3_GetStatusString(status, TRUE));
		return -1;
	}

	status = ADMXRC3_ReadDMA(hCard, 0, 0, outInter, NUM_RAYS * sizeof(float), addr_outInter);
	if (status != ADMXRC3_SUCCESS) {
		fprintf(stderr,"outInter read DMA transfer failed: %s\n", ADMXRC3_GetStatusString(status, TRUE));
		return -1;
	}
	//---------------------------------------------------
	//---------------------------------------------------
	//---------------------------------------------------

	for(i = 0 ; i < NUM_RAYS ; i++)
	{
		printf(" outIds[%d] = %d \n", i, outIds[i]);
		printf(" outInter[%d] = %f \n\n", i, outInter[i]);
	}
	printf("\n");

	status = ADMXRC3_Close(hCard);
	if (status != ADMXRC3_SUCCESS) {
		fprintf(stderr,"Failed to close card with index %lu: %s\n", (unsigned long)index, ADMXRC3_GetStatusString(status, TRUE));
		return -1;
	}

	return 0;
}
