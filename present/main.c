#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

static void swap(int *ar, int i, int j)
{
    int temp = ar[i];
    ar[i] = ar[j];
    ar[j] = temp;
}
    
static int partition(int *ar, int start, int end)
{
    int sentry = ar[end];
    int i = start, j;
    
    for (j = start; j < end; j++) {
        if (ar[j] < sentry) {
            swap(ar, i, j);
            i++;
        }
    }
    swap(ar, i, end);
    return i;
}

static void qsort1(int *ar, int start, int end) 
{
    int pos;
    
    if (start >= end)
        return;
    
    pos = partition(ar, start, end);
    qsort1(ar, start, pos - 1);
    qsort1(ar, pos + 1, end);
}

static int binary_search(int *ar, int size, int k)
{
    int low = 0;
    int high = size - 1;
    int mid;
       
    if (k < ar[0])
        return 0;
    if (k >= ar[size - 1])
        return size;
    
    while (low <= high) {
        mid = low + ((high - low) >> 1);
        if (ar[mid] <= k && ar[mid + 1] > k)
            return mid + 1;
        else if (ar[mid] > k)
            high = mid - 1;
        else
            low = mid + 1;
    }
    return 0;
}

static void print_array(int *ar, int size)
{
    int i;
    
    for ( i = 0; i < size; i++)
        printf("%d ", ar[i]);
    printf("\n");
}

static int calc_num(int *ar, int size, int k)
{
    int i, sum = 0;
    
    qsort1(ar, 0, size - 1);
//    print_array(ar, size);
    for (i = 0; i < size; i++) {
        sum += ar[i];
	if (sum < 0 || sum > k)
		break;
    }

    return i;
    
//    print_array(ar, size);
//    return binary_search(ar, size, k);
    
}

int main() {

    /* Enter your code here. Read input from STDIN. Print output to STDOUT */   
    int n, k, size = 0;
    int *ar, i, temp;
    FILE *fp;
    
    fp = fopen("input.txt", "r");
    fscanf(fp, "%d %d\n", &n, &k);
    ar = (int *)malloc(n * sizeof(int));
    for (i = 0; i < n; i++) {
        fscanf(fp, "%d", &temp);
        if (temp <= k) {
            ar[size] = temp;
            size++;
        }
    }
    printf("%d\n", calc_num(ar, size, k));
    free(ar);
    return 0;
}

