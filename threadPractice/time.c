#include <pthread.h>
#include <stdio.h>
int x;


void* iterate( void* arg){
	int i;
	for(i = 0; i< 1000; i++){
		printf("My ID: %d\n", *(int*) arg);
	}
	return NULL;
}

int main(){
	pthread_t newthread;
	pthread_t secondThread;
	void * exitStatus;
	printf("(%d)\n", x );
	int x1 = 1, x2= 2;

	pthread_create(&newthread, NULL, iterate, &x1);
	pthread_create(&secondThread, NULL, iterate, &x2);

	pthread_join(newthread, &exitStatus);
	pthread_join(secondThread, &exitStatus);
	printf("(%d)\n", x );

	return 0;
}

