#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "data_structs.h"

#ifndef _LEGAL_POSITION_H_
#define _LEGAL_POSITION_H_

int legal_position(int A, int T, int L, int *coords);

bool legal_pos(int A, int T, int L, point *LH, point *RH, point *LF, point *RF);

#endif
