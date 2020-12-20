#include "remove_head.h"

void remove_head(Node **list) {
	if (list == NULL) {
		return;
	}
	else if (*list == NULL) {
		return;
	}
	else {
		Node* temp = (*list);
		*list = temp->next;
		free(temp);
	}

}