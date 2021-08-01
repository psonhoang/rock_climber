CCBIN=/usr/bin/gcc
CC=$(CCBIN) -Wall -Wextra -std=c99 -pedantic -g

default: climb

legal_position: lib/*.h data_structs.c legal_position.c legal_position_main.c
	$(CC) -O3 -o legal_position data_structs.c legal_position.c legal_position_main.c -lm

climb: lib/*.h data_structs.c heap.c legal_position.c climb.c climb_main.c
	$(CC) -O3 -o climb data_structs.c heap.c legal_position.c climb.c climb_main.c -lm

.PHONY: clean
clean:
	rm -Rf *.o *.dSYM *.txt *.vscode climb legal_position

.PHONY: package
package:
	tar -cvzf climber-handin.tgz *