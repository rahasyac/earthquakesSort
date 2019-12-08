/*
 Name: FNU Rahasya Chandan
 UTA Id: 1000954962
 Using bubble sort to sort EarthQuake data and time it
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

//buffer size
#define BUFFER_SIZE 50
//buffer big size
#define BUFFER_BIG_SIZE 300
//input file name
#define INPUT_FILENAME "all_month.csv"
//each line contains 22 columns
#define RECORD_SIZE 22

typedef struct EarthQuake{
	char time[BUFFER_SIZE];
	double latitude;
	double longitude;
	double depth;
	double mag;
	char magType[BUFFER_SIZE];
	int nst;
	double gap;
	double dmin;
	double rms;
	char net[BUFFER_SIZE];
	char id[BUFFER_SIZE];
	char updated[BUFFER_SIZE];
	char place[BUFFER_SIZE];
	char type[BUFFER_SIZE];
	double horizontalError;	
	double depthError;
	double magError;
	int magNst;
	char status[BUFFER_SIZE];
	char locationSource[BUFFER_SIZE];
	char magSource[BUFFER_SIZE];	
	
} EarthQuake;

int readNumLines(){

	FILE *fp; //File structure pointer
	char buffer[BUFFER_BIG_SIZE]; //enough for one line from csv
	int lines = -1;  //number of lines, ignore header line

	//open file for reading
	fp = fopen(INPUT_FILENAME, "r"); 

	if (fp == NULL)
	{
		printf("Error while opening the file.\n");
		exit(EXIT_FAILURE);
	}

	//read line by line until end of file
	while (fgets(buffer, BUFFER_BIG_SIZE, fp) != NULL){

		lines++;
	}

	//close file
	fclose(fp);

	return lines;
}


void loadData(EarthQuake* dataArray, int size){

	FILE *fp; //File structure pointer
	char buffer[BUFFER_BIG_SIZE]; //enough for one line from csv
	int lines = 0;  //number of lines
	int i; //loop control variable
	char tokens[RECORD_SIZE][BUFFER_SIZE]; //data of a column

	//open file for reading
	fp = fopen(INPUT_FILENAME, "r"); 

	//ignore header line
	fgets(buffer, BUFFER_BIG_SIZE, fp);

	//read line by line until end of file
	for (i = 0; i < size; i++)
	{
		fgets(buffer, BUFFER_BIG_SIZE, fp);
				
		sscanf(buffer, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],\"%[^\"]\",%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^\n]\n", 
			tokens[0], tokens[1], tokens[2], tokens[3], tokens[4], tokens[5], tokens[6], 
			tokens[7], tokens[8], tokens[9], tokens[10], tokens[11], tokens[12], tokens[13], 
			tokens[14], tokens[15], tokens[16], tokens[17], tokens[18], tokens[19], tokens[20], tokens[21]);
										
		strcpy(dataArray[lines].time, tokens[0]);
				
		if (strcmp(tokens[1], "") == 0)
		{//empty data
			dataArray[lines].latitude = 0;
		}else{
			sscanf(tokens[1], "%lf", &dataArray[lines].latitude);
		}
				
		if (strcmp(tokens[2], "") == 0)
		{//empty data
			dataArray[lines].longitude = 0;
		}else{
			sscanf(tokens[2], "%lf", &dataArray[lines].longitude);
		}
				
		if (strcmp(tokens[3], "") == 0)
		{//empty data
			dataArray[lines].depth = 0;
		}else{
			sscanf(tokens[3], "%lf", &dataArray[lines].depth);
		}
				
		if (strcmp(tokens[4], "") == 0)
		{//empty data
			dataArray[lines].mag = 0;
		}else{
			sscanf(tokens[4], "%lf", &dataArray[lines].mag);
		}
							
		strcpy(dataArray[lines].magType, tokens[5]);				
			
		if (strcmp(tokens[6], "") == 0)
		{//empty data
			dataArray[lines].nst = 0;
		}else{
			sscanf(tokens[6], "%d", &dataArray[lines].nst);
		}
				
		if (strcmp(tokens[7], "") == 0)
		{//empty data
			dataArray[lines].gap = 0;
		}else{
			sscanf(tokens[7], "%lf", &dataArray[lines].gap);
		}
				
		if (strcmp(tokens[8], "") == 0)
		{//empty data
			dataArray[lines].dmin = 0;
		}else{
			sscanf(tokens[8], "%lf", &dataArray[lines].dmin);
		}
				
		if (strcmp(tokens[9], "") == 0)
		{//empty data
			dataArray[lines].rms = 0;
		}else{
			sscanf(tokens[9], "%lf", &dataArray[lines].rms);
		}
								
		strcpy(dataArray[lines].net, tokens[10]);
							
		strcpy(dataArray[lines].id, tokens[11]);
								
		strcpy(dataArray[lines].updated, tokens[12]);
							
		strcpy(dataArray[lines].place, tokens[13]);
							
		strcpy(dataArray[lines].type, tokens[14]);
							
		if (strcmp(tokens[15], "") == 0)
		{//empty data
			dataArray[lines].horizontalError = 0;
		}else{
			sscanf(tokens[15], "%lf", &dataArray[lines].horizontalError);
		}
								
		if (strcmp(tokens[16], "") == 0)
		{//empty data
			dataArray[lines].depthError = 0;
		}else{
			sscanf(tokens[16], "%lf", &dataArray[lines].depthError);
		}
								
		if (strcmp(tokens[17], "") == 0)
		{//empty data
			dataArray[lines].magError = 0;
		}else{
			sscanf(tokens[17], "%lf", &dataArray[lines].magError);
		}
								
		if (strcmp(tokens[18], "") == 0)
		{//empty data
			dataArray[lines].magNst = 0;
		}else{
			sscanf(tokens[18], "%d", &dataArray[lines].magNst);
		}
								
		strcpy(dataArray[lines].status, tokens[19]);
					
		strcpy(dataArray[lines].locationSource, tokens[20]);	
							
		strcpy(dataArray[lines].magSource, tokens[21]);				

		lines = lines + 1;
	}

	//close file
	fclose(fp);
}

/*bubble sort on partial array*/
void bubleSort(EarthQuake* dataArray, int from, int to){

	//printf("Call bubble sort from %d to %d\n", from, to);

	int i, j; //loop control variables

	for (i = from; i < to; i++)
	{
		for (j = i + 1; j <= to; j++)
		{
			if (dataArray[i].latitude > dataArray[j].latitude)
			{
				//swap
				EarthQuake temp = dataArray[i];
				dataArray[i] = dataArray[j];
				dataArray[j] = temp;
			}
		}
	}
}


