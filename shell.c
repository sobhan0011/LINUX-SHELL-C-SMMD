// C Program to design a shell in Linux
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <linux/limits.h>

#define MAX_COM 1000 // max number of letters to be supported
#define MAX_LIST 100 // max number of commands to be supported

// Clearing the shell using escape sequences
#define clear() printf("\033[H\033[J")

// Functions:
void firstWordOfFile(const char* fileAddress) {
    FILE* ptr;
    char ch;
    int sw = 0;
 
    ptr = fopen(fileAddress, "r");
 
    if (NULL == ptr)
        printf("file can't be opened \n");
 
    do {
        ch = fgetc(ptr);
        if(ch != ' ') {
            sw = 1; // If the file starts with spaces.
        }
        if (sw == 1 && ch == ' ')
            break;
        
        printf("%c", ch);

    } while (ch != EOF);
 
    fclose(ptr);
}

void mostRepeatedWordInFile(const char* fileAddress) {
    FILE* ptr;
    char* line = NULL;
    size_t len = 0;
    ssize_t read;
    ptr = fopen(fileAddress, "r");
    if (NULL == ptr)
    {
        fprintf(stderr, "file can't be opened \n");
        fclose(ptr);
        return;
    }
    
    char jomle[MAX_COM];
    char* words[MAX_COM];
    int word_counters[MAX_COM] = {0};
    char* new_word;
    int i;
    printf("%Here is most repeated word in your file:\n")
    while ((read = getline(&line, &len, ptr)) != -1) {
    	i = 0;
	    new_word = strtok(line, " ");
	    while (new_word != NULL) {
	        words[i] = new_word;
	        i++;
	        new_word = strtok(NULL, " ");
	    }
	    for (int j = 0; j < i; ++j) {
	        for (int k = j + 1; k < i; ++k) {
	            if (strcmp(words[j], words[k]) == 0) {
	                word_counters[j] += 1;
	            }
	        }
	    }
    }
    
    
    int max = 0;
    int index;
    for (int j = 0; j < i; ++j) {
        if (max < word_counters[j]) {
            max = word_counters[j];
            index = j;
        }
    }
    printf("%s", words[index]);

	fclose(ptr);
    return;
}

void space_delete(const char* fileAddress) {
    FILE* ptr;
    ptr = fopen(fileAddress, "r");
    if (NULL == ptr)
    {
        fprintf(stderr, "file can't be opened \n");
        fclose(ptr);
        return;
    }
    
    printf("Here is your file without any white spaces:/n");
    while ((read = getline(&line, &len, ptr)) != -1)
       for (i = 0; i < read; i++)
           if (line[i] != ' ' && line[i] != '/n' && line[i] != '/t')
		   		printf("%c", line[i]);
    fclose(ptr);
    return;
}

void noneCommentLines(const char* fileAddress) {
    FILE* ptr;
    ptr = fopen(fileAddress, "r");
    char* line = NULL;
    size_t len = 0;
    ssize_t read;
    int sw = 0, i = 0;

    if (NULL == ptr)
    {
        fprintf(stderr, "file can't be opened \n");
        fclose(ptr);
        return;
    }

	printf("Here are none comment lines of your file:/n");
    while ((read = getline(&line, &len, ptr)) != -1) {
    sw = 0;
       for (i = 0; i < read && sw == 0; i++) {
           if (line[i] != ' ')
           {
               sw = 1;
               break;
           }
       }
       if (sw == 1 && line[i] != '#')
            printf("%s", line);
    }

    fclose(ptr);
    return;
}

void numberOfLines(const char* fileAddress) {
    FILE* ptr;
    ptr = fopen(fileAddress, "r");
    if (NULL == ptr)
    {
        fprintf(stderr, "file can't be opened \n");
        fclose(ptr);
        return;
    }
    char* line = NULL;
    size_t len = 0;
    ssize_t read;
    int i = 0;
    while ((read = getline(&line, &len, ptr)) != -1)
       i++;
    printf("Your file has %d line(s).", i);
    fclose(ptr);
    return;
} 

