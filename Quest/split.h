typedef struct node {
  char *data;
  struct node *next;
} Node;

void split(Node * numbers, Node ** evens, Node ** odds);