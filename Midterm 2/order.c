// Spring 2018 Final #2

#include <stdio.h>
#include <stdlib.h>

#define works 0
#define allocFail 1
#define badArgs 2
#define tax 1.08
#define maxOrder 100

typedef struct order {
	int quantity;
	double price;
	double taxRate;
} order;

int newOrder(order **new, int q, double p)
{
	// Validate Arguments
	if (p <= 0 || q > maxOrder || new == NULL)
	{
		printf("BAD ARGS\n");
		return badArgs;
	}
	// Allocate space for new
	*new = malloc(sizeof(order));
	if (*new == NULL)
	{
		free(new);
		printf("ALLOC FAILED\n");
		return allocFail;
	}
	// Initialize New
	(*new)->quantity = q;
	(*new)->price = p;
	(*new)->taxRate = tax;
	return works;
}

int main()
{

	order *order1;
	order1 = malloc(sizeof(order));
	order1->quantity = 3;
	order1->price = 2;
	order1->taxRate = tax;
	printf("Q: %i, P: %f, T: %f\n", order1->quantity, order1->price, order1->taxRate);

	order *order2;
	order2 = malloc(sizeof(order));
	newOrder(&order2, 5, 5);
	printf("Q: %i, P: %f, T: %f\n", order2->quantity, order2->price, order2->taxRate);

	free(order1);
	free(order2);
}