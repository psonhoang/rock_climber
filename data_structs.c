#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include "lib/data_structs.h"

/* Point */
point* new_point(int x, int y) {
	point *newPoint = malloc(sizeof(point));
	if(newPoint == NULL) return NULL;
	newPoint->x = x;
	newPoint->y = y;

	return newPoint;
}

bool is_same_point(point *p1, point *p2) {
	return (p1->x == p2->x) && (p1->y == p2->y);
}

double distance(point *p1, point *p2) {
	return sqrt((p1->x - p2->x) * (p1->x - p2->x) + 
		(p1->y - p2->y) * (p1->y - p2->y));
}

/* Position */
position *new_position(point *LH, point *RH, point *LF, point *RF) {
	position *new_pos = malloc(sizeof(position));
	if(new_pos == NULL) return NULL;
	new_pos->LH = LH;
	new_pos->RH = RH;
	new_pos->LF = LF;
	new_pos->RF = RF;

	return new_pos;
}

void free_position(position *pos) {
	if(pos == NULL) return;
	free(pos->LH);
	free(pos->RH);
	free(pos->LF);
	free(pos->RF);
	free(pos);

	return;
}

bool is_same_position(position *pos, point *LH,  point *RH, point *LF, point *RF) {
	return is_same_point(pos->LH, LH) && is_same_point(pos->RH, RH) &&
			is_same_point(pos->LF, LF) && is_same_point(pos->RF, RF);
}

/* List of holds */
hold_node* new_hold_node(point *coord) {
	hold_node *newHoldNode = malloc(sizeof(hold_node));
	if(newHoldNode == NULL) return NULL;
	newHoldNode->coord = coord;
	newHoldNode->next = NULL;

	return newHoldNode;
}

holds_list* new_holds_list() {
	holds_list *new_holds_list = malloc(sizeof(holds_list));
	if(new_holds_list == NULL) return NULL;

	new_holds_list->length = 0;
	new_holds_list->head = NULL;
	new_holds_list->last = NULL;

	return new_holds_list;
}

void add_hold_node(holds_list *holds, point *hold) {
	if(holds == NULL || hold == NULL) return;
	hold_node *new_hold = new_hold_node(hold);
	if(holds->head == NULL) {
		holds->head = new_hold;
		holds->last = new_hold;
	} else {
		holds->last->next = new_hold;
		holds->last = new_hold;
	}

	holds->length++;

	return;
}

void free_holds_list(holds_list *holds) {
	hold_node *current = holds->head;
	hold_node *temp  = current;
	while(current != NULL) {
		current = current->next;
		free(temp->coord);
		free(temp);
		temp = current;
	}

	free(holds);

	return;
}

/* Move & List of moves (moves sequence) */
move* new_move(char* body_part, point *coord, double cost) {
	move *newMove = malloc(sizeof(move));
	if(newMove == NULL) return NULL;
	newMove->body_part = body_part;
	newMove->coord = coord;
	newMove->cost = cost;
	newMove->next = NULL;

	return newMove;
}

bool is_same_move(move *m1, char *body_part, point *coord) {
	return strcmp(m1->body_part, body_part) == 0 && is_same_point(m1->coord, coord);
}

moves_sequence* init_sequence() {
	moves_sequence *new_sequence = malloc(sizeof(moves_sequence));
	if(new_sequence == NULL) return NULL;
	new_sequence->count = 0;
	new_sequence->first_move = NULL;
	new_sequence->last_move = NULL;

	return new_sequence;
}

void add_move(moves_sequence *moves, char* body_part, point *coord, double cost) {
	move* newMove = new_move(body_part, coord, cost);
	if(moves == NULL || newMove == NULL) return;
	if(moves->count == 0 && moves->first_move == NULL) {
		moves->first_move = newMove;
		moves->last_move = newMove;
	} else {
		moves->last_move->next = newMove;
		moves->last_move = newMove;
	}

	moves->count++;
}

bool contains_move(moves_sequence *moves, char *body_part, point *coord) {
	move *curr = moves->first_move;
	while(curr != NULL) {
		if(is_same_move(curr, body_part, coord)) return true;
		curr = curr->next;
	}

	return false;
}

void free_moves(moves_sequence *moves) {
	move *current_move = moves->first_move;
	move *temp = current_move;
	while(current_move != NULL) {
		current_move = current_move->next;
		free(temp->coord);
		free(temp);
		temp = current_move;
	}

	free(moves);

	return;
}
