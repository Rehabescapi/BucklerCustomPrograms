#include <pthread.h>
#include <stdio.h>
int x;
pthread_mutex_t lock;
int counter;

void* iterate( void* arg){
	pthread_mutex_lock(&lock);

	int count = ++ counter;
	printf("\n Job Started %d", count);
	

	
	while(x < 1000000000)
	{
		
		x++;
		
	}
	
	
	printf("\n Job finished %d",(int) count);
	pthread_mutex_unlock(&lock);
	return 0;
}

int main(){
	counter = 0;
	pthread_t newthread;
	pthread_t secondThread;
	void * exitStatus;
	x=0;
	printf("(%d)\n", x );
	int x1 = 1, x2= 2;

	pthread_create(&newthread, NULL, iterate, &x1);
	pthread_create(&secondThread, NULL, iterate, &x2);

	pthread_join(newthread, &exitStatus);
	pthread_join(secondThread, &exitStatus);
	pthread_mutex_destroy(&lock);
	printf("\n(%d)\n", x );

	return 0;
}

