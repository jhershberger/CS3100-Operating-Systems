CC=g++-4.8
CXXFLAGS=-std=c++11 -O3 

Anomaly: Anomaly.cpp
	 $(CC) Anomaly.cpp -o Anomaly $(CXXFLAGS)
	 
clean: 
	rm Anomaly