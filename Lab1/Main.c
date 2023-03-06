#include<stdio.h>
#include<sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include<sys/wait.h>


#define MAX_STR_SIZE 100
char user_input[MAX_STR_SIZE];
char *command_arr[MAX_STR_SIZE];//[MAX_STR_SIZE];
int background_flag;
int number_commands;

void read_input(void);
void parse_input(void);
int is_builtin(void);
void execute_command(void);





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
    if (command_arr[1][0] == '"')
    for(int i = 1; i <= strlen(command_arr[1]); i++) {
        if(command_arr[1][i] == '"') break;
        printf("%c", command_arr[1][i]);
    }
    printf("%s, \n",getenv("x"));
    return;
}
void builtin_export(void){
    return;
}


int main(void){
    // do
    //     parse_input(read_input())
    //     evaluate_expression():
    //     switch(input_type):
    //         case shell_builtin:
    //             execute_shell_bultin();
    //         case executable_or_error:
    //             execute_command():

    // while command_is_not_exit

    do{
        read_input();
        parse_input ();
        if (is_builtin()){
            // execute_shell_bultin();
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
    printf("Enter your command: \n");
    fgets(user_input, MAX_STR_SIZE, stdin);
    user_input[strcspn(user_input, "\n")] = '\0';
}


void parse_input (void){
    int input_length = 0;
    number_commands = 0;
    background_flag = 0;
    char * temp = strtok (user_input," ");

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
    // child_id = fork()
    // if child:
    //     execvp(command parsed)
    //     print("Error)
    //     exit()
    // else if parent and foreground:
	//  waitpid(child)
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
            signal(SIGCHLD,SIG_IGN);
        }
        return;
    }
}