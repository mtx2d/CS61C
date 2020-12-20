#include "quest.h"

void print_tree(TREE *p)
{
	if (p) {
		printf("(");
		print_tree(p->L);
		printf(" %d ", p->N);
		print_tree(p->R);
		printf(")");
	} else {
		printf(".");
	}
}