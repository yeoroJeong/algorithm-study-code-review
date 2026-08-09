// 1949. 등산로 조성
// https://swexpertacademy.com/main/code/problem/problemDetail.do?contestProbId=AV5PoOKKAPIDFAUq&
#include <iostream>
#include <vector>
using namespace std;

// inputs
int sizeN, cutCount;
int map[8][8];
bool visited[8][8];
// left, right, up, down
int dx[4] = { -1, 1, 0, 0 };
int dy[4] = { 0, 0, -1, 1 };
// maximum length of path
int mxLength = -1;

void search(int xPos, int yPos, int curHeight, int length, int isCut)
{
    mxLength = max(mxLength, length);
    visited[xPos][yPos] = true;

    for (int i = 0; i < 4; i++)
    {
        int nextX = xPos + dx[i];
        int nextY = yPos + dy[i];

        if (nextX < 0 || nextX >= sizeN || nextY < 0 || nextY >= sizeN) continue;
        if (visited[nextX][nextY]) continue;

        int nextHeight = map[nextX][nextY];

        if (nextHeight < curHeight)
        {
            // possible to go down
            search(nextX, nextY, nextHeight, length + 1, isCut);
        }
        else if (!isCut)
        {
            if (nextHeight - curHeight < cutCount)
            {
                // possible to cut and go down
                // 높이는 높을수록 유리하므로 (현재 높이-1)까지만 깎음
                search(nextX, nextY, curHeight - 1, length + 1, true);
            }
        }
    }
    visited[xPos][yPos] = false; // restore to visit

    return;
}


int main()
{
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    int T;
    cin >> T;
    for (int tc = 0; tc < T; tc++)
    {
        cin >> sizeN >> cutCount;

        int maxHeight = -1;
        for (int i = 0; i < sizeN; i++)
        {
            for (int j = 0; j < sizeN; j++)
            {
                cin >> map[i][j];
                maxHeight = max(map[i][j], maxHeight);
            }
        }
        // collect highest start x,y points
        vector<pair<int, int>> startPoints;
        for (int i = 0; i < sizeN; i++)
        {
            for (int j = 0; j < sizeN; j++)
            {
                if (map[i][j] == maxHeight)
                {
                    startPoints.push_back({ i,j });
                }
            }
        }
        mxLength = -1;
        for (auto point : startPoints)
        {
            search(point.first, point.second, map[point.first][point.second], 1, false);
        }

        cout << "#" << tc + 1 << " " << mxLength << "\n";
    }
    return 0;
}