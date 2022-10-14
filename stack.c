#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#define POP   'P'
#define PUSH  'U'
#define PRINT 'R'
#define EMPTY 'E'
#define EXIT  'X'

struct Data {
	char key;
	int value;
};

struct Node {
	struct Data *data;
	struct Node *next;
};

struct Node* pop(struct Node **top);
struct Node* push(struct Node **top, struct Node *node);
void empty_stack(struct Node **top);
void print_stack(const struct Node *top);
void print_data(const struct Data *data);
struct Node* create_node(struct Data *data);
void free_node(struct Node *node);
struct Data* read_data();
char read_option();
void do_option(struct Node **top, char option);
void exit_memory_error(struct Node **top);
char read_char(const char *format, ...);
int read_int(const char *format, ...);
void clear_screen();
void clear_buffer();

int main() {
	struct Node *top = NULL;
	char option;
	do {
		clear_screen();
		option = read_option();
		clear_screen();
		do_option(&top, option);
		clear_buffer();
	} while (option != EXIT);
	return 0;
}

struct Node* pop(struct Node **top) {
	struct Node *node = *top;
	if (*top != NULL) {
		*top = (*top)->next;
	}
	return node;
}

struct Node* push(struct Node **top, struct Node *node) {
	node->next = *top;
	*top = node;
	return *top;
}

void empty_stack(struct Node **top) {
	struct Node *next = NULL;
	while (*top != NULL) {
		next = (*top)->next;
		free_node(*top);
		*top = next;
	}
}

void print_stack(const struct Node *top) {
	while (top != NULL) {
		print_data(top->data);
		fputs(", ", stdout);
		top = top->next;
	}
}

void print_data(const struct Data *data) {
	printf("(%c: %d)", data->key, data->value);
}

struct Node* create_node(struct Data *data) {
	struct Node *node = malloc(sizeof(*node));
	if (node != NULL) {
		node->data = data;
	}
	return node;
}

void free_node(struct Node *node) {
	free(node->data);
	free(node);
}

struct Data* read_data() {
	struct Data *data = malloc(sizeof(*data));
	if (data != NULL) {
		data->key = read_char("Key: ");
		data->value = read_int("Value: ");
	}
	return data;
}

char read_option() {
	return toupper(
		read_char(
			"<%c> POP\n"
			"<%c> PUSH\n"
			"<%c> PRINT\n"
			"<%c> EMPTY\n"
			"<%c> EXIT\n\n"
			"Option: ",
			POP,
			PUSH,
			PRINT,
			EMPTY,
			EXIT
		)
	);
}

void do_option(struct Node **top, char option) {
	struct Node *node = NULL;
	struct Data *data = NULL;
	switch (option) {
		case POP:
			node = pop(top);
			if (node != NULL) {
				puts("Popped");
				print_data(node->data);
				free_node(node);
			} else {
				puts("Stack is empty!");
			}
			break;
		case PUSH:
			data = read_data();
			if (data == NULL) {
				exit_memory_error(top);
			}
			node = create_node(data);
			if (node == NULL) {
				free(data);
				exit_memory_error(top);
			}
			push(top, node);
			break;
		case PRINT:
			print_stack(*top);
			break;
		case EMPTY:
			puts("Emptying stack...");
			empty_stack(top);
			break;
		case EXIT:
			puts("Exiting...");
			empty_stack(top);
			break;
		default:
			puts("Invalid option!");
			break;
	}
}

void exit_memory_error(struct Node **top) {
	fputs("Error allocating memory!\n", stderr);
	empty_stack(top);
	exit(1);
}

char read_char(const char *format, ...) {
	va_list args;
	char value;
	va_start(args, format);
	vfprintf(stdout, format, args);
	value = getchar();
	clear_buffer();
	va_end(args);
	return value;
}

int read_int(const char *format, ...) {
	va_list args;
	int value, n;
	va_start(args, format);
	do {
		vfprintf(stdout, format, args);
		n = scanf("%d", &value);
		clear_buffer();
	} while (n != 1);
	va_end(args);
	return value;
}

void clear_screen() {
	int i;
	for (i = 0; i < 50; ++i) {
		putchar('\n');
	}
}

void clear_buffer() {
	signed char c;
	do {
		c = getchar();
	} while (c != '\n' && c != EOF);
}
