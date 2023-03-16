#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
// #define MAX_STR_SIZE 100
// int *glbl_matrix_1;
// int *glbl_matrix_2;
int *glbl_matrix_3_r_th;
int *glbl_matrix_3_e_th;

// int glbl_row_1,glbl_row_2,glbl_column_1,glbl_column_2;
char *glbl_output_filename;

typedef struct
{
 int var1,var2;
}test_struct;

typedef struct
{
    int row_1,row_2,col_1,col_2;
    int* matrix_1, *matrix_2;
    int* matrix_3_tpm, *matrix_3_tpr, *matrix_3_tpe;
    int current_row,current_column;
    struct timeval stop_tpm, start_tpm;
    struct timeval stop_tpr, start_tpr;
    struct timeval stop_tpe, start_tpe;
}combined_matrix_struct;
combined_matrix_struct glbl_matrices;

void read_matrix_size (int *row_par, int *column_par, char *file_name){
    char* extension = ".txt";
    char fileSpec[strlen(file_name)+strlen(extension)+1];
    FILE *fp;

    snprintf( fileSpec, sizeof( fileSpec ), "%s%s", file_name, extension );

    fp = fopen( fileSpec, "r" );
    fscanf(fp, "row=%d col=%d", row_par,column_par);
    // printf("%d %d", *row_par, *column_par)
    fclose(fp);
}

// void populate_matrix (int *row_par, int *column_par, char *file_name,int matrix[*row_par][*column_par]){
void populate_matrix (int row_par, int column_par, char *file_name, int *matrix){

    char* extension = ".txt";
    char fileSpec[strlen(file_name)+strlen(extension)+1];
    FILE *fp;
    // printf("**\n%d, %d\n**\n", row_par,column_par);
    snprintf( fileSpec, sizeof( fileSpec ), "%s%s", file_name, extension );
    fp = fopen( fileSpec, "r" );
    fscanf(fp, "%*[^\n]\n");

    for(int i=0;i<row_par;i++)
    {
        for(int j=0;j<column_par;j++)
        {
            // printf("%d, %d\n", i,j);
            // int temp;
            fscanf(fp, "%d ",(matrix+i*column_par+j));
            // printf("%d ",temp);
        }
    }    
}

void thread_per_matrix(combined_matrix_struct* lcl_matrices, char *file_name){
    gettimeofday(&glbl_matrices.start_tpm, NULL); //start checking time
    // int matrix_3[*row_par_1][*column_par_2];
    int out_row = lcl_matrices->row_1;
    int out_col = lcl_matrices->col_2;
    int inner_dim = lcl_matrices->col_1;
    int *lcl_matrix_1 = lcl_matrices->matrix_1;
    int *lcl_matrix_2 = lcl_matrices->matrix_2;
    int *matrix_3 = lcl_matrices->matrix_3_tpm;

    for (int i = 0; i < out_row; i++) {
        for (int j = 0; j < out_col; j++) {
            *(matrix_3 + i * out_col+ j) = 0;
            for (int k = 0; k < inner_dim; k++) {
                *(matrix_3 + i * out_col+ j) += *(lcl_matrix_1+i*inner_dim+k) * *(lcl_matrix_2+k*out_col+j);
            }
        }
    }
    printf("\nThe matrix is : \n");
    for (int i = 0; i < out_row; i++) {
        printf("\n");
        for (int j = 0; j < out_col; j++) {
            printf("%d\t",*(matrix_3 + i * out_col+ j));
        }
    }
    printf("\n\n"); 
    gettimeofday(&glbl_matrices.stop_tpm, NULL); //end checking time
}

//Each thread computes single element in the resultant matrix
void *multiply_row(void* arg){
    combined_matrix_struct *lcl_matrices;
    lcl_matrices = (combined_matrix_struct *) arg;
    // int current_row = (int *)arg;
    int current_row = lcl_matrices->current_row;
    int out_row = lcl_matrices->row_1;
    int out_col = lcl_matrices->col_2;
    int inner_dim = lcl_matrices->col_1;
    int *lcl_matrix_1 = lcl_matrices->matrix_1;
    int *lcl_matrix_2 = lcl_matrices->matrix_2;
    int *matrix_3 = glbl_matrices.matrix_3_tpr;
    // printf("%d ", out_col);
    // printf("%d %d %d %d\n", out_row,out_col,inner_dim, current_row);
    for (int j = 0; j < out_col; j++) {
        // printf("%d",j);
        *(matrix_3+current_row*out_col+j) = 0;
        for (int k = 0; k < inner_dim; k++) {
            *(matrix_3+current_row*out_col+j) += *(lcl_matrix_1+current_row*inner_dim+k) * *(lcl_matrix_2+k*out_col+j);
        }
        // printf("%d", *(glbl_matrix_3_r_th+current_row*out_col+j));
    }
    pthread_exit(1);
}

