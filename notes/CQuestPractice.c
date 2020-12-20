#include <stdio.h>

typedef struct {
	// Actual array of data
	int* data;
	// Memory allocated for data
	int allocated_size;
	// Number of elements actually in the queue
	int occupied_size;
	// Location where the next append_left should be placed.
	int left_location;
	// Location where the next append_right should be placed.
	int right_location;
	} int_dqueue_t;


int_dqueue_t* create_int_dqueue () {
	int_dqueue_t* q = malloc(sizeof(int_dqueue_t));
	q->occupied_size = 0;
	q->allocated_size = 4;
	q->data = malloc(sizeof(int) * q->allocated_size);
	q->left_location = 1;
	q->right_location = 2;
	return q;
}
/* 
Summer 2019 Final: #3 
print_reverse_deque: prints each valid element in the array from the end to the
front (left to right) with each element on a newline

expand_buffer: which takes in a queue that is full and reallocs circular buffer while
maintaining the previous ordering. You can assume all calls to realloc succeed 
*/

void print_reverse_dqueue (int_dqueue_t* q) {
	for (int i = 0; i < q->occupied_size; i++) {
		int location = q->right_location - i + 1;
		if (location < 0) {
			location = q->right_location - i + 1 + q->allocated_size;
		}
	printf ("%i\n", q->data[location]);
	}
}


// Fall 2018 Final #M2 Part 2
uint8_t mystery (uint8_t *A) {   
	return *A ? (*A & mystery(A+1)) : 0xFF;
}

/*
Questions
1. Where is A stored?
	--> A is a local variable (pointer) --> Stored in stack space
2. Evaluate: (char) mystery("GROW")
	GROW: 01000111, 01010010, 01001111, 01010111
	mystery(A): *A = 0100 0111 --> *A & mystery(A+1) = 0000 0000
	mystery(A): *A = 0101 0010 --> *A & mystery(A+1) = 0000 0000
	mystery(A): *A = 0100 1111 --> *A & mystery(A+1) = 0001 0000
	mystery(A): *A = 0101 0111 --> *A & mystery(A+1) = 0101 0000
					 1111 1111

	mystery(A): *A = 0000 0000 --> 1111 1111
	Result: Anding all the bits --> 0100 0010 --> B

3. A two-character string is passed into mystery
that makes it return the uint8_t value 0 (not the
character “0”). The first character is “M”, the
second character is a number from 1-9. Which?
M = 0100 1101
Number: 0011 0 _ _ _ --> 1101 & 0010 would make it 0 --> 2
*/

/* Fall 2015 Final M1-1B*/
int global = 0;

int* func() 
{
	int* arr = malloc(10 * sizeof(int));
	return arr;
}
int main1() 
{
	char* str = "hello world";
	char str2[100] = "cs61c";
	int* a = func();
	return 0;
}

/*
Where are the following STORED in memory?
*str: returns the value of a string literal, which is stored in STACK
a: a pointer declared in a function (points to what another pointer points to)--> STACK
arr[0]: arr is a pointer to malloc'ed data. a[0] is obtained from the malloc'ed data --> HEAP
str2[0]: When a char array gets declared (even w/ a string literal)--chars are copied over into array
	- Since str2 was declared locally, each char placed into STACK space
arr: arr is a pointer that is declared locally --> STACK
*/

/* Fall 2019 Question #5 */
char *arr[] = {"Go", "Bears"};
int main2() {
	char **arrPtr = arr;
	char *dest[2];
	int j;
	for (int i = 0; i < 2; i++) {
		char *currString = *arrPtr;
		dest[i] = (char *) malloc(strlen(currString) + 1);
		for (j = 0; j < strlen(currString); j++) {
			dest[i][j] = currString[j] & ~(1 << 5); // ⇐ Hint: Focus on this line! ⇐
		}
		dest[i][j] = '\0';
		arrPtr++;
	}
	printf("%s %s", dest[0], dest[1]);
}

