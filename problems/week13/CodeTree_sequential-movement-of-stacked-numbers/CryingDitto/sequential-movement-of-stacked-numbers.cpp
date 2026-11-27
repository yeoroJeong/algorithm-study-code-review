#include <iostream>
#include <vector>
#include <deque>
using namespace std;

int n, m;
int move_nums[100];

vector<int> numsGrid[20][20];
// 번호로 위치 찾을 수 있어야 함
vector<pair<int, int>> numToPos;

int dx[8] = { -1, -1, -1, 0, 0, 1,1,1 };
int dy[8] = { -1, 0, 1, -1, 1, -1, 0, 1 };
void search(int numToMove, int x, int y)
{
    int maxVal = 0;
    // tempMax 못 찾으면 제자리에 있어야 하므로.
    int px = x;
    int py = y;
    for (int i = 0; i < 8; i++)
    {
        int nx = x + dx[i];
        int ny = y + dy[i];
        if (nx < 0 || ny < 0 || nx >= n || ny >= n)
        {
            continue;
        }
        int tempMax = 0;
        for (int num : numsGrid[nx][ny])
        {
            if (num > tempMax)
            {
                tempMax = num;
            }
        }

        if (tempMax > maxVal)
        {
            maxVal = tempMax;
            px = nx;
            py = ny;
        }

    }

    if (x == px && y == py) return;
    // 찾은 maxVal 자리로 이동
    // 옮기려는 값 위치에서 옮기려는 값 나올 때까지 뒤에서 줄줄이 빼서 temp에 담아두었다가
    // 이사갈 자리 뒤쪽에 넣기

    vector<int> tempVec;
    while (numsGrid[x][y].back() != numToMove)
    {
        int num = numsGrid[x][y].back();
        numsGrid[x][y].pop_back();
        tempVec.push_back(num);
    }

    numsGrid[x][y].pop_back();
    numsGrid[px][py].push_back(numToMove);
    numToPos[numToMove] = { px, py };

    while (tempVec.size() > 0)
    {
        int num = tempVec.back();
        tempVec.pop_back();
        numsGrid[px][py].push_back(num);
        numToPos[num] = { px, py };
    }
}


int main() {
    cin >> n >> m;
    numToPos.resize(n * n + 1);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int number;
            cin >> number;
            numsGrid[i][j].push_back(number);
            numToPos[number] = { i, j };
        }
    }

    for (int i = 0; i < m; i++) {
        cin >> move_nums[i];
    }

    // Please write your code here.
    for (int i = 0; i < m; i++)
    {
        int curNum = move_nums[i];
        pair<int, int> pos = numToPos[curNum];
        search(curNum, pos.first, pos.second);
    }
    

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (numsGrid[i][j].size() == 0)
            {
                cout << "None\n";
                continue;
            }
            for (int idx = numsGrid[i][j].size() - 1; idx >= 0; idx--)
            {
                cout << numsGrid[i][j][idx] << " ";
            }
            cout << "\n";
        }
    }

    return 0;
}
