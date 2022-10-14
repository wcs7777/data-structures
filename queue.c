#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#define DEQUEUE 'D'
#define ENQUEUE 'E'
#define PRINT   'P'
#define EMPTY   'M'
#define EXIT    'X'

struct Data {
	char key;
	int value;
};

struct Node {
	struct Data *data;
	struct Node *next;
};

struct Node *dequeue(struct Node **head, struct Node **tail);
struct Node *enqueue(
	struct Node **head,
	struct Node **tail,
	struct Node *node
);
void print_queue(const struct Node *head);
void print_data(const struct Data *data);
void empty_queue(struct Node **head, struct Node **tail);
struct Node* create_node(struct Data *data);
void free_node(struct Node *node);
struct Data* read_data();
char read_option();
void do_option(struct Node **head, struct Node **tail, char option);
void exit_memory_error(struct Node **head, struct Node **tail);
char read_char(const char *format, ...);
int read_int(const char *format, ...);
void clear_screen();
void clear_buffer();

int main() {
	struct Node *head = NULL;
	struct Node *tail = NULL;
	char option;
	do {
		clear_screen();
		option = read_option();
		clear_screen();
		do_option(&head, &tail, option);
		clear_buffer();
	} while (option != EXIT);
	return 0;
}

struct Node *dequeue(struct Node **head, struct Node **tail) {
	struct Node *node = *head;
	if (*head != NULL) {
		*head = (*head)->next;
	}
	if (*head == NULL) {
		*tail = NULL;
	}
	return node;
}

struct Node *enqueue(
	struct Node **head,
	struct Node **tail,
	struct Node *node
) {
	if (*tail != NULL) {
		(*tail)->next = node;
	} else {
		*head = node;
	}
	*tail = node;
	node->next = NULL;
	return *head;
}

void print_queue(const struct Node *head) {
	while (head != NULL) {
		print_data(head->data);
		fputs(", ", stdout);
		head = head->next;
	}
}

void print_data(const struct Data *data) {
	printf("(%c: %d)", data->key, data->value);
}

void empty_queue(struct Node **head, struct Node **tail) {
	struct Node *next = NULL;
	while (*head != NULL) {
		next = (*head)->next;
		free_node(*head);
		*head = next;
	}
	*tail = *head;
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
			"<%c> Dequeue\n"
			"<%c> Enqueue\n"
			"<%c> Print\n"
			"<%c> Empty\n"
			"<%c> Exit\n"
			"\nOption: ",
			DEQUEUE,
			ENQUEUE,
			PRINT,
			EMPTY,
			EXIT
		)
	);
}

void do_option(struct Node **head, struct Node **tail, char option) {
	struct Node *node = NULL;
	struct Data *data = NULL;
	switch (option) {
		case DEQUEUE:
			node = dequeue(head, tail);
			if (node != NULL) {
				puts("Dequeued");
				print_data(node->data);
				free_node(node);
			} else {
				puts("Queue is empty!");
			}
			break;
		case ENQUEUE:
			data = read_data();
			if (data == NULL) {
				exit_memory_error(head, tail);
			}
			node = create_node(data);
			if (node == NULL) {
				free(data);
				exit_memory_error(head, tail);
			}
			enqueue(head, tail, node);
			break;
		case PRINT:
			print_queue(*head);
			break;
		case EMPTY:
			puts("Emptying queue...");
			empty_queue(head, tail);
			break;
		case EXIT:
			puts("Exiting...");
			empty_queue(head, tail);
			break;
		default:
			puts("Invalid option!");
			break;
	}
}

void exit_memory_error(struct Node **head, struct Node **tail) {
	fputs("Error allocating memory!\n", stderr);
	empty_queue(head, tail);
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
