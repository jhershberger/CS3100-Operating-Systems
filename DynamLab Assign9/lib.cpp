/* These are the functions for the library to be dynamically loaded from main, 
extern c allows the program to be compiled in C or C++ and avoids name mangling */ 

extern "C"

//this is the recursive way to find the nth fibonacci number
int fibCalc(int n)
{
	//base case
	if(n == 0)
		return 0;
	else if(n == 1)
		return 1;
	else //recursion
		return fibCalc(n-1) + fibCalc(n-2);
}

extern "C"

//this is the e estimate function, it will use the taylor series
//1 + 1/1! + 1/2! ... + 1/n!
double eCalc(int n)
{
	double factorial = 1;
	double e = 1;
	
	//set up a loop to calculate the iterations
	for(int i = 1; i < n; i++)
	{
		factorial = factorial * i;
		e = e + 1/factorial;
	}
	return e;
}

extern "C"

/*This is the nth digit of pi computation function,it will use the series 
4-4/3+4/5-...+4/(2i+1), it will then estimate pi to the nth digit */
double piCalc(int n)
{
	double pi = 4.0;
	int oscillate = -1; //this variable will oscillate the sign of the series term
	int iterations = 100; //this is how many iterations we will do to calculate pi
	double seriesTotal = 0.0;
	double seriesTerm;

	//set up the loop to calculate pi
	for(int i = 0; i < iterations; i++)
	{
		oscillate *= -1;
		seriesTerm = oscillate/double((2*i + 1));
		seriesTotal += seriesTerm; //keeps a running total
		
	} 

	
		//calculate pi
		pi = pi * seriesTotal;	
	
	return pi;
	
}


