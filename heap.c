#include <stdio.h>
#include <stdlib.h>
#include "lib/heap.h"

double cost(elem *i, heap *H, move *last_move) {
	if(is_same_point(i, H->last_hold)) return  0;
	if(last_move == NULL) return distance(i, H->last_hold) + distance(i, H->first_hold);

	return last_move->cost + distance(i, H->last_hold) + distance(i, H->first_hold);
}

bool has_priority(elem *p1, elem *p2, heap *H) {
	if(is_same_point(p1, H->last_hold)) return true;
	if(is_same_point(p2, H->last_hold)) return false;
	if(distance(p1, H->last_hold) < distance(p2, H->last_hold)) return true;
	else if(distance(p1, H->last_hold) == distance(p2, H->last_hold)) {
		return distance(p1, H->first_hold) > distance(p2, H->first_hold);
	}

	return false;
}

heap *empty_heap(point *first_hold, point *last_hold, int length) {
	heap *H = malloc(sizeof(heap));
	if(H == NULL) return NULL;
	H->first_hold = new_point(first_hold->x, last_hold->y);
	H->last_hold = new_point(last_hold->x, last_hold->y);
	H->length = length+1;
	H->data = malloc(H->length * sizeof(elem));
	H->next = 1;

	return H;
}

void free_heap(heap *H) {
	free(H->first_hold);
	free(H->last_hold);
	for(int i = 1; i < H->next; i++) {
		free(H->data[i]);
	}
	free(H->data);
	free(H);

	return;
}

/* Percolate the element at index i down to its location that satisifies the heap order property */ 
void percolateDown(heap *H, int i) {
	int size = H->next-1;
	int target = 0;
	while(2*i <= size) {
		int left = 2 * i;
		int right = left + 1;
		if(right > size || distance(H->last_hold, H->data[left]) < distance(H->last_hold,H->data[right])) {
			target = left;
		} else {
			target = right;
		}
		if(distance(H->last_hold, H->data[target]) < distance(H->last_hold, H->data[i])) {
			elem *temp = H->data[i];
			H->data[i] = H->data[target];
			H->data[target] = temp;
			i = target;
		} else break;
	}
}

void percolateDown_h(heap *H, int i, move *last_move) {
	int size = H->next-1;
	int target = 0;
	while(2*i <= size) {
		int left = 2 * i;
		int right = left + 1;
		if(right > size 
			|| cost(H->data[left], H, last_move) < cost(H->data[right], H, last_move)
			|| distance(H->data[left], H->last_hold) < distance(H->data[left], H->last_hold)) {
			target = left;
		} else {
			target = right;
		}
		if(cost(H->data[target], H, last_move) < cost(H->data[i], H, last_move)) {
			elem *temp = H->data[i];
			H->data[i] = H->data[target];
			H->data[target] = temp;
			i = target;
		} else break;
	}
}

/* Percolate the element at index i up to its location that satisfies the heap order property */
void percolateUp(heap *H, int i) {
	while(i > 1 && distance(H->last_hold, H->data[i]) < distance(H->last_hold, H->data[i/2])) {
		elem *temp = H->data[i];
		H->data[i] = H->data[i/2];
		H->data[i/2] = temp;
		i = i/2;
	}
}

void percolateUp_h(heap *H, int i, move *last_move) {
	while(i > 1 && (cost(H->data[i], H, last_move) < cost(H->data[i/2], H, last_move) 
					|| distance(H->last_hold, H->data[i]) < distance(H->last_hold, H->data[i/2]))) {
		elem *temp = H->data[i];
		H->data[i] = H->data[i/2];
		H->data[i/2] = temp;
		i = i/2;
	}
}

void insert(heap *H, elem *p) {
	H->data[H->next] = p;
	percolateUp(H, H->next);
	H->next++;
}

void insert_h(heap *H, elem *p, move *last_move) {
	H->data[H->next] = p;
	percolateUp_h(H, H->next, last_move);
	H->next++;
}

elem* deleteMin(heap *H) {
	/* Assuming that H is not empty */
	elem *min = H->data[1];	// the root
	H->data[1] = H->data[H->next-1];
	H->next--;

	if(H->next > 1) percolateDown(H, 1);

	return min;
}

elem* deleteMin_h(heap *H, move *last_move) {
	/* Assuming that H is not empty */
	elem *min = H->data[1];	// the root
	H->data[1] = H->data[H->next-1];
	H->next--;

	if(H->next > 1) percolateDown_h(H, 1, last_move);

	return min;
}
