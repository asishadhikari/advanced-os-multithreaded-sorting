# Advanced OS - multithreaded Sorting with MergeSort

## Project URL: https://github.com/asishadhikari/advanced-os-multithreaded-sorting

### Assumptions:
- [X] Integers in **data.txt** are separated by newline ```\n``` character
- [X] The last line does not have a newline character 
- [X] No empty line exists
- [X] OS allows creation of n threads for n input size

### To Run:
* compile program as ```gcc -o sort sort.c -l pthread``` 
  * *You might get some compiler warnings, ignore them as they are part of safety check.*
* run the executable with accompanying test file as ```./sort data.txt```
* make sure that any test file you run **do not conflict with the assumptions!!**