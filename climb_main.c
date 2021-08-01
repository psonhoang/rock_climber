#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib/data_structs.h"
#include "lib/legal_position.h"
#include "lib/climb.h"

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

void read_wall(const char *FILENAME, holds_list *holds) {
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
    		int x = 0;
    		int y = 0;
    		int counter = 1;
    		while(tokenPtr != NULL) {
    			if(counter == 1) {
    				// First number in line is x-coordinate
    				x = atoi(tokenPtr);
    				counter++;
    			} else if(counter == 2) {
    				// Second number in line is y-coordinate
    				y = atoi(tokenPtr);
    			}
    			tokenPtr = strtok(NULL, " \n");
    		}
    		// Add hold to holds list
    		add_hold_node(holds, new_point(x, y));
    	}
    }

}

int main(int argc, char *argv[]) {
	if(argc == 3) {
		/* Input files */
		char *SIZE_FILE = argv[1];
		char *WALL_FILE = argv[2];

		/* Read arm, torso, leg sizes */
		int arm, torso, leg = 0;
		read_sizes(SIZE_FILE, &arm, &torso, &leg);
		
		/* Read holds' coordinates */
		holds_list *holds = new_holds_list();
		read_wall(WALL_FILE, holds);

		/* Climb */
		climb(holds, arm, torso, leg);

		free_holds_list(holds);
	} else {
		printf("Invalid number of commandline arguments!\n");
	}

	return 0;
}