/*
Where do the following POINT in memory?
arr: Array of character pointers; variable name is a pointer to first element of array
	- arr declared globally, so contents of array placed in STATIC part of memory
arr[0]: arr[0] is a pointer to the first element in the array (a string literal): pointing to first char in that literal, which exists in STATIC
dest: array declared in a local function; all elements exist in stack --> dest will be a pointer to the first element --> STACK
dest[0]: malloc is called to allocate space for dest[0] --> dest[0] becomes a pointer to data living in HEAP space
&arrPtr: arrayPtr is declared in main2 (pointing to arr), so its address is a pointer into STACK
*/


/* Summer 2019 Midterm 1 #2 */

#include <stdlib.h>
#include <stdbool.h>

bool fetch_data (char* buf);
char* receive_buffer;
bool is_complete = false;

int main3(int argc, char* argv[]) {
	receive_buffer = malloc(100*sizeof(char));
	if (!receive_buffer) {
		return -1;
	}
	fetch_data(receive_buffer);
	free(receive_buffer);
	return 0;
}

void fetch_data(char* buf) 
{
	int len = receive_data(receive_buffer);
	if (len == 0) {
		return; // HERE
	}
	else {
		fetch_data(buf + len);
	}
}

/*
All the following evaluate to an address value.
Where do the following's address values evaluate to in memory right before executing HERE?
	- Equivalent to asking where do the following point to?
	- Ex: This address point to ___, which exists in ___ memory
receive_buffer: receive_buffer itself is a pointer variable to malloc'ed data. 
The pointer POINTS to receive_buffer[0], data allocated w/ malloc, thus in HEAP
&(receive_buffer[0]): receive_buffer[0] exists in HEAP b/c malloc, so the address for it
is also accessed via the heap
&receive_buffer: receive_buffer is a pointer variable that was declared globally.
Thus, an address to the pointer would also be accessed from STATIC where variable was born
&argc: argc is an arbitrary local variable defined in main. Thus, its address is also in STACK
&is_complete: is_complete was a variable defined globally. Thus, its address also exists in STACK
&fetch_data: fetch_data is a function name--which exists in CODE. Thus, its address also exists in CODE
buf: buf is a pointer to receive_buffer. Thus, it's a pointer to the first element of receive_buffer,
which was malloc'ed, so its address exists in HEAP
*/


/* Fall 2018 Midterm 1 #3*/
#include <stdio.h>
#include <stdlib.h>
typedef struct Song {
	char *title;
	char *artist;
} Song;

Song * createSong() {
	Song* song = (Song*) malloc(sizeof(Song));
	song->title = "this old dog";
	char artist[100] = "mac demarco";
	song->artist = artist;
	return song;
}

int main4(int argc, char **argv) {
	Song *song1 = createSong();
	printf("%s\n", "Song written:");
	printf("%s\n", song1->title); // print statement #1
	printf("%s\n", song1->artist); // print statement #2
	Song song2;
	song2.title = malloc(sizeof(char)*100);
	strcpy(song2.title, song1->title);
	song2.artist = "MAC DEMARCO";
	printf("%s\n", "Song written:");
	printf("%s\n", song2.title); // print statement #3
	printf("%s\n", song2.artist); // print statement #4
	return 0;
}

/* What type of address does each value evaluate to?

song1: A pointer to a song instance created locally, whose data is malloc'ed
in createSong(). Thus, *song1 --> HEAP
song1->title: Attribute is a pointer to a string literal, so it evaluates to
the string literal --> STATIC
song1->artist: The data in the array is ripped from a string literal but inserted
into an array that was created in STACK. Thus, evaluating that array and getting 
*(song->artist) would yield something from STACK
&song2: Song2 is declared locally, so its address is in STACK. Accessing *(&song2)
will still give a song instance that exists in STACK
song2.title: The data of title was malloc'ed, so accessing it will yield HEAP

*/


char *mood;
char *copy_message (char *msg) 
{
	char *x = malloc(sizeof (char) * (strlen (msg) + 1));
	strncpy (x, msg, strlen (msg));
	x[strlen (x)] = ‘/0’; /**** 6 ****/
	return x;
}