void sort(EarthQuake* dataArray, int from, int to, int numProcesses){
	
	int i; //loop control variable
	int status; //wait status
	int left, right; //location of subarray to copy data

	if (from >= to)
	{
		return;
	}

	if (numProcesses == 1)
	{
		bubleSort(dataArray, from, to);
	}else{

		//divide

		//child process ids
		pid_t childPIDs[2];

		//middle position of array
		int middle = (to + from) / 2;

		//two children
		for (i = 0; i < 2; i++)
		{
			childPIDs[i] = fork(); 
			if (childPIDs[i] >= 0) // fork was successful 
			{
				if (childPIDs[i] == 0) // first child process 
				{
					if (i == 0)
					{
						sort(dataArray, from, middle, numProcesses / 2);
					}else{
						sort(dataArray, middle + 1, to, numProcesses / 2);
					}	

					//printf("\nChild %d exit\n", i); 

					exit(0);
				}

			}else // fork failed 
			{ 
				printf("\nFork failed!\n"); 
				exit(0);
			} 
		}
		
		//merge
		//parent will merge two half sorted array
		wait(&status);
		wait(&status);

		//printf("\nParent got: child exit\n"); 

		//merge
		//create array
		EarthQuake* tempDataArray = (EarthQuake*)malloc((to - from + 1) * sizeof(EarthQuake));
		i = 0;
		left = from;
		right = middle + 1;

		while (left <= middle && right <= to)
		{			
			if (dataArray[left].latitude <= dataArray[right].latitude)
			{					
				tempDataArray[i++] = dataArray[left++];
			}else{
				tempDataArray[i++] = dataArray[right++];
			}
			
		}
		while (left <= middle)
		{
			tempDataArray[i++] = dataArray[left++];
		}

		while (right <= to)
		{
			tempDataArray[i++] = dataArray[right++];
		}

		//copy back to data
		for (i = from; i <= to; i++)
		{
			dataArray[i] = tempDataArray[i - from];
		}

		//free resource
		free(tempDataArray);
	}	
}


int main(){

	//number of child processes
	int numProcesses = 2;

	int i; //loop control variable

	//time to sort
	struct timespec begintime; 
	struct timespec endtime; 

	EarthQuake* dataArray; //array of pointers (each pointer to the EarthQuake structure)
	int numLines = readNumLines();

	dataArray = (EarthQuake*)mmap(NULL, numLines * sizeof(EarthQuake), PROT_READ | PROT_WRITE, 
               MAP_SHARED | MAP_ANONYMOUS, -1, 0);

	//can create array?
	if (dataArray == NULL)
	{
		printf("Error while allocate the memory.\n");
		exit(EXIT_FAILURE);
	}

	printf("Please enter number of processes: 2, 4, 8,...: ");
	scanf("%d", &numProcesses);

	//load data
	loadData(dataArray, numLines);

	clock_gettime(CLOCK_REALTIME, &begintime);

	/*sort*/
	sort(dataArray, 0, numLines - 1, numProcesses);

	clock_gettime(CLOCK_REALTIME, &endtime);
	if (endtime.tv_nsec < begintime.tv_nsec) {
        endtime.tv_nsec += 1000000000;
        endtime.tv_sec--;
    }

	 printf("Elapsed time: %ld.%09ld\n", (long)(endtime.tv_sec - begintime.tv_sec),
        endtime.tv_nsec - begintime.tv_nsec);

	/*testing ONLY
	for (i = 0; i < 20; i++)
	{
		printf("%lf %s\n", dataArray[i].latitude, dataArray[i].time);
	}*/

    munmap(dataArray, numLines * sizeof(EarthQuake));
	return 0;
}
