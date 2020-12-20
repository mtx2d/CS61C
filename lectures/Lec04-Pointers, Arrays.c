Lecture 4: Pointers and Arrays 

Variable Declaration
- Declaring C variables: int a = 0; b = 10;
- int x; // x be default holds garbage value. Rational? Speed
- Heisenbug: bugs difficult to reproduce (change when debugging)
- Address is the memory location, value: "value" stored at a location
- Pointer: variable that holds the address of a variable (points to the
  location in memory)
int *p, x; // Hold garbage values by default, * symbol declares a pointer
x = 3; // x takes on the value of 3, p still holds garbage
p = &x; // Now, p holds the address of x through the & symbol
z = *p; // Value at address p assigned to z
printf("Value of x: %i\n", *p); // Gives value of x through dereference operator
*p = 5; // The new value of x is 5

Pointers/Parameter Passing
- Java/C pass parameters BY VALUE
  - Functions/methods get a copy of the parameter
- Benefit of passing by reference: avoids having to make a copy of entire argument
- Pointers can point to any data type (int, char, struct)
  - Usually can only point to one type
  - void * can point to anything (generic pointer)--avoid using mostly

// Using pass by value
void addOne(int x) {
  x += 1;
}
int y = 3;
addOne(y); // y value will remain at 3 b/c of pass by value

void realAddOne(int *p) {
  *p = *p + 1;
}
int y = 3;
realAddOne(&y); // Final value of y will actually change by passing in address


Pointers
- After declaring pointer, holds "garbage" by default
  - Local variables in C are not initialized--can contain anything
- Benefits of a pointer: faster to pass a pointer when working w/ struct or array
- Drawbacks of pointers: source of bugs w/ dangling references & memory leaks
- Pointers an point to ny data type (int, char, struct) with the exception of void *
  which can point to anything
- Pointer to a function: int (*fn) (void *, void *) = &foo;
  - fn is a function accepting 2 void * pointers, returning an int, and
    starts off pointing to address of foo
  - To call: (*fn) (x, y)--same thing as calling foo(x, y)
- Null pointer: cannot write/read a null pointer (FALSE value)
- Key idea: a C pointer is an abstracted memory address

void f() {
  /*
    - ERROR: The result of trying to force a pointer to point to a location that
      it does not naturally occupy
  */
  int *ptr;
  *ptr = 5; // Tried to force a value before pointing to any location
}

/* Working with structs & pointers */
typedef struct {
  int x; int y;
} Point;
Point p1;
Point p2;
Point *paddr; //paddr is a pointer to a Point
paddr = &p2; // paddr is assigned to address of p2
int h = paddr->x; // Assign h the value of paddr's x attribute
int h1 = (*paddr).x; // Equivalent as the notation above using dereference
p1 = p2; // p1 gets the values of p2 (NOT the same storage)

Arrays
- Declaring an array: int ar[2];
  - 2 element integer array (garbage values by default)
int ar[] = {55, 23};
  - Declaring and filling (initializing) a 2 element array
- Accessing elements: ar[1] --> returns 23
- Arrays are similar (almost identical) to pointers
  - char *string & char string[] almost the same
    - Differences: incrementing, declaration of filled arrays
- Array variable acts as a pointer to first element
  - ar[0] is SAME as *ar
  - ar[2] is SAME as *(ar + 2)
- Arrays in C have no way of knowing their own length
  - Makes it easy to try to access past the end of an array
  - Also makes it necessary to pass an arrays size into a function
- Segmentation fault: attempting to write to somewhere forbidden
- Bus error: lack of alignment

char *foo() {
  /*
  ERROR
  - char *foo tells us we are returning a pointer to a string
    - char string[32] is a local array
    - string is valid in the local frame, but once returned, we no longer have
      access, making it invalid once outside the function
  */
    char string[32];
    ...
    return string;
}

/* Altering Pointers */
void incrementPtr(int *p) {
  p += 1;
}
int A[3] = {10, 20, 30}; // Initialize an array, A
int *q = A; // Declares int *q; then, assigns q to A
incrementPtr(q); // q changes locally, but doesn't make fundamental changes
// Result: *q = 10 (doesn't actually get modified)

void incrementPtrReal(int **h) { // Passing a pointer to a pointer: a handle
  *h += 1; // q is now a pointer to the next element in the array
}
int A[3] = {10, 20, 30};
int *q = A;
incrementPtrReal(&q); // Passing in the address of the pointer
// Result: *q = 20

/* Passing in a pointer to a function */

/* Declaring the outline for x10, x2, mutate_map, print_array functions */
int x10(int), x2(int);
void mutate_map(int [], int n, int (*) (int));
void print_array(int [], int n);

int x2(int n) { return 2 * n; }
int x10(int n) { return 10 * n; }
void mutate_map(int A[], int n, int (*fp)(int)) {
  // int (*fp) (int) --> Takes in a pointer to a function. That function takes in an integer
  // as input, and returns an integer
  for (int i = 0 i < n; i++)
    A[i] = (*fp)(A[i]); // Dereference the function pointer to actually call the function
}
void print_array(int A[], int n) {
  for (int i = 0; i < n; i++) {
    printf("%d ", A[i]);
  }
  printf("\n");
}




