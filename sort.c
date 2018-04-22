#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

struct tracker{
	int low;
	int high;
	int is_sorter;
};


int *nums;

void* thread_entry_point(void *specs);
void merge_sort(int left, int right);
void merge(int left, int right, int mid);


int main(int argc, char *argv[]) {
	FILE *fp;
	fp = fopen(argv[1], "r");
	char *number = NULL;
	size_t len = 0;
	int count = -1;
	ssize_t line;
	//count number of numbers in the text file
	while ((line = getline(&number, &len, fp)) != -1)
		count++;
	
	//allocate space for all numbers
	nums = malloc(count*sizeof(int));
	fp = fopen(argv[1], "r");
	printf("Numbers originally in file: \n");
	int i = 0;
	while ((line = getline(&number, &len, fp)) != -1) {
		nums[i] = atoi(number);
		i++;
		printf("%s", number);
	}

	//declare the threads including merger thread
	pthread_t thread_1;
	pthread_t thread_2;
	pthread_t thread_12;
	
	//divide the numbers in file according to index
	struct tracker t_1;
	t_1.low = 0;
	t_1.high = count/2;
	t_1.is_sorter = 1;
	printf("\n\nNumbers to be sorted by thread 1: \n");
	for (i = 0; i <= t_1.high; i++) 
		printf("%d ", nums[i]);
	

	struct tracker t_2;
	t_2.low = (count/2) + 1;
	t_2.high = count;
	t_2.is_sorter = 1;
	printf("\n\nNumbers to be sorted by thread 2: \n");
	for (i = t_2.low; i <= t_2.high; i++) 
		printf("%d ", nums[i]);

	// Do mering for the third thread
	struct tracker t_12;
	t_12.low = 0;
	t_12.high = count;
	t_12.is_sorter = 0;

	pthread_create(&thread_1, NULL, thread_entry_point, &t_1);
	pthread_create(&thread_2, NULL, thread_entry_point, &t_2);
	pthread_join(thread_1, NULL);
	pthread_join(thread_2, NULL);

	pthread_create(&thread_12, NULL, thread_entry_point, &t_12);
	pthread_join(thread_12, NULL);

	printf("\n\nThe numbers after sorted are : \n");
	for (i = 0; i <= count; i++) 
		printf("%d ", nums[i]);
	
	printf("\n");
	pthread_exit(0);

	return 0;
}

void* thread_entry_point(void *specs){
	struct tracker *temp = specs;
	//if it is a sorting thread
	if (temp->is_sorter == 1) {
		merge_sort(temp->low, temp->high);
	} else {
		//it is the merger thread
		int mid = (temp->low + temp->high)/2;
		merge(temp->low, temp->high, mid);
	}

}

void merge_sort(int left, int right) {
	if (left < right) {
		int mid = (left + right)/2;
		merge_sort(left, mid);
		merge_sort(mid+1, right);
		merge(left, right, mid);
	}
}

void merge(int left, int right, int mid) {
	int first_half = mid - left +1;
	int second_half = right - mid;
	int *first = malloc(first_half * sizeof(int));
	int *last = malloc(second_half * sizeof(int));
	
	int i, j;
	// Fill the two arrays with nums 
	for (i = 0; i < first_half; i++) {
		first[i] = nums[left+i];
	} 

	for (j = 0; j < second_half; j++) {
		last[j] = nums[mid+1+j];
	}

	i = 0; j = 0;
	int k;
	k = left;
	while (i < first_half && j < second_half) {
		if (first[i] <= last[j]) {
			nums[k] = first[i];
			i++;		
		} else {
			nums[k] = last[j];
			j++;
		}
		k++;
	}

	// Check the length of a array and merge
	while (i < first_half) {
		nums[k] = first[i];
		i++; k++;
	}

	while (j < second_half) {
		nums[k] = last[j];
		j++; k++;	
	}

}
