#include<stdio.h>

int shortest = 1 << 30;

int shortest_length(int *a, int *b, int node1, int node2, int nodes, int curr)
{
	int i = 0;
	int temp;

	for (i = 0; i < nodes; i++) {
		if (a[i] == node1 && b[i] == node2) {
			curr++;
			if (curr < shortest)
				shortest = curr;
			return curr;
		} else if (a[i] == node1) {
			temp = (1 + shortest_length(a, b, b[i], node2, nodes, curr));
			if (temp < shortest)
				shortest = temp;
			break;
		}
	}

	if (i == nodes)
		return 1 << 30;

	return shortest;
}

int main(void)
{
	int i, j, nodes, edges;
	float total = 0.0;
	int a[100], b[100], c;

	i = 0;
	while(1) {
		scanf("%d %d ", &a[i], &b[i]);
		if (a[i] == 0 && b[i] == 0)
			break;
		i++;
	}

	edges = i;
	nodes = 4;

	for (i = 0; i < nodes; i++) {
		for (j = 0; j < nodes; j++) {
			if (i != j) {
				c = shortest_length(a, b, i+1, j+1, edges, 0);
				printf("%d -> %d: %d\n", i+1, j+1, c);
				total += c;
				
			}
		}
	}

	if (nodes == 0 || nodes == 1)
		return 0;

	total = total / (nodes * (nodes - 1));
	printf("%f\n", total);

	return 0;
}


