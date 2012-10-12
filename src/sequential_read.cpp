#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sstream>
#include <fstream>
#include <unistd.h>

using namespace std;

#define FILENAME "rand_no"

int main(int argc, char* argv[])
{
	/*All variable declarations */
	time_t startTime, endTime;
	startTime = time(NULL);
	
	stringstream bufferStream;
	ifstream fp;
	int readNo;
	
	double sum = 0;
	int count = 0; 
	
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

	while( fp >> readNo )
	{
		sum += (double)readNo;
		count++;
	}
	double avg = sum/count;
	cout<<"Sum is "<<sum<<"\tand count is: "<<count;
/*
 * Added precision and fixed to see the difference between averages.
 * Average is more or less same for a random distribution, so need to 
 * increase precision
 */
 	endTime = time(NULL);
	cout<<"\nTime Taken to execute the complete code in secs = "<<difftime(endTime, startTime)<<"\n";
	
 	cout.precision(16);
	cout<<"\nAverage is: "<<fixed<<avg<<"\n";
	
	fp.close();
	return (0);
}
