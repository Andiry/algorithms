#include<stdio.h>
#include<malloc.h>
#include<string.h>

struct node
{
	int value;
	struct node *left;
	struct node *right;
	struct node *next;
	struct node *prev;
	struct node *father;
};

struct node* insert(struct node *root, struct node *new)
{
	struct node *temp, *dad;

	if (root == NULL) {
		root = new;
		new->father = NULL;
	} else {
		temp = root;
		while (temp) {
			dad = temp;
			if (temp->value > new->value)
				temp = temp->left;
			else
				temp = temp->right;
		}

		if (dad->value > new->value)
			dad->left = new;
		else
			dad->right = new;
		new->father = dad;
	}

	return root;
}

void enqueue(struct node *node, struct node **head)
{
	struct node *temp;

	if (node == NULL)
		return;

	if (*head == NULL) {
		*head = node;
		node->next = NULL;
	} else {
		temp = *head;
		while(temp->next)
			temp = temp->next;
		temp->next = node;
	}
}
	
struct node *dequeue(struct node **head)
{
	struct node *temp;

	if (*head == NULL) {
		return NULL;
	} else {
		temp = *head;
		*head = (*head)->next;
		return temp;
	}
}

void print_tree(struct node *root)
{
	struct node *head = NULL;
	struct node *node = NULL;

	enqueue(root, &head);

	while(node = dequeue(&head)) {
		printf("%d\n", node->value);
		enqueue(node->left, &head);
		enqueue(node->right, &head);
	}
}

struct node* create_tree(void)
{
	struct node *array;
	struct node *root = NULL;
	int i;

	array = malloc(sizeof(struct node) * 7);

	memset(array, sizeof(struct node) * 7, 0);

	array[0].value = 10;
	array[1].value = 14;
	array[2].value = 6;
	array[3].value = 4;
	array[4].value = 12;
	array[5].value = 16;
	array[6].value = 8;

	for (i = 0; i < 7; i++) {
		root = insert(root, &array[i]);
	}

	return root;
}

int main(void)
{
	struct node *root;

	root = create_tree();

	print_tree(root);

	return 0;
}
