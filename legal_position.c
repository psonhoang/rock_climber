#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "lib/data_structs.h"
#include "lib/legal_position.h"

int legal_position(int A, int T, int L, int *coords) {
	int x1 = coords[0];
	int y1 = coords[1];
	int x2 = coords[2];
	int y2 = coords[3];
	int x3 = coords[4];
	int y3 = coords[5];
	int x4 = coords[6];
	int y4 = coords[7];

	/* coords are within [0, 1000) range */
	bool hasValidCoords = (x1 >= 0 && x1 < 1000 &&
						y1 >= 0 && y1 < 1000 &&
						x2 >= 0 && x2 < 1000 &&
						y2 >=0 && y2 < 1000 &&
						x3 >= 0 && x3 < 1000 &&
						y3 >= 0 && y3 < 1000 &&
						x4 >= 0 && x4 < 1000 &&
						y4 >= 0 && y4 < 1000);

	point *l_hand = new_point(x1, y1);
	point *r_hand = new_point(x2, y2);
	point *l_foot = new_point(x3, y3);
	point *r_foot = new_point(x4, y4);

	/* "Two limbs can share the same hold" */
	bool twoLimbsPerHold = (!(is_same_point(l_hand, r_hand) && is_same_point(r_hand, l_foot)) &&
						!(is_same_point(l_hand, r_hand) && is_same_point(r_hand, r_foot)) &&
						!(is_same_point(l_foot, r_foot) && is_same_point(r_foot, l_hand)) &&
						!(is_same_point(l_foot, r_foot) && is_same_point(r_foot, r_hand)));
	// bool twoLimbsPerHold = true;

	bool hasValidTorso = false;
	point *top_torso = new_point(0, 0);
	point *low_torso = new_point(0, 0);
	for(int y = T; y < 1000; y++) {
		for(int x = 0; x < 1000; x++) {
			top_torso->x = x;
			top_torso->y = y;
			low_torso->x = x;
			low_torso->y = y - T;
			hasValidTorso = (distance(l_hand, top_torso) <= A && distance(r_hand, top_torso) <= A &&
							distance(l_foot, low_torso) <= L && distance(r_foot, low_torso) <= L);

			if(!hasValidTorso) {
				top_torso->y = y - T;
				low_torso->y = y;
				hasValidTorso = (distance(l_hand, top_torso) <= A && distance(r_hand, top_torso) <= A &&
								distance(l_foot, low_torso) <= L && distance(r_foot, low_torso) <= L);
			}

			if(hasValidTorso) break;
		}

		if(hasValidTorso) break;
	}

	free(l_hand);
	free(r_hand);
	free(l_foot);
	free(r_foot);
	free(top_torso);
	free(low_torso);

	if(T < 1000 && T >= 0 && A > 0 && L > 0 && hasValidCoords && twoLimbsPerHold && hasValidTorso) return 1;
	
	return 0;
}

bool legal_pos(int A, int T, int L, point *LH, point *RH, point *LF, point *RF) {
	int coords[8];
	coords[0] = LH->x;
	coords[1] = LH->y;
	coords[2] = RH->x;
	coords[3] = RH->y;
	coords[4] = LF->x;
	coords[5] = LF->y;
	coords[6] = RF->x;
	coords[7] = RF->y;

	if(legal_position(A, T, L, coords) == 1) return true;
	
	return false;
}
