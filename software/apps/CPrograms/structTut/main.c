#include <stdio.h>

typedef struct
{
	int age;
	float weight;
}person;

int main()
{	


	volatile person*  structPTR = (person*)  0x4000000;
	// use for defining objects of the structure

    
	// use for printing the addresses
	printf("%p is the address of the age\n", structPTR );
	structPTR += 1; // SHIFTS ONE INT DOWN. 
	printf("%p is the address for the weight", structPTR);

    return 0;
}
