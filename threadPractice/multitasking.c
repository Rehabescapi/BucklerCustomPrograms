#include <stdlib.h>
#include <stdio.h>
#include <time.h>



float x; // Value that gets updated. 
static int counter = 0;
typedef void notifyProcedure(int); // Type of notify proc.
struct element {
	notifyProcedure* listener; // Pointer to notify procedure.
	struct element* next; // Pointer to the next item. 
};
typedef struct element element_t; // Type of list elements.
element_t* head = 0; // Pointer to 
element_t* tail = 0;
//Procedure to add a listener
void addListener(notifyProcedure* listener){
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
	printf("Listener %d added\n", ++ counter);
	
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
	//pthread_t threadID1, threadID2, threadID3, threadID4;
	

	//void* exitStatus;
		addListener(&print);
		addListener(&print);
		addListener(&print);
		addListener(&print);
		getTime(timer2);


		
		
		update(17.2500);
		

		getTime(timer2);



		update(15.25);
		getTime( timer2);
		


		update(13.25);
		getTime( timer2);
		



//printf("timestamp: %d\n", timestamp );
		


	}