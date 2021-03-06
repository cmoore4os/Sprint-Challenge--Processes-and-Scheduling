#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define PROMPT "lambda-shell$ "

#define MAX_TOKENS 100
#define COMMANDLINE_BUFSIZE 1024
#define DEBUG 1  // Set to 1 to turn on some debugging output, or 0 to turn off

/**
 * Parse the command line.
 *
 * YOU DON'T NEED TO MODIFY THIS!
 * (But you should study it to see how it works)
 *
 * Takes a string like "ls -la .." and breaks it down into an array of pointers
 * to strings like this:
 *
 *   args[0] ---> "ls"
 *   args[1] ---> "-la"
 *   args[2] ---> ".."
 *   args[3] ---> NULL (NULL is a pointer to address 0)
 *
 * @param str {char *} Pointer to the complete command line string.
 * @param args {char **} Pointer to an array of strings. This will hold the result.
 * @param args_count {int *} Pointer to an int that will hold the final args count.
 *
 * @returns A copy of args for convenience.
 */
char **parse_commandline(char *str, char **args, int *args_count)
{
    char *token;

    *args_count = 0;

    token = strtok(str, " \t\n\r");  // split str at these chars; this gets the first token

    while (token != NULL && *args_count < MAX_TOKENS - 1) {   // this get the rest of the tokens up to 100
        args[(*args_count)++] = token;

        token = strtok(NULL, " \t\n\r");
    }

    args[*args_count] = NULL; // sets the last arg to NULL

    return args;
}

void execProgram (char **args) { // don't understand why ** and not *

	pid_t pid; // gets a process id
	int isFinished; // used by wait to signal child process is finished

	 /* forks a child process, but throws error if not able to fork*/
	if ((pid = fork()) < 0) {
			  printf("ERROR: Unable to fork\n");
			  exit(1);
		 }
		 /* if fork() worked: execute program if there are args or throw error if no args*/
     else if (pid == 0) {
          if (execvp(*args, args) < 0) {
               printf("ERROR:Unable to run command\n");
               exit(1);
          }
     }/* for the parent: wait for child to complete  */
     else {
          while (wait(&isFinished) != pid){
			continue;  /* keeps looping over and over till child finish */
		}

     }
}

/**
 * Main
 */
int main(void)
{
    // Holds the command line the user types in
    char commandline[COMMANDLINE_BUFSIZE]; // commandline can be no more than 1024 chars

    // Holds the parsed version of the command line
    char *args[MAX_TOKENS];

    // How many command line args the user typed
    int args_count;

    // Shell loops forever (until we tell it to exit)
    while (1) {
        // Print a prompt
        printf("%s", PROMPT);
        fflush(stdout); // Force the line above to print

        // Read input from keyboard
        fgets(commandline, sizeof commandline, stdin);

        // Exit the shell on End-Of-File (CRTL-D)
        if (feof(stdin)) {
            break;
        }

        // Parse input into individual arguments
        parse_commandline(commandline, args, &args_count);

        if (args_count == 0) {
            // If the user entered no commands, do nothing
            continue;
        }

        // Exit the shell if args[0] is the built-in "exit" command
        if (strcmp(args[0], "exit") == 0) {
            break;
        }
        // do something here if the args_count != 0; maybe run command
        //int execvpe( const char * file,
             //char * const argv[],
             //char * const envp[] );
        if (args_count) {
			// add event to exec
			execProgram(args);
		}
        #if DEBUG

        // Some debugging output

        // Print out the parsed command line in args[]
        for (int i = 0; args[i] != NULL; i++) {
            printf("%d: '%s'\n", i, args[i]);
        }

        #endif
    }

    return 0;
}
