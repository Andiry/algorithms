#include <iostream>
#include <cstring>

#define MAX_SIZE 1024

using namespace std;

int main(void)
{
    char line[MAX_SIZE];
    while (cin.getline(line, MAX_SIZE))
    {
        // 根據空白切割字串
        char *token = strtok(line, " ");
        while (token)
        {
            // 顛倒印出字串
            int length = strlen(token);
            for (int i = length - 1; i >= 0; i--)
            {
                cout << token[i];
            }

            token = strtok(NULL, " ");
            if (token) { cout << " "; }
        }

        cout << endl;
    }


    return 0;
}
