/** Ch. 5: Pointers and Arrays **/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

/**
Notes
- Unary operator (*): applied to pointer, it accesses object pointer points to
- alloc(n): returns pointer to n consecutive character positions
- afree(p): releases storage to be used later
- Storage used by alloc/afree is stack (LIFO)
- malloc/free: standard library's implementation of alloc/afree
- Define allocbuf as character array that alloc() hands out
  - allocp: pointer to the next free element
  - When alloc() is called asking for n characters, check to see if allocbuf has
    sufficient room
      - Yes? Return allocp & increment by n characters
      - No? alloc() returns 0
**/

/* Function that shows how to use pointers */
void unaryOperatorExample() {
  int x = 1, y = 2, z[10];
  int *ip; // *ip defined as a pointer (that's an integer)
  ip = &x; // ip: points to the address of x
  y = *ip; // y takes on the value that ip points to: 1
  *ip = 0; // Value of x: 0
  ip = &z[0]; // ip now points to first element of array z
  y = *ip + 1; // Find what ip points to, adds 1, assigns result to y
  *ip += 1; // Whatever ip points to is incremented by 1
}

/* Function to show the connection b/w arrays and pointers */
void arrayExample() {
  int *pa; // Arbitrary pointer pa
  int a[10]; // An array of 10 integers
  pa = &a[0]; // pa is now a pointer to array a --> contains address of a[0]
  int x;
  x = *pa; // Value of x = a[0]
  x = *(pa + 1); // Value of x = a[1]
  pa = a; // Has the same effect as pa = &a[0]
  // C evaluates a[i] as *(a+i)
}


/* alloc and afree functions */
#define ALLOCSIZE 10000 // Define available space
static char allocbuf[ALLOCSIZE]; // Storage to be allocated
static char *allocp = allocbuf; // Pointer to next free position

/* Function that returns a pointer to n characters */
char *alloc(int n)
{
  /* Check if sufficient space is available */
  if (allocbuf + ALLOCSIZE - allocp >= n)
  {
    allocp += n; // Increment the space used up
    return allocp - n; // Return the starting location in allocbuf of the space
  }
  else
  {
    return 0; // Not enough space available
  }
}

/* Function to free storage pointed to by p */
void afree(char *p)
{
  /* Make sure that p is indeed inside allocbuf */
  if (p >= allocbuf && p < allocbuf + ALLOCSIZE)
  {
    allocp = p;
  }
}

/* Function to demonstrate properties of strings in C */
void stringExample()
{
  // Key concept: Strings in C are just an array of characters
  char *message;
  message = "New message to test"; // message is a pointer to the character array
  // message can be modified to point elsewhere. Can't modify string contents
  char _message = "New message to test"; // _message: array w/ the characters + '/0'
  // The storage for _message is fixed
}

/** strcpy for arrays **/
void strcopy1 (char *s, char *t) {
  int i;
  i = 0; // Incrementer i
  while ((s[i] = t[i]) != '\0')
    i++; // Same length array: increment which index we're examining in the array
}

/** strcpy for pointers **/
void strcopy2 (char *s, char *t) {
  int i;
  i = 0; // Incrementer i
  while ((*s = *t) != '\0'){ // Keep going till we reach the terminating character
    // Value at given place in array in t --> assigned to s at same location
    s++; // s is a pointer, so (s+i) yields the next element in the array
    t++; // Same logic applies for t
  }
}

/** String comparison: return <0 if s <t, 0 if s == t, >0 if s > t (as arrays) **/
int strcomp1 (char *s, char *t) {
  for (int i = 0; s[i] == t[i]; i++) { /** Keep incrementing so long as each array's
    current value is equal **/
    if (s[i] == '\0') // We've reached the end for both
      return 0;
  }
  return *s - *t;
}

/** String comparison w/ pointers **/
int strcomp2 (char *s, char *t) {
  int i = 0;
  while (*s == *t) {
    if (*s == '\0') {
      return 0;
    }
    s++;
    t++;
  }
  return *s - *t;
}

/* Function return nth month using a pointer array */
char *month_name(int n)
{
  // Pointer array allows for "sub-arrays" to be of variable length
  static char *name[] = {
    "Invalid", "Jan", "Feb", "Mar","Apr",
    "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
  };
  return (n < 1 || n > 12) > name[0] : name[n];
}




void multiDArrays() {
  int nums[2][3] = {
    {5, 10, 15},
    {3, 6, 9}
  };
  printf("%i %i\n", nums[0][2], nums[1][1]); // Indexing a multi-D Array
  /**
  Notes on passing a 2-D array into a function
    - Must include # of col's, # of rows is irrelevant
  f(int arr[2][10])--> VALID
    - Array w/ # of rows &  columns specified
  f(int arr[][10]) --> VALID
    - Array w/o # of rows specified
  f (int (*arr)[10]) --> VALID
    - Parameter is pointer to array of 10 integers
  **/
}

int main() {
  multiDArrays();
}
