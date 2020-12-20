#include <stdlib.h>
#include <stdio.h>
#include <string.h>


typedef struct node {
  char *data;
  struct node *next;
} Node;



void freeNode(Node* lst) {
	if (lst == NULL)
	{
		return;
	}
	else if (lst->next == NULL) {
		free(lst->data);
		free(lst);
	}
	else {
		freeNode(lst->next);
		free(lst->data);
		free(lst);
	}
}
void split(Node *numbers, Node **evens, Node **odds) {
	if (numbers == NULL) {
		return;
	}
	int oddCounter = 0;
	int evenCounter = 0;
	Node *source = numbers;
	Node *evensList = (Node *) malloc(sizeof(Node));
	Node *oddsList = (Node *) malloc(sizeof(Node));
	while (source != NULL && source->data != NULL)
	{
		// Allocate space for a new node & create attributes
		int pos = (int)strlen(source->data) - 1;
		char finalChar = source->data[pos];
		Node* temp = (Node *) malloc(sizeof(Node));
		if (temp == NULL) {
			return;
		}
		temp->data = (char *) malloc(sizeof(char));
		if (temp->data == NULL) {
			return;
		}
		strcpy(temp->data, source->data);

		// Even
		if (finalChar == '0' || finalChar == '2' || finalChar == '4' || finalChar == '6' || finalChar == '8')
		{
			if (evenCounter == 0) {
				evensList = temp;
				evenCounter++;
			}
			else
			{
				Node* lastPtr = evensList;
				while (lastPtr->next != NULL) {
					lastPtr = lastPtr->next;
				}
				lastPtr->next = temp;
			}
		}
		else // Odd
		{
			if (oddCounter == 0) {
				oddsList = temp;
				oddCounter++;
			}
			else
			{
				Node* lastPtr = oddsList;
				while (lastPtr->next != NULL) {
					lastPtr = lastPtr->next;
				}
				lastPtr->next = temp;
			}
		}
		// Free the data

		// Increment the pointer
		source = source->next;
	}
	// Set the evens and oddslist
	*evens = evensList;
	*odds = oddsList;

	// Free the original list via the helper function above
	//freeNode(numbers);

    return;
}

int main() {

	Node* node1 = (Node*) malloc(sizeof(Node));
	node1->data = (char*)malloc(sizeof(char));
	node1->data = "100";
	node1->next = NULL;
	Node* node2 = (Node*) malloc(sizeof(Node));
	node2->data = (char*)malloc(sizeof(char));
	node2->data = "105";
	node2->next = NULL;
	Node* node3 = (Node*) malloc(sizeof(Node));
	node3->data = (char*)malloc(sizeof(char));
	node3->data = "102";
	node3->next = NULL;
	Node* node4 = (Node*) malloc(sizeof(Node));
	node4->data = (char *)malloc(sizeof(char));
	node4->data = "103";
	node4->next = NULL;
	node1->next = node2;
	node2->next = node3;
	node3->next = node4;
	printf("data: %s %s %s %s\n", node1->data, node1->next->data, node1->next->next->data, node1->next->next->next->data);

	Node *evensTest;
	Node *oddsTest;
	split(node1, &evensTest, &oddsTest);
	printf("odd->data: %s\n", oddsTest->data);
	printf("odd->next->data: %s\n", oddsTest->next->data);
	printf("evens->data: %s\n", evensTest->data);
	printf("evens->next->data: %s\n", evensTest->next->data);

    return 0;
}