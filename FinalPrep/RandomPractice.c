Summer 2019 Midterm 1

Question 3: Debug (if needed) the following functions

a. 
/* Copy ints from one array to a new one */
int copy_ints(int* arr) {
	int* new = malloc(sizeof(arr));
	for (int i = 0; i < sizeof(arr); i++) {
		*(new + i) = *(arr + i);
	}
	return new;
}

- Problem is on line 7 and 8: sizeof(arr) returns the size of the pointer, not the length of the array
	- Would instead need to preset the number of elements we hope to allocate

b.

/* Interpret integer as a boolean value */
char* boolToString(int i) {
	char* retVal;
	if (i == 0) {
		retVal = "false";
	}
	else {
		retVal = "true";
	}
	return retVal;
}
- This function works as is b/c a string literal exists in code space, not stack space
	- Thus, we can still access the value of retVal after returning the pointer

c.
/* Return copy of a string with a  null terminator */
char* nullTerm(char* str, unsigned int len) {
	char* copy = (char*) malloc(sizeof(char) * len);
	for (int i = 0; i < len; i++)
		copy[i] = *(str++);
	copy[len] = '\0';
	return &copy;
}

- Problem is on line 37: failed to allocate space for the null terminator
	- Should instead be sizeof(char) * (len + 1)
- Another problem is on line 41: we want to return copy, not the address of copy

d.

typedef struct intNode {
	int value;
	struct intNode* next;
} intNode_t;

intNode_t* append(intNode*, int value) {
	intNode_t* new = malloc(sizeof(intNode_t*));
	new->value = value;
	(*new).next = current;
	return new;
}

- Problem is on line 55: we should not be mallocing the size of the pointer. Rather, we want to malloc
  space for the actual struct
  	- Proper implementation: malloc(sizeof(intNode_t))

Question 5: Implement strncpy in RISC-V
- Want to copy first n characters from source into destination
- If null terminator is reached, then copy that value into destination and halt
- Do not need to null terminate if first n characters reached before
a0: address of destination
a1: address of source
a2: number of characters to copy over

strncpy:
	add t0, x0, x0 # Current length
	______
LOOP:
	beq ___t0, a2___ End # If we already copied n characters --> go to end
	___add t1, a1, t0___ # t1: Holds address of character of source array to be loaded
	___lb t2, 0(t1)___ # t2: Holds actual character at address
	___add t3, a0, t0___ # t3: Holds address of destination to store character
	___sb t2, 0(t3)___ # Store character from held in t2 into t3
	___addi t0, t0, 1___ # Increment the counter
	bne ___t3, x0___ LOOP # Loop if not at null terminator
End:
	______
	___ret___



