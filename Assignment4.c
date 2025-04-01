#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 4
#define REQUESTS 20

void sort_list(int *list, int size);

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
	printf("\nLOOK DISK SCHEDULING ALGORITHM:\n\n");
	
	int *sorted_requests = malloc(REQUESTS * sizeof(int));
	for(int i=0; i<REQUESTS; i++){
		sorted_requests[i] = cylinder_numbers[i];
	} // creating copy of array for look

	sort_list(sorted_requests, REQUESTS);
	
	
	// Finding starting point
	if(strcmp(initial_disk_direction, "LEFT") == 0){

		head_movements = abs(sorted_requests[0]-initial_disk_position) + abs(sorted_requests[0]-sorted_requests[REQUESTS-1]);
		
		int max_left = sorted_requests[0];
		int max_left_idx = 0;
		for(int i=1; i<REQUESTS; i++){
			if(sorted_requests[i] > initial_disk_position){
				break;			
			}
			if(sorted_requests[i] > max_left && sorted_requests[i] <= initial_disk_position){
				max_left = sorted_requests[i];
				max_left_idx = i;			
			}
		}
		// Printing order
		for(int i=max_left_idx; i>=0; i--){
			printf("%d, ", sorted_requests[i]);
		}
		for(int i=max_left_idx+1; i<REQUESTS; i++){

			if(i == REQUESTS-1){
				printf("%d", sorted_requests[i]);
			}else{
				printf("%d, ", sorted_requests[i]);
			}
		}
	}else{

		head_movements = abs(sorted_requests[REQUESTS-1]-initial_disk_position) + abs(sorted_requests[0]-sorted_requests[REQUESTS-1]);
		
		int min_right = sorted_requests[REQUESTS-1];
		int min_right_idx = REQUESTS-1;
		for(int i=REQUESTS-2; i>=0; i--){
			if(sorted_requests[i] < initial_disk_position){
				break;
			}
			else if(sorted_requests[i] < min_right && sorted_requests[i] >= initial_disk_position){
				min_right = sorted_requests[i];
				min_right_idx = i;
			}
		}
		// Printing order
		for(int i=min_right_idx; i<REQUESTS; i++){
			printf("%d, ", sorted_requests[i]);
		}
		for(int i=min_right_idx-1; i>=0; i--){
			if(i == 0){
				printf("%d", sorted_requests[i]);
			}else{
				printf("%d, ", sorted_requests[i]);
			}
		}
	}
	printf("\n\nLOOK - Total head movements = %d\n",head_movements);
	free(sorted_requests);
	head_movements = 0;

    // C-LOOK

    return 0;
}

// Selection sort for request lists.
void sort_list(int *list, int size) {
	for(int i=0; i<size; i++){
		int min = list[i];
		int min_idx = i;
		for(int j=i+1; j<size; j++){
			if(list[j] < min){
				min = list[j];
				min_idx = j;
			}
		}
		int temp = list[i];
		list[i] = list[min_idx];
		list[min_idx] = temp;
	}
}
