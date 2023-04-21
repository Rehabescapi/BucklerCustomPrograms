#include <stdlib.h>
#include <stdio.h>
#include <time.h>



int x; // Value that gets updated. 
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
		head->listener = listener;
		head->next = 0;
		tail = head;
	}else{
		tail->next = malloc (sizeof(element_t));
		tail = tail->next;
		tail->listener = listener;
		tail->next = 0;

	}
	
}

/** procedure to update x.
**/

void update (int newx){
	x = newx;
	int i =0;

	element_t* elementX = head;
	while(elementX !=0){
		(* (elementX-> listener)) (newx);
		elementX = elementX->next;
		i++;
	}
	printf("Current Count is %d\n",i);
}

//Example of a notify procedure.
void print (int arg ){
	printf("System Parameter is %d\n", arg);
}



void delay(int number_of_seconds)
{
	
   int second = 1;
   
    // Converting time into milli_seconds
   time_t start, end;
   start = time(NULL);
 
  for(int i = 1; i <=number_of_seconds; i++){
   	time_t wait = second+time(NULL);
   	while(time(NULL) < wait){}
   	printf("%d\n", i);
   }
   end=time(NULL);
  printf("program take %d second\n",(unsigned int)difftime(end,start));
  

}

int main(void) {
	//pthread_t threadID1, threadID2, threadID3, threadID4;
	void* exitStatus;

	

   

		
		addListener(&print);
		addListener(&print);
		addListener(&print);
		addListener(&print);
		

		update(1);


		//printf("timestamp: %d\n", timestamp );
		delay(2);
		
        // delay of one second
       
       // printf("%d seconds have passed\n", i + 1);
    


		update(2);

		//printf("timestamp: %d\n", timestamp );
		delay(2);
		
		update(4);
		//printf("timestamp: %d\n", timestamp );
		


	}