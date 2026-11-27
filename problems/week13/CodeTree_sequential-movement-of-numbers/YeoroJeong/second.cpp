#include <iostream>
#include<climits>
using namespace std;
int board[21][21];
int N;
int dr[8] = {0, 1, 0, -1, 1, -1, 1, -1};
int dc[8] = {1, 0, -1, 0, 1, -1, -1, 1};
void swap(int r1, int c1, int r2, int c2) {
    int temp = board[r1][c1];
    board[r1][c1] = board[r2][c2];
    board[r2][c2] = temp;
}
void turn(int cnt) {
    if (cnt == N * N + 1) return;
    // 앞선 교환으로 위치가 달라질 수 있으므로 이번 차례의 수를 격자에서 다시 찾는다.
    int cur_r, cur_c;
    for (int i = 1; i <= N; i++) {
        bool find = false;
        for (int j = 1; j <= N; j++) {
            if (board[i][j] == cnt) {
                cur_r = i;
                cur_c = j;
                find = true;
                break;
            }
        }
        if (find) break;
    }
    // 대각선을 포함한 여덟 이웃 중 가장 큰 수가 교환 상대다.
    int cur_max = INT_MIN;
    int next_r, next_c;
    for (int i = 0; i < 8; i++) {
        int temp_r = cur_r + dr[i];
        int temp_c = cur_c + dc[i];
        if (temp_r <= 0 || temp_r > N || temp_c <= 0 || temp_c > N) continue;
        if (board[temp_r][temp_c] > cur_max) {
            next_r = temp_r;
            next_c = temp_c;
            cur_max = board[temp_r][temp_c];
        }
    }
    swap(cur_r, cur_c, next_r, next_c);
    // 교환 결과를 반영한 격자에서 다음 수의 차례를 시작한다.
    turn(cnt + 1);
}
int main() {
    // Please write your code here.
    int M;
    cin >> N >> M;
    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= N; j++) {
            cin >> board[i][j];
        }
    }
    for (int i = 0; i < M; i++) {
        turn(1);
    }
    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= N; j++) {
            cout << board[i][j] << " ";
        }
        cout << "\n";
    }

    return 0;
}
