#include <iostream>
#include<map>
#include <vector>
#include<climits>
#include<algorithm>
using namespace std;
map<char, pair<int, int>> direct;
int N, T, M;
struct ball {
    int cur_r;
    int cur_c;
    int weight;
    char cur_dir;
};
vector<ball> ball_list;
char reverse(char dir) {
    if (dir == 'U') {
        return 'D';
    }
    if (dir == 'D') {
        return 'U';
    }
    if (dir == 'L') return 'R';
    return 'L';
}
void move() {
    for (int i = 0; i < M; i++) {
        ball & cur = ball_list[i];
        if (cur.weight == -1) continue;
        char c_dir = cur.cur_dir;
        int c_r = cur.cur_r;
        int c_c = cur.cur_c;

        int nxt_r = c_r + direct[c_dir].first;
        int nxt_c = c_c + direct[c_dir].second;
        // 벽을 만나면 이번 턴은 제자리에서 방향만 뒤집는다.
        if (nxt_r <= 0 || nxt_r > N || nxt_c <= 0 || nxt_c > N) {
            nxt_r = c_r;
            nxt_c = c_c;
            c_dir = reverse(c_dir);
        }
        cur.cur_r = nxt_r;
        cur.cur_c = nxt_c;
        cur.cur_dir = c_dir;

    }
}
void solv(int cnt) {
    if (cnt >= T) return;
    move();
    // 큰 번호부터 확인해 같은 칸의 무게를 가장 큰 번호의 구슬에 모으고 그 방향을 유지한다.
    for (int i = M - 1; i >= 0; i--) {
        if (ball_list[i].weight == -1) continue;
        ball & cur = ball_list[i];
        for (int j = i - 1; j >= 0; j--) {
            if (ball_list[j].weight == -1) continue;
            ball & chk_ball = ball_list[j];
            if (cur.cur_c == chk_ball.cur_c && cur.cur_r == chk_ball.cur_r) {
                cur.weight += chk_ball.weight;
                // 흡수된 구슬은 무게를 -1로 표시해 다음 이동과 최종 집계에서 제외한다.
                chk_ball.weight = -1;
            }
        }
    }
    solv(cnt + 1);
}
int main() {
    // Please write your code here.
    cin >> N >> M >> T;
    direct['U'] = {-1, 0};
    direct['D'] = {1, 0};
    direct['L'] = {0, -1};
    direct['R'] = {0, 1};
    for (int i = 1; i <= M; i++) {
        int r, c, w;
        char d;
        cin >> r >> c >> d >> w;
        ball_list.push_back({r, c, w, d});
    }
    solv(0);
    int result_cnt = 0;
    int result_max = INT_MIN;

    for (int i = 0; i < M; i++) {
        if (ball_list[i].weight == -1) continue;
        result_cnt++;
        result_max = max(result_max, ball_list[i].weight);
    }
    cout << result_cnt << " " << result_max;
    return 0;
}
