#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cstdlib>
#include <cmath>
#include <dlfcn.h>
#include <iomanip>

/*this function will simulate a command line*/
void shell();

/*this function will parse the arguments from the simulated command line */
char** getCommand();

/*this is the main function, it will dynamically load the fib, pi, e
functions from the library formula-calc.so*/
int main(int argc, char** pArgv)
{
	//call the shell function
	shell();
	
	return 0;
}

/*this function will be set up almost identical to the shell
program except it will dynamically load the functions */
void shell()
{
	/*simulate a command line that reads which formula the user
	wants to calculate*/
	
	//load formula-calc.so so in the loop we can run the functions we need
	void* lib_handle;
	
	//create variable for the formula calculations
	int fibC;
	double eC;
	double piC;
		
	//set up a bool that will exit the loop when set to false
	bool status = true; //true so the loop starts
	
	//create const char* to represent the commands: fib, e, pi, and quit
	const char* fib = "fib";
	const char* e = "e";
	const char* pi = "pi";
	const char* quit = "quit";
	
	//create a char** for the parsed arguments
	char **parsedArgs = (char**)malloc(sizeof(char*));
	
	//start the loop
	while(status)
	{
		std::cout << "[cmd]: ";
		parsedArgs = getCommand();
		
		//load the library
		lib_handle = dlopen("./formula-calc.so", RTLD_LAZY);
		
		//error check
		if(!lib_handle)
		{
			printf("Couldn't open formula-calc.so! The program will now close. \n");
			
			//exit the program
			exit(1);
		}
		
		/*check for fib, pi, or e. If either of them are there then 
		load the function for it*/
		if(strcmp(parsedArgs[0], fib) == 0)
		{
			//create a variable for the calculation and load fibCalc from formula-calc.so
			int (*function)(int) = (int (*)(int))dlsym(lib_handle, "fibCalc");
			
			if (dlerror() != NULL)
			{
				 std::cout << "Couldn't find 'fibCalc'" << std::endl;
				 break;
			}
			
			//create the variable for the fibonacci sequence number
			int seq = atoi(parsedArgs[1]);
			
			//n has to be within the 0-14 range
			if(seq < 0 || seq > 14)
			{
				//let the user know it was invalid and break out 
				std::cout << "Invalid entry, the range for fibCalc is 0-14. \n";
	
			}
			else
			{
				//update fibC
				fibC = function(seq);
				
				//display the answer
				std::cout << "Answer: " << fibC << std::endl;
			}
			
			
			
		}
		else if(strcmp(parsedArgs[0], e) == 0)
		{
			//create a variable for the calculation and load eCalc
			double (*function1)(int) = (double (*)(int))dlsym(lib_handle, "eCalc");
			
			if (dlerror() != NULL)
			{
				 std::cout << "Couldn't find 'eCalc'" << std::endl;
				 break;
			}
			
			//create the nth iteration of e variable
			int n = atoi(parsedArgs[1]);
			
			if(n < 1 || n > 30)
			{
				std::cout << "Invalid range. The range for eCalc is 1-30. \n";
				
			}
			else 
			{
				//update eC
				eC = function1(n);
				
				//display the answer
				std::cout << "Answer: " << std::setprecision(10) << eC << std::endl;
			}
			
			
		}
		else if(strcmp(parsedArgs[0], pi) == 0)
		{
			//create a variable for the calculation and load piCalc
			double (*function2)(int) = (double(*)(int))dlsym(lib_handle, "piCalc");
			
			if (dlerror() != NULL)
			{
				 std::cout << "Couldn't find 'piCalc'" << std::endl;
				 break;
			}
			
			//create the nth digit of pi variable
			int dig = atoi(parsedArgs[1]);
			
			if(dig < 1 || dig > 10)
			{
				std::cout << "Invalid range. The range for piCalc is 1-10. \n";
				
			}
			else 
			{
				//update eC
				piC = function2(dig);
				
				//display the answer
				std::cout << "Answer: " << std::setprecision(dig) << piC << std::endl;
			}
			
			
		}
		else if(strcmp(parsedArgs[0], quit) == 0)
		{
			//exit the program 
			status = false;
			
		}
		else
		{
			//this means the command isn't recognized
			std::cout << "Unrecognized command. Recognized commands are fib, e, pi, and quit. \n";
			status = true;
		}
		
		dlclose(lib_handle);
		
	}
}

/*this is the getCommand function, it will get the command from the simulated
command line and the parse it and return the cstring array.*/
char** getCommand()
{
	/*Ill use getline to get the command from the user, ill use strtok to 
	tokenize the command. My process is exactly like the shell program. */
	std::string command; 
	command.clear();
	
	//use getline 
	std::getline(std::cin, command);
	
	if(command.empty() == true)
	{
		/*if the user enters a blank command, it will show up as an underscore in the history so they 
		can tell a blank command was entered*/
		command = "_"; 
	}
	
	
    //This is me converting the string to a const char
    const char *c = command.c_str();

    //make a copy of c so we can parse it
    char *cmd = strdup(c);

    /*Im going to use strtok to parse my arguments, I learned about strtok first by googling parsing arguments and then
    I read more about it on cplusplus.com. It will separate the arguments when it comes across a white space */
    char *args = strtok(cmd, " ");

    /*this is an array of the cstrings that we are tokenizing, this will allow us to access the different arguments
    to send to our actual execution function. I had to look up how to set it up because i kept getting a segfault so
    i figured out i had to use malloc */
    char** parsedArgs = (char**)malloc(sizeof(char*));

    //this is the index I'll use to iterate through the parsedArgs array
    int index = 0;

    /*this loop will set up each element of the parsed arguments array by going through and
    tokenizing each separate phrase that the user entered into the command line*/
    while(args != NULL)
    {
		parsedArgs[index] = args; 
		index++;
		args = strtok(NULL, " , \n");

    }

    //return the tokenized cstring 
    return parsedArgs;
	
	
}

