#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib/data_structs.h"
#include "lib/heap.h"
#include "lib/legal_position.h"
#include "lib/climb.h"

void print_moves(moves_sequence *moves) {
	if(moves->count == 0) {
		printf("Already reached last hold in initial position!\n");
		return;
	}

	move *current_move = moves->first_move;
	while(current_move != NULL) {
		printf("%s %d %d\n", current_move->body_part, current_move->coord->x, current_move->coord->y);
		current_move = current_move->next;
	}

	return;
}

position* init_position(holds_list *holds, int A, int T, int L) {
	point *holds_coords[holds->length];
	point *first_hold = holds->head->coord;
	point *last_hold = holds->last->coord;

	// Fill up holds_coords
	hold_node *curr = holds->head;
	heap *holds_heap = empty_heap(first_hold, last_hold, holds->length);
	while(curr != NULL) {
		insert(holds_heap, curr->coord);
		curr = curr->next;
	}
	for(int i = 0; i < holds->length; i++) {
		holds_coords[i] = deleteMin(holds_heap);
	}
	free_heap(holds_heap);

	// Sort holds_coords based on distance from last hold
	for(int i = 0; i < holds->length; i++) {
		for(int j = 0; j < i; j++) {
			if(distance(last_hold, holds_coords[i]) < distance(last_hold, holds_coords[j])) {
				point *temp = holds_coords[i];
				holds_coords[i] = holds_coords[j];
				holds_coords[j] = temp;
			}
		}
	}

	point *LH;
	point *RH;
	point *LF;
	point *RF;
	
	// Find positions for hands and feet closest to last hold
	for(int l = 0; l < holds->length; l++) {
		for(int m = 0; m < holds->length; m++) {
			LH = new_point(holds_coords[l]->x, holds_coords[l]->y);
			RH = new_point(holds_coords[l]->x, holds_coords[l]->y);
			if(first_hold->x <= last_hold->x && legal_pos(A, T, L, LH, RH, first_hold, holds_coords[m])) {
				LF = new_point(first_hold->x, first_hold->y);
				RF = new_point(holds_coords[m]->x, holds_coords[m]->y);
				return new_position(LH, RH, LF, RF);
			} else if(legal_pos(A, T, L, LH, RH, holds_coords[m], first_hold)) {
				LF = new_point(holds_coords[m]->x, holds_coords[m]->y);
				RF = new_point(first_hold->x, first_hold->y);
				return new_position(LH, RH, LF, RF);
			}
			free(LH);
			free(RH);
		}
	}

	return NULL;
}

bool finished(position *current_pos, point *last_hold) {
	return is_same_point(current_pos->LH, last_hold) || is_same_point(current_pos->RH, last_hold);
}

move* get_next_move(heap *H, holds_list *holds, moves_sequence *moves, int A, int T, int L, position *pos) {
	hold_node *curr = holds->head;
	while(curr != NULL) {
		if(moves->count == 0) insert_h(H, new_point(curr->coord->x, curr->coord->y), NULL);
		else insert_h(H, new_point(curr->coord->x, curr->coord->y), moves->last_move);
		curr = curr->next;
	}
	bool legal_lh = false;
	bool legal_rh = false;
	bool legal_lf = false;
	bool legal_rf = false;
	point *next_move = NULL;
	while(!legal_lh && !legal_rh && !legal_lf && !legal_rf) {
		// Get smallest hold away from last hold that can make a legal move
		if(next_move != NULL) free(next_move);

		if(moves->count == 0) next_move = deleteMin_h(H, NULL);
		else next_move = deleteMin_h(H, moves->last_move);
		
		legal_lh = legal_pos(A, T, L, next_move, pos->RH, pos->LF, pos->RF) &&
					!is_same_position(pos, next_move, pos->RH, pos->LF, pos->RF);
		legal_rh = legal_pos(A, T, L, pos->LH, next_move, pos->LF, pos->RF) && 
					!is_same_position(pos, pos->LH, next_move, pos->LF, pos->RF);
		legal_lf = legal_pos(A, T, L, pos->LH, pos->RH, next_move, pos->RF) &&
					!is_same_position(pos, pos->LH, pos->RH, next_move, pos->RF);
		legal_rf = legal_pos(A, T, L, pos->LH, pos->RH, pos->LF, next_move) &&
					!is_same_position(pos, pos->LH, pos->RH, pos->LF, next_move);
	}

	point* closest = pos->LH;
	char *closest_limb = "LH";
	double rh_dist = distance(pos->RH, next_move);
	double lf_dist = distance(pos->LF, next_move);
	double rf_dist = distance(pos->RF, next_move);
	if((!legal_lh || distance(closest, next_move) > rh_dist 
		|| contains_move(moves, closest_limb, next_move)) 
		&& rh_dist > 0 && !contains_move(moves, "RH", next_move) && legal_rh) {
		closest = pos->RH;
		closest_limb = "RH";
	}
	if(((!legal_lh && !legal_rh) || distance(closest, next_move) > lf_dist 
		|| contains_move(moves, closest_limb, next_move)) 
		&& lf_dist > 0 && !contains_move(moves, "LF", next_move) && legal_lf) {
		closest = pos->LF;
		closest_limb = "LF";
	}
	if(((!legal_lh && !legal_rh && !legal_lf) || distance(closest, next_move) > rf_dist 
		|| contains_move(moves, closest_limb, next_move)) 
		&& rf_dist > 0 && !contains_move(moves, "RF", next_move) && legal_rf) {
		closest = pos->RF;
		closest_limb = "RF";
	}

	// Update limb's position
	closest->x = next_move->x;
	closest->y = next_move->y;

	double cost = distance(next_move, H->first_hold) + distance(next_move, H->last_hold);

	if(moves->count > 0) cost = moves->last_move->cost + distance(next_move, H->last_hold);

	if(strcmp(closest_limb, "LH") == 0 || strcmp(closest_limb, "RH") == 0) cost += A;
	else cost += L;

	return new_move(closest_limb, next_move, cost);
}

void climb(holds_list *holds, int A, int T, int L) {
	moves_sequence *moves = init_sequence();
	point *first_hold = holds->head->coord;
	point *last_hold = holds->last->coord;
	/* Has access to:
		- Wall matrix
		- List of holds (first hold + last hold)
		- A, T, L
	*/

	/* Initial position */
	position *pos = init_position(holds, A, T, L);
	// position *pos = new_position(new_point(200, 60), new_point(220, 80), new_point(180, 20), new_point(220, 80));
	printf("Initial position: LH=(%d, %d); RH=(%d, %d); LF=(%d, %d); RF=(%d, %d)\n", pos->LH->x, pos->LH->y, pos->RH->x, pos->RH->y, pos->LF->x, pos->LF->y, pos->RF->x, pos->RF->y);
	/* Moves priority queue */
	heap *H = empty_heap(first_hold, last_hold, holds->length);
	/* PSEUDOCODE
		- Find all available moves with current position
		- Put availble moves into priority heap (priority heap size of number of holds)
		- Take optimal move
		- Repeat until a limb reaches last point
	*/
	while(!finished(pos, last_hold)) {
		move *next_move = get_next_move(H, holds, moves, A, T, L, pos);
		printf("%s:	%d %d\n", next_move->body_part, next_move->coord->x, next_move->coord->y);
		add_move(moves, next_move->body_part, next_move->coord, next_move->cost);
		free(next_move);
		free_heap(H);
		H = empty_heap(first_hold, last_hold, holds->length);
	}

	printf("%d moves\n", moves->count);
	print_moves(moves);

	free_position(pos);
	free_heap(H);
	free_moves(moves);
	
	return;
}
