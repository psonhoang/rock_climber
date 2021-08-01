#include <stdbool.h>
#include "data_structs.h"

#ifndef _HEAP_H_
#define _HEAP_H_

typedef point elem;

typedef struct heap_header {
	elem **data;
	point *first_hold;
	point *last_hold;
	int length;
	int next;
} heap;

double cost(elem *i, heap *H, move *last_move);

bool has_priority(elem *p1, elem *p2, heap *H);

heap* empty_heap(point *first_hold, point *last_hold, int length);

void free_heap(heap *H);

void insert(heap *H, elem *i);

void insert_h(heap *H, elem *i, move *last_move);

elem* deleteMin(heap *H);

elem* deleteMin_h(heap *H, move *last_move);

void percolateDown(heap *H, int i);

void percolateDown_h(heap *H, int i, move *last_move);

void percolateUp(heap *H, int v);

void percolateUp_h(heap *H, int v, move *last_move);

#endif
