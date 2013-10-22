#include <iostream>
#include <cstring>

using namespace std;

int main(void)
{
    int n;
    cin >> n;

    bool first = true;
    for (int i = 0; i < n; i++)
    {
        int a, f;
        cin >> a >> f;

        string wave;
        char number[10];

        // 畫出上波浪
        for (int j = 1; j < a; j++)
        {
            memset(number, (j + '0'), j);
            number[j] = '\0';

            wave += number;
            wave += '\n';
        }

        // 畫出下波浪
        for (int j = a; j > 0; j--)
        {
            memset(number, (j + '0'), j);
            number[j] = '\0';

            wave += number;
            wave += '\n';
        }

        for (int j = 0; j < f; j++)
        {
            // 判別是否該換行
            if (first)  { first = false; }
            else        { cout << endl; }

            // 輸出波浪
            cout << wave;
        }
    }

    return 0;
}
