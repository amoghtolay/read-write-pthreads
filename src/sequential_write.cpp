#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sstream>
#include <fstream>
#include <unistd.h>

using namespace std;
/* Defining MACROS */
/*
 * NUM_ENTRIES defines the no. of entries that have to be written in 
 * the file
 * NUM_WRITE_BUF defines the no. of random nos to be updated to the
 * intermidiate temporary buffer string
 */ 
#define NUM_ENTRIES 1000000000
/*
 * The following macro (NUM_WRITE_BUF) should always be a proper divisor
 * of NUM_ENTRIES. This is because is the buffer is written an integer
 * number of times, so NUM_ENTRIES/NUM_WRITE_BUF cant be a fraction
 */
#define NUM_WRITE_BUF 100000

#define FILENAME "rand_no"


int main(int argc, char* argv[])
{
	/*All variable declarations */
	time_t startTime, endTime;
	startTime = time(NULL);
	stringstream bufferStream;
	ofstream fp;
	int randNo;

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
	for(int j=NUM_ENTRIES; j>0; j-=NUM_WRITE_BUF)
	{
		for(int i=NUM_WRITE_BUF; i>0; i--)
		{
			randNo = rand();
			bufferStream << randNo << "\n";
		}
		fp << bufferStream.str();
		bufferStream.str(std::string());
	}
	endTime = time(NULL);
	cout<<"Time Taken to execute the complete code in secs = "<<endTime - startTime<<"\n";
	fp.close();
	return (0);
}
