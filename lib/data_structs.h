#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#ifndef _DATA_STRUCTS_H_
#define _DATA_STRUCTS_H_

/* Point */
typedef struct point {
	int x;
	int y;
} point;

point* new_point(int x, int y);

bool is_same_point(point *p1, point *p2);

double distance(point *p1, point *p2);

/* Position */
typedef struct position {
	point *LH;
	point *RH;
	point *LF;
	point *RF;
} position;

position *new_position(point *LH, point *RH, point *LF, point *RF);

void free_position(position *pos);

bool is_same_position(position *pos, point *LH, point *RH, point *LF, point *RF);

/* List of holds */
typedef struct hold_node {
	point *coord;
	struct hold_node *next;
} hold_node;

typedef struct hold_list {
	int length;
	hold_node *head;
	hold_node *last;
} holds_list;

hold_node* new_hold_node(point *coord);

holds_list* new_holds_list();

void add_hold_node(holds_list *holds, point *hold);

void free_holds_list(holds_list *holds);

/* Move & List of moves (moves sequence) */
typedef struct move {
	char* body_part;
	point *coord;
	double cost;
	struct move *next;
} move;

typedef struct sequence {
	int count;
	double path_cost;
	move *first_move;
	move *last_move;
} moves_sequence;

move* new_move(char* body_part, point *coord, double cost);

bool is_same_move(move *m1, char *body_part, point *coord);

moves_sequence* init_sequence();

void add_move(moves_sequence *moves, char* body_part, point *coord, double cost);

bool contains_move(moves_sequence *moves, char *body_part, point *coord);

void free_moves(moves_sequence *moves);

#endif
