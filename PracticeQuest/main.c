//#include "remove_head.h"


#include <stdio.h>
#include <stdlib.h>
typedef struct node {
  char *data;
  struct node *next;
} Node;

void remove_head(Node **list);


/*
Write a function remove_head that, given a singly linked list, removes the head of the list. 
If the list is empty, simply return.
*/

int main(int argc, char *argv[]) {
	Node *test1 = (Node*) malloc(sizeof(Node));
	test1->data = "TEST 1";
	test1->next = NULL;
	remove_head(&test1);
	remove_head(&test1);
	free(test1);

	Node *test2 = (Node*) malloc(sizeof(Node));
	test2->data = "TEST 2";
	test2->next = NULL;


	Node *test3 = (Node*) malloc(sizeof(Node));
	test3->data = "TEST 3";
	test3->next = test2;
	remove_head(&test3);
	printf("New head data: %s\n", test3->data);
	remove_head(&test3);
	printf("Is there a head? %i\n", test3 == NULL);

}