void print_int (int *p) {
	printf (“%d\n”, *p); /**** 7 ****/
}

void print_msg (char *str) {
	char *cpy = calloc (strlen (str) + 1, 1);
	strncpy (cpy, str, strlen (str));
	printf (“%s\n”, cpy); /**** 8 ****/
}

char *a () {
	char res[7] = “ rules”;
	return res;
}
char *b () {
	char *var = “cs 61c”;
	return var;
}
void c () {
	printf (“%s\n”, a ());     /**** 9 ****/
	printf (“%s\n”, b ());    /**** 10 ****/
}

int main5() {
	int y;
	mood = malloc (3);
	strcpy (mood, “hi”);
	copy_message (mood);
	print_int(&y);
	print_msg(mood);
	c ();
}

/*
Each of the following evaluates to an ADDRESS. Where does this
ADDRESS POINT TO in memory?

mood: mood is a pointer declared globally whose data was malloc'ed. 
Thus, its address points to HEAP space
&mood: mood is a pointer declared globally. &mood points to mood:
which was thus born into STATIC space
var: var is a pointer created locally and points to a string literal.
Thus, var points to the first character in a string literal, which exists
in STATIC space
res: res is an array. It points to the first element of that array (which
contains the characters yanked from static space and placed locally). Thus,
the first char exists in STACK space
print_int: print_int is a function declaration: which points to CODE.
*/

/*
Legal vs. illegal
Legal​​: All addresses dereferenced are addresses that the program is allowed to read.

Initialized​​: Is there actual meaningful data in contents (data at each address) or is it garbage.

Always Illegal​​: This line will always dereference an address the program doesn’t have explicit access

Possibly Legal​​: The operation could result in only dereferences of legal addresses but it’s also possible that in other runs 
on the program illegal accesses occur.

6. Possibly Legal: works if the final character of the malloc'ed space happens to be null terminator
7. Initialized: y isn't ever initialized, but still has an address
8. Legal: calloc automatically initializes data to zero, so null pointer is present
9. Illegal: Array declared locally (even if yanking chars from a string literal) can't be accessed outside frame
10. Legal: string literal is returned which exists in static space, so definitely have access

*/


/* 
Spring 2018 Final #1 
Convert from base (2-9) to base 10
*/
unsigned int base convert(unsigned int num, unsigned int base)
{
	unsigned int value = 0, power = 0;
	while (num > 0) {
		value += (num % 10) * power;
		power = power * base;
		num = num/10;
	}
	return value;
}


/* 
Spring 2018 Final #2
*/

typedef struct order {
	int quantity;
	double price;
	double tax;
} order_t;

#define OK 0 // Executed right
#define NOMEM 1 // Couldn't allocate memory
#define BAD_ARG 2 // Invalid argument given 

#define taxRate 1.08
#define maxOrder 100

int newOrder(order_t **new, int quantity, double unitPrice)
{
	// Validate args
	if (new == NULL || quantity > maxOrder || unitPrice <= 0) {
		return BAD_ARG;
	}
	// Allocate new
	*new = (struct order_t*) malloc(sizeof(order_t));
	if (new == NULL)
		return NOMEM;

	// Initialize new
	(*new)->quantity = quantity;
	(*new)->price = unitPrice;
	(*new)->tax = taxRate;
	return OK;
}


order_t *blueMonday;
double total;
ret_t ret;

// Use new_order() to initialize blue_monday w/ q = 10, p = 3.5
ret = new_order(&blueMonday, 10, 3.5);



/* Spring 2018 Final #2a */

/*
PART A
Return a pointer to the last non-NULL node of a linked list 
*/

typedef struct listNode {
	char *str;
	struct listNode *next;
} listNode;

listNode* findEnd (listNode *start) {
	if (start == NULL) {
		return start;
	}
	listNode* searcher = start;
	while (searcher->next != NULL) {
		searcher = searcher->next;
	}
	return searcher;
}

/*
PART B
Generate a linked list in reverse order of a 
BST w/ data stored only in leaf nodes --> return NULL if BST is empty
- Have access to listNode, treeNde, and findEnd
*/

