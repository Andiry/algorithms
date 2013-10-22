#include <iostream>

using namespace std;

int main(void)
{
    int n, c;
    while (cin >> n >> c)
    {
        // 變數初始化
        bool *prime = new bool[n];
        for (int i = 0; i < n; i++)
        {
            prime[i] = true;
        }

        // 紀錄哪些數是質數
        int num = 1;
        for (int i = 2; i <= n; i++)
        {
            if (prime[i - 1])
            {
                num++;
                for (int j = 2; j * i <= n; j++)
                {
                    prime[j * i - 1] = false;
                }
            }
        }

        cout << n << " " << c << ":";

        int i, x, p = 2 * c - (num % 2 ? 1 : 0);

        // 跳過不需輸出的質數
        x = 0;
        for (i = 0; x < (num - p) / 2; i++)
        {
            if (prime[i]) { x++; }
        }

        // 開始輸出的質數
        x = 0;
        for ( ; x < p && i < n; i++)
        {
            if (prime[i])
            {
                cout << " " << i + 1;
                x++;
            }
        }

        cout << endl << endl;
    }

    return 0;
}
