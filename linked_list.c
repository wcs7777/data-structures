#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#define INSERT  'I'
#define REMOVE  'R'
#define PRINT   'P'
#define FREE    'F'
#define SEARCH  'S'
#define REVERSE 'V'
#define EXIT    'E'

struct Data {
	char key;
	int value;
};

struct Node {
	struct Data *data;
	struct Node *next;
};

struct Node* insert_node_after(
	struct Node **head,
	struct Node *previous_node,
	struct Node *node
);
struct Node* insert_node_before(
	struct Node **head,
	struct Node *next_node,
	struct Node *node
);
struct Node* find_by_key(struct Node *head, char key);
struct Node* find_previous_node(struct Node *head, struct Node *node);
struct Node* last_node(struct Node *head);
struct Node* remove_node(struct Node **head, struct Node *node);
struct Data* create_data(char key, int value);
struct Node* create_node(struct Data *data);
void free_list(struct Node **head);
void reverse_list(struct Node **head);
void print_data(const struct Data *data);
void print_node(const struct Node *node);
void print_list(const struct Node *head);
char read_option();
int do_insert(struct Node **head);
int do_remove(struct Node **head);
int do_search(struct Node **head);
int do_option(struct Node **head, char option);
char read_char(const char *prompt, ...);
int read_int(const char *prompt, ...);
void clear_buffer();
void clear_screen();

int main() {
	struct Node *head = NULL;
	char option;
	do {
		clear_screen();
		option = read_option();
		clear_screen();
		if (do_option(&head, option)) {
			free_list(&head);
			return 1;
		}
		clear_buffer();
	} while (option != EXIT);
	return 0;
}

struct Node* insert_node_after(
	struct Node **head,
	struct Node *previous_node,
	struct Node *node
) {
	if (previous_node == NULL) {
		previous_node = last_node(*head);
	}
	if (previous_node != NULL) {
		node->next = previous_node->next;
		previous_node->next = node;
	} else {
		node->next = NULL;
		*head = node;
	}
	return node;
}

struct Node* insert_node_before(
	struct Node **head,
	struct Node *next_node,
	struct Node *node
) {
	struct Node *previous_node = find_previous_node(*head, next_node);
	if (previous_node != NULL) {
		return insert_node_after(head, previous_node, node);
	} else {
		node->next = *head;
		*head = node;
		return node;
	}
}

struct Node* find_by_key(struct Node *head, char key) {
	while (head != NULL && head->data->key != key) {
		head = head->next;
	}
	return head;
}

struct Node* find_previous_node(struct Node *head, struct Node *node) {
	while (head != NULL && head->next != node) {
		head = head->next;
	}
	return head;
}

struct Node* last_node(struct Node *head) {
	while (head != NULL && head->next != NULL) {
		head = head->next;
	}
	return head;
}

struct Node* remove_node(struct Node **head, struct Node *node) {
	if (*head != node) {
		struct Node *previous = find_previous_node(*head, node);
		previous->next = node->next;
	} else {
		*head = node->next;
	}
	return node;
}

struct Data* create_data(char key, int value) {
	struct Data *data = (struct Data*)malloc(sizeof(struct Data));
	data->key = key;
	data->value = value;
	return data;
}

struct Node* create_node(struct Data *data) {
	struct Node *node = (struct Node*)malloc(sizeof(struct Node));
	node->data = data;
	return node;
}

void free_list(struct Node **head) {
	while (*head != NULL) {
		struct Node *node = remove_node(head, *head);
		free(node->data);
		free(node);
	}
	*head = NULL;
}

void reverse_list(struct Node **head) {
	struct Node *current = *head;
	struct Node *last = NULL;
	struct Node *previous = NULL;
	struct Node *node = current != NULL ? current->next : NULL;
	while (current != last) {
		while (node != last) {
			current->next = node->next;
			node->next = current;
			if (current == *head) {
				*head = node;
			} else if (previous != NULL) {
				previous->next = node;
			}
			previous = node;
			node = current->next;
		}
		last = current;
		current = *head;
		previous = NULL;
		node = current != NULL ? current->next : NULL;
	}
}

