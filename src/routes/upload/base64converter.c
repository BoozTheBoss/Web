// C program to implement
// the above approach
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 2*10^9

// Driver code
int main()
{
	FILE* fileptr;
	char ch;
    char buffer[BUFFER_SIZE];
    char all_read_file [2*10^9];

	// Opening file in reading mode
	fileptr = fopen("Books.wav", "r");

	if (NULL == fileptr) {
		printf("file can't be opened \n");
	}

	printf("content of this file are %lu \n", sizeof(*fileptr));
    int counter = 0;
    while (1) {
    size_t num_read = fread(all_read_file, 1, BUFFER_SIZE, fileptr);
    counter++;
    
    if (num_read == 0) // end of file.
        break;    
    }
    printf("%s", all_read_file); 
    printf("%d", counter);

	// Closing the file
	fclose(fileptr);
	return 0;
}
