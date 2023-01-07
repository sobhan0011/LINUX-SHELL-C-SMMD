// Linux Shell
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <linux/limits.h>

#define COMMAND_MAX_LENGTH 1000
#define MAX_NUMBER_OF_COMMAND 100

// Clearing the shell using escape sequences
#define clear() printf("\033[H\033[J")

void first_segment_of_lines_of_file(const char* file_address) {
    FILE* ptr;
    char* line = NULL;
    size_t len = 0;
    ssize_t read;
    char ch;
    int sw = 0;

    ptr = fopen(file_address, "r");

    if (NULL == ptr)
    {
        fprintf(stderr, "file can't be opened. \n");
        fclose(ptr);
        return;
    }
    printf("Here are the first segment of lines in your file:\n");

    while ((read = getline(&line, &len, ptr)) != -1) {
        sw = 0;
        for (int i = 0; i < read; i++) {
            ch = line[i];
            if (ch != ' ' && ch != '\t' && ch != '\n')
                sw = 1; // If the file starts with spaces.

            if (sw == 1 && (ch == ' ' || ch == '\t' ||  ch == '\n'))
                break;

            if (sw == 0)
                continue;

            printf("%c", ch);
        }
        printf("\n");
    }
    fclose(ptr);
}

// Extracts characters present in `src` between `m` and `n` (excluding `n`)
char* substr(const char *src, int m, int n)
{
    // get the length of the destination string
    int len = n - m;

    // allocate (len + 1) chars for destination (+1 for extra null character)
    char *dest = (char*)malloc(sizeof(char) * (len + 1));

    // start with m'th char and copy `len` chars into the destination
    strncpy(dest, (src + m), len);

    // return the destination string
    return dest;
}

void most_repeated_word_in_file(const char* file_address) {
    FILE* ptr;
    ptr = fopen(file_address, "r");
    char* line = NULL;
    size_t len = 0;
    ssize_t read;
    char* words[COMMAND_MAX_LENGTH];
    int word_counters[COMMAND_MAX_LENGTH] = {0};
    char ch;
    int sw, start_index, end_index, count = 0;

    if (NULL == ptr)
    {
        fprintf(stderr, "file can't be opened. \n");
        fclose(ptr);
        return;
    }

    printf("Here is most repeated word in your file:\n");
    while ((read = getline(&line, &len, ptr)) != -1) {
        sw = 0;
        for (int i = 0; i < read; i++) {
            ch = line[i];
            if (ch != ' ' && ch != '\t' && ch != '\n' && sw == 0)
            {
                start_index = i;
                end_index = i;
                sw = 1;
            } // If the file starts with spaces.

            if (sw == 1 && (ch == ' ' || ch == '\t' ||  ch == '\n' || i == read - 1))
            {
                words[count] = substr(line, start_index, i);
                count++;
                sw = 0;
            }
        }
    }

    for (int j = 0; j < count; ++j)
        for (int k = j + 1; k < count; ++k)
            if (strcmp(words[j], words[k]) == 0)
                word_counters[j] += 1;

    int max = 0;
    int index;
    for (int j = 0; j < count; j++)
        if (max < word_counters[j]) {
            max = word_counters[j];
            index = j;
        }
    printf("%s", words[index]);
    fclose(ptr);
}

void space_delete(const char* file_address) {
    FILE* ptr;
    ptr = fopen(file_address, "r");
    char* line = NULL;
    size_t len = 0;
    ssize_t read;
    int i = 0;
    if (NULL == ptr)
    {
        fprintf(stderr, "file can't be opened. \n");
        fclose(ptr);
        return;
    }

    printf("Here is your file without any white spaces:\n");
    while ((read = getline(&line, &len, ptr)) != -1)
        for (i = 0; i < read; i++)
            if (line[i] != ' ' && line[i] != '\n' && line[i] != '\t')
                printf("%c", line[i]);
    fclose(ptr);
}

