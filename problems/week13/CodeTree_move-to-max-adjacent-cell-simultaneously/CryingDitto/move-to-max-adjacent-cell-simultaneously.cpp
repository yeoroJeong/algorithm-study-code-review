#include <iostream>
#include <queue>
#include <map>
using namespace std;

int n, m, t;
int a[20][20];
int r[400], c[400];

// 우선순위 상하좌우
int dx[4] = {-1, 1, 0, 0};
int dy[4] = {0, 0, -1, 1};

int mCnt = 0;

void search() {

    map<pair<int, int>, vector<int>> marbles;
    for (int i = 0; i < mCnt; i++) {
        int x = r[i];
        int y = c[i];

        int curVal = a[x][y];
        int maxVal = 0;
        for (int i = 0; i < 4; i++) {
            int nx = x + dx[i];
            int ny = y + dy[i];
            if (nx < 0 || ny < 0 || nx >= n || ny >= n) {
                continue;
            }
            if (a[nx][ny] > maxVal) {
                maxVal = a[nx][ny];
            }

        }

        for (int i = 0; i < 4; i++) {
            int nx = x + dx[i];
            int ny = y + dy[i];
            if (nx < 0 || ny < 0 || nx >= n || ny >= n) {
                continue;
            }
            if (a[nx][ny] == maxVal) {
                // 우선순위 있으므로 찾으면 바로 탈출
                // 다음 위치를 어딘가에 기록
                marbles[{nx, ny}].push_back(i);
                break;
            }
        }

    }

    mCnt = 0;
    for (auto kvPair : marbles) {
        auto xy = kvPair.first;
        vector<int> nums = kvPair.second;

        if (nums.size() > 1) {
            continue;
        }
        r[mCnt] = xy.first;
        c[mCnt] = xy.second;
        mCnt++;
    }
}

int main() {
    cin >> n >> m >> t;

    mCnt = m;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> a[i][j];
        }
    }

    for (int i = 0; i < m; i++) {
        int x, y;
        cin >> x >> y;
        r[i] = x - 1;
        c[i] = y - 1;
    }

    // Please write your code here.
    for (int time = 1; time <= t; time++) {
        search();
    }
    cout << mCnt;
    return 0;
}
