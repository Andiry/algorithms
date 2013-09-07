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

struct node* find_next(struct node *node)
{
	struct node * temp, *dad;

	if (node->right == NULL) {
		if (!node->father)
			return NULL;
		temp = node->father;
		if (temp->left == node) {
			dad = node->father;
		} else {
			while(temp) {
				dad = temp->father;
				if (dad && dad->left == temp)
					break;
				temp = dad;
			}
		}
		node->next = dad;
		if (dad)
			dad->prev = node;
		return dad;
	} else {
		temp = node->right;
		while(temp->left)
			temp = temp->left;

		node->next = temp;
		temp->prev = node;
		return temp;
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
	struct node *root, *begin, *temp;

	root = create_tree();
	begin = root;

	while(begin->left)
		begin = begin->left;

	temp = begin;
	while(temp) {
		printf("%d\n", temp->value);
		temp = find_next(temp);
	}

	return 0;
}
