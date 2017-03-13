#include<stdio.h>
#include<queue>
#include<ctime>
#include<cstdlib>


int main(int argc, char** pArgv)
{
	
	//create variable to keep track of the number of faults and anomalies
	int faults, anomaly;
	
	//create an array for the results that will hold the number of page faults for each frame size
	int results[2][100];
	
	//initialize the array to 0's
	for(int res = 0; res < 100; res++)
	{
		results[0][res] = 0;
		results[1][res] = 0;
	}

	int pageSize = 1000;
	int fifo; //index for frame
	srand(time(NULL)); //seed so it will generate a different random num each time
	
	//initialization of key variables
	int sequence = 1;
	fifo = 0;
	faults = 0;
	anomaly = 0;
	
	//create the array variables to simulate the anomaly
	int page[pageSize];
	int frame[100];
	
	//print out a greeting and explanation to the user
	printf("---Belady's Anomaly Simulator--- \n");
	printf("# of sequences tested: 100 \n");
	printf("Length of memory reference string: 1000 \n");
	printf("Frames of physical memory: 100 \n");
	
	
	//set up a loop to do 100 sequences
	for(int seq = 0; seq < 100; seq++)
	{	
		//initialize the frame array 1-100
		for(int i = 0; i < 100; i++)
		{
			frame[i] = i+1;
		}
		
		//iterate through the array to set it up 
		for(int j=0; j < pageSize; j++)
		{
			page[j] = rand() % 250 + 1; //random page from 1-250

		}
		
		/*for each sequence search the frame array for all 1000 elements of the page array,
		this will occur 100 times as there are 100 sequences, each time with 
		a different page array*/
		for(int p = 0; p < 1000; p++)
		{
			//check for a page fault 
			if(frame[seq] != page[p])
			{
				//page fault
				faults++;
				results[1][seq] = faults; //record the number of page faults at each frame size
				frame[fifo] = page[p]; //replace the first in element with the page that caused a fault
				fifo++; //update the first in index
				
			}
			else
			{
				faults = 0; //reset the faults to keep the records accurate for each frame size
			}
			
		
			//if the fifo reaches a point that is unrealistic then reset it	
			if(fifo == 100)
			{
				//reset fifo back to 0 
				fifo = 0;
			}	
			
		}
		
		//check for anomalies
		if((results[1][seq] - results[1][seq-1]) > 200 && seq > 1 && results[1][seq] > 500)
		{
			anomaly++;
			printf("Anomaly found! \n");
			printf("Sequence: %i \n", sequence);
			printf("Page Faults: %i ", results[1][seq]);
			printf("at frame size: %i \n", seq + 1);
		}
		
		//count the sequences
		sequence++;
	}
	
	//print out how many anomalies occurred
	printf("Anomaly detected: %i ", anomaly);
	printf("times \n");
	
	return 0;
}

