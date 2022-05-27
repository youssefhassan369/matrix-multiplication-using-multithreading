
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

int matrix1[250][250];
int matrix2[250][250];
int result_element_by_element[250][250];
int result_row_by_row[250][250];
int rows1=0;
int rows2=0;
int columns1=0;
int columns2=0;




void load(char *filename){
        FILE* fp;
        fp=fopen(filename,"r");
        if (!fp)
            {
            printf("Error opening file\n");
            }
             
        
            printf("Loaded Successfully..!\n");
            printf(".................................\n\n\n");
            fseek(fp, 0, SEEK_SET);


            fscanf(fp,"%d",&rows1);
            fscanf(fp,"%d",&columns1);
            int temp;
            for (int i=0; i<rows1; i++) {
                
                for (int j=0; j<columns1; j++) {
                    fscanf(fp,"%d",&temp);
                    matrix1[i][j]=temp;
                                                }
                                        }
            
            fscanf(fp,"%d",&rows2);
            fscanf(fp,"%d",&columns2);
            
            for (int i=0; i<rows2; i++) {
                
                for (int j=0; j<columns2; j++) {
                    fscanf(fp,"%d",&temp);
                    matrix2[i][j]=temp;
                }
            }
    fclose(fp);

  
    
}



void printmatrix1(){
    printf("%d\t%d\n",rows1,columns1);
    for (int i=0; i<rows1; i++) {
        for (int j=0; j<columns1; j++) {
            printf("%d\t",matrix1[i][j]);
        }
        printf("\n");
    }
}
    
    void printmatrix2(){
        printf("%d\t%d\n",rows2,columns2);
        for (int i=0; i<rows2; i++) {
            for (int j=0; j<columns2; j++) {
                printf("%d\t",matrix2[i][j]);
            }
            printf("\n");
        }
    }


void print_Result_element_by_element(){

printf("Element by element\n");
    
    for (int i=0; i<rows1; i++) {
        for (int j=0; j<columns2; j++) {
            printf("%d\t",result_element_by_element[i][j]);
        }
        printf("\n");
    }
}

void *calcElement(void *arg){
    int *array=arg;
    int row=array[0];
    int column=array[1];
    int element =0;

    
    for (int k=0; k<columns1; k++) {
        element=element+matrix1[row][k]*matrix2[k][column];
    }
    result_element_by_element[row][column]=element;

    free(arg);
}

void multiplyByElement(pthread_t *threads_elements){
int k=0;

    for (int i=0; i<rows1; i++) {
        for (int j=0; j<columns2; j++) {
            int *array=malloc(2*sizeof(int));
            array[0]=i;
            array[1]=j;

            pthread_create(&threads_elements[k],NULL,&calcElement, array);
            k++;
        }
    }
    for(int s=0;s<rows1*columns2;s++){
    pthread_join(threads_elements[s],NULL);
}
   } 
   
   
   
   
   //--------------------------------------------------------------------------------------------------------------------------
   //--------------------------------------------------------------------------------------------------------------------------
   
   
   
   
   
   int calc_Element(int row,int column){
   int element=0;
   for (int k=0; k<columns1; k++) {
        element=element+matrix1[row][k]*matrix2[k][column];
    }
    return element;
 
   }
   
   void *calcRow(void *arg){
    int *row=arg;
    
    for (int k=0; k<columns2; k++) {
    result_row_by_row[*row][k]=calc_Element(*row,k);
    }


    free(arg);
}
   
   void multiplyByRows(pthread_t *threads_Rows){

	int k=0;
	
    for (int i=0; i<rows1; i++) {
    int *row=malloc(sizeof(int));
    *row=i;
            pthread_create(&threads_Rows[k],NULL,&calcRow,row);
            k++;
        }
    
    for(int s=0;s<rows1;s++){
    pthread_join(threads_Rows[s],NULL);
}
   }
   
   

   void printResultByRows(){
   printf("Row by Row\n");
   
    for (int i=0; i<rows1; i++) {
        for (int j=0; j<columns2; j++) {
            printf("%d\t",result_row_by_row[i][j]);
        }
        printf("\n");
    }
   
   }
   
    
    

int main(int argc,char * argv[]) {
    clock_t start1, end1,start2,end2;
    double cpu_time_used;
    load(argv[1]);
    
    
    pthread_t threads_elements[rows1*columns2];
    start1=clock();
    multiplyByElement(threads_elements);
    end1=clock();
    cpu_time_used = ((double) (end1 - start1)) / CLOCKS_PER_SEC;
    print_Result_element_by_element();
    printf("Time taken = %f\n\n",cpu_time_used);
    printf("------------------------------------------------------------------\n\n");
    
    pthread_t threads_Rows[rows1];
    start2=clock();
    multiplyByRows(threads_Rows);
    end2=clock();
    cpu_time_used = ((double) (end2 - start2)) / CLOCKS_PER_SEC;
    printResultByRows();
    printf("Time taken = %f\n\n\n",cpu_time_used);
    
    return 0;
}

