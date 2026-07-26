// 2001. 파리 퇴치
// https://swexpertacademy.com/main/code/problem/problemDetail.do?contestProbId=AV5PzOCKAigDFAUq
#include <iostream>
#include <vector>
using namespace std;

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    int T;
    cin >> T;
    for (int tc = 0; tc < T; tc++)
    {
        int size, rangeSize;
        cin >> size >> rangeSize;

        vector<vector<int>> mat(size, vector<int>(size, 0));

        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                cin >> mat[i][j];
            }
        }
        int maxSum = 0;
        for (int i = 0; i < size - rangeSize + 1; i++)
        {
            for (int j = 0; j < size - rangeSize + 1; j++)
            {
                int curSum = 0;

                for (int kx = 0; kx < rangeSize; kx++)
                {
                    for (int ky = 0; ky < rangeSize; ky++)
                    {
                        int nx = i + kx;
                        int ny = j + ky;
                        curSum += mat[nx][ny];
                    }
                }
                maxSum = curSum > maxSum ? curSum : maxSum;
            }
        }

        cout << "#" << tc + 1 << " " << maxSum << "\n";
    }
    return 0;
}