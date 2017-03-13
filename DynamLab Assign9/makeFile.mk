CC=g++-4.8
CXXFLAGS=-std=c++11 -ldl -lm 

Main: main.o formula-calc 
	 $(CC) -o program main.o -L'pwd' -Wl,-rpath,'pwd' $(CXXFLAGS)

	 
main.o: main.cpp
	$(CC) -c main.cpp 
	
formula-calc: lib.cpp
	gcc -fPIC -c lib.cpp
	gcc -shared -o formula-calc.so lib.cpp


clean: 
	rm *.o 
	rm formula-calc.so* 
	rm Main
	