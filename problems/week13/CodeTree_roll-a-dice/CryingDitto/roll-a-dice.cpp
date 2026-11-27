#include <iostream>

using namespace std;

// 격자 크기, 주사위 굴릴 횟수
int n, m;
int r, c; // 주사위 초기 위치
char directions[10000];
int dir[10000];

// 이미 해당 위치에 숫자가 적혀 있었더라도, 새로 숫자가 적히게 됩니다. 
// grid로 숫자 관리해줘야 함
int grid[101][101]; // 1~100

int dx[4] = { -1, 1, 0, 0 };
int dy[4] = { 0, 0, -1, 1 };

// 주사위 초기 상태
int vertical[3] = { 5, 1, 2 }; // vertical[1] 이 위
int horizontal[4] = { 4, 1, 3, 6 }; // horizontal[1]이 위

// 주사위 반대쪽 숫자 구하는 함수
int getPair(int num)
{
    return 7 - num;
}

int getDirIdx(char d)
{
    if (d == 'U') return 0;
    if (d == 'D') return 1;
    if (d == 'L') return 2;
    return 3;
}

int roll(int idx)
{
    if (directions[idx] == 'U')
    {
        int center = vertical[2];
        int up = horizontal[1];
        int down = getPair(horizontal[1]);

        vertical[0] = up;
        vertical[2] = down;
        vertical[1] = center;
        horizontal[1] = center;
        horizontal[3] = getPair(center);
    }
    else if (directions[idx] == 'D')
    {
        int center = vertical[0];
        int up = getPair(horizontal[1]);
        int down = horizontal[1];

        vertical[0] = up;
        vertical[2] = down;
        vertical[1] = center;
        horizontal[1] = center;
        horizontal[3] = getPair(center);
    }

    if (directions[idx] == 'L')
    {
        int first = horizontal[0];
        for (int i = 0; i < 3; i++)
        {
            horizontal[i] = horizontal[i + 1];
        }
        horizontal[3] = first;
        vertical[1] = horizontal[1];
    }
    else if (directions[idx] == 'R')
    {
        int last = horizontal[3];
        for (int i = 3; i > 0; i--)
        {
            horizontal[i] = horizontal[i - 1];
        }
        horizontal[0] = last;
        vertical[1] = horizontal[1];
    }

    return horizontal[3];
}

int main() {
    cin >> n >> m >> r >> c;

    for (int i = 0; i < m; i++) {
        cin >> directions[i];
    }

    // Please write your code here.
    // horizontal[3]이 실제로 바닥에 찍히는 숫자임

    int x = r - 1;
    int y = c - 1;
    grid[x][y] = horizontal[3];
    for (int i = 0; i < m; i++)
    {
        int dirIdx = getDirIdx(directions[i]);

        int nx = x + dx[dirIdx];
        int ny = y + dy[dirIdx];
        // 해당 방향으로 주사위를 굴렸을 때 격자 밖으로 벗어나게 되는 경우라면, 
        // 주사위를 굴리지 않고 그다음 과정을 수행해야만 합니다.
        if (nx < 0 || ny < 0 || nx >= n || ny >= n)
        {
            continue;
        }
        int bottom = roll(i);
        grid[nx][ny] = bottom;

        x = nx;
        y = ny;
    }

    int total = 0;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            total += grid[i][j];
        }
    }
    cout << total;
    return 0;
}
