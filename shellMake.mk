CC=g++-4.8
CXXFLAGS=-std=c++11

Shell: Shell.cpp
	$(CC) Shell.cpp -o Shell $(CXXFLAGS)
	
clean:
	rm *.o
	rm Shell
