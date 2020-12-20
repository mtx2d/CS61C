

// CSM Midterm 2 Review #2

int order = 66;

char *execute(int a) {
	char *s = "I am the senate";
	return s;
}

int main() {
	char t[] = "Name";
	int *reason = malloc(sizeof(int) * 50);
	execute(order);
	return 0;
}

Where the following being stored?

1. order: global variable, so it is stored in STATIC memory
2. execute: function name, stored in CODE
3. s: local variable, so it exists in STACK
4. s[0]: *s is a string literal, so stored in STATIC memory
5. t: local variable, so stored in STACK
6. t[0]: char arrays copy over string literals, so pointer to it exists in STACK
7. reason[0]: reason is a pointer to heap space, so reason[0] stored in HEAP