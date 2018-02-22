#include <stdio.h>
#include <stdlib.h> 

#define NUM_THREADS 2

//to keep track of index of subarrays
typedef struct tracker{
	int lower_index;
	int higher_index;
} tracker;

//keep track of elements in the array
int count = 0;
int *list;


void *merge_sort(void *boundary){
	
}



int main(int argc, char** argv)
{
    FILE *fp;
    fp = fopen(argv[1], "r");
    if(!fp) return 1;

    //the number of integers in file
    for (char c = getc(fp); c != EOF; c = getc(fp))
        if (c == '\n') // Increment count if this character is newline
            count+=1;
    
     //reset filepointer to populate all integers
    fseek(fp, 0, SEEK_SET);
    list = (int*)malloc(sizeof(int)*(count++));
    int i = 0;
    //populate array with index 0 ->= count
    while(!feof(fp))
    {
    	fscanf(fp,"%d",&list[i]);
    	i++;
    }
    //initialise indices 
	tracker root;
	root.lower_index = 0;
	root.higher_index = count-1;
	printf("%p\n", list);
	pthread_t root_thread;

	pthread_create(&thread,NULL,merge_sort);
    
    return 0;
}