void thread_per_row(void){
    gettimeofday(&glbl_matrices.start_tpr, NULL); //start checking time
    pthread_t threads[glbl_matrices.row_1];

    for (int i = 0; i < glbl_matrices.row_1; i++) {
        combined_matrix_struct *lcl_matrices;
        lcl_matrices=(combined_matrix_struct *)malloc(sizeof(combined_matrix_struct));
        memcpy(lcl_matrices, &glbl_matrices, sizeof(combined_matrix_struct));
        lcl_matrices->current_row = i;
        pthread_create(&threads[i], NULL, multiply_row, (void*)(lcl_matrices));
    }
 
    // joining and waiting for all threads to complete
    for (int i = 0; i < glbl_matrices.row_1; i++)
        pthread_join(threads[i], NULL);   
     printf("\nThe matrix is : \n");
    for(int i=0;i<glbl_matrices.row_1;i++)
    {
        printf("\n");
        for(int j=0;j<glbl_matrices.col_2;j++)
        {
            printf("%d\t",*(glbl_matrices.matrix_3_tpr + i *glbl_matrices.col_2 + j));
        }
    }
    printf("\n\n"); 
    gettimeofday(&glbl_matrices.stop_tpr, NULL); //end checking time
}


void *multiply_element(void* arg){
    combined_matrix_struct *lcl_matrices;
    lcl_matrices = (combined_matrix_struct *) arg;
    // int current_row = (int *)arg;
    int current_row = lcl_matrices->current_row;
    int current_column = lcl_matrices->current_column;
    int out_row = lcl_matrices->row_1;
    int out_col = lcl_matrices->col_2;
    int inner_dim = lcl_matrices->col_1;
    int *lcl_matrix_1 = lcl_matrices->matrix_1;
    int *lcl_matrix_2 = lcl_matrices->matrix_2; 
    int *matrix_3 = glbl_matrices.matrix_3_tpe;
   
   *(matrix_3+current_row*out_col+current_column) = 0;
    
    for (int k = 0; k < inner_dim; k++) {
        *(matrix_3+current_row*out_col+current_column) += *(lcl_matrix_1+current_row*inner_dim+k) * *(lcl_matrix_2+k*out_col+current_column);
    }
    pthread_exit(1);
}

void thread_per_element(void){
    gettimeofday(&glbl_matrices.start_tpe, NULL); //start checking time
    pthread_t threads[glbl_matrices.row_1][glbl_matrices.col_2];

    for (int i = 0; i < glbl_matrices.row_1; i++) {
        for (int j = 0; j < glbl_matrices.col_2; j++) {
            combined_matrix_struct *lcl_matrices;
            lcl_matrices=(combined_matrix_struct *)malloc(sizeof(combined_matrix_struct));
            memcpy(lcl_matrices, &glbl_matrices, sizeof(combined_matrix_struct));
            lcl_matrices->current_row = i;
            lcl_matrices->current_column = j;
            pthread_create(&threads[i][j], NULL, multiply_element, (void*)(lcl_matrices));
        }
    } 
    
    // joining and waiting for all threads to complete
    for (int i = 0; i < glbl_matrices.row_1; i++) {
        for (int j = 0; j < glbl_matrices.col_2; j++) {
            pthread_join(threads[i][j], NULL);  
        }
    }
     printf("\nThe matrix is : \n");
    for(int i=0;i<glbl_matrices.row_1;i++)
    {
        printf("\n");
        for(int j=0;j<glbl_matrices.col_2;j++)
        {
            printf("%d\t",*(glbl_matrices.matrix_3_tpe + i *glbl_matrices.col_2 + j));
        }
    }
    printf("\n\n"); 
    gettimeofday(&glbl_matrices.stop_tpe, NULL); //end checking time
}
void main (int argc, char **argv){


    read_matrix_size (&glbl_matrices.row_1, &glbl_matrices.col_1,"test3/a");
    read_matrix_size (&glbl_matrices.row_2, &glbl_matrices.col_2,"test3/b");

    glbl_matrices.matrix_1 = (int*) malloc(glbl_matrices.row_1 * glbl_matrices.col_1 * sizeof(int));
    glbl_matrices.matrix_2 = (int*) malloc(glbl_matrices.row_2 * glbl_matrices.col_2 * sizeof(int));
    
    populate_matrix (glbl_matrices.row_1, glbl_matrices.col_1, "test3/a", glbl_matrices.matrix_1);
    populate_matrix (glbl_matrices.row_2, glbl_matrices.col_2, "test3/b", glbl_matrices.matrix_2);
    
    glbl_matrices.matrix_3_tpm = (int*) malloc(glbl_matrices.row_1 * glbl_matrices.col_2 * sizeof(int));
    glbl_matrices.matrix_3_tpr = (int*) malloc(glbl_matrices.row_1 * glbl_matrices.col_2 * sizeof(int));
    glbl_matrices.matrix_3_tpe = (int*) malloc(glbl_matrices.row_1 * glbl_matrices.col_2 * sizeof(int));

    thread_per_matrix(&glbl_matrices, "c");
    thread_per_row();
    thread_per_element();

    // printf("Seconds taken by method 1%lu\n", stop.tv_sec - start.tv_sec);
    printf("Microseconds taken by method 1: %lu\n", glbl_matrices.stop_tpm.tv_usec - glbl_matrices.start_tpm.tv_usec);
    printf("Microseconds taken by method 2: %lu\n", glbl_matrices.stop_tpr.tv_usec - glbl_matrices.start_tpr.tv_usec);
    printf("Microseconds taken by method 3: %lu\n", glbl_matrices.stop_tpe.tv_usec - glbl_matrices.start_tpr.tv_usec);


    free(glbl_matrices.matrix_1);
    free(glbl_matrices.matrix_2); 
    free(glbl_matrices.matrix_3_tpm);
    free(glbl_matrices.matrix_3_tpr);
    free(glbl_matrices.matrix_3_tpe);


}