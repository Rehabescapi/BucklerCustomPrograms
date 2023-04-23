#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>


float x; // Value that gets updated. 
static int counter = 0;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
//pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;



typedef void notifyProcedure(int); // Type of notify proc.
struct element {
	notifyProcedure* listener; // Pointer to notify procedure.
	struct element* next; // Pointer to the next item. 
};
typedef struct element element_t; // Type of list elements.
element_t* head = 0; // Pointer to 
element_t* tail = 0;
//Procedure to add a listener
void* addListener(notifyProcedure* listener){
	pthread_mutex_lock(&lock);
	if(head == 0) {
		head = malloc(sizeof(element_t));
		head->next = 0;
		head->listener = listener;
		tail = head;
	}else{
		tail->next = malloc (sizeof(element_t));
		tail = tail->next;
		tail->listener = listener;
		tail->next = 0;

	}
	pthread_mutex_unlock(&lock);
	printf("Listener %d added\n", ++counter);
	return 0;
	
}

/** procedure to update x.
**/

void update (float newx){
	x = newx;
	int i =0;

	element_t* elementX = head;
	while(elementX !=0){
		(* (elementX-> listener)) (newx);
		elementX = elementX->next;
		i++;
	}
	
}

//Example of a notify procedure.
void print (float arg ){
	printf("Listeners added are %d, System Parameter is %.6f\n",counter,  arg);
}






void getTime(time_t timer)
{
	time_t now;
	now = time(NULL);
	float timestamp = (float) (clock() - timer)/1000.0F;
	printf("\n  timestamp :%.8f \n", timestamp);

}



int main(void) {
	time_t timer2 = clock();
	pthread_t threadID1, threadID2, threadID3, threadID4;
	void* exitStatus;

	pthread_create(&threadID1, NULL, addListener, &print);
	pthread_create(&threadID2, NULL, addListener, &print);
	pthread_create(&threadID3, NULL, addListener, &print);
	pthread_create(&threadID4, NULL, addListener, &print);


	
	pthread_join(threadID1,&exitStatus);
	pthread_join(threadID2,&exitStatus);
	pthread_join(threadID3,&exitStatus);
	pthread_join(threadID4,&exitStatus);
	
	int j =0;
	while(j <5){
		update(j);
		getTime(timer2);
		j++;
	}


/*
		getTime(timer2);


		
		
		update(17.2500);
		

		getTime(timer2);



		update(15.25);
		getTime( timer2);
		


		update(13.25);
		getTime( timer2);
		

*/

//printf("timestamp: %d\n", timestamp );
		


	}