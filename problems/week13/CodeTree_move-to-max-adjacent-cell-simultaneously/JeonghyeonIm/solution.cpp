/*
정수가 가장 큰 인접한 곳으로 동시에 이동

1이상 100이하 N * N 크기 격자판
M개의 구슬이 서로 다른 위치에 시작해 1초에 한 번(상 하 좌 우)인접 중 가장 큰 값이 적혀 있는 위치로 동시에 이동.
위치가 여러개인 경우, 상 -> 하 -> 좌 -> 우 방향대로 우선순위

T초 후 남아 있는 구슬의 수 출력

충돌: 이동이후 2개 이상의 구슬이 한 좌표에 있다면 모두 삭제
*/

#include <iostream>
#include <map>
#include <set>
#include <vector>
using namespace std;

int n, m, t;
int a[20][20]; // 격자
int r[400], c[400]; // 구슬 좌표

int dr[4] = {-1, 1, 0, 0}; // 상 -> 하 -> 좌 -> 우
int dc[4] = {0, 0, -1, 1};

int main() {
    cin >> n >> m >> t;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> a[i][j];
        }
    }

    for (int i = 0; i < m; i++) {
        cin >> r[i] >> c[i];
        r[i]--;
        c[i]--;
    }

    int rst = m;
    // Please write your code here.
    for (int i = 1; i <= t; i++) { // t초 동안
        for (int j = 0; j < m; j++) { // 구슬 이동 처리
            int mxidx = 0, mx = 0;
            int c_r = r[j];
            int c_c = c[j];
            if (c_r == -1 && c_c == -1) { // 삭제된 구슬 안함
                continue;
            }
            for (int k = 0; k < 4; k++) {
                int n_r = c_r + dr[k];
                int n_c = c_c + dc[k];
                if (n_r < 0 || n_r >= n || n_c < 0 || n_c >= n) {
                    continue;
                }
                if (mx < a[n_r][n_c]) {
                    mx = a[n_r][n_c];
                    mxidx = k;
                }
            }

            r[j] = c_r + dr[mxidx];
            c[j] = c_c + dc[mxidx];
        }

        map<pair<int, int>, int> cnt; // key: (r, c)좌표, value: 구슬번호
        set<int> key; // 중복 구슬
        for (int j = 0; j < m; j++) {
            if (r[j] == -1 && c[j] == -1) { // 삭제된 구슬 안함
                continue;
            }

            if (cnt.count({r[j], c[j]})) { // 존재
                key.insert(cnt[{r[j], c[j]}]);
                r[j] = -1;
                c[j] = -1;
                rst -= 1;
            } else {
                cnt[{r[j], c[j]}] = j;
            }
        }
        for (auto x : key) {
            r[x] = -1;
            c[x] = -1;
            rst -= 1;
        }
    }

    cout << rst;

    return 0;
}