void print_data(const struct Data *data) {
	printf("(%c: %d)", data->key, data->value);
}

void print_node(const struct Node *node) {
	print_data(node->data);
}

void print_list(const struct Node *head) {
	while (head != NULL) {
		print_node(head);
		head = head->next;
		if (head != NULL) {
			fputs(", ", stdout);
		} else {
			putchar('\n');
		}
	}
}

char read_option() {
	const char option = read_char(
		"Linked list operation options\n"
		"\t<%c> INSERT\n"
		"\t<%c> REMOVE\n"
		"\t<%c> PRINT\n"
		"\t<%c> FREE\n"
		"\t<%c> SEARCH\n"
		"\t<%c> REVERSE\n"
		"\t<%c> EXIT\n"
		"\tOption: ",
		INSERT,
		REMOVE,
		PRINT,
		FREE,
		SEARCH,
		REVERSE,
		EXIT
	);
	return toupper(option);
}

int do_insert(struct Node **head) {
	const char key = read_char("Key: ");
	const int value = read_int("Value: ");
	char answer = read_char(
		"Type of inserting\n"
		"\t<A> After\n"
		"\t<B> Before\n"
		"\t<F> First\n"
		"\t<L> Last\n"
		"\tType: "
	);
	struct Data *data = create_data(key, value);
	struct Node *node = NULL;
	if (data == NULL) {
		return 1;
	}
	node = create_node(data);
	if (node == NULL) {
		free(data);
		return 1;
	}
	answer = toupper(answer);
	if (answer == 'A' || answer == 'L') {
		struct Node *previous_node = NULL;
		if (answer == 'A') {
			const char previous_key = read_char("Previous key: ");
			previous_node = find_by_key(*head, previous_key);
		} else {
			previous_node = last_node(*head);
		}
		insert_node_after(head, previous_node, node);
	} else {
		struct Node *next_node = NULL;
		if (answer == 'B') {
			const char next_key = read_char("Next key: ");
			next_node = find_by_key(*head, next_key);
		} else {
			next_node = *head;
		}
		insert_node_before(head, next_node, node);
	}
	return 0;
}

int do_remove(struct Node **head) {
	const char key = read_char("Key: ");
	struct Node *node = find_by_key(*head, key);
	if (node != NULL) {
		remove_node(head, node);
		fputs("Removed: ", stdout);
		print_node(node);
		free(node->data);
		free(node);
	} else {
		puts("Not found!");
	}
	return 0;
}

int do_search(struct Node **head) {
	const char key = read_char("Key: ");
	struct Node *node = find_by_key(*head, key);
	if (node != NULL) {
		fputs("Found: ", stdout);
		print_node(node);
	} else {
		puts("Not found!");
	}
	return 0;
}

int do_option(struct Node **head, char option) {
	switch (option) {
		case INSERT:
			return do_insert(head);
		case REMOVE:
			return do_remove(head);
		case PRINT:
			print_list(*head);
			return 0;
		case FREE:
			free_list(head);
			return 0;
		case SEARCH:
			return do_search(head);
		case REVERSE:
			reverse_list(head);
			return 0;
		case EXIT:
			puts("exiting");
			free_list(head);
			return 0;
		default:
			puts("Invalid option!");
	}
	return 0;
}

char read_char(const char *prompt, ...) {
	va_list args;
	char value;
	va_start(args, prompt);
	vfprintf(stdout, prompt, args);
	value = getchar();
	clear_buffer();
	va_end(args);
	return value;
}

int read_int(const char *prompt, ...) {
	va_list args;
	int value, n;
	va_start(args, prompt);
	do {
		vfprintf(stdout, prompt, args);
		n = scanf("%d", &value);
		clear_buffer();
	} while (n != 1);
	va_end(args);
	return value;
}

void clear_buffer() {
	signed char ch;
	do {
		ch = getchar();
	} while (ch != '\n' && ch != '\0' && ch != EOF);
}

void clear_screen() {
	int i;
	for (i = 0; i < 100; ++i) {
		putchar('\n');
	}
}
