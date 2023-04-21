#include <pthread.h>
#include <stdio.h>
int x;


void* iterate( void* arg){
	while( x< 1000000000){
		x++;
	
}
}

int main(){
	pthread_t newthread;
	pthread_t secondThread;
	void * exitStatus;
	printf("(%d)\n", x );

	pthread_create(&newthread, NULL, iterate, NULL);
	pthread_create(&secondThread, NULL, iterate, NULL);

	pthread_join(newthread, exitStatus);
	pthread_join(secondThread, exitStatus);
	printf("(%d)\n", x );

	return 0;
}

