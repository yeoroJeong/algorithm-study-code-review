/*
수의 순차적 이동

1이상 N * N이하의 수들이 정확히 한 번씩만 등장하는, N * N 크기의 격자판
M번의 턴에 걸쳐 수들을 이동
한 번의 턴에는 수 1이 적힌 위치에서부터 수 N*N이 적힌 위치까지 순서대로 하나씩 보면서 특정 조건에 맞춰 다들 한 번씩 움직임
조건: 각 위치에서 8방향으로 인접한 칸들 중 카장 큰 수와 가운데 칸의 수를 교환

M번의 턴을 거친 이후 격자판의 상태 출력

map으로 각 숫자의 위치 기억하자
*/

#include <iostream>
#include <unordered_map>
#include <algorithm>
using namespace std;

int n, m;
int grid[20][20];

int dr[8] = {-1, -1, -1, 0, 0, 1, 1, 1}; // 8방향 탐색
int dc[8] = {-1, 0, 1, -1, 1, -1, 0, 1};

int main() {
    cin >> n >> m;
    unordered_map<int, pair<int, int>> num; // key: 숫자번호, valude: 숫자번호가 위치한 좌표(r, c)
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> grid[i][j];
            num[grid[i][j]] = {i, j};
        }
    }

    // Please write your code here.
    for (int i = 0; i < m; i++) { // 턴 수 만큼
        for (int j = 1; j <= n * n; j++) {
            pair<int, int> rc = num[j];
            int c_r = rc.first;
            int c_c = rc.second;
            int mxidx = 0, mx = 0; // 어느방향으로 갔을 때 최대냐
            for (int k = 0; k < 8; k++) { // 최대 값 찾음
                int n_r = c_r + dr[k];
                int n_c = c_c + dc[k];
                if (n_r < 0 || n_r >= n || n_c < 0 || n_c >= n) {
                    continue;
                }
                if (mx < grid[n_r][n_c]) {
                    mx = grid[n_r][n_c];
                    mxidx = k;
                }
            }
            int m_r = c_r + dr[mxidx]; // swap
            int m_c = c_c + dc[mxidx];
            swap(grid[c_r][c_c], grid[m_r][m_c]);
            swap(num[j], num[mx]);
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << grid[i][j] << " ";
        }
        cout << "\n";
    }

    return 0;
}
