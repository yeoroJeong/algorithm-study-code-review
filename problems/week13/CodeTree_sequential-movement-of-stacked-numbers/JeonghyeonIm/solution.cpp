/*
쌓인 수들의 순차적 이동

1이상 N*N 이하 정수들이 정확히 한 번씩 등장, N * N 크기의 격자판 정보 주어짐
M번에 걸쳐 정수들을 이동하려고 함

이동할 정수들의 번호가 주어지고, 각 정수들은 특정 조건에 맞춰 움직이게 됨
조건: 8방향 인접칸중 가장 큰 값이 적혀있는 정수가 있는 곳으로 이동.
        인접한 칸에 여러 정수가 쌓여 있다면 이 정수 중 가장 큰 값

다 같이 이동

M번의 움직임 이후 상태를 출력하는 프로그램(위에 적힌 수부터 출력)
*/

#include <iostream>
#include <vector>
#include <unordered_map>
using namespace std;

int n, m;
int grid[20][20];
int move_nums[100];

int dr[8] = {-1, -1, -1, 0, 0, 1, 1, 1}; // 8방
int dc[8] = {-1, 0, 1, -1, 1, -1, 0, 1};

int main() {
    cin >> n >> m;
    vector<vector<int>> rst(n * n, vector<int>()); // 결과 출력
    unordered_map<int, int> ivrt; // key: 숫자, value: idx

    for (int i = 0; i < n * n; i++) {
        int input;
        cin >> input;
        rst[i].push_back(input);
        ivrt[input] = i;
    }

    for (int i = 0; i < m; i++) {
        cin >> move_nums[i];
    }

    // Please write your code here.
    for (int i = 0; i < m; i++) {
        int idx = ivrt[move_nums[i]]; // 움직일 idx
        int mxidx = 0, mx = 0;
        int r = idx / n, c = idx % n;
        for (int j = 0; j < 8; j++) { // 8방향 순회하여 제일 큰 숫자와 제일 큰 숫자가 들어있는 idx 찾음
            int nr = r + dr[j], nc = c + dc[j];
            if (nr < 0 || nr >= n || nc < 0 || nc >= n) { // 격자 밖
                continue;
            }
            int nidx = nr * n + nc;
            for (auto x : rst[nidx]) {
                if (mx < x) {
                    mx = x;
                    mxidx = nidx;
                }
            }
        }
        if (mxidx == 0 && mx == 0) {
            continue;
        }
        vector<int> temp;
        while (rst[idx].back() != move_nums[i]) { // 위에부터 꺼내서
            int t = rst[idx].back();
            temp.push_back(t);
            rst[idx].pop_back();
        }
        rst[idx].pop_back();
        temp.push_back(move_nums[i]);
        while (!temp.empty()) { // 다시 반대로 넣기
            rst[mxidx].push_back(temp.back());
            ivrt[temp.back()] = mxidx;
            temp.pop_back();
        }
    }

    for (int i = 0; i < n * n; i++) {
        if (rst[i].empty()) {
            cout << "None";
        } else {
            for (int j = rst[i].size() - 1; j >= 0; j--) {
                cout << rst[i][j] << " ";
            }
        }
        cout << "\n";
    }
    return 0;
}