void firstTenLines(const char* fileAddress) {
    FILE* ptr;
    ptr = fopen(fileAddress, "r");
    if (NULL == ptr)
    {
        fprintf(stderr, "file can't be opened \n");
        fclose(ptr);
        return;
    }
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    int i = 0;
    printf("Here are first ten lines of your file:/n");
    while ((read = getline(&line, &len, ptr)) != -1 && i++ < 10)
        printf("%s", line);
    fclose(ptr);
    return;
}

void getPath() {
   char cwd[PATH_MAX];
   if (getcwd(cwd, sizeof(cwd)) != NULL) {
       printf("Current working dir: %s\n", cwd);
   } else {
       perror("getcwd() error");
   }
}

// Help command builtin
void openHelp()
{
	puts("\n***WELCOME TO MY SHELL HELP***"
		"\nCopyright @ Suprotik Dey"
		"\n-Use the shell at your own risk..."
		"\nList of Commands supported:"
		"\n>cd"
		"\n>ls"
		"\n>exit"
		"\n>all other general commands available in UNIX shell"
		"\n>pipe handling"
		"\n>improper space handling");

	return;
}


// Greeting shell during startup
void init_shell()
{
	clear();
	printf("\n\n\n\n******************"
		"************************");
	printf("\n\n\n\t****MY SHELL****");
	printf("\n\n\t-USE AT YOUR OWN RISK-");
	printf("\n\n\n\n*******************"
		"***********************");
	char* username = getenv("USER");
	printf("\n\n\nUSER is: @%s", username);
	printf("\n");
	sleep(1);
	clear();
}

// Function to take input
int takeInput(char* str)
{
	char* buf;
	buf = readline("\n>>> ");
	if (strlen(buf) != 0) {
		add_history(buf);
		strcpy(str, buf);
		return 0;
	} 
    else
		return 1;
}

// Function to print Current Directory.
void printDir()
{
	char cwd[1024];
	getcwd(cwd, sizeof(cwd));
	printf("\nDir: %s", cwd);
}

// Function to execute builtin commands
int ownCmdHandler(char** parsed)
{
    int NoOfOwnCmds = 10, i, switchOwnArg = 0;
    char* ListOfOwnCmds[NoOfOwnCmds];
    char* username;

    ListOfOwnCmds[0] = "exit";
    ListOfOwnCmds[1] = "cd";
    ListOfOwnCmds[2] = "help";
    ListOfOwnCmds[3] = "hello";
    ListOfOwnCmds[4] = "fwf"; // First Word of a File.
    ListOfOwnCmds[5] = "mr"; // Most Repeated word.
    ListOfOwnCmds[6] = "sd"; // Space Delete.
    ListOfOwnCmds[7] = "nc"; // None Comment.
    ListOfOwnCmds[8] = "nlf"; // Number of Lines in a File.
    ListOfOwnCmds[9] = "ftl"; // First Ten Lines.

    for (i = 0; i < NoOfOwnCmds; i++) {
        if (strcmp(parsed[0], ListOfOwnCmds[i]) == 0) {
            switchOwnArg = i + 1;
            break;
        }
    }

    switch (switchOwnArg) {
        case 1:
            printf("\nGoodbye\n");
            exit(0);
        case 2:
            chdir(parsed[1]);
            return 1;
        case 3:
            openHelp();
            return 1;
        case 4:
            username = getenv("USER");
            printf("\nHello %s.\nMind that this is "
                   "not a place to play around."
                   "\nUse help to know more..\n",
                   username);
            return 1;
        case 5:
            firstWordOfFile(parsed[1]);
            return 1;
        case 6:
            mostRepeatedWordInFile(parsed[1]);
            return 1;
        case 7:
            space_delete(parsed[1]);
            return 1;
        case 8:
            noneCommentLines(parsed[1]);
            return 1;
        case 9:
            numberOfLines(parsed[1]);
            return 1;
        case 10:
            firstTenLines(parsed[1]);
            return 1;
        default:
            break;
    }

    return 0;
}


// Function where the system command is executed
void execArgs(char** parsed, int exec_flag)
{
	getPath();
	// Forking a child
	pid_t pid = fork();

	if (pid == -1) {
		printf("\nFailed forking child..");
		return;
	}
	else if (pid == 0 && exec_flag == 0) {
        ownCmdHandler(parsed);
	}
    else if (pid == 0) {
    if (execvp(parsed[0], parsed) < 0) {
        printf("\nCould not execute command..");
    }
    exit(0);
	} else {
		// waiting for child to terminate
		wait(NULL);
		return;
	}
}