void none_comment_lines(const char* file_address) {
    FILE* ptr;
    ptr = fopen(file_address, "r");
    char* line = NULL;
    size_t len = 0;
    ssize_t read;
    int sw = 0, i = 0;

    if (NULL == ptr)
    {
        fprintf(stderr, "file can't be opened. \n");
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
}

void number_of_lines_of_file(const char* file_address) {
    FILE* ptr;
    ptr = fopen(file_address, "r");
    if (NULL == ptr)
    {
        fprintf(stderr, "file can't be opened. \n");
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
}

void first_ten_lines_of_file(const char* file_address) {
    FILE* ptr;
    ptr = fopen(file_address, "r");
    if (NULL == ptr)
    {
        fprintf(stderr, "file can't be opened. \n");
        fclose(ptr);
        return;
    }
    char* line = NULL;
    size_t len = 0;
    ssize_t read;
    int i = 0;
    printf("Here are first ten lines of your file:/n");
    while ((read = getline(&line, &len, ptr)) != -1 && i++ < 10)
        printf("%s", line);
    fclose(ptr);
}

void get_path() {
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL)
        printf("Current working dir: %s\n", cwd);
    else
        fprintf(stderr, "Problem in getcwd() error\n");
}

void open_help()
{
    puts("\n***WELCOME TO SHELL HELP***"
         "\n-Use the shell at your own risk..."
         "\nList of Commands supported:"
         "\n>cd"
         "\n>ls"
         "\n>exit"
         "\n>mr"
         "\n>fwf"
         "\n>sd"
         "\n>nc"
         "\n>nlf"
         "\n>ftl"
         "\n>all other general commands available in UNIX shell"
         "\n>pipe handling"
         /*"\n>improper space handling"*/);
    return;
}

void init_shell()
{
    clear();
    printf("\n\n\n\n******************"
           "************************");
    printf("\n\n\n\t****LINUX SHELL****");
    printf("\n\n\t-CREATED BY SOBHAN MAHMOODI AND MEHRSA DEHNAVI-");
    printf("\n\n\n\n*******************"
           "***********************");
    char* user_name = getenv("USER");
    printf("\n\n\nUSER is: @%s", user_name);
    printf("\n");
    sleep(1);
    clear();
}

int take_input(char* str)
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

void print_directory()
{
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    printf("\nDir: %s", cwd);
}

int our_command_handler(char** parsed)
{
    int number_of_own_commands = 10, i, switch_our_arg = 0;
    char* list_of_our_commands[number_of_own_commands];
    char* user_name;

    list_of_our_commands[0] = "exit";
    list_of_our_commands[1] = "cd";
    list_of_our_commands[2] = "help";
    list_of_our_commands[3] = "hello";
    list_of_our_commands[4] = "fwf"; // First Word of a File.
    list_of_our_commands[5] = "mr"; // Most Repeated word.
    list_of_our_commands[6] = "sd"; // Space Delete.
    list_of_our_commands[7] = "nc"; // None Comment.
    list_of_our_commands[8] = "nlf"; // Number of Lines in a File.
    list_of_our_commands[9] = "ftl"; // First Ten Lines.


    for (i = 0; i < number_of_own_commands; i++) {
        if (strcmp(parsed[0], list_of_our_commands[i]) == 0) {
            switch_our_arg = i + 1;
            break;
        }
    }

    switch (switch_our_arg) {
        case 1:
            printf("\nGoodbye\n");
            exit(0);
        case 2:
            chdir(parsed[1]);
            return 1;
        case 3:
            open_help();
            return 1;
        case 4:
            user_name = getenv("USER");
            printf("\nHello %s.\n"
                   "\nUse help to know more..\n",
                   user_name);
            return 1;
        case 5:
            first_segment_of_lines_of_file(parsed[1]);
            return 1;
        case 6:
            most_repeated_word_in_file(parsed[1]);
            return 1;
        case 7:
            space_delete(parsed[1]);
            return 1;
        case 8:
            none_comment_lines(parsed[1]);
            return 1;
        case 9:
            number_of_lines_of_file(parsed[1]);
            return 1;
        case 10:
            first_ten_lines_of_file(parsed[1]);
            return 1;
        default:
            break;
    }

    return 0;
}

// For command execution
void exec_args(char** parsed, int exec_flag)
{
    //get_path();
    // Forking a child
    pid_t pid = fork();

    if (pid == -1) {
        fprintf(stderr, "Failed forking child\n");
        return;
    }
    else if (pid == 0 && exec_flag == 0) {
        our_command_handler(parsed);
    }
    else if (pid == 0) {
        if (execvp(parsed[0], parsed) < 0) {
            fprintf(stderr, "Could not execute command.\n");
        }
        exit(0);
    } else {
        // waiting for child to terminate
        wait(NULL);
        return;
    }
}

// For piped commands executing
void exec_args_piped(char** parsed, char** parsed_pipe)
{
    //get_path();
    // 0 for read end
    // 1 for write end
    int pipe_function[2];
    pid_t p1, p2;

    if (pipe(pipe_function) < 0) {
        fprintf(stderr, "Pipe couldn't be initialized\n");
        return;
    }
    p1 = fork();
    if (p1 < 0) {
        fprintf(stderr, "Couldn't fork\n");
        return;
    }

    if (p1 == 0) {
        // Child 1 is executing and write at the write end
        close(pipe_function[0]);
        dup2(pipe_function[1], STDOUT_FILENO);
        close(pipe_function[1]);

        if (execvp(parsed[0], parsed) < 0) {
            fprintf(stderr, "Couldn't execute command 1.\n");
            exit(0);
        }
    } else {
        // Parent is executing
        p2 = fork();
        if (p2 < 0) {
            fprintf(stderr, "Couldn't fork\n");
            return;
        }

        /// Child 2 is executing and read from the read end
        if (p2 == 0) {
            close(pipe_function[1]);
            dup2(pipe_function[0], STDIN_FILENO);
            close(pipe_function[0]);
            if (execvp(parsed_pipe[0], parsed_pipe) < 0) {
                fprintf(stderr, "Couldn't execute command 2.\n");
                exit(0);
            }
        } else {
            // parent is executing and waiting for two children
            wait(NULL);
            wait(NULL);
        }
    }
}

int is_our_command(char** parsed) {
    int number_of_own_commands = 10, i, switch_our_arg = 0;
    char* list_of_our_commands[number_of_own_commands];

    list_of_our_commands[0] = "exit";
    list_of_our_commands[1] = "cd";
    list_of_our_commands[2] = "help";
    list_of_our_commands[3] = "hello";
    list_of_our_commands[4] = "fwf"; // First Word of a File.
    list_of_our_commands[5] = "mr"; // Most Repeated word.
    list_of_our_commands[6] = "sd"; // Space Delete.
    list_of_our_commands[7] = "nc"; // None Comment.
    list_of_our_commands[8] = "nlf"; // Number of Lines in a File.
    list_of_our_commands[9] = "ftl"; // First Ten Lines.

    for (i = 0; i < number_of_own_commands; i++) {
        if (strcmp(parsed[0], list_of_our_commands[i]) == 0) {
            switch_our_arg = i + 1;
            break;
        }
    }
    return switch_our_arg >= 1 ? 1 : 0;
}

// Finding pipe
int parse_pipe(char* str, char** str_piped)
{
    for (int i = 0; i < 2; i++) {
        str_piped[i] = strsep(&str, "|");
        if (str_piped[i] == NULL)
            break;
    }

    if (str_piped[1] == NULL)
        return 0; // returns zero if no pipe is found.
    else
        return 1;
}

// Parsing command words
void parse_space(char* str, char** parsed)
{
    for (int i = 0; i < MAX_NUMBER_OF_COMMAND; i++) {
        parsed[i] = strsep(&str, " ");
        if (parsed[i] == NULL)
            return;
        if (strlen(parsed[i]) == 0)
            i--;
    }
    return;
}

int processString(char* str, char** parsed, char** parsed_pipe)
{
    char* str_piped[2];
    int piped;

    piped = parse_pipe(str, str_piped);

    if (piped) {
        parse_space(str_piped[0], parsed);
        parse_space(str_piped[1], parsed_pipe);
    }
    else
        parse_space(str, parsed);

    if (is_our_command(parsed))
        return 0;
    else
        return 1 + piped;
}

void add_to_history_file(char* str) {
    FILE* ptr;
    ptr = fopen("history.txt","a");
    if (NULL == ptr)
    {
        fprintf(stderr, "file can't be opened. \n");
        fclose(ptr);
        return;
    }
    fprintf(ptr, "%s\n", str);
    fclose(ptr);
}

void sigintHandler(int sig_num)
{
    signal(SIGINT, sigintHandler);
    printf("\n Cannot be terminated using Ctrl+C \n");
    fflush(stdout);
}

int main()
{
    char input_string[COMMAND_MAX_LENGTH], *parsed_args[MAX_NUMBER_OF_COMMAND];
    char* parsed_args_piped[MAX_NUMBER_OF_COMMAND];
    int exec_flag = 0;
    init_shell();

    signal(SIGINT, sigintHandler);

    while (1) {
        print_directory();

        if (take_input(input_string))
            continue;

        add_to_history_file(input_string);

        exec_flag = processString(input_string, parsed_args, parsed_args_piped);
        // execflag returns zero if there is no command
        // or it is a builtin command,
        // 1 if it is a simple command
        // 2 if it is including a pipe.

        // execute
        if (exec_flag == 2)
            exec_args_piped(parsed_args, parsed_args_piped);
        else
            exec_args(parsed_args, exec_flag);
    }
    return 0;
}