typedef struct treeNode {
	char *str;
	struct treeNode *left, *right;
} treeNode;

listNode *flatten(treeNode *root)
{
	if (root == NULL)
		return NULL;
	if (root->str != NULL) {
		/* Having characters there means it's a root node */
		listNode *curr = (struct listNode *) malloc(sizeof(struct listNode)); // Allocating space for new node
		curr->str = (char*) malloc(strlen(root->str) + 1); // Allocating space to copy a string
		strcopy(curr->str, root->str); // Copy string in
		return curr;
	}
	listNode *leftList = flatten(root->left);
	listNode *rightList = flatten(root->right);
	listNode *rightEnd = findEnd(rightList);

	// Make the linked list
	if (rightEnd == NULL) {
		return leftList;
	} else {
		rightEnd->next = leftList;
		return rightList;
	}

}

/* Spring 2020 Midterm 1 */

/* 1a--gets # bytes in string including null terminator */
int get_strlen(char* str)
{
	// FILL IN HERE
	/*
	Correct Options
	return strlen(str) + 1;
	sizeof(*str);
	*/
}

/* 1b-get the ith element of an array */
int getElem(int* arr, int i)
{
	return arr[i];
	return *(arr + i);
}

/* 1c--what does this print in 32-bit little endian? */
int main6()
{
	int doThis = 0x6C697665; // doThis set to an 32 bit hex #
	char *dont = (char *) (&doThis); // Pointing to the address of doThis... (convert to pointer of 1 byte at a time)
	printf("A: "); // Prints A: 
	for (int i = 0; i < 4; i++) {
		// Start at least significant byte: 65 --> 76 --> 69 --> 6C
		printf("%c", dont[i]); //dont points to address of doThis--array will therefore access doThis
	}
	printf("\n");
}

/*
Print result
A: 6C 69 76 65 --> evil
*/

/* 1c --Does this work? If not, where's the error? Else, what gets printed? */
#include <stdio.h>
#include <string.h>
int main7() 
{
	char *boo = “go cardinals!”; // Declaring a pointer to a string literal
	char *cheer = “go bears!!!!”; // Declaring a pointer to a different string literal
	printf(“%s”, cheer); // Properly prints go bears!!!!
	for (int i = 0; i < strlen(cheer); i++) {
		boo[i] = cheer[i]; // Wait... we can't change a string literal
	}
	boo = cheer; // Legal in the current setup
	boo[0] = 'a'; // Illegal b/c it's pointing to a string literal
	char newBoo[13];
	newBoo[0] = 'a'; // Legal: it's an array now
	printf(“%s”, boo);
}

/*
Yes--crash occurs at boo[i] = cheer[i] (can't set a pointer pointing to a string literal)
	- Can only change what the pointer is pointing to
	- Change *boo to an array
*/

/* Question 4
Each of the following evaluate to an address in memory. Where do they POINT?
*/

typedef struct node {
	void *data;
	struct node *left;
	struct node *right;
} node;

node* newNode (void *data) {
	node *n = (node*) malloc(sizeof(node)); // n points to heap space
	n->data = data; // n->data points to whatever was passed in
	n->left = NULL; // n->left initialized to NULL
	n->right = NULL; // n->right initialized to NULL
	return n;
}

int main8()
{
	char *r = "CS 61C Rocks!"; // r is a pointer to a string literal
	char s[] "CS61C Sucks!"; // s is a pointer to first elem of array declared locally
	node n1; // Uninitialized node
	n1.data = (void *) r; // n1.data points to the string literal r
	// root becomes a pointer to new node whose data points to address of main8)
	node *root = newNode((void *) &main8); // root is a pointer to a new node object
	root->left = malloc(strlen(r) + 1); // root->left is a pointer to heap space
	root->right = newNode((void*) s); // root->right is a pointer to a new node object
	root->right->left = newNode((void*) r); // root->right->left is a pointer to a new node object
	root->right->right = newNode((void*) &printf); //root->right->right is a pointer to a new node object
	root->left = &n1; // root->left is a pointer to the address of n1 (which was created locally--STACK)
}

