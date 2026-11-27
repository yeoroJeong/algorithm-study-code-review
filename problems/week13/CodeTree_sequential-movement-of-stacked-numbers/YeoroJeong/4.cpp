#include <iostream>
#include<deque>
#include <map>
#include<climits>
#include<algorithm>
using namespace std;

struct part {
    int max_val;
    deque<int> stck;
};
map<int, pair<int, int>> pos;
part board[21][21];
int N, M;
int dr[8] = {1, 0, -1, 0, 1, -1, 1, -1};
int dc[8] = {0, 1, 0, -1, 1, -1, -1, 1};

void move(int move_idx) {
    int cur_r = pos[move_idx].first;
    int cur_c = pos[move_idx].second;

    // 각 칸에 저장해 둔 최댓값을 비교해 여덟 이웃 중 목적지를 고른다.
    int max_side = INT_MIN;
    int nxt_r;
    int nxt_c;
    for (int i = 0; i < 8; i++) {
        int temp_r = cur_r + dr[i];
        int temp_c = cur_c + dc[i];

        if (temp_r <= 0 || temp_r > N || temp_c <= 0 || temp_c > N) continue;
        if (board[temp_r][temp_c].max_val > max_side) {
            max_side = board[temp_r][temp_c].max_val;
            nxt_r = temp_r;
            nxt_c = temp_c;
        }
    }
    if (max_side == -1) return;

    deque<int>& temp = board[cur_r][cur_c].stck;
    deque<int>& next = board[nxt_r][nxt_c].stck;
    deque<int> to_apd;
    // 덱 앞이 맨 위이므로 지정한 수를 꺼낼 때까지 그 위의 수들도 함께 옮겨 담는다.
    while (!temp.empty()) {
        int temp_num = temp.front();
        to_apd.push_back(temp_num);
        temp.pop_front();
        if (temp_num == move_idx) break;
    }
    // 기존 최댓값이 함께 떠났을 수 있어 남은 수로 출발 칸의 최댓값을 다시 구한다.
    board[cur_r][cur_c].max_val = -1;
    for (auto num : temp) {
        board[cur_r][cur_c].max_val = max(board[cur_r][cur_c].max_val, num);
    }
    // 묶음의 아래쪽부터 목적지 맨 위에 올려 쌓인 순서를 유지한다.
    while (!to_apd.empty()) {
        int temp_num = to_apd.back();
        board[nxt_r][nxt_c].max_val = max(board[nxt_r][nxt_c].max_val, temp_num);
        next.push_front(temp_num);
        pos[temp_num] = {nxt_r, nxt_c};
        to_apd.pop_back();
    }

}
int main() {
    // Please write your code here.
    cin >> N >> M;
    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= N; j++) {
            int num;
            cin >> num;
            pos[num] = {i, j};
            board[i][j].max_val = num;
            board[i][j].stck.push_back(num);
        }
    }
    for (int i = 0; i < M; i++) {
        int to_move;
        cin >> to_move;
        move(to_move);
    }
    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= N; j++) {
            if (board[i][j].stck.empty()) {
                cout << "None\n";
                continue;
            }
            for (auto num : board[i][j].stck) {
                cout << num << " ";
            }
            cout << "\n";

        }
    }
    return 0;
}
