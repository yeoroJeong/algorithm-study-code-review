#include <iostream>
#include<map>
#include<set>
#include<vector>
struct ball {
    int r;
    int c;
    char dir;
    bool is;
};

using namespace std;
int N, M;
char reverse(char cur_dir) {
    if (cur_dir == 'U') return 'D';
    if (cur_dir == 'D') return 'U';
    if (cur_dir == 'R') return 'L';
    return 'R';
}
map<char, pair<int, int>> direct;
vector<ball> balls;
int cnt[51][51];
void move(int idx) {
    ball & cur = balls[idx];
    if (!cur.is) return;
    int temp_r = cur.r + direct[cur.dir].first;
    int temp_c = cur.c + direct[cur.dir].second;
    if (temp_r <= 0 || temp_r > N || temp_c <= 0 || temp_c > N) {
        // 벽을 만나면 한 턴을 제자리에서 보내고 다음 턴에 반대로 움직인다.
        cur.dir = reverse(cur.dir);
        return;
    }
    cur.r = temp_r;
    cur.c = temp_c;
}
// 이동이 모두 끝난 뒤 각 칸의 구슬 수를 세어 충돌을 판정한다.
void remove() {
    for (int i = 0; i < M; i++) {
        ball & cur = balls[i];
        if (!cur.is) continue;
        cnt[cur.r][cur.c]++;
    }
    for (int i = 0; i < M; i++) {
        ball & cur = balls[i];
        if (!cur.is) continue;
        if (cnt[cur.r][cur.c] != 1) {
            cur.is = false;
        }
        // 충돌 칸은 첫 구슬을 지운 뒤에도 개수가 0이어서 뒤의 구슬들도 제거된다.
        cnt[cur.r][cur.c] = 0;
    }
}
void one_cycle() {
    int cnt = 0;
    // 벽에서 멈추는 턴까지 포함하면 2N턴 뒤에는 생존 구슬의 위치와 방향이 반복된다.
    while (cnt < 2 * N) {
        for (int i = 0; i < M; i++) {
            if (!balls[i].is) continue;
            move(i);
        }
        remove();
        cnt++;
    }
}
int main() {
    // Please write your code here.
    int T;
    cin >> T;
    direct['U'] = {-1, 0};
    direct['D'] = {1, 0};
    direct['L'] = {0, -1};
    direct['R'] = {0, 1};
    for (int test_case = 1; test_case <= T; test_case++) {
        balls.clear();
        cin >> N >> M;
        for (int i = 1; i <= M; i++) {
            int x, y;
            char d;
            cin >> x >> y >> d;
            balls.push_back({x, y, d, true});
        }
        one_cycle();

        int result = 0;
        for (int i = 0; i < M; i++) {
            if (balls[i].is) result++;
        }
        cout << result << "\n";
    }
    return 0;
}
