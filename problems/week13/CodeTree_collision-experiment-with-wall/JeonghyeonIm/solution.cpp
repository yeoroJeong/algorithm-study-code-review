/*
벽이 있는 충돌 실험

M개의 구슬 N*N격자 안에 놓여있음
각 구슬은 모두 1초에 한 칸씩 동일한 속도로 정해진 방향으로 움직임
구슬이 벽에 부딪히면 움직이지 않고 방향만 반대
두 개 이상의 구슬이 충돌하게 되면 부딪힌 구슬 모두 사라짐
충돌: 두 구슬이 이동 후 같은 위치에 있는 경우에만

아주 오랜시간이 흐른 후에도 여전히 격자 안에 남아있는 구슬의 개수 출력
*/

#include <iostream>
#include <vector>
using namespace std;

int T;
int N, M;
int x[2500];
int y[2500];
char d[2500];
int cnt[50][50];

int main() {
    cin >> T;
    char idx[256];
    idx['U'] = 0;
    idx['D'] = 1;
    idx['R'] = 2;
    idx['L'] = 3;
    int dr[4] = {-1, 1, 0, 0}; // U D R L
    int dc[4] = {0, 0, 1, -1};

    for (int t = 0; t < T; t++) {
        cin >> N >> M;
        for (int i = 0; i < M; i++) {
            cin >> x[i] >> y[i] >> d[i];
            x[i]--;
            y[i]--;
        }

        // Please write your code here.
        int rst = M;
        for (int i = 0; i < 2 * N; i++) {
            for (int j = 0; j < M; j++) { // 구슬 이동
                int c_r = x[j], c_c = y[j];
                if (c_r == -1 && c_c == -1) { // 삭제 된 구슬 처리 안함
                    continue;
                }
                int n_r = c_r + dr[idx[d[j]]];
                int n_c = c_c + dc[idx[d[j]]];
                if (n_r < 0 || n_r >= N || n_c < 0 || n_c >= N) { // 방향만 바꿈
                    if (d[j] == 'U') {
                        d[j] = 'D';
                    } else if (d[j] == 'D') {
                        d[j] = 'U';
                    } else if (d[j] == 'R') {
                        d[j] = 'L';
                    } else if (d[j] == 'L') {
                        d[j] = 'R';
                    }
                } else { // 이동
                    x[j] = n_r;
                    y[j] = n_c;
                }
            }
            // 충돌 처리
            vector<int> del;
            for (int j = 0; j < M; j++) {
                if (x[j] == -1) {
                    continue;
                }
                cnt[x[j]][y[j]]++; // 칸별 구슬 개수 세기
            }
            for (int j = 0; j < M; j++) {
                if (x[j] == -1) {
                    continue;
                }
                if (cnt[x[j]][y[j]] >= 2) { // 삭제 대상
                    del.push_back(j);
                }
            }
            for (int j = 0; j < M; j++) {
                if (x[j] == -1) {
                    continue;
                }
                cnt[x[j]][y[j]] = 0; // 카운트 초기화
            }
            for (int k : del) { // 삭제
                x[k] = -1;
                y[k] = -1;
                rst--;
            }
        }
        cout << rst << "\n";
    }

    return 0;
}