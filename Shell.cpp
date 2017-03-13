#include <iostream>
#include <iomanip>
#include <chrono>
#include <ctime>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void shell();
char** getCommand();
double forkAndExecute(char **, int);


int main(int argc, char** pArgv)
{
	/*I'll have a loop running that will read input from the command line */
	shell();

	return 0;
}


/*this function will get the command the user entered in the command line and split it up into the elements of
a char array and return the char array so that other functions can easily distinguish between the arguments. */
char** getCommand()
{
	/*I'm going to use getline to read the command the user wants, I will then convert the string to a const char* with
	the function c_str(). I will then make a copy of that data into a normal cstring and parse it into the separate
	arguments that I will use for actually executing the command the user wants*/
	std::string command;
	command.clear();

	//use getline to get the command
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
		parsedArgs[index] = args; //this won't work, get a seg fault.
		index++;
		args = strtok(NULL, " , \n");

    }

    //return the tokenized cstring so that
    return parsedArgs;
}

/*This is the fork and execute function, it will fork and see if it was successful, if successful execlp
will be called and the arguments the user entered will attempt to execute. If an error occurs anywhere
the function will return a -1 and then the loop will continue to run.*/
double forkAndExecute(char** parsedArgs, int index)
{
    //create a process id variable
    pid_t pid;
	
	//these are the variables to keep track of how long we are waiting on child processes
	std::chrono::time_point<std::chrono::system_clock> start, end;

    //fork and see what it returns
    pid = fork();

    /*if pid == 0 then this is the child process and we'll use execlp with the parsedArgs
    if pid < 0 then an error occurred, otherwise we are in the parent process and we'll
    wait for the child process to finish running.*/
    if(pid < 0) //error occurred
    {
        printf("Error occurred when forking. \n");

        //return a negative 1 to indicate that the error occurred and we can prompt the user again
        return 0.0;
    }
    else if (pid == 0)
    {
		
        //pass in the arguments to execlp
        if(execvp(parsedArgs[index], parsedArgs) == -1) //an error occurred
        {
            printf("An error occurred with execlp, unrecognized command. \n");
			return -1.0;
        }
      

    }
    else //parent process
    {
		
		//start the clock to record time 
		start = std::chrono::system_clock::now();
		
		//wait for the child proccess to finish
        wait(NULL);
		
		
    }
	
	//record the end time after waiting for the child process
	end = std::chrono::system_clock::now();
	
	std::chrono::duration<double> elapsedTime = end - start;
	double timeSpent = (double)elapsedTime.count();
	
    return timeSpent; //the time spent waiting on processes
}


/*this function will present the user with the line [cmd]: and then will continue looping until the user enters
a command and any modifications to the command such as ls -a, etc. */
void shell()
{
	
	//this is a sum variable to keep track of how long we are waiting on child processes
	double totalTime = 0.0;
	double thisTime = 0.0;
	
	bool status = true; //set it to true to start the loop

	//const to compare with the parsed arguments
	const char* exit = "exit";
	const char* ptime = "ptime";
	const char* history = "history";
	const char* ctrl = "^";
	const char* blank = "";
	
	char **parsedArgs = (char**)malloc(sizeof(char*));
	char **savedCmds; 
	
	int histIndex = 0;
	int parsedIndex;
	int arg;
	
	//I'll use a loop to keep the [cmd]: prompting the user
	while(status)
	{
		
		printf("[cmd]: ");
		parsedArgs = getCommand();
		
		//set up the history array
		parsedIndex = 0;
		while(parsedArgs[parsedIndex] != NULL)
		{
			if((strcmp(parsedArgs[parsedIndex], ctrl)) == 0 && parsedArgs[parsedIndex+1] != NULL)
			{
				parsedIndex += 2; //this make it so i dont store the ^ commands
			}
			else{
				savedCmds[histIndex] = parsedArgs[parsedIndex]; //copying the contents
				parsedIndex++;
				histIndex++;
			}
			
			
		}
		
		
		//check if the user entered 'exit'
		if(strcmp(parsedArgs[0], exit) == 0)
		{
			status = false; //exit the loop
		}
		else if(strcmp(parsedArgs[0], ptime) == 0)
		{
			std::cout << "Time spent on child processes: " << std::fixed << totalTime << " seconds. \n Or " << totalTime*1000 
				<< " milliseconds or " << totalTime*1000000 << " microseconds. \n"; 
			status = true;
		}
		else if(strcmp(parsedArgs[0], history) == 0)
		{
			//display the history of the commands
			for(int i = 0; i < histIndex; i++)
			{
				/*if((strcmp(savedCmds[i], ctrl)) == 0 && savedCmds[i+1] != NULL)
				{
					std::cout << savedCmds[i] << savedCmds[i+1] << "\n";
				}
				else*/
					std::cout << savedCmds[i] << "\n";
			}
		} 
		else if(strcmp(parsedArgs[0], ctrl) == 0 && parsedArgs[1] != NULL)
		{
			
			arg = atoi(parsedArgs[1]); //convert the argument to an int to fork and execute the right argument

			if(arg - 1 >= 0)
			{
				/*I was having trouble getting this to work with any add ons to the command, such as "ls -alt", 
				the trouble was getting the -alt to come along but nothing else. I either use this very specific example
				or I was bringing along the next element in the savedCmds array which isn't what I wanted. This way,
				the command that you want will be able to be executed just without add ons. I also ran out of time
				otherwise I would've figured it out*/
				parsedArgs[0] = savedCmds[arg-1]; //minus 1 since it is on a 1 based scale
				
				parsedArgs[1] = NULL;
				
				thisTime = forkAndExecute(parsedArgs, 0); 
			
				//see if we encountered an error
				if(thisTime == -1.0)
				{
					break; //kick out and read the command again
					
				}
				else
					totalTime += thisTime; //total amount of time
				
			}
			else
			{
				std::cout << "Command not recognized \n";
			}
			
			//continue the loop if that statement fails
			
		}
		else //they chose none of the made up commands 
		{
			thisTime = forkAndExecute(parsedArgs, 0);
			
			//see if we encountered an error
			if(thisTime == -1.0)
			{
				break; //kick out and read the command again
				
			}
			else
				totalTime += thisTime; //total amount of time
			
		}

	}
	
	free(parsedArgs); //free up the memory we used malloc to create


}
