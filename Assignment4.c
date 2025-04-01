#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define BUFFER_SIZE 4
#define REQUESTS 20
#define MAX_CYLINDER 299
#define MIN_CYLINDER 0

void sort_list(int *list, int size);
void scan_algorithm(int sorted_requests[], int initial_disk_position, bool start_left);
void c_scan_algorithm(int sorted_requests[], int initial_disk_position, bool start_left);

int main(int argc, char *argv[]) {

    // Verifying command line arguments
    if(argc != 3){
        printf("Invalid number of command arguments.");
        return 1;
    }

    int initial_disk_position = atoi(argv[1]);
	bool start_left;

	// get initial disk direction
	if (strcmp(argv[2], "LEFT") == 0) {
		start_left = true;
	} else if (strcmp(argv[2], "RIGHT") == 0) {
		start_left = false;
	} else {
		printf("Invalid format of initial direction argument [LEFT/RIGHT].");
		return 1;
	}

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
	printf("Direction of Head: %s\n\n", argv[2]);

	// TODO: remove
	//int *sorted_requests = malloc(REQUESTS * sizeof(int));
	int sorted_requests[REQUESTS];
	for(int i=0; i<REQUESTS; i++){
		sorted_requests[i] = cylinder_numbers[i];
	}

	sort_list(sorted_requests, REQUESTS);

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
	scan_algorithm(sorted_requests, initial_disk_position, start_left);

    // C-SCAN
	c_scan_algorithm(sorted_requests, initial_disk_position, start_left);

    // LOOK
	printf("\nLOOK DISK SCHEDULING ALGORITHM:\n\n");

	// Checking starting point
	if(start_left){

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
	head_movements = 0;

    // C-LOOK

	// TODO: remove
	//free(sorted_requests);
    return 0;
}

void scan_algorithm(int sorted_requests[], int initial_disk_position, bool start_left) {
	printf("\nSCAN DISK SCHEDULING ALGORITHM:\n\n");
	int head_movements;
	int start_idx;

	// Checking if we start left or right
	if (start_left) {
		// Do we have to do a return trip or not?
		bool go_back = sorted_requests[REQUESTS - 1] > initial_disk_position;

		// Calculating total required head movement
		if (go_back) {
			head_movements = abs(initial_disk_position - MIN_CYLINDER) + abs(MIN_CYLINDER - sorted_requests[REQUESTS - 1]);
		} else {
			head_movements = abs(initial_disk_position - sorted_requests[0]);
		}

		// Finding the index of the first request to serve
		start_idx = REQUESTS - 1;
		for (int i = 0; i < REQUESTS - 1; i++) {
			if (sorted_requests[i] <= initial_disk_position && sorted_requests[i + 1] > initial_disk_position) {
				start_idx = i;
				break;
			}
		}

		// Printing the order of requests
		if (go_back) {
			// First trip
			for (int i = start_idx; i >= 0; i--) {
				printf("%d, ", sorted_requests[i]);
			}
			// Return trip
			for (int i = start_idx + 1; i < REQUESTS; i++) {
				printf("%d, ", sorted_requests[i]);
			}
		} else {
			// Only one trip
			for (int i = REQUESTS - 1; i >= 0; i--) {
				printf("%d, ", sorted_requests[i]);
			}
		}

	} else {
		// Do we have to do a return trip or not?
		bool go_back = sorted_requests[0] < initial_disk_position;

		// Calculating total required head movement
		if (go_back) {
			head_movements = abs(initial_disk_position - MAX_CYLINDER) + abs(MAX_CYLINDER - sorted_requests[0]);
		} else {
			head_movements = abs(initial_disk_position - sorted_requests[REQUESTS - 1]);
		}

		// Finding the index of the first request to serve
		start_idx = 0;
		for (int i = REQUESTS - 1; i > 0; i--) {
			if (sorted_requests[i] >= initial_disk_position && sorted_requests[i - 1] < initial_disk_position) {
				start_idx = i;
				break;
			}
		}

		// Printing the order of requests
		if (go_back) {
			// First trip
			for (int i = start_idx; i < REQUESTS; i++) {
				printf("%d, ", sorted_requests[i]);
			}
			// Return trip
			for (int i = start_idx - 1; i >= 0; i--) {
				printf("%d, ", sorted_requests[i]);
			}
		} else {
			// Only one trip
			for (int i = 0; i < REQUESTS; i++) {
				printf("%d, ", sorted_requests[i]);
			}
		}
	}
	printf("\n\nSCAN - Total head movements: %d\n", head_movements);
}

void c_scan_algorithm(int sorted_requests[], int initial_disk_position, bool start_left) {
	printf("\nC-SCAN DISK SCHEDULING ALGORITHM:\n\n");
	int head_movements;
	int start_idx;

	// Checking if we start left or right
	if (start_left) {
		// Do we have to do a return trip or not?
		bool go_back = sorted_requests[REQUESTS - 1] > initial_disk_position;

		// Finding the index of the first request to serve
		start_idx = REQUESTS - 1;
		for (int i = 0; i < REQUESTS - 1; i++) {
			if (sorted_requests[i] <= initial_disk_position && sorted_requests[i + 1] > initial_disk_position) {
				start_idx = i;
				break;
			}
		}

		// Calculating total required head movement
		if (go_back) {
			head_movements = abs(initial_disk_position - MIN_CYLINDER) + abs(MIN_CYLINDER - MAX_CYLINDER) + abs(MAX_CYLINDER - sorted_requests[start_idx + 1]);
		} else {
			head_movements = abs(initial_disk_position - sorted_requests[0]);
		}

		// Printing the order of requests
		if (go_back) {
			// First trip
			for (int i = start_idx; i >= 0; i--) {
				printf("%d, ", sorted_requests[i]);
			}
			// Return trip
			for (int i = REQUESTS - 1; i > start_idx; i--) {
				printf("%d, ", sorted_requests[i]);
			}
		} else {
			// Only one trip
			for (int i = REQUESTS - 1; i >= 0; i--) {
				printf("%d, ", sorted_requests[i]);
			}
		}

	} else {
		// Do we have to do a return trip or not?
		bool go_back = sorted_requests[0] < initial_disk_position;

		// Finding the index of the first request to serve
		start_idx = 0;
		for (int i = REQUESTS - 1; i > 0; i--) {
			if (sorted_requests[i] >= initial_disk_position && sorted_requests[i - 1] < initial_disk_position) {
				start_idx = i;
				break;
			}
		}

		// Calculating total required head movement
		if (go_back) {
			head_movements = abs(initial_disk_position - MAX_CYLINDER) + abs(MAX_CYLINDER - MIN_CYLINDER) + abs(MIN_CYLINDER - sorted_requests[start_idx - 1]);
		} else {
			head_movements = abs(initial_disk_position - sorted_requests[REQUESTS - 1]);
		}

		// Printing the order of requests
		if (go_back) {
			// First trip
			for (int i = start_idx; i < REQUESTS; i++) {
				printf("%d, ", sorted_requests[i]);
			}
			// Return trip
			for (int i = 0; i < start_idx; i++) {
				printf("%d, ", sorted_requests[i]);
			}
		} else {
			// Only one trip
			for (int i = 0; i < REQUESTS; i++) {
				printf("%d, ", sorted_requests[i]);
			}
		}
	}
	printf("\n\nC-SCAN - Total head movements: %d\n", head_movements);
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
