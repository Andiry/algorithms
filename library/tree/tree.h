#include<deque>
#include<stack>

using namespace std;

struct node
{
	int value;
	struct node *left;
	struct node *right;
};

struct node * format_random_tree(int length, int seed, int range)
{
	struct node * root;

	root = new struct node[length];
	if (!root)
		return NULL;

	srand(seed);
	for (int i = 0; i < length; i++) {
		root[i].value = rand() % range;
		if (i * 2 + 1 < length)
			root[i].left = &root[i * 2 + 1];
		else
			root[i].left = NULL;
		if (i * 2 + 2 < length)
			root[i].right = &root[i * 2 + 2];
		else
			root[i].right = NULL;
	}

	return root;
}

void free_tree(struct node * root)
{
	delete [] root;
}

void midorder_tree(struct node * root)
{
	struct node * p = root;
	std::stack<struct node *> stack;

	printf("Midorder:\n");
	while(!stack.empty() || p) {
		while(p) {
			stack.push(p);
			p = p->left;
		}

		if (!stack.empty()) {
			p = stack.top();
			stack.pop();
			printf("%d\t", p->value);
		}

		p = p->right;
	}
	printf("\n");
}

void preorder_tree(struct node * root)
{
	struct node * p = root;
	std::stack<struct node *> stack;

	printf("Preorder:\n");
	stack.push(p);
	while(!stack.empty()) {
		p = stack.top();
		stack.pop();
		printf("%d\t", p->value);
	
		if (p->right)
			stack.push(p->right);
		if (p->left)
			stack.push(p->left);
	}
	printf("\n");
}

void postorder_tree(struct node * root)
{
	struct node * p = root;
	std::stack<struct node *> stack;

	printf("Postorder:\n");
	while(!stack.empty() || p) {

		if (p) {
			stack.push(p);
			if (p->right)
				stack.push(p->right);
			else if (p->left)
				stack.push(p->left);
		} else {
			p = stack.top();
			stack.pop();
			printf("%d\t", p->value);
		}
	}
	printf("\n");
}

void layer_tree(struct node * root)
{
	struct node * head = root;
	struct node * temp;
	std::deque<struct node *> queue;

	if (!head)
		return;

	printf("Layerorder:\n");
	queue.push_back(head);

	while(!queue.empty()) {
		temp = queue.front();
		printf("%d\t", temp->value);
		queue.pop_front();

		if (temp->left)
			queue.push_back(temp->left);
		if (temp->right)
			queue.push_back(temp->right);
	}
	printf("\n");
}
