#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct node {
	struct node *L;
	struct node *R;
	uint8_t N;
} TREE;

void *CS61C_malloc(size_t size);
void CS61C_free(void *ptr);

uint8_t bias2sm(uint8_t bias);
TREE *incr_tree(TREE *p);
void free_tree(TREE *p);

void print_tree(TREE *p);