#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib/data_structs.h"
#include "lib/legal_position.h"

// #define SIZE_FILE	"size.txt"
// #define COORDS_FILE	"coordinates.txt"

void read_sizes(const char *FILENAME, int *arm, int *torso, int *leg) {
	char buffer[500];
	FILE *fp;
	int line = 0;

	if ((fp = fopen(FILENAME, "r")) == NULL) {
        printf("Could not open %s\n", FILENAME);
        exit(1);
    }

    while(!feof(fp)) {
    	if(fgets(buffer, 500, fp) != NULL) {
             if(line == 0) *arm = atoi(buffer);
             else if(line == 1) *torso = atoi(buffer);
             else if(line == 2) *leg = atoi(buffer);
             line++;
        }
    }
}

void read_coordinates(const char *FILENAME, int *coords) {
	char buffer[500];
	FILE *fp;

	if ((fp = fopen(FILENAME, "r")) == NULL) {
        printf("Could not open %s\n", FILENAME);
        exit(1);
    }

    while(!feof(fp)) {
    	if(fgets(buffer, 500, fp) != NULL) {
    		char *tokenPtr;
    		tokenPtr = strtok(buffer, " \n");
    		int i = 0;
    		while(tokenPtr != NULL) {
    			coords[i] = atoi(tokenPtr);
    			tokenPtr = strtok(NULL, " \n");
    			i++;
    		}
        }
    }
}

int main(int argc, char *argv[]) {
	if(argc == 3) {
		char *SIZE_FILE = argv[1];
		char *COORDS_FILE = argv[2];
		int arm, torso, leg = 0;

		read_sizes(SIZE_FILE, &arm, &torso, &leg);
		// printf("Arm: %d\n", arm);
		// printf("Torso: %d\n", torso);
		// printf("Leg: %d\n", leg);

		int coords[8];

		read_coordinates(COORDS_FILE, coords);

		printf("%d\n", legal_position(arm, torso, leg, coords));
	} else {
		printf("Invalid number of command line arguments!\n");
	}

	return 0;
}
