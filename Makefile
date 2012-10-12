all:
	g++ -Wall src/sequential_write.cpp -o sequentialWrite
	g++ -Wall -pthread src/parallel_write.cpp -o parallelWrite
	g++ -Wall -pthread src/parallel_read.cpp -o parallelRead
	g++ -Wall src/sequential_read.cpp -o sequentialRead
clean:
	rm -rf sequentialRead parallelRead sequentialWrite parallelWrite
