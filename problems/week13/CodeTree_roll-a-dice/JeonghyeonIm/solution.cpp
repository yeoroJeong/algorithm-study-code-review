/*
주사위 던지기
6개의 면 이루어져 있는 주사위
1~6숫자, 마주보는 면 끼리 적혀있는 숫자의 합 7

각 주사위가 격자판에 닿을 때마다 주사위의 아랫면 숫자 적힘(overwrite)
격자 밖으로 벗어나는 경우라면 => 다음 과정 수행
최종적으로 격자판에 적혀있는 숫자의 총합 계산 프로그램

지금 현재 상태와
상하좌우 상태 저장
*/

#include <iostream>
#include <vector>
using namespace std;

int n, m;
int r, c;
char directions[10000];

int main() {
    cin >> n >> m >> r >> c;
    r--;
    c--;
    vector<vector<int>> grid(n, vector<int>(n, 0));
    for (int i = 0; i < m; i++) {
        cin >> directions[i];
    }

    // Please write your code here.
    int u = 1, d = 6, l = 4, ri = 3, f = 2, b = 5; // 위 아래 왼 오 앞 뒤
    grid[r][c] = d;
    for (int i = 0; i < m; i++) {
        if (directions[i] == 'L' && c - 1 >= 0) {
            int t = d;
            d = l;
            l = u;
            u = ri;
            ri = t;
            c--;
        } else if (directions[i] == 'R' && c + 1 < n) {
            int t = d;
            d = ri;
            ri = u;
            u = l;
            l = t;
            c++;
        } else if (directions[i] == 'U' && r - 1 >= 0) {
            int t = d;
            d = b;
            b = u;
            u = f;
            f = t;
            r--;
        } else if (directions[i] == 'D' && r + 1 < n) {
            int t = d;
            d = f;
            f = u;
            u = b;
            b = t;
            r++;
        }
        grid[r][c] = d;
    }

    int sum = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            sum += grid[i][j];
        }
    }
    cout << sum;
    return 0;
}
