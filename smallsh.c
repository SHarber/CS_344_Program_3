/*******************************************************************************************************
 * Author:			Sarah Harber
 * Last Update:		8/6/17
 * Description: 	Create a shell in C that will run command line instructions.
 * Specifications:	- Shell will support 3 built in commands: exit, cd, status.
 * 					- Comments will begin with '#'.
 * 					- If it is a background process & will be the last character of the command.
 * 					- A colon is the symbol as a prompt for the command line.
 * 					- Shell will allow for redirection of standard input & standard output
 * 					- Shell will support both foreground and background processes.
 * 					
 ******************************************************************************************************/

// Set Header Files
	#include <signal.h>
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include <unistd.h>
	#include <sys/stat.h>
	#include <fcntl.h>
	#include <sys/wait.h>
	#include <sys/types.h>

// Declare Neccesary Variables
	int Debug = 0; 				// Debugging Variable.
	char buffer[2048]; 			// Character Array Buffer
	char *b = buffer; 			// Pointer to Buffer
	size_t bufsize = 512; 		// Size_t of Buffer size
	size_t arguments; 			// Arguments temp holds buffer
	int toexit = 0; 			// Int Variable to exit.
	pid_t bgpid[100]; 			// An array to hold process ids of background processes.
	int num_bg_Processes = 0; 	// Number of background Processes.  
	pid_t lastFGProcessPID; 	// Variable to hold last fg process.
	char * args[512]; 			// Variable to store arguments;
	int numberOfArguments; 		// Variable to store the current number of arguments.
	int status= 0;					// Variable to hold status of foreground process.
// Declare function definitions
	void getInput(); 			// Display command line & get input.
	void builtInCommands(); 	// Check to see if command is built in & execut if it is.
	void otherCommands(); 		// All other Commands will be handled. 

/****************************************************************
 *			  			 MAIN FUNCTION
 ***************************************************************/
 int main()
 {
	while(toexit == 0)
	{
		getInput();
		builtInCommands();
	}
 }

 /******************************************************************************************************
 *				 					Define Functions Declared Above
 ******************************************************************************************************/


/****************************************************************
 * Function Name: 	getInput
 * Description: 	Function to display the command line and
 *					get user input.
 * Specifications:	If user inputs a comment (line that begins
 * 					with '#' or a blank line, the command line
 * 					will reprompt the user for a new command.
 **************************************************************/
void getInput()
{
	// Create do loop to loop while user is inputting a comment or a blank line.
	do{
		// Clear buffer
			memset(buffer, '\0', sizeof(buffer));

		// Flush out prompt
			fflush(stdin);
			fflush(stdout);

		// Print Command Line Prompt
			printf(": ");

		// Flush out stdin again.
			fflush(stdin);

		// Get user's input into promptInput
			arguments = getline(&b,&bufsize,stdin);
		
		// Set Debug Test Prints
	
	}while(buffer[0] == '\n' || buffer[0] == '#');

	// Split buffer into tokens & store in arg 
		char * token; // Set char pointer to hold tokens temporarily.
		token = strtok(buffer, " \n"); // Get First Token (Will not be null)
		numberOfArguments = 0; // Reset number of Arguments to 0.

	// While There are still tokens, store the current token and get the next.	
	while(token != NULL && numberOfArguments < 512)
	{
		args[numberOfArguments] = token;
		if(Debug ==1)
			printf("token %d: %s \n", numberOfArguments, args[0]);
		numberOfArguments++;
		token=strtok(NULL, "  \n");
	}	


}


/***************************************************************
 * Function Name: 	builtInCommands
 * Description: 	Function to handle 3 built in commands. 
 * Specifications:	Fuction will handle exit, status and cd.  
 * 					Exit will exit the shell. cd will change
 * 					directories.  Without arguments, it will
 * 					change to the HOME environment.  Status
 * 					will print out either the exit status or
 * 					the terminating signal of the last 
 * 					foreground process.
 **************************************************************/
void builtInCommands()
{	
	int errorTest; // Set error test variable	


	if(strcmp(args[0], "exit") == 0) // If user entered "exit"
	{
		toexit=1; // Change exit status of toexit to 1.		
	}	
	else if (strcmp(args[0], "status") == 0)
	{
		if (WIFEXITED(status))
			printf("Exit Status: %d\n", WEXITSTATUS(status));
		else 
			printf("Terminating Signmal %d\n", status);

	}
	else if (strcmp(args[0], "cd") == 0)
	{
		// Check to see if there is only one argument
		if (numberOfArguments ==1) // If only one then go to HOME directory.		
		{
			errorTest = chdir(getenv("HOME"));
			
			// Check cd worked.
			if (errorTest == -1)
				perror("chdir");
			else if (Debug ==1) // PRINT FOR DEBUGGING
				printf("Changed to HOME directory\n");
		}

		else if (numberOfArguments == 2) // Built in command only takes 2 commands
		{
			// If a second argument then change to path directed to by user.
			errorTest = chdir(getenv(args[1]));
			
			// Check cd Worked
			if (errorTest == -1)
				perror("chdir");
			else if (Debug ==1) // PRINT FOR DEBUGGING
				printf("Changed to %s  directory\n", args[1]);

		}
		else // If more than 2 arguments
			otherCommands(); // Invalid for built in shell move to other commands

	}
	else 
		otherCommands();
}


/***************************************************************
 * Function Name: 	otherCommands
 * Description: 	Function to nonBuiltInCommands 
 * Specifications:	Fuction will handle exit, status and cd.  
 **************************************************************/
void otherCommands()
{


}


