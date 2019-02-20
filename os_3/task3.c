#include <stdio.h>
#include <pthread.h>

//gcc ** -o ** lpthread
int data = 10; //define Global variables
//thread
void* thread(void *arg)  
{  
    data = 100;
    printf( "This is a thread and data = %d.\n", data);
    
}  
int main( int argc, char *argv[] )  
{  
    pthread_t th; 
    int arg = 10; 
    int ret;  
    int *thread_ret = NULL;  
    //creat thread
    ret = pthread_create( &th, NULL, thread, &arg );  
    if( ret != 0 ){  
        printf( "Create thread error!\n");  
        return -1;  
    }  
    printf( "This is the main process and data = %d\n" ,data); 
	//visit thread 
    pthread_join( th, (void**)&thread_ret );  
      
    return 0;  
}  
