#include <iostream>
#include <vector>
#include <map>
using namespace std;

int T;
int N, M;

struct Marble {
    int mId;
    int x;
    int y;
    int dir;
};

vector<Marble> marbleList;

int dx[4] = {-1, 1, 0, 0};
int dy[4] = {0, 0, -1, 1};
int getDir(char d) {
    if (d == 'U') return 0;
    if (d == 'D') return 1;
    if (d == 'L') return 2;
    return 3;
}
int getReverseDir(int d) {
    if (d < 2) {
        return 1 - d;
    }
    return 2 + (3 - d);
}

void simulate() {
    int marbleGrid[50][50] = {0}; // 각 자리에 모인 구슬 개수

    for (int i = 0; i < M; i++) {
        Marble& curM = marbleList[i];

        int nx = curM.x + dx[curM.dir];
        int ny = curM.y + dy[curM.dir];

        if (nx < 0 || ny < 0 || nx >= N || ny >= N) {
            curM.dir = getReverseDir(curM.dir);
        }
        else {
            curM.x = nx;
            curM.y = ny;
        }

        marbleGrid[curM.x][curM.y]++;
    }

    vector<Marble> temp;
    for (const auto& cur : marbleList) {
        if (marbleGrid[cur.x][cur.y] == 1) {
            temp.push_back(cur);
        }
    }
    marbleList.swap(temp);
    M = marbleList.size();
}

int main() {
    cin >> T;

    for (int tc = 0; tc < T; tc++) {
        cin >> N >> M;

        marbleList.clear();
        for (int i = 0; i < M; i++) {
            // 구슬 정보
            int x, y; char dir;
            cin >> x >> y >> dir;
            marbleList.push_back({i + 1, x - 1, y - 1, getDir(dir)});
        }

        // Please write your code here.

        // 아주 오랜 시간이 흐른 후에도 => 결국 구슬은 방향이 상하 또는 좌우로만 움직이므로 2n번 정도 움직이면 결과가 나오지 않을까?

        for (int time = 1; time <= 2 * N; time++) {
            if (M <= 1) break; // 구슬이 하나만 남으면 조기 종료
            simulate();
        }

        cout << M << "\n";
    }

    return 0;
}
