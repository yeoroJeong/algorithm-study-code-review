/*
M개의 구슬 N * N 격자 안에 놓여 있음
각 구슬은 일정 속도 갖고 정해진 방향으로 움직임

방향 바꾸는데 시간 소요X

움직이고 난 이후: 동일 위치 구슬 K개 이하라면 문제 없음
K개 초과일경우, 우선순위 높은 구슬 K개만 살아남고 나머지 구슬들은 전부 사라지게 됨.
우선순위 1. 구슬의 속도높 2. 구슬의 번호높

T초가 지난 이후에 격자 안에 남아 있는 구슬의 개수 출력

좌표 => 번호 리스트
*/

#include <iostream>
#include <cmath>
#include <vector>
#include <unordered_map>
#include <map>
using namespace std;

int n, m, t, k;
int r[2500], c[2500];
char d[2500];
int v[2500];
int cnt[51][51];

int main() {
    cin >> n >> m >> t >> k;
    map<int, int> conv1; // key : 우선순위 값(속도, 번호 순), value : 번호
    for (int i = 0; i < m; i++) {
        cin >> r[i] >> c[i] >> d[i] >> v[i];
        conv1[v[i] * 2500 + i] = i; // 속도가 같아도 덮어쓰지 않게
    }

    // Please write your code here.
    char idx[256];
    idx['U'] = 0;
    idx['D'] = 1;
    idx['L'] = 2;
    idx['R'] = 3;
    int dr[4] = {-1, 1, 0, 0};
    int dc[4] = {0, 0, -1, 1};
    int rst = m; // 구슬 개수(출력)
    while (t--) {
        for (int i = 0; i < m; i++) { // 구슬 움직이기
            if (r[i] == -1 && c[i] == -1) { // 삭제된 구슬은 움직이지 않음
                continue;
            }
            int c_r = r[i], c_c = c[i];
            int sp = (n == 1) ? 0 : v[i] % (2 * (n - 1)); // 한 주기는 제자리로 돌아오므로 줄임
            int n_r = c_r + (dr[idx[d[i]]] * sp);
            int n_c = c_c + (dc[idx[d[i]]] * sp);

            while (n_r < 1 || n_r > n) { // 벽 반사 (여러 번 튕겨도 처리)
                if (n_r < 1) n_r = 2 - n_r;
                else n_r = 2 * n - n_r;
                d[i] = (d[i] == 'U') ? 'D' : 'U';
            }
            while (n_c < 1 || n_c > n) {
                if (n_c < 1) n_c = 2 - n_c;
                else n_c = 2 * n - n_c;
                d[i] = (d[i] == 'L') ? 'R' : 'L';
            }
            r[i] = n_r;
            c[i] = n_c;
        }
        // 충돌 처리
        map<pair<int, int>, vector<int>> conv2; // key : 좌표, value : 구슬 번호들
        for (int i = 0; i < m; i++) { // 카운트 하고
            if (r[i] == -1 && c[i] == -1) {
                continue;
            }
            cnt[r[i]][c[i]]++;
            conv2[{r[i], c[i]}].push_back(i);
        }
        for (int i = 0; i < m; i++) { // 만약 k 보다 크면 처리
            if (r[i] == -1 && c[i] == -1) {
                continue;
            }
            if (cnt[r[i]][c[i]] > k) {
                int del_cnt = cnt[r[i]][c[i]] - k; // 몇개 지워야 되냐
                cnt[r[i]][c[i]] = 0;
                vector<int> lst = conv2[{r[i], c[i]}];
                for (auto& [key, value] : conv1) {
                    for (auto x : lst) {
                        if (x == value) {
                            r[x] = -1;
                            c[x] = -1;
                            rst--;
                            del_cnt--;
                            break;
                        }
                    }
                    if (del_cnt == 0) { // 다 지웠으면 끝내
                        break;
                    }
                }
            }
        }
        for (int i = 0; i < m; i++) { // 초기화
            if (r[i] == -1 && c[i] == -1) {
                continue;
            }
            cnt[r[i]][c[i]] = 0;
        }
    }

    cout << rst;
    return 0;
}