// Function where the piped system commands is executed
void execArgsPiped(char** parsed, char** parsedpipe)
{
	// 0 is read end, 1 is write end
	int pipefd[2];
	pid_t p1, p2;

	if (pipe(pipefd) < 0) {
		printf("\nPipe could not be initialized");
		return;
	}
	p1 = fork();
	if (p1 < 0) {
		printf("\nCould not fork");
		return;
	}

	if (p1 == 0) {
		// Child 1 executing..
		// It only needs to write at the write end
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);

		if (execvp(parsed[0], parsed) < 0) {
			printf("\nCould not execute command 1..");
			exit(0);
		}
	} else {
		// Parent executing
		p2 = fork();

		if (p2 < 0) {
			printf("\nCould not fork");
			return;
		}

		// Child 2 executing..
		// It only needs to read at the read end
		if (p2 == 0) {
			close(pipefd[1]);
			dup2(pipefd[0], STDIN_FILENO);
			close(pipefd[0]);
			if (execvp(parsedpipe[0], parsedpipe) < 0) {
				printf("\nCould not execute command 2..");
				exit(0);
			}
		} else {
			// parent executing, waiting for two children
			wait(NULL);
			wait(NULL);
		}
	}
}

int is_our_command(char** parsed) {
    int no_of_own_cmds = 10, i, switchOwnArg = 0;
    char* ListOfOwnCmds[no_of_own_cmds];

    ListOfOwnCmds[0] = "exit";
    ListOfOwnCmds[1] = "cd";
    ListOfOwnCmds[2] = "help";
    ListOfOwnCmds[3] = "hello";
    ListOfOwnCmds[4] = "fwf"; // First Word of a File.
    ListOfOwnCmds[5] = "mr"; // Most Repeated word.
    ListOfOwnCmds[6] = "sd"; // Space Delete.
    ListOfOwnCmds[7] = "nc"; // None Comment.
    ListOfOwnCmds[8] = "nlf"; // Number of Lines in a File.
    ListOfOwnCmds[9] = "ftl"; // First Ten Lines.

    for (i = 0; i < no_of_own_cmds; i++) {
        if (strcmp(parsed[0], ListOfOwnCmds[i]) == 0) {
            switchOwnArg = i + 1;
            break;
        }
    }
    return switchOwnArg >= 1 ? 1 : 0;
};

// function for finding pipe
int parsePipe(char* str, char** strpiped)
{
	int i;
	for (i = 0; i < 2; i++) {
		strpiped[i] = strsep(&str, "|");
		if (strpiped[i] == NULL)
			break;
	}

	if (strpiped[1] == NULL)
		return 0; // returns zero if no pipe is found.
	else {
		return 1;
	}
}

// function for parsing command words
void parseSpace(char* str, char** parsed)
{
	int i;

	for (i = 0; i < MAX_LIST; i++) {
		parsed[i] = strsep(&str, " ");

		if (parsed[i] == NULL)
			break;
		if (strlen(parsed[i]) == 0)
			i--;
	}
}

int processString(char* str, char** parsed, char** parsedpipe)
{
	char* strpiped[2];
	int piped = 0;

	piped = parsePipe(str, strpiped);

	if (piped) {
		parseSpace(strpiped[0], parsed);
		parseSpace(strpiped[1], parsedpipe);
	} 
    else
		parseSpace(str, parsed);

	if (is_our_command(parsed))
		return 0;
	else
		return 1 + piped;
}

int main()
{
	char inputString[MAX_COM], *parsedArgs[MAX_LIST];
	char* parsedArgsPiped[MAX_LIST];
	int execFlag = 0;
	init_shell();

	while (1) {
		printDir();

		if (takeInput(inputString))
			continue;

		execFlag = processString(inputString, parsedArgs, parsedArgsPiped);
		// execflag returns zero if there is no command
		// or it is a builtin command,
		// 1 if it is a simple command
		// 2 if it is including a pipe.

		// execute
        if (execFlag == 2)
            execArgsPiped(parsedArgs, parsedArgsPiped);
        else
			execArgs(parsedArgs, execFlag);
	}
	return 0;
}
