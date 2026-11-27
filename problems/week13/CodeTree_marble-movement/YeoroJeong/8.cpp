#include <iostream>
#include<unordered_map>
#include<vector>
#include<set>
using namespace std;
unordered_map<char, pair<int, int>> direct;
struct ball {
    int r;
    int c;
    int vel;
    char dir;
    bool is;
    int idx;
};
// 속도가 느린 구슬부터, 속도가 같으면 번호가 작은 구슬부터 정렬한다.
struct CompBall {
    bool operator()(const ball& b1, const ball& b2) const {
        if (b1.vel != b2.vel) return b1.vel < b2.vel;
        return b1.idx < b2.idx;
    }
};

set<ball, CompBall> to_comp[51][51];
vector<ball> balls;
int N, M, K;
char reverse(char cur_d) {
    if (cur_d == 'U') return 'D';
    if (cur_d == 'D') return 'U';
    if (cur_d == 'R') return 'L';
    return 'R';
}

void move() {
    for (int i = 0; i < M; i++) {
        ball & cur = balls[i];
        if (!cur.is) continue;
        int cur_r = cur.r;
        int cur_c = cur.c;
        int cnt = cur.vel;
        for (int i = 0; i < cnt; i++) {
            int temp_r = cur_r + direct[cur.dir].first;
            int temp_c = cur_c + direct[cur.dir].second;
            if (temp_r <= 0 || temp_r > N || temp_c <= 0 || temp_c > N) {
                // 다음 칸이 격자 밖이면 같은 걸음에서 방향을 뒤집어 반대쪽으로 한 칸 이동한다.
                cur.dir = reverse(cur.dir);
                temp_r = cur_r + direct[cur.dir].first;
                temp_c = cur_c + direct[cur.dir].second;
            }
            cur_r = temp_r;
            cur_c = temp_c;
        }
        cur.r = cur_r;
        cur.c = cur_c;
    }
}
void remove() {
    for (int i = 0; i < M; i++) {
        ball & cur = balls[i];
        if (!cur.is) continue;
        to_comp[cur.r][cur.c].insert(cur);
    }
    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= N; j++) {
            if (to_comp[i][j].size() <= K) {
                to_comp[i][j].clear();
                continue;
            }
            // 정렬된 앞쪽부터 초과분을 제거해 이 칸에 우선순위가 높은 K개를 남긴다.
            int erase_cnt = to_comp[i][j].size() - K;
            int cnt = 0;
            for (auto cur : to_comp[i][j]) {
                if (cnt == erase_cnt) break;
                if (!cur.is) continue;
                balls[cur.idx].is = false;
                cnt++;
            }
            to_comp[i][j].clear();

        }
    }
}
int main() {
    // Please write your code here.
    int T;
    cin >> N >> M >> T >> K;
    direct['U'] = {-1, 0};
    direct['D'] = {1, 0};
    direct['L'] = {0, -1};
    direct['R'] = {0, 1};
    for (int i = 0; i < M; i++) {
        int st_r, st_c, v;
        char d;
        cin >> st_r >> st_c >> d >> v;
        balls.push_back({st_r, st_c, v, d, true, i});
    }
    // 모든 구슬이 각자 속도만큼 이동한 뒤, 최종 도착 칸을 기준으로 충돌을 처리한다.
    for (int i = 0; i < T; i++) {
        move();
        remove();
    }
    int result = 0;
    for (int i = 0; i < M; i++) {
        if (balls[i].is) result++;
    }
    cout << result;
    return 0;
}