/*
root: Is equal to pointer n, which points to malloc'ed--> HEAP
root->data: Points to address of a function name --> CODE
root->left: Points to address of a node object created locally --> STACK
root->left->data: Points to whatever n1.data points to: which is r --> STATIC
root->right->data: Points to an array passed in from a local function --> STACK
root->right->left->data: Points to r, a string literal that was passed in --> STATIC
&newNode: Points to an address of a function name--> CODE
*/

/* 
Q5

a. Modify the code below so that the original string is properly printed and so that there are no memory
leaks or undefined behavior.

b. Get the 5th smallest element

c. Check is an unsigned int is a power of 2
*/

void clue1(unsigned int* info, int len) {
	char* info_to_print = malloc(len * sizeof(char));
	for (int i = 0; i < len; i++) {
		info_to_print[i] = (char) (info[i] >> 24);
	}
	printf(“%s\n”, info_to_print);
	free(info_to_print);
}

void qsort(void *base, size_t num, size_t size, int (*comparator) (const void*, const void*));
/* qsort takes in a pointer to an array, # elements, size of each element, & address of comparison function
--> comparison function returns negative is first element is less than second, 0 if same,
or 1 if first element is bigger */

int comp(void *p1, void *p2)
{
	double a = *((double *) p1); // Cast the pointer: THEN dereference
	double b = *((double *) p2); // Same as above
	return a - b; // Just needs to return neg, 0, or pos--not necessarily -1, 0, 1
}

void clue2(double* info2, int len) {
	qsort(info2, len, sizeof(double), &comp);
	printf("%i\n", (int)info2[4]); // Should be cast to int
}

void powOf2(unsigned int* stairs, int len)
{
	int matchingEntries = 0;
	for (int i = 0; i < len; i++) {

		if (~(stairs[i] & stairs[i]-1)) {
			matchEntries ++;
		}

	}
	printf("%x\n", matchingEntries);
}

/* Summer 2020 Midterm 1 */

/*
2--Each evaluate to an address in memory. Where do they "point?"
*/
typedef struct node {
	void *data;
	struct node *next;
	struct node *prv;
} node;

void push_back(node *list, void *data) {
	node *n = (node*) malloc(sizeof(node)); // n points to heap space
	n->data = data; // n->data: points to whatever was passed in as data
	n->nxt = list; // n->nxt: points to whatever was passed in as list
	n->prv = list->prv; // n->prv: points to whatever list->prv points to
	list->prv->nxt = n; //list->prv->nxt: points to whatever the n points to (heap space)
	list->prv = n; // list->prv: points to whatever n points to as well (heap space)
}

int main9() {
	char *r = "CS 61C Rocks!"; // r is a pointer to a string literal
	char s[] = "CS 61C Sucks!"; // s is a pointer to an array of chars (pointing to the first char)
	node sentinel; // Initializing an empty sentinel (data, next, prev are all NULL)
	sentinel.nxt = &sentinel; // sentinel.nxt: points to address of sentinel (which is a local variable)
	sentinel.prv = &sentinel; //sentinel.prv: points to address of sentinel (which is a local variable)
	push_back(&sentinel, r); // sentinel.next->prev->next points to r; sentinel.prv->prv->next = r;
	push_back(&sentinel, s);
	push_back(&sentinel, &sentinel);
	push_back(&sentinel, calloc(sizeof(s) + 1, sizeof(char)));
}

/*
&sentinel: the address of sentinel points to sentinel: which is a local variable --> STACK
sentinel.nxt->nxt->data: sentinel.nxt is a pointer to sentinel's address. --> STACK
&push_back: address of a function points to a function --> CODE
sentinel.nxt->data: STACK
sentinel.prv->prv->data: Code
sentinel.nxt->data: Static
sentinel.prv->prv->data: Stack
sentinel.prv->data: Heap
sentinel.prv->prv: Heap??
*/

/*
How many bytes were not freed?
*/







