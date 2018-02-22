#include <stdio.h>
#include <stdlib.h> 
int main(int argc, char** argv)
{
    FILE *fp;
    fp = fopen(argv[1], "r");
    if(!fp) return 1;

    //get the number of integers in file before allocating dynamic memory
    int count = 0;
    for (char c = getc(fp); c != EOF; c = getc(fp))
        if (c == '\n') // Increment count if this character is newline
            count = count + 1;
    
    //the list of numbers
    int* list = (int*)malloc(sizeof(int)*(count+1));
    //reset the filepointer
    fseek(fp, 0, SEEK_SET);

    char temp = '\n';
    int i = 0;
    //populate array with index 0 ->= count
    while(!feof(fp))
    {
    	fscanf(fp,"%d",&list[i]);
    	i++;
    }

    
    
    return 0;
}

