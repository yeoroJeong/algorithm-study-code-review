#include <iostream>
#include <deque>
#include<map>
using namespace std;
int N, M, K;
int board[102][102];
map<char, pair<int, int>> direct;
// 머리는 별도 좌표로 관리하고, 덱에는 꼬리부터 머리 바로 뒤까지 몸통을 담는다.
deque<pair<int, int>> snake;
int main() {
    // Please write your code here.
    cin >> N >> M >> K;

    direct['U'] = {-1, 0};
    direct['D'] = {1, 0};
    direct['L'] = {0, -1};
    direct['R'] = {0, 1};

    for (int i = 0; i < M; i++) {
        int apple_r, apple_c;
        cin >> apple_r >> apple_c;
        board[apple_r][apple_c] = 1;
    }
    int cur_r = 1;
    int cur_c = 1;
    int T = 0;
    for (int i = 0; i < K; i++) {
        bool end = false;
        char d;
        int p;
        cin >> d >> p;
        for (int j = 0; j < p; j++) {
            int next_r = cur_r + direct[d].first;
            int next_c = cur_c + direct[d].second;
            // 벽에 부딪히는 마지막 한 칸도 경과 시간에 포함한다.
            if (next_r <= 0 || next_r > N || next_c <= 0 || next_c > N) {
                end = true;
                T++;
                break;
            }

            if (end) break;

            // 사과를 먹으면 꼬리를 남기고 이전 머리를 몸통에 추가해 한 칸 자란다.
            if (board[next_r][next_c] == 1) {
                board[next_r][next_c] = 0;
                snake.push_back({cur_r, cur_c});
            }
            else if (!snake.empty()) { 
                snake.pop_front(); 
                
                snake.push_back({cur_r, cur_c});
            }
            T++;
            cur_r = next_r;
            cur_c = next_c;

            // 몸통을 갱신한 뒤 검사하므로 이번 이동에서 꼬리가 비운 칸은 지나갈 수 있다.
            for (auto pos : snake) {
                if (pos.first == cur_r && pos.second == cur_c) {
                    end = true;
                    break;
                }
            }
            if (end) break;

        }
        if (end) break;
    }
    cout << T;
    return 0;
}
