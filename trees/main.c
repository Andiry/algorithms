#include<stdio.h>
#include<stdlib.h>

#include"../library/common.h"
#include"../library/array/array.h"
#include"../library/tree/tree.h"

int main(int argc, char **argv)
{
	struct node * root;
	int length = 12;
	int seed, ret;
	int range = 256;
	std::vector<struct node *>path_v;

	if (argc < 2)
		seed = 124;
	else
		seed = atoi(argv[1]);

	root = format_random_tree(length, seed, range);
	if (!root)
		goto out;

	layer_tree(root);
	midorder_tree(root);
	preorder_tree(root);
	postorder_tree(root);
	find_path(root, 12, path_v);

out:
	free_tree(root);
	return ret;
}
