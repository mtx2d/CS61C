#include <stddef.h>
#include "ll_cycle.h"

int ll_has_cycle(node *head) {
    /* your code here */
    if (head == NULL || head->next == NULL || head->next->next == NULL)
    {
    	return 0;
    }
    node *hare = head->next->next;
   	node *tortoise = head->next;
   	while (tortoise != hare)
   	{
   		if (hare == NULL || hare->next == NULL || hare->next->next == NULL)
    	{
    		return 0;
    	}
    	else
    	{
    		tortoise = tortoise->next;
    		hare = hare->next->next;
    	}
   	}
   	return 1;
}