#include <stdio.h>

/* 
Pointer arithmetic 
int x;
int* pointer;
pointer = &x;
pointer + n
    - Adds n + sizeof(object pointer points to) to the memory address
pointer - n
    - Subtracts n * sizeof(object pointer points to) to memory address
*/

/*
Little vs. big Endian
- Little Endian: Least significant digit stored first in memory
    - Let's say we have the byte: 0xAB
    - B is stored first, A is stored after
- Big Endian: Most significant digit stored first in memory
    - Let's say we have the byte: 0xAB
    - A is stored first, B is stored after
*/

void pointerExercise()
{
    /* Assume address of first element of x is 0x1868 */
    /* sizeof(char) = 1, sizeof(int) = 4 */
    int x[5]; // Declare an int array of size 5
    x[0] = 254; // 1st element is 254: 0x000000FE
    x[1] = 649; // 2nd: 0x00000289
    x[2] = 971; // 3rd: 0x000003CB
    x[3] = 1678; // 4th: 0x0000068E
    /* 0x000000FE 0x00000289 0x000003CB 0x0000074C */
    char *y = (char*) x; // y is declared a char pointer by casting the pointer to x as a char (no longer an int)
    printf("%p\n", x); // Print memory address of x: 0x1868 (the first element)
    printf("%d\n", *(x+1)); // x+1 adds 4 to memory address --> *(x+1) uses dereference operator to yield the 2nd element in the array: 649
    printf("%d\n", x[2]); // Access 3rd element in the array: 971
    printf("%d\n", x[4]); // Access 5th element in the array: garbage (b/c it's uninitialized)
    printf("%p\n", x+9); // 0x1868 + 9*sizeof(int) = 0x1868 + 9*4 = 0x1868 + 0x24 = 0x187C
    printf("%p\n", &x); // Get the address of x: 0x1868 (same as the first element of x)
    printf("WARNING: %d\n", x[15]); // Potential error: segfault b/c x[15] isn't defined
    printf("%d\n", *y); // Little Endian: Returns the byte FE --> -2
    printf("%d\n", *(y+3)); // 000000FE --> Returns 4th chunk in this byte sequence: 00
    printf("%d\n", *(y+4)); // 00000289 --> Returns 0th byte in the second int: 89
    printf("%d\n", *(y+5)); // 00000289 --> Returns 1st byte in second int: 02
    printf("%d\n", *(y+12)); // 0000068E --> Returns 1st byte of 4th int: 8E
    printf("%p\n", y+1); // Only adds one b/c y is a char pointer --> 0x1868 + 1 = 0x1869
}

typedef struct Point {
    int x;
    int y;
} Point;

void changeX1(Point pt) {
    pt.x = 11;
}

void changeX2(Point *pt) {
    /* The following line will ERROR: Can't use . operator for a struct pointer in changeX2 */
     // pt.x = 12;
}

void changeX3(Point *pt) {
    pt->x = 13;
}

void changeX4(Point *pt) {
    (*pt).x = 14;
}

void changeX5(Point *pt) {
    pt = 15;
}

/* Function to demonstrate pointer/struct behavior */
void PointChangers()
{
    Point p1 = {1, 2}; // Initialize a point w/ values x = 1, y = 2
    changeX1(p1); // p1.x value is locally changed to 11
    printf("p1: %d\n", p1.x); // In this function, p1.x stays at 1

    Point p2; // Uninitialized p2 w/ garbage x, y values
    changeX1(p2); // p2.x is locally changed to 12
    printf("p2: %d\n", p2.x); // This function: p2.x remains garbage

    
    Point p3; // Uninitialized p2 w/ x,y holding garbage
    changeX2(&p3); // Pass the address of p3 into changeX2 --> ERROR
    printf("p3: %d\n", p3.x);
    

    Point *p4 = (Point *) malloc(sizeof(Point)); // p4 pointer to malloc() space for a Point struct (default values: x = 0, y = 0)
    printf("p4 prev: %d %d \n", p4->x, p4->y);
    changeX3(p4); // Pass pointer into changeX3 --> p4's x value is modified to 13
    printf("p4 post: %d\n", p4->x);

    Point *p5 = (Point *) calloc(1, sizeof(Point)); // p5 pointer to calloc() space for a Point struct: (default values: x = 0, y = 0)
    changeX5(p5); // Pointer value is modified locally to 15
    printf("p5: %d\n", p5->x); // Doesn't affect p5 locally (still, for p5: x = 0, y = 0)

    Point p6; // Uninitialized p6 w/ garbage x, y values by default
    changeX4(&p6); // Passing a pointer to address of p6 into changeX4 --> Dereference the pointer to modify its x attribute to 14
    printf("p6: %d\n", p6.x); 

}

/* Analyzing the memory for the following lines of code */
#define SPOCK 1701 // Preprocessor
int KIRK = 1701; // Defining a global variable

/* Function named sulu takes in scotty as a parameter & returns its square */
int sulu(int scotty) { 
    return scotty * scotty;
}
int mainFunc()  {
    int *chekov = malloc(sizeof(int) * 1701); // checkov is a pointer for 1701 ints w/ malloc()
    if (chekov) free(chekov); // Free the pointer if it isn't null
    sulu(SPOCK); // Calling sulu w/ SPOCK as a parameter
    return 0;
}

/*
1. sulu exists in CODE: function in source code
2. checkov exists in STACK: acts as a local variable storing a pointer in mainFunc
3. *chevkov exists in HEAP: dereferenced pointer accessing malloc()'s returned heap memory
4. KIRK exists in STATIC: it is a variable defined outside any function
5. scotty exists in STACK: it is a parameter for a function
6. SPOCK right before sulu(SPOCK) is called exists in CODE: it is defined by the C preprocessor
*/

#include <string.h>

void stringCheck1()
{
    char *a = "hello";
    int len;
    sizeof(a); // sizeof(a) == 4 b/c it's a pointer & sizeof(pointer to anything) = 4

    // Valid methods for getting the length of a
    len = strlen(a);
    len = 0; while (*a++) {len++;}
    len = 0; while (*a++) {++len;}
    char *a2 = "foobar";
    strlen(a2); // Would result in 6--null terminator doesn't count
    char a3 = 'f'; // strlen(a3) would give an error b/c it's not a char pointer
    char a4[] = {'f', 'a', 'n', '\0'}; // strlen(a) = 3 --> valid declaration of a string
    char a5[] = {'b', 't', 's'}; // Representations would error b/c no null terminator
    char *a6 = "foo\0bar"; // strlen(a6) = 3 --> Only reads up till the null termiator
}

#define PASSWORD "test a pass"
char *check1(char *comp)
{
    if (comp = PASSWORD){ // FAULTY: Sets comp to password instead of checking for equality
        return "granted";
    }
    return "denied";
}

char *check2 (char *comp)
{
    if (comp == PASSWORD) { // FAULTY: Compares the pointers instead of the string values; should use strcmp
        return "granted";
    }
    return "denied";
}

void willSegfault()
{
    // ERROR: Requesting an absurd amount of space
    // Should always check if a pointer is null post malloc
    char *x = malloc(0xFFFFFFFF * sizeof(char));
    x[0] = '!';
}

void faulty1()
{
    int *x = malloc(20);
    x[0] = x[1] = 1;
    x[2] = x[0] + x[1];
    x[3] = 99;
    // MEMORY LEAK: Never frees malloc'ed data
}

void faulty2()
{
    char *x = "patrick";
    printf("%s\n", s);
    free(x); // FAULTY: Can't free w/o prior malloc/realloc/calloc

}

int main()
{
    //pointerExercise();
    //PointChangers();
    printf("%s\n", check("tester"));
    printf("%s\n", check("test a pass"));
    char *x = malloc(0xFFFFFFFF * sizeof(char));
    x[0] = '!';
    printf("%c\n", x);
}