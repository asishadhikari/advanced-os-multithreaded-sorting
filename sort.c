#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <unistd.h>

//to keep track of index of subarrays
typedef struct tracker
{
	int lower_index;
	int higher_index;
	int initial_thread;
} tracker;

//keep track of elements in the array
int count = 0;
//the array of integers
int *list;
int thread_count = 0;
//sort and merge elements within given boundary 
void *merge(int lower_index, int higher_index)
{
	//separate the subarray into two parts (as threads must merge two arrays)
	int mid = (lower_index+higher_index)/2;
	int left = lower_index;
	int right = mid+1;
	
	//sub array to hold the values in this boundary to later update global list
	int sub_array[higher_index - lower_index +1];
	int current = 0;
	while(left <= mid && right <=higher_index )
	{
		/*append smallest of elements from sublist passed by 
				threads into sub_array*/
		if (list[left] < list[right]){
			sub_array[current++] = list[left++];
		}else{
			sub_array[current++] = list[right++];
		}
	}

	//append any remaining elements in left part to sub array
	while(left<=mid) sub_array[current++] = list[left++];
	//append any remaining elements in right part to sub array
	while(right<=higher_index) sub_array[current++] = list[right++];

	/*replace the given boundaries in the global list with that of sorted
		sub array*/

	for (int i = 0; i < (higher_index - lower_index +1) ; i++) list[lower_index+i] = sub_array[i];
	
	return;
}


void *merge_sort(void *boundary){
	tracker* sub_indices = (tracker*) boundary; 
	//base case
	 if(sub_indices->lower_index >= sub_indices->higher_index) 
		return;
	if (sub_indices->initial_thread==1|| sub_indices->initial_thread==2) {
		//to synchronise printf
		if(sub_indices->initial_thread==2) sleep(1);
		printf("\n\nThe partition for thread %d is \n",sub_indices->initial_thread-1);
		for (int i = sub_indices->lower_index; i < sub_indices->higher_index; ++i)
		{
			printf("%d  ",list[i]);
		}
	}

	int mid = (sub_indices->lower_index+sub_indices->higher_index)/2;
	tracker sub_boundaries[2];
	//each thread spawns two childs
	pthread_t threads[2];
	//update the tracker for the threads
	sub_boundaries[0].lower_index = sub_indices->lower_index;
	sub_boundaries[0].higher_index = mid;
	sub_boundaries[0].initial_thread=5;
	sub_boundaries[1].lower_index = mid+1;
	sub_boundaries[1].higher_index = sub_indices->higher_index;
	sub_boundaries[1].initial_thread=5;
	thread_count++;
	/*create two threads for each threads and call merge sort with the
	  boundaries*/
	pthread_create(&threads[0],NULL,merge_sort,&sub_boundaries[0]);
	pthread_create(&threads[1],NULL,merge_sort,&sub_boundaries[1]);
	
	pthread_join(threads[0],NULL);
	pthread_join(threads[1],NULL);	
	
	//resume execution from here when thread done 
	merge(sub_indices->lower_index,sub_indices->higher_index);
	pthread_exit(NULL);


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
    printf("\nThe original list is: \n");
    for (int i = 0; i < count; ++i)
    {
    	printf("%d  ",list[i]);
    }
    //initialise indices 
	tracker root_boundary[2];
	root_boundary[0].lower_index = 0;
	root_boundary[0].higher_index = count/2;
	root_boundary[0].initial_thread=1;
	root_boundary[1].lower_index = (count/2)+1;
	root_boundary[1].higher_index = count-1;
	root_boundary[1].initial_thread=2;

	pthread_t thread[2];
	//root thread creates child threads
	pthread_create(&thread[0],NULL,merge_sort,&root_boundary[0]);
	pthread_create(&thread[1],NULL,merge_sort,&root_boundary[1]);
	pthread_join(thread[0],NULL);
	pthread_join(thread[1],NULL);
	merge(0,count-1);
	//array is sorted at this point
	printf("\n\nThe sorted array is: \n");
	for (int i = 0; i < count; ++i)
	{
		printf("%d  ",list[i]);
	}
    
    return 0;
}

