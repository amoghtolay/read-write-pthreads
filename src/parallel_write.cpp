#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sstream>
#include <fstream>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

using namespace std;

/*Defining Macros */
#define NUM_ENTRIES 1000000000
#define FILENAME "rand_no"

/*GLOBAL VARIABLES */
/*Creating MUTEX*/
pthread_mutex_t bufferMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t completeFlagMutex = PTHREAD_MUTEX_INITIALIZER;

stringstream bufferStream;
/*
 * These integers represent the no. of times each of these threads switches control (to obtain mutex)
 * ie. the no. of times writing operation is performed and the no of times string is generated
 */
int thread1=0;
int thread2=0;

ofstream fp;
int temp = 1;

/*
 * Variable to store whether or not the thread1 execution is complete.
 * Mutex contention issue is thus resolved because now only main contests
 * to obtain the mutex only after completion of thread1
 */ 
bool isCompleteThread1 = false;

long count = NUM_ENTRIES;

/*
 * This thread generates random numbers and stores them into a buffer.
 */
 
void *generateString(void *threadid)
{
   int randNo = 0;
   while(count>0)
   {
      thread1++;
	   /*generate random no. */
	   randNo = rand();
	   /*
	    * Lock the mutex to update stream buffer, write random no to stream buffer and then unlock mutex
	    */
	   pthread_mutex_lock( &bufferMutex );
	   bufferStream << randNo <<"\n";
	   pthread_mutex_unlock( &bufferMutex );

	   count--;
	}
	/*
	 * When control steps outside the above while loop, exit
	 */
	pthread_exit(NULL);
}

/*
 * This reads numbers from the buffer and writes them to file
 */
void *writeString(void *threadid)
{
	while(1)
	{
		/*
		 * The following are boolean variables
		 * isFinishWrite is equated to the flag variable coming from 
		 * main (Globally defined) which indicates completion of 
		 * thread1 execution 
		 * isEmptyBuffer is to check if the stream is empty, if yes,
		 * then loop again
		 */
		bool isFinishWrite = false;
		bool isEmptyBuffer = true;
		
		/*
		 * Obtain mutex lock on buffer, to check if buffer is empty or not
		 * assign true if it is empty
		 */
		pthread_mutex_lock( &bufferMutex );
		if(bufferStream.rdbuf()->in_avail() == 0)
			isEmptyBuffer = true;
		else isEmptyBuffer = false;
		pthread_mutex_unlock( &bufferMutex );
		
		/*
		 * Obtain mutex on count mutex, if count<=0, then assign false
		 */
		pthread_mutex_lock( &completeFlagMutex );
		isFinishWrite = isCompleteThread1;
		pthread_mutex_unlock( &completeFlagMutex );
		/*
		 * If buffer is empty as well as count is 0, then exit
		 */
		if(isEmptyBuffer)
			if(isFinishWrite)
				pthread_exit(NULL);
		
		/*
		* if buffer is not empty, irrespective of whether count = 0 or not, 
		* just obtain buffer mutex and commit to file
		*/
		if(!isEmptyBuffer)
		{
			pthread_mutex_lock( &bufferMutex );
			fp << bufferStream.str();
			bufferStream.str(std::string());
			pthread_mutex_unlock( &bufferMutex );
			thread2++;
		}	
		/*
		 * The loop ends here. This loop will continue until it recieves
		 * the flag from the main function that the execution of thread2
		 * is complete
		*/
	}
	pthread_exit(NULL);
}

int main(int argc, char* argv[])
{
	/*All variable declarations */
	time_t startTime, endTime;
	startTime = time(NULL);
	int returnJoinValueThread1;
	
	char filename[256] = FILENAME;
	if(argc>=2)
		strcpy(filename, argv[1]);
	
	/*File opened here*/
	fp.open(filename);
	if(!fp)
	{
		cout<<"FILE COULD NOT BE OPENED \n";
		exit(EXIT_FAILURE);
	}

	/* initialize random seed*/
	srand ( time(NULL) );

	/*Opening and creating threads here */
	pthread_t threads[2];
	
	/*Creating and calling the thread */
	int t=0;
	int threadReturn1;
	threadReturn1 = pthread_create(&threads[0], NULL, generateString, (void *)t);
	if (threadReturn1)
    {
		printf("ERROR; return code from pthread_create() is %d\n", threadReturn1);
        exit(EXIT_FAILURE);
    }
	
	/*Thread2*/
	t=1;
	int threadReturn2;
	threadReturn2 = pthread_create(&threads[1], NULL, writeString, (void *)t);
    if (threadReturn2)
    {
         printf("ERROR; return code from pthread_create() is %d\n", threadReturn2);
         exit(EXIT_FAILURE);
    }

	returnJoinValueThread1 = pthread_join( threads[0], NULL);
	
	if( count<=0 && returnJoinValueThread1 == 0 )
	{
		pthread_mutex_lock( &completeFlagMutex );
		isCompleteThread1 = true;
		pthread_mutex_unlock( &completeFlagMutex );
	}
	pthread_join( threads[1], NULL);
	
	endTime = time(NULL); 
	cout<<"Thread1 = "<<thread1<<"\nThread2 = "<<thread2<<"\n";
	cout<<"Time Taken to execute the complete code in secs = "<<endTime - startTime<<"\n";
	fp.close();
return 0;
}
