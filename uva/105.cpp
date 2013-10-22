#include <iostream>
#include <string.h>

#define MAX_WEIGHT 10000

using namespace std;

int main(void)
{
    int height[MAX_WEIGHT + 1];
    memset(height, 0, sizeof(int) * (MAX_WEIGHT + 1));

    int l, h, r, bound = 0;
    while (cin >> l >> h >> r)
    {
        if (r > bound) { bound = r; }

        for (int i = l; i < r; i++)
        {
            if (h > height[i])
            {
                height[i] = h;
            }
        }
    }

    int current = 0;
    for (int i = 0; i < bound; i++)
    {
        if (height[i] != current)
        {
            cout << i << " " << height[i] << " ";
            current = height[i];
        }
    }

    cout << bound << " " << height[bound] << endl;


    return 0;
}
