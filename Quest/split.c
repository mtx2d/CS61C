#include "split.h"
#include <string.h>

void *CS61C_malloc(size_t size);
void CS61C_free(void *ptr);


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
		CS61C_free(lst->data);
		CS61C_free(lst);
	}
	else {
		freeNode(lst->next);
		CS61C_free(lst->data);
		CS61C_free(lst);
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
	freeNode(numbers);

    return;
}

