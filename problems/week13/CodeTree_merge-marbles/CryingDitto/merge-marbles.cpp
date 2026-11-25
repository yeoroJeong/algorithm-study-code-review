#include <iostream>
#include <vector>
#include <map>
using namespace std;

int n, m, t;
int r[2500], c[2500];
int dirIdx[2500];
int w[2500];

struct marble {
    int mIdx;
    pair<int, int> pos;
    int dir;
    int weight;
};
vector<marble> marbleList;

// up down left right
int dx[4] = { -1, 1, 0, 0 };
int dy[4] = { 0, 0, -1, 1 };

int getDirIdx(char dir) {
    if (dir == 'U') return 0;
    if (dir == 'D') return 1;
    if (dir == 'L') return 2;
    return 3;
}

int getReverseDir(int dir) {
    if (dir < 2) {
        return (3 - dir) % 2;
    }
    return (2 + (3 - dir));
}

void simulate() {
    map<pair<int, int>, vector<marble>> grid;

    for (int i = 0; i < m; i++) {
        // 구슬 좌표 돌면서 위치에다 구슬 번호 할당
        // 구슬 번호는 i+1
        marble curMarble = marbleList[i];
        int x = curMarble.pos.first;
        int y = curMarble.pos.second;
        int nx = x + dx[curMarble.dir];
        int ny = y + dy[curMarble.dir];

        if (nx < 0 || ny < 0 || nx >= n || ny >= n) {
            grid[{x, y}].push_back({ curMarble.mIdx, {x, y}, getReverseDir(curMarble.dir), curMarble.weight });
            continue;
        }
        grid[{nx, ny}].push_back({ curMarble.mIdx, {nx, ny}, curMarble.dir, curMarble.weight });

    }

    int mCnt = 0;
    for (auto kvPair : grid) {
        pair<int, int> pos = kvPair.first;

        // 구슬 무게는 합산, 방향은 가장 큰 번호의 구슬 방향, 번호도 가장 큰 번호
        int weightSum = 0;
        int dirNext = -1;
        int maxMnum = -1;

        for (marble& mb : kvPair.second) {
            weightSum += mb.weight;
            if (maxMnum < mb.mIdx)
            {
                maxMnum = mb.mIdx;
                dirNext = mb.dir;
            }
        }

        marbleList[mCnt] = { maxMnum, pos, dirNext, weightSum };
        mCnt++;
    }
    m = mCnt;
}

int main() {
    cin >> n >> m >> t;

    marbleList.resize(m);
    for (int i = 0; i < m; i++) {
        char dir;
        cin >> r[i] >> c[i] >> dir >> w[i];

        dirIdx[i] = getDirIdx(dir);
        marbleList[i] = { i + 1, {r[i]-1, c[i]-1}, dirIdx[i], w[i] };

    }

    // Please write your code here.
    for (int time = 1; time <= t; time++)
    {
        simulate();
    }

    // T초 후에도 여전히 남아있는 구슬의 수와 가장 무거운 구슬의 무게
    int maxWeight = 0;
    for (int i = 0; i < m; i++)
    {
        if (maxWeight < marbleList[i].weight)
        {
            maxWeight = marbleList[i].weight;
        }
    }
    cout << m << " " << maxWeight;

    return 0;
}
