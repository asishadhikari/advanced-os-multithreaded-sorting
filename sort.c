#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>


//to keep track of index of subarrays
typedef struct tracker{
	int lower_index;
	int higher_index;
} tracker;

//keep track of elements in the array
int count = 0;
//the array of integers
int *list;


void *merge_sort(void *boundary){
	tracker* sub_indices = (tracker*) boundary; 
	//base case
	if(sub_indices->lower_index >= sub_indices->higher_index) 
		return;
	int mid = (sub_indices->lower_index+sub_indices->higher_index)/2;
	tracker sub_boundaries[2];
	//each thread spawns two childs
	pthread_t threads[2];
	//update the tracker for the threads
	sub_boundaries[0].lower_index = sub_indices->lower_index;
	sub_boundaries[0].higher_index = mid;
	sub_boundaries[1].lower_index = mid+1;
	sub_boundaries[1].higher_index = sub_indices->higher_index;
	
	/*create two threads for each threads and call merge sort with the
	  boundaries*/
	pthread_create(&threads[0],NULL,merge_sort,&sub_boundaries[0]);
	pthread_create(&threads[1],NULL,merge_sort,&sub_boundaries[1]);

	pthread_join


}



int main(int argc, char** argv)
{
    FILE *fp;
    fp = fopen(argv[1], "r");
    if(!fp) return 1;

    //the number of integers in file
    for (char c = getc(fp); c != EOF; c = getc(fp))
        if (c == '\n') 
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
	tracker root_boundary;
	root_boundary.lower_index = 0;
	root_boundary.higher_index = count-1;
	pthread_t thread;
	//root thread creates child threads
	pthread_create(&thread,NULL,merge_sort,&root_boundary);
	pthread_join(thread,NULL);

	//array is sorted at this point
	for (int i = 0; i < count; ++i)
	{
		printf("%d\n",list[i]);
	}
    
    return 0;
}

