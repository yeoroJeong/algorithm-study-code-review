/*
합쳐지는 구슬들

M개의 구슬이 N * N 격자 안에 놓여있음.
각 구슬은 상하좌우 중 한 방향으로 이동, 1초에 한 칸 움직임
각 구슬에는 번호가 매겨져 있음. 구슬마다 무게 존재
구슬의 번호는 입력순

구슬이 벽에 부딪히면 움직이는 방향이 반대로 뒤집혀 동일한 속도로 움직(방향 전환만 1초 시간 소요 해당 구슬 맵 위치 변환는 없음)
충돌: 두 개 이상의 구슬이 같은 위치로 오게 될 경우
충돌이 발생하면 해당 위치에 있던 구슬은 합쳐짐
구슬의 무게: 모인 모든 구슬의 합
방향, 번호: 가장 큰 번호가 매겨져 있는 구슬


출력: T초가 지났을 때 남아있는 '구슬의 개수' 및 '가장 무거운 구슬의 무게'
*/

#include <iostream>
#include <map>
#include <algorithm>
using namespace std;

int n, m, t;
int r[2500], c[2500];
char d[2500]; // 방향(U, D, R, L)
int w[2500]; // 무게

int dr[4] = {-1, 1, 0, 0}; // U D R L순
int dc[4] = {0, 0, 1, -1};
int main() {
    int idx[256];
    idx['U'] = 0;
    idx['D'] = 1;
    idx['R'] = 2;
    idx['L'] = 3;

    cin >> n >> m >> t;
    int rst_cnt = m, rst_heavy = 0;
    for (int i = 0; i < m; i++) {
        cin >> r[i] >> c[i] >> d[i] >> w[i];
        r[i] -= 1;
        c[i] -= 1;
        rst_heavy = max(rst_heavy, w[i]);
    }

    // Please write your code here.

    for (int i = 1; i <= t; i++) {
        for (int j = 0; j < m; j++) { // 구슬 이동 OR 방향 전환
            int c_r = r[j];
            int c_c = c[j];
            if (c_r == -1 && c_c == -1) { // 삭제된 구슬
                continue;
            }
            int n_r = c_r + dr[idx[d[j]]];
            int n_c = c_c + dc[idx[d[j]]];
            if (n_r < 0 || n_r >= n || n_c < 0 || n_c >= n) { // 격자 벗어남 => 방향 만 반대
                if (d[j] == 'U') {
                    d[j] = 'D';
                } else if (d[j] == 'D') {
                    d[j] = 'U';
                } else if (d[j] == 'R') {
                    d[j] = 'L';
                } else if (d[j] == 'L') {
                    d[j] = 'R';
                }
            } else { // 격자 안벗어남 => 이동
                r[j] = n_r;
                c[j] = n_c;
            }
        }
        // 충돌 처리
        map<pair<int, int>, int> s; // key: 해당 좌표(r,c) value: 무게 합
        map<pair<int, int>, int> num; // ket: 해당 좌표(r,c) value: 구슬 번호 <= 가장 큰 번호
        for (int j = 0; j < m; j++) {
            if (r[j] == -1 && c[j] == -1) { // 삭제된 구슬
                continue;
            }
            if (num.count({r[j], c[j]})) { // 충돌
                r[num[{r[j], c[j]}]] = -1; // 기존꺼 삭제 처리
                c[num[{r[j], c[j]}]] = -1;
                num[{r[j], c[j]}] = j;
                s[{r[j], c[j]}] += w[j];
                rst_cnt -= 1;
                rst_heavy = max(rst_heavy, s[{r[j], c[j]}]);
                w[j] = s[{r[j], c[j]}];
            } else { // 아직 충돌 x
                num[{r[j], c[j]}] = j;
                s[{r[j], c[j]}] += w[j];
            }
        }
    }

    cout << rst_cnt << " " << rst_heavy;
    return 0;
}
