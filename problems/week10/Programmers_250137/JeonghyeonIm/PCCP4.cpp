/*
n * m 크기 격자 모양의 퍼즐판 주어짐
빨간색 수레, 파란색 수레 하나씩 존재.
각 수레들은 자신의 시작 칸에서 자신의 도착 칸까지 이동해야 함
모든 수레들을 각자의 도착 칸으로 이동시 퍼즐 풀 수 있음

각 턴마다 반드시 모든 수레를 상하좌우로 인접한 칸 중 한 칸으로 움직여야 함
규칙
1. 수레는 벽이나 격자 판 밖으로 이동 불가능
2. 수레는 자신이 방문했던 칸 움직일 수 없음
3. 자신의 도착 칸에 위치한 수레는 움직 x(해당 칸 고정)
4. 동시에 두 수레를 같은 칸으로 움직일 수는 없음
5. 수레끼리 자리를 바꾸며 움직일 수 없음

퍼즐을 푸는데 필요한 턴의 최솟값 return
퍼즐을 풀 수 없는 경우 0 return

입력 maze[i][j]
0: 빈칸
1: 빨간 수레 시작 칸
2: 파란 수레 시작 칸
3: 빨간 수레 도착 칸
4: 파란 수레 도착 칸
5: 벽
*/
#include <string>
#include <vector>

using namespace std;

int n, m, ans = 1e9;
int dx[4] = {-1, 1, 0, 0};
int dy[4] = {0, 0, -1, 1};
vector<vector<int>> g; // maze 복사본
bool visR[4][4], visB[4][4]; // <= visited, maze길이 최대 4
int rgx, rgy, bgx, bgy; // 빨강/파랑 도착 칸

void dfs(int rx, int ry, int bx, int by, int turn) {
    if (turn >= ans) { // 이미 찾은 최소 턴 이상이면 중단
        return; 
    }

    // 둘 다 도착하면 정답 갱신 (위의 가지치기로 turn < ans 보장)
    if (rx == rgx && ry == rgy && bx == bgx && by == bgy) {
        ans = turn;
        return;
    }

    bool rDone = (rx == rgx && ry == rgy); // red가 이미 도착했는지 확인
    bool bDone = (bx == bgx && by == bgy); // blue가 이미 도착했는지 확인

    // i(red): 0~3 = 상하좌우 이동, 4 = 제자리(도착한 수레만) <= 규칙 3
    for (int i = (rDone ? 4 : 0); i < (rDone ? 5 : 4); i++) {
        int nrx = rx + (i < 4 ? dx[i] : 0); // 4면 이미 도착한것으로 처리 즉 이동 안함
        int nry = ry + (i < 4 ? dy[i] : 0);
        if (nrx < 0 || nrx >= n || nry < 0 || nry >= m) continue; // 격자 밖
        if (g[nrx][nry] == 5) continue; // 벽
        if (i < 4 && visR[nrx][nry]) continue; // 규칙 2
        
        // j(blue): 0~3 = 상하좌우 이동, 4 = 제자리(도착한 수레만) <= 규칙 3
        for (int j = (bDone ? 4 : 0); j < (bDone ? 5 : 4); j++) {
            int nbx = bx + (j < 4 ? dx[j] : 0); // 4면 이미 도착한것으로 처리 즉 이동 안함
            int nby = by + (j < 4 ? dy[j] : 0);
            if (nbx < 0 || nbx >= n || nby < 0 || nby >= m) continue; // 격자 밖
            if (g[nbx][nby] == 5) continue; // 벽
            if (j < 4 && visB[nbx][nby]) continue; // 규칙 2

            // 규칙 4: 같은 칸으로 이동 금지
            if (nrx == nbx && nry == nby) continue;
            // 규칙 5: 자리 교환 금지
            if (nrx == bx && nry == by && nbx == rx && nby == ry) continue;
            
            
            // 제자리 고정이 아닐경우 visited 처리
            if (i < 4) {
                visR[nrx][nry] = true;
            }
            if (j < 4) {
                visB[nbx][nby] = true;
            }

            dfs(nrx, nry, nbx, nby, turn + 1);

            if (i < 4) visR[nrx][nry] = false; // 원복 <= backtraking
            if (j < 4) visB[nbx][nby] = false;
        }
    }
}

int solution(vector<vector<int>> maze) {
    int answer = 0;

    g = maze; // 전역 변수 g에 미로 복사 (dfs 안에서 벽 판별용)
    n = maze.size(); // 행 개수(row)
    m = maze[0].size(); // 열 개수(column)
    int rsx, rsy, bsx, bsy; // 빨강/파랑 시작 좌표 (x=행, y=열)

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (maze[i][j] == 1) { // 1 = 빨강 시작 칸, 좌표 저장
                rsx = i;
                rsy = j;
            }
            else if (maze[i][j] == 2) { // 2 = 파랑 시작 칸, 좌표 저장
                bsx = i;
                bsy = j;
            }
            else if (maze[i][j] == 3) { // 3 = 빨강 도착 칸, 좌표 저장
                rgx = i;
                rgy = j;
            }
            else if (maze[i][j] == 4) { // 4 = 파랑 도착 칸, 좌표 저장
                bgx = i;
                bgy = j;
            }
        }
    }

    visR[rsx][rsy] = true; // 빨강 시작 칸 방문 처리 (규칙 2: 다시 돌아오지 못하게)
    visB[bsx][bsy] = true; // 파랑 시작 칸 방문 처리

    dfs(rsx, rsy, bsx, bsy, 0); // 두 수레의 시작 위치, 0턴에서 탐색 시작

    answer = (ans == 1e9) ? 0 : ans; // 한 번도 도착 못 했으면 0, 아니면 최소 턴 수
    return answer;
}