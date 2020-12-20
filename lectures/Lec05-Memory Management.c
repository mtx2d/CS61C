Lecture 5-Memory Management


Dynamic Memory Allocation
- sizeof() operation gives size in bytes
  - sizeof(type) is better practice
- Example: int ar[3]; sizeof(ar) --> 12 (there are 12 total bytes)
- malloc(): allocates room for a new thing to point to
ptr = (int *) malloc (sizeof(int));
  - ptr points to space in memory w/ a size of (sizeof(int)) bytes
  - (int *) indicates what goes into that space (an integer)--casting
    - Necessary b/c malloc() by default returns a void * pointer
- Allocating space for an array of n integers: ptr = (int *) malloc (n*sizeof(int))
  - Need to make sure that the spaces reserved for the array are contiguous
- Calling malloc(): memory location contains garbage by default
- free(ptr): dynamically free up space
- Bad practices: free() same piece of memory twice, free() something not attained
  from malloc()
- Pointers: more efficient  (speed/ease of use) for passing struct/array
  - Also causes bugs in software, dangling reference/memory leaks
- realloc(p, size): resize previously allocated block at p to a new size
  - p NULL? --> realloc() is the same as malloc()
  - size = 0? -->  relloac() behaves like free()
  - Ultimately returns new addresss from memory block 


/* Function illustrating realloc(p, size) */
void reallocDemo()
{
  int *ip; // Integer pointer
  ip = (int *) malloc (10 * sizeof(int)); // Setting aside the memory for 10 ints--but the space contains garbage
  // Make the values at the address not garbage
  ip = (int *) realloc(ip, 20 * sizeof(int)); // Check if ip is null? If not, reallocate space for 20 ints (moving original 10 values over)
  realloc(ip, 0); // Check if ip is null? If not, same thing as calling free(ip)
}

/* Function that shows array implementation */
void arrayDemo() {
  int *p, *q, x; // Initialize pointer p, pointer q, and int x
  int a[4]; // Declare an array a holding garbage--4 contiguous spaces in memory
  p = (int *) malloc(sizeof(int)); // Allocate space for an int; p now points to that address
  q = &x; // q is a pointer to x
  *p = 1; // Change: malloc space now contains the value 1
  *q = 2; // Change: q now points to 2
  *a = 3; // Change: first element in the array now is 3, &a = a
}

/* Struct below will create a linked list of strings */
struct Node {
  char *value; // String contained by this node
  struct Node *next; // Pointer to another "linked" node
};
typedef struct Node *List; // typedef: defines a new type (List is a type now)

/* Create an empty list */
List ListNew (void) {
  return NULL;
}

/* Function that adds a string to an existing list */
List addList(List list, char *string) {
  // Below, malloc is called to provide with sufficient space for a struct Node
  struct Node *node = (struct Node*) malloc (sizeof(struct Node));
  // Now, allocate space for node's value to contain string
  node->value = (char*) malloc(strlen(string) + 1);
  strcpy(node->value, string); // Copy in string value
  node->next = list; // The next value of node is the previous list
  return node; // Return the newly created node
}

C Structures
- struct: data structure made up of simpler data types
  - Similar to classes in Java w/o methods/inheritance
- Structs typically contain pointers to other objects instead of the direct data
  itself
struct point {
  int x;
  int y;
}
struct point *p; // Creating a pointer to a struct
(*p).x is the same as p->x // -> operator dereferences  extracts structure's field


Storage Locations
- Structure declaration: no memory is  (only declares a type)
- Variable declaration: does allocate memory
  - Local variables: int i; struct Node list;
  - Dynamic allocation at runtime: ptr = (struct Node *) malloc (sizeof(struct Node) *n)
  - Data declared outside procedure (before main)
    - Has global scope (accessible from any function in the file)
- 3 pools of memory in C: static storage, the stack, the heap
  - Static storage: global variables reside here--contains permanent variables
    that are always present throughout the program
      - Does not grow/shrink; all the variables declared outside main
  - The Stack: stores local variables, parameters, & return addresses
    - Grows downward; OS prevents stack & heap from accessing one another
  - The Heap: data that exists until deallocated (dynamic malloc storage)
    - Grows upward, contains space requested for pointers through malloc(), size
      adjusts dynamically
- Program address space contains 3 layers above code
- Code/Static storage space: never grows/shrinks, so easy to deal with

Stack Management
- Stack space: frames created/destroyed in (last in, first out) order
  - Stack frame includes: "instruction" address, parameters, space for other local variables
    - "Instruction address": where did the frame leave off if another function is invoked?
    - Stack grows downward with more function calls
    - Grows back up when functions return a value (variables will cont. to live in memory, even after return
      renders them inaccessible)
  - Form contiguous memory blocks w/ a pointer to the top frame
  - After procedure ends, frame removed from stack & memory freed for new stack frames

/* Demonstrating LIFO nature of a stack */
void func1() {func2(0);} // Starts as top frame of stack
void func2(int a1) {func3(a1);} // Second frame of stack
void func3(int a2) {func4(a2);} // Third frame--function return results in frame removed second
void func4(int a3) {} // Last frame--once function called, frame is removed first

Heap Management
- Heap: contains large amounts of memory, not necessarily contiguous
  - malloc(): draws uninitialized memory from the heap
- Heap space:  trickier b/c needs to be able to allocate/deallocate memory any time
  - Want malloc() & free() to run quickly w/ minimal memory overhead
  - Problem is fragmentation: free memory exists in small bits & pieces but do not
    have large, contiguous blocks of memory

Dealing with Fragmentation
- Block of memory contains SIZE & POINTER to next block
  - Free blocks kept in circular linked list, w/ pointer unused in allocated block
- malloc(): scours free list for a sufficiently large block. Not found? Request
  more memory from operating system. Cannot do this? Request fails
- free(): check if blocks adjacent to freed block is free
  - Adjacent blocks free? They form a large, contiguous block. Else? Just add it
    to all the other chumps in the free list
- 3 possible solutions for memory request in malloc(): best-fit, first-fit, next-fit
  - Best fit: smallest block big enough for a request
    - Takes a lot of time to potentially find the "best" solution b/c has to search
      through the entire linked list
  - First fit: first block encountered that is large enough
  - Next-fit: first fit, but resuming search from the previously saved location


/* Function demonstrating danger of writing off end of arrays */
void faultyArrayDemo()
{
  int *foo = (int *) malloc(sizeof(int) * 100); // Pointer that allocates space for 100 ints
  int i;
  for (i = 0; i <= 100; ++i)
  {
    foo[i] = 0; // Eventually tries to write to 100th elem: the space might not be present
  }
}

/* Next 2 functions demonstrating error of trying to return a pointer to a stack */
int* ptr() {
  int y; // Arbitrary integer y
  y = 3; // Assign its value to 3
  return &y; // Return local variable's address--BAD PRACTICE
}
main() {
  int *stackAddr, content;
  stackAddr = ptr(); // Creating a ptr w/ previous function--Has stack frame for main and ptr()
  // After *ptr() returns, ptr() frame is GONE 
  content = *stackAddr;
  printf("%d", content); // Content at new address from the pointer: 3--but also creates a stack frame for printf()
  // printf() function performed some action to change what was present in where the 3 used to reside
  content = *stackAddr; // *stackAddr contains garbage now
  printf("%d", content); // Content at address from pointer: garbage value
  // Rationale: printf overwrote the contents for y in the local variable's address
}

/* Function showing why NOT to use space after freeing */
void freeCorruptionDemo()
{
  struct foo *f;
  // Do something
  f = malloc(sizeof(struct foo));
  // Do something
  free(f);
  // Do something
  bar(f->a); // After free is called on a space, there's no guarantee that the memory location isn't corrupted
}

/* Function that shows error in realloc w/o accounting for movement of data to different memory */
void reallocErrorDemo()
{
  struct foo *f = (struct foo *) malloc(sizeof(struct foo) * 10); // Reserving space for 10 struct foo's
  struct foo *g = f; // Pointer g dedicated to the same space
  f = realloc(sizeof(struct foo) * 20); // No guarantee g points to the same thing b/c data can be relocated with realloc
}

/* Function showing the use of free in the wrong location */
void wrongFreeDemo()
{
  struct foo *f = (struct foo *) malloc(sizeof(struct foo) * 10);
  f++; // Increment what f is pointing to
  free(f); // Causes confusion b/w malloc & free: might erase other data or corrupt storage
}

/* Function demonstrating error in calling free twice */
void doubleFreeErrorDemo()
{
  struct foo *f = (struct foo *) malloc(sizeof(struct foo) * 10);
  free(f);
  free(f); // Double free: Could potentially corrupt malloc data
}

/* Function demonstrating a memory leak */
int *p1 = NULL; // Assume *p1 is a global pointer
void memoryLeakDemo()
{
  p1 = malloc(2 * sizeof(int)); // Assign malloc'ed space for 2 integers
  // Do something
  p1++; // p1 was originally a global variable: changing it inside this function doesn't inform global of changes
}