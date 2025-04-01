#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 4
#define REQUESTS 20

int main(int argc, char *argv[]) {

    // Verifying command line arguments
    if(argc != 3){
        printf("Invalid number of command arguments.");
        return 1;
    }

    int initial_disk_position = atoi(argv[1]);
    const char *initial_disk_direction = argv[2];

    // Reading request file
    FILE *fptr = fopen("request.bin","rb");
    if(fptr == NULL){
        printf("Error opening file.");
        return 1;
    }

    int cylinder_numbers[REQUESTS];
    int idx = 0;

    // Creating list of cylinder numbers
	while(fread(&cylinder_numbers[idx], sizeof(int), 1, fptr) == 1) {
        idx += 1;
        if(idx >= REQUESTS) {
            break;
        }
    }

    fclose(fptr);

	printf("Total Requests = %d\n", REQUESTS);
	printf("Initial Head Position: %d\n", initial_disk_position);
	printf("Direction of Head: %s\n\n", initial_disk_direction);

    // FCFS
	printf("FCFS DISK SCHEDULING ALGORITHM:\n\n");

	int head_movements = abs(cylinder_numbers[0]-initial_disk_position);

	printf("%d", cylinder_numbers[0]);
	for(int i=1; i<REQUESTS; i++){
		printf(", %d", cylinder_numbers[i]);
		head_movements += abs(cylinder_numbers[i] - cylinder_numbers[i-1]);
	}
	printf("\n\nFCFS - Total head movements = %d\n",head_movements);
	
	head_movements = 0;

    // SSTF
	printf("\nSSTF DISK SCHEDULING ALGORITHM:\n\n");

	int position = initial_disk_position;
	int *cylinder_numbers_copy = malloc(REQUESTS * sizeof(int));
	for(int i=0; i<REQUESTS; i++){
		cylinder_numbers_copy[i] = cylinder_numbers[i];
	} // creating copy of array for sstf

	for(int i=0; i<REQUESTS; i++){
		int min_dist = 301;
		int min_idx = -1;
		for(int j=0; j<REQUESTS; j++){
			if(abs(cylinder_numbers_copy[j]-position) < min_dist){
				min_dist = abs(cylinder_numbers_copy[j]-position);
				min_idx = j;
			}
		}
		head_movements += min_dist;
		if(i != REQUESTS -1){
			printf("%d, ", cylinder_numbers_copy[min_idx]);
		}else{
			printf("%d", cylinder_numbers_copy[min_idx]);
		} // for printing uniformly
				
		position = cylinder_numbers_copy[min_idx];
		cylinder_numbers_copy[min_idx] = 9000; // Mark as visited

	}
	printf("\n\nSSTF - Total head movements = %d\n",head_movements);
	free(cylinder_numbers_copy);
	head_movements = 0;

    // SCAN

    // C-SCAN

    // LOOK

    // C-LOOK

    return 0;
}