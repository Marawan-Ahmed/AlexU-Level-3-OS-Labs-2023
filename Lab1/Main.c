#include<stdio.h>
#include<sys/types.h>
#include <unistd.h>


#define MAX_STR_SIZE 100
char user_input[MAX_STR_SIZE];
char  command_arr[MAX_STR_SIZE][MAX_STR_SIZE];

void read_input(void);
void parse_input(void);
int is_builtin(void);

void main(void){
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

        }
        else{
            
        }


    }
    while (1);
}
void read_input(void){
    printf("Enter your command: \n");
    // scanf("%s", user_input);
    fgets(user_input, MAX_STR_SIZE, stdin);
    // printf("You wrote: %s\n", user_input);
}


void parse_input (void){
    int input_length = 0;
    int number_commands = 0;
    while((user_input[input_length] != '\0')){
        int j = 0;
        while((user_input[input_length] != ' ') && (user_input[input_length] != '\0') ){
            command_arr[number_commands][j] = user_input[input_length];
            j++;
            input_length++;
        }
        command_arr[number_commands] [j] = '\0';
        number_commands++;
    }
    // printf("Your command (%d) is:", number_commands);
    for (int i = 0; i < (number_commands - 1); i++){
        // command_arr[i] = command_arr[i];
        // printf("%s", command_arr[i]);
    }
    // printf("%s.\n", command_arr[number_commands-1]);
    // printf("Your command is: %s,", command_arr[0]);
    // printf(" %s,", command_arr[1]);
    // printf(" %s\n", command_arr[2]);
}

int is_builtin(void){
    printf("(%s)", command_arr[0]);
    if((command_arr[0] == "cd\0")||(command_arr[0] == "echo")){
        printf("built in\n");
        return 1;
    }
    else{
        printf("not built in\n");
        return 0;
    }
}

// void execute_command(void){
//     // child_id = fork()
//     // if child:
//     //     execvp(command parsed)
//     //     print("Error)
//     //     exit()
//     // else if parent and foreground:
// 	//  waitpid(child)
//     int child_id = fork();
//     if (child_id < 0){
//         fprintf(stderr,"Fork Failed");
//         exit();
//     }

//     else if (child_id == 0){
//         /*child process*/
//         char* argument_list[] = {"ls", "-l", NULL}; // NULL terminated array of char* strings

//         // Ok! Will execute the command "ls -l"
//         execvp("ls", argument_list);
//         printf("Error");
//         exit();
//     }

// }

// void parent_main(void){
//     // register_child_signal(on_child_exit())
//     // setup_environment()
//     // shell()


//     }