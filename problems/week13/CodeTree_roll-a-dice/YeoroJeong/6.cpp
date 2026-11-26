#include <iostream>
#include<deque>
#include<map>
using namespace std;
int N, M;
int board[101][101];
deque<int> up_down;
deque<int> left_right;
map<char, pair<int, int>> direct;

int main() {
    // Please write your code here.
    int r, c;
    cin >> N >> M >> r >> c;
    int cur_num = 6;
    board[r][c] = cur_num;
    direct['U'] = {-1, 0};
    direct['D'] = {1, 0};
    direct['R'] = {0, 1};
    direct['L'] = {0, -1};

    // 각 덱의 앞뒤에 상·하 또는 좌·우로 굴렸을 때 바닥에 닿을 옆면을 저장한다.
    up_down.push_back(5);
    up_down.push_back(2);

    left_right.push_back(4);
    left_right.push_back(3);

    for (int i = 0; i < M; i++) {
        char dir;
        cin >> dir;
        int temp_r = r + direct[dir].first;
        int temp_c = c + direct[dir].second;
        if (temp_r <= 0 || temp_r > N || temp_c <= 0 || temp_c > N) continue;

        r = temp_r;
        c = temp_c;

        // 마주 보는 두 면의 합은 7이다. 굴리는 축의 옆면은 기존 윗면과 바닥면으로 바뀐다.
        if (dir == 'U' || dir == 'D') {
            deque<int> temp_ud;
            int to_front = (dir == 'U') ? (7 - cur_num) : cur_num;
            int to_back = (dir == 'U') ? cur_num : (7 - cur_num);
            cur_num = (dir == 'U') ? up_down.front() : up_down.back();

            temp_ud.push_back(to_front);
            temp_ud.push_back(to_back);
            up_down = temp_ud;

            board[r][c] = cur_num;
        }
        else {
            deque<int> temp_lr;
            int to_front = (dir == 'L') ? (7 - cur_num) : cur_num;
            int to_back = (dir == 'L') ? (cur_num) : (7 - cur_num);
            cur_num = (dir == 'L') ? left_right.front() : left_right.back();
            temp_lr.push_back(to_front);
            temp_lr.push_back(to_back);

            left_right = temp_lr;
            board[r][c] = cur_num;
        }
    }
    // 다시 방문한 칸에는 마지막 바닥 눈만 남으므로 최종 격자의 값을 합산한다.
    int result = 0;
    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= N; j++) {
            result += board[i][j];
        }
    }
    cout << result;
    return 0;
}
