#include<stdio.h>
#include<sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include<sys/wait.h>

#include <time.h>

#define MAX_STR_SIZE 100
char user_input[MAX_STR_SIZE];
char *command_arr[MAX_STR_SIZE];//[MAX_STR_SIZE];
char command_parameter[MAX_STR_SIZE];

int background_flag;
int number_commands;

void read_input(void);
void parse_input(void);
int is_builtin(void);
void execute_command(void);
void signal_handler(int);

void setup_environment(void){
    char directory[MAX_STR_SIZE];
    getcwd(directory,sizeof(directory));
    chdir(directory);
}

void builtin_cd(void)
{
    if ((number_commands==2) || !(strcmp(command_arr[1], "~"))){
        chdir(getenv("HOME"));
    }
    else if (chdir(command_arr[1]) != 0) {
        printf("error in cd\n");
    }
}

void builtin_echo(void)
{
    for(int i = 0; i <= strlen(command_arr[1]); i++) {
        if (command_arr[1][i] == '"') continue;
        printf("%c",command_arr[1][i]);
    }
    for(int j = 2; j < (number_commands-1); j++){
        if (j == (number_commands -2)) command_arr[j][strlen(command_arr[j])-1] ='\0';
        printf(" %s", command_arr[j]);
    }
    printf("\n");
    return;
}

void builtin_export(void){
    putenv(strdup(command_parameter));
    return;
}


int main(void){
    setup_environment();
    do{
        read_input();
        parse_input ();
        if (is_builtin()){
            if(!(strcmp(command_arr[0],"cd"))){
                builtin_cd();
            }
            else if (!(strcmp(command_arr[0],"echo"))){
                builtin_echo();
            }
            else if (!(strcmp(command_arr[0],"export"))){
                builtin_export();
            }
        }
        else if(!(strcmp(command_arr[0],"exit"))){
            return;
        }

        else{
            execute_command();    
        }
    }
    while (1);
    return 0;
}

void read_input(void){
    printf("Enter your command: ");
    char input_str_buffer[MAX_STR_SIZE]; 
    fgets(input_str_buffer, MAX_STR_SIZE, stdin);
    input_str_buffer[strcspn(input_str_buffer, "\n")] = '\0';
    int input_index;
    int output_index = 0;

    for (input_index = 0; input_index < strlen(input_str_buffer)+1;input_index++){
        if (input_str_buffer[input_index] == '$'){
            char temp [MAX_STR_SIZE];
            temp[0] = input_str_buffer[input_index+1];
            temp[1] = '\0';
            strcpy(temp, getenv(temp));
            for (int j = 0; j<strlen(temp); j++){
                if (temp[j] !='"'){
                    user_input[output_index] = temp[j];
                    output_index++;
                }
            }
            input_index++;
        }
        else{
            user_input[output_index] = input_str_buffer[input_index];
            output_index++;
        }
    }
}

void parse_input (void){
    int input_length = 0;
    number_commands = 0;
    background_flag = 0;

    char * temp = strtok (user_input," ");
    strcpy(command_parameter, user_input+strlen(temp)+1);
    while (temp != NULL)
    {
        if (!(strcmp(temp, "&"))){
            background_flag = 1;
            temp = strtok (NULL, " ");           
            continue;
        }
        command_arr[number_commands] = temp;
        temp = strtok (NULL, " ");
        number_commands++;
    }
    command_arr[number_commands++] = NULL;
}

int is_builtin(void){
    if(!(strcmp(command_arr[0], "cd")) || !(strcmp(command_arr[0], "echo")) || !(strcmp(command_arr[0], "export"))){
        return 1;
    }
    else{
        return 0;
    }
}

void execute_command(void){
    pid_t status;

    int child_id = fork();
    if (child_id < 0){
        fprintf(stderr,"Fork Failed");
        exit(0);
    }

    else if (child_id == 0){
        /*child process*/
        if(execvp(command_arr[0], command_arr)<0){
            printf("Error\n");
        }
        exit(0);
    }
    else {
        // waiting for child to terminate
        if(!(background_flag)){
            waitpid(child_id,NULL,0);
        }
        else{
            signal(SIGCHLD,signal_handler);
        }
        return;
    }
}

void signal_handler(int sig)
{
    while (waitpid((pid_t)(-1), 0, WNOHANG) > 0) {}

    time_t rawtime;
    struct tm * timeinfo;

    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    FILE *f = fopen("/media/marawan/Disk/Documents/C programing/OS/OS/Lab1/file.txt", "a");
    if (f == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }

    fprintf(f, "Child was closed in %s \n.........\n", asctime (timeinfo));

    fclose(f);
}