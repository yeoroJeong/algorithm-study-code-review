#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

// 0:상 / 1:하 / 2:좌  / 3:우
int dx[4] = {-1, 1, 0, 0};
int dy[4] = {0, 0, -1, 1};

int n, m, t;

struct Bead {
    int dir; // 현재 공 번호의 방향
    int weight = 0; // 무게
    pair<int, int> pos; // 위치
    bool isAvail = true; // 합쳐졌는지 안 합쳐졌는지 확인용
};

map<int, Bead> beads;

int main() {
    cin >> n >> m >> t;

    // 구슬 정보 입력
    for (int i = 1; i <= m; i++) {
        int x, y, weight;
        char dir;
        cin >> x >> y >> dir >> weight;
        beads[i].pos = {x, y};
        beads[i].weight = weight;
        switch (dir) {
        case 'U':
            {
                beads[i].dir = 0;
                break;
            }
        case 'D':
            {
                beads[i].dir = 1;
                break;
            }
        case 'L':
            {
                beads[i].dir = 2;
                break;
            }
        case 'R':
            {
                beads[i].dir = 3;
                break;
            }

        }
    }

    // Please write your code here.
    // 벽에 부딪혔을 때 방향이 바뀐다.
    // 두 개 이상의 구슬이 같은 위치로 오게 된다면 이는 충돌이 발생하게 되고,
    // 충돌이 일어나면 해당 위치에 있던 구슬들은 전부 합쳐지게 됩니다

    // 가장 큰 번호가 매겨져 있는 구슬의 방향을 따르게 되고,
    // 번호는 마찬가지로 충돌이 일어난 구슬들 중 가장 큰 번호를 갖게 됩니다.

    /*
        map으로 관리
    */
    return 0;
}
