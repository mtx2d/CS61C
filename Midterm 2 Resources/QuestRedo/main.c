#include "quest.h"

void *CS61C_malloc(size_t size) {
	void *ptr = malloc(size);
	printf("malloc: %p\n",ptr);
	return ptr;
}

void CS61C_free(void *ptr) {
	printf("free  : %p\n",ptr);
	free(ptr);
}

int main(int argc, char *argv[])  {
	TREE a,b,c,d,e,*p;

	b.N = 10; // <-- PUT VALUES HERE to test your incr_tree
	a.N = 11; //  a    <-- this is the tester tree we provide
	c.N = 12; // b c   <-- print_tree prints it like this
	e.N = 13; //    d  <-- ((. b .) a (. c ((. e .) d .)))
	d.N = 14; //   e

	// Set up the tester tree for you, don't worry about this
	a.L = &b; a.R = &c;  c.R = &d;  d.L = &e;
	b.L = b.R = c.L = d.R = e.L = e.R = NULL;

    //Part A tests:
    printf("bias2sm(0 = -127)=%d\n", bias2sm(0)); // 0->255
	printf("bias2sm(1 = -126)=%d\n", bias2sm(1)); // 1->254
	printf("bias2sm(2 = -125)=%d\n", bias2sm(2)); // 2->253
	printf("bias2sm(3 = -124)=%d\n", bias2sm(3)); // 3->252
	printf("bias2sm(4 = -123)=%d\n", bias2sm(4)); // 4->251

	printf("bias2sm(125 = -2)=%d\n", bias2sm(125)); // 125 -> 130
	printf("bias2sm(126 = -1)=%d\n", bias2sm(126)); // 126 -> 129
	printf("bias2sm(127 = 0)=%d\n", bias2sm(127)); // 127 -> 0
	printf("bias2sm(128 = 1)=%d\n", bias2sm(128)); // 128 -> 1
	printf("bias2sm(129 = 2)=%d\n", bias2sm(129)); // 129 -> 2

	printf("bias2sm(253 = 126)=%d\n", bias2sm(253)); // 253 -> 126
	printf("bias2sm(254 = 127)=%d\n", bias2sm(254)); // 254 -> 127

	// Print the tree before incr_tree, call incr_tree and free it
	print_tree(&a); printf("\n");
	print_tree(p = incr_tree(&a)); printf("\n");
	free_tree(p);
	return 0; 
} 