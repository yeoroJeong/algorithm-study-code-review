#include <iostream>
#include <vector>
using namespace std;

int n, mTurn;
int grid[20][20];

vector<pair<int, int>> posList;

int dx[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
int dy[8] = {-1, 0, 1, -1, 1, -1, 0, 1};

pair<int, int> maxPos;
int findMaxVal(int x, int y) {
    int maxVal = 0;
    for (int i = 0; i < 8; i++) {
        int nx = x + dx[i];
        int ny = y + dy[i];
        if (nx < 0 || ny < 0 || nx >= n || ny >= n) {
            continue;
        }

        if (grid[nx][ny] > maxVal) {
            maxVal = grid[nx][ny];
            maxPos = {nx, ny};
        }
    }

    return maxVal;
}

int main() {
    cin >> n >> mTurn;
    posList.resize(n * n + 1);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> grid[i][j];
            posList[grid[i][j]] = {i, j};
        }
    }

    // Please write your code here.
    for (int m = 0; m < mTurn; m++) {

        for (int index = 1; index <= n * n; index++) {
            int x = posList[index].first;
            int y = posList[index].second;
            int maxVal = findMaxVal(x, y);

            // 위치 업데이트
            posList[index] = maxPos;
            posList[maxVal] = {x, y};
            grid[maxPos.first][maxPos.second] = grid[x][y];
            grid[x][y] = maxVal;
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
