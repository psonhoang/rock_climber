#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "data_structs.h"
#include "heap.h"

#ifndef _CLIMB_H_
#define _CLIMB_H_

#define ROWS	1000
#define COLS	1000

void print_moves(moves_sequence *moves);

position* init_position(holds_list *holds, int A, int T, int L);

bool finished(position *current_pos, point *last_hold);

move* get_next_move(heap *H, holds_list *holds, moves_sequence *moves, int A, int T, int L, position *pos);

void climb(holds_list *holds, int A, int T, int L);

#endif
