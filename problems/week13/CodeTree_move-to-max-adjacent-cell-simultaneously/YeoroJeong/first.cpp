#include <iostream>
#include<set>
#include<climits>
using namespace std;

int board[101][101];
int dr[4] = {-1, 1, 0, 0};
int dc[4] = {0, 0, -1, 1};
set<pair<int, int>> balls;
int main() {
    // Please write your code here.
    int N, M, T;
    cin >> N >> M >> T;
    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= N; j++) {
            cin >> board[i][j];
        }
    }
    for (int i = 1; i <= M; i++) {
        int st_r, st_c;
        cin >> st_r >> st_c;
        balls.insert({st_r, st_c});
    }
    for (int i = 0; i < T; i++) {
        set<pair<int, int>> temp;
        set<pair<int,int>> to_erase;
        for (auto pos : balls) {
            int cur_max = INT_MIN;
            int next_r, next_c;
            // 상·하·좌·우 순서로 비교해 값이 같으면 먼저 본 방향을 택한다.
            for (int j = 0; j < 4; j++) {
                int temp_r = pos.first + dr[j];
                int temp_c = pos.second + dc[j];
                if (temp_r <= 0 || temp_r > N || temp_c <= 0 || temp_c > N) continue;
                if (board[temp_r][temp_c] > cur_max) {
                    next_r = temp_r;
                    next_c = temp_c;
                    cur_max = board[temp_r][temp_c];
                }
            }
            // 이미 도착할 구슬이 있는 칸은 충돌 목록에 넣는다.
            if (temp.find({next_r, next_c}) != temp.end()) {
                to_erase.insert({next_r,next_c});
            }
            else {

                temp.insert({next_r, next_c});
            }

        }
        // 모든 도착지를 정한 뒤 지워야 세 개 이상이 모인 칸도 함께 비워진다.
        for(auto pos: to_erase){
            temp.erase(pos);
        }
        balls = temp;
    }
    cout << balls.size();

    return 0;
}
