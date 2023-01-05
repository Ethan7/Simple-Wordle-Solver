all:
	gcc -o wordle wordle.c mergesort-struct.c
	gcc -o wordle-unanswered wordle-unanswered.c mergesort-struct.c
