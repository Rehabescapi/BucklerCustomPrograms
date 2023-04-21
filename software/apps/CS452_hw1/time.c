#include <pthread.h>

int x;


void* iterate( void* arg){
	while( x< 1000000){
		x++;
	
}


int main(){
	pthread_t newthread;


	pthread_create(&newthread, NULL, iterate, NULL);

	pthread_join(&newthread, NULL);
}

