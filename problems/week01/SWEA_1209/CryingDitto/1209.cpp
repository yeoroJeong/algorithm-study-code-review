// 1209.[S / W 문제해결 기본] 2일차 - Sum
// https://swexpertacademy.com/main/code/problem/problemDetail.do?contestProbId=AV13_BWKACUCFAYh
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    for (int tc = 0; tc < 10; tc++)
    {
        int tNum;
        cin >> tNum;
        int mat[100][100] = { 0 };
        int colMax = 0, rowMax = 0, rightToLeft = 0, leftToRight = 0;

        for (int i = 0; i < 100; i++)
        {
            int curCol = 0, curRow = 0;

            for (int j = 0; j < 100; j++)
            {
                int num;
                cin >> num;
                // row 누적합
                curRow += num;

                // col 누적합
                if (i > 0)
                {
                    mat[i][j] = mat[i - 1][j] + num;
                }
                else
                {
                    mat[i][j] = num;
                }

                // 대각선 합
                if (i == j)
                {
                    leftToRight += num;
                }
                if (i + j == 99)
                {
                    rightToLeft += num;
                }

                // col 최댓값 계산
                if (i == 99)
                {
                    colMax = mat[i][j] > colMax ? mat[i][j] : colMax;
                }
            }
            rowMax = curRow > rowMax ? curRow : rowMax;
        }

        int ans = max({rowMax, colMax, leftToRight, rightToLeft})
        /*if (ans < rowMax)
        {
            ans = rowMax;
        }
        if (ans < colMax)
        {
            ans = colMax;
        }
        if (ans < leftToRight)
        {
            ans = leftToRight;
        }
        if (ans < rightToLeft)
        {
            ans = rightToLeft;
        }*/
        cout << "#" << tNum << " " << ans << "\n";
    }
    return 0;
}