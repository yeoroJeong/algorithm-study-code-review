/*
문제: 1949. [모의 SW 역량테스트] 등산로 조성
링크: https://swexpertacademy.com/main/code/problem/problemDetail.do?contestProbId=AV5PoOKKAPIDFAUq
작성자: 최수빈(sbyy77dev)
알고리즘: DFS, 백트래킹

[문제 요약]
N * N 맵에서 제일 높은 봉우리부터 시작해 상하좌우로 뻗어가는 가장 긴 등산로를 찾는 문제다.
진행하다 막히면 딱 한 번, 최대 K만큼 땅을 깎아서 길을 낼 수 있다.

[핵심 아이디어]
단순 DFS 탐색에 '한 번 깎을 수 있는 기회(flag)'를 포함한 백트래킹을 적용한다.
가장 긴 경로를 찾아야 하므로 모든 가능한 루트를 다 파봐야 함.

[풀이 과정]
1. 맵 입력을 받으면서 제일 높은 봉우리의 높이와 그 좌표들을 리스트에 싹 다 저장해둔다.
2. 저장해둔 각 봉우리 좌표에서 각각 DFS 탐색을 시작한다.
3. 상하좌우를 살피며 현재보다 낮은 곳이면 그냥 방문 처리 후 재귀를 탄다.
4. 현재보다 높거나 같은 곳인데, 아직 공사 찬스가 남아있고(flag == false) K 이하로 깎아서 갈 수 있는 곳이라면?
   -> 내리막 조건만 만족하도록 딱 '현재 높이 - 1'까지만 깎고 DFS를 돈 다음, 빠져나올 때 다시 원래 높이로 복구해준다(백트래킹).

[복잡도]
- 시간 복잡도: 최대 N=8, 봉우리 최대 5개. 최악의 경우라도 N이 워낙 작아서 4방향 완전탐색(DFS)으로 3초 안에 넉넉하게 통과함.
- 공간 복잡도: 9x9 짜리 2차원 배열 map, visited 정도만 쓰니까 O(N^2)으로 아주 널널하다.
- 입력 제한에서 가능한 이유: N이 8 이하로 매우 작기 때문에 O(4^(N*N))에 가까운 백트래킹을 돌아도 가지치기가 많이 되어 시간 초과가 나지 않는다.

[막혔던 부분과 오답 원인]
- 첫 번째 삽질: 
  DFS 안에서 4방향을 반복문으로 도는데, 깎아도 못 가는 지형일 때 무심코 `return;`을 때려버렸다. 
  이렇게 하니까 남은 다른 방향들은 확인도 안 해보고 그 길로 탐색이 싹 다 종료되어 버리는 대참사가 일어남.
  -> AI 힌트를 받고 남은 방향을 마저 탐색해야 한다는 걸 깨달아서 `return;`을 지우는 것으로 해결했다.
  
- 두 번째 삽질 (변수 스코프 문제):
  로직을 다 고쳤는데도 계속 오답이 떠서 애를 먹었다. 알고 보니 `ny`, `nx`를 전역 변수로 선언해버린 게 치명적이었다. 
  재귀를 깊숙이 탔다가 빠져나오면 전역 변수인 `ny`, `nx` 값은 마지막 탐색 위치로 덮어씌워져 버린다.
  결국 `visited[ny][nx] = false;`로 방문을 해제할 때, 내가 지금 서있는 곳이 아니라 엉뚱한 곳의 방문을 풀고 있었던 거다.
  -> 재귀 함수 힌트를 듣고 바로 유레카 침. 반복문 안에서 `int ny = y + dy[i];` 처럼 지역 변수로 선언하게끔 수정하니까 바로 정답 떴다. 백트래킹할 때 좌표 변수는 무조건 지역 변수로 살려두자!
*/

#include<iostream>
#include<vector>
#include<algorithm>
#include<cmath>
typedef long long ll;
using namespace std;
 
 
int n = 0, k = 0;
int map[9][9];
bool visited[9][9];
int mxlen = 0;
// 상 좌 하 우
int dy[4] = {-1, 0, 1, 0};
int dx[4] = {0, -1, 0, 1};
bool flag = false;
 
void dfs(int y, int x, int len) {
    for (int i = 0; i < 4; i++) {
        int ny, nx;
        ny = y + dy[i];
        nx = x + dx[i];
        if (ny >= 0 && ny < n && nx >= 0 && nx < n) {
            if (map[y][x] > map[ny][nx] && (!visited[ny][nx])) { // 등산로 가능
                visited[ny][nx] = true;
                mxlen = max(mxlen, len + 1);
                dfs(ny, nx, len + 1);
                visited[ny][nx] = false;
            }
            else if(map[y][x] <= map[ny][nx] && (!visited[ny][nx])) {    // 등산로 불가능, 공사해서 되는지 확인
                if (map[y][x] > map[ny][nx] - k && (!flag)) {
                    // 최소한으로 공사
                    int tmp = map[ny][nx];
                    map[ny][nx] = map[y][x] - 1;
                    flag = true;
                    visited[ny][nx] = true;
                    mxlen = max(mxlen, len + 1);
                    dfs(ny, nx, len + 1);
                    visited[ny][nx] = false;
                    map[ny][nx] = tmp;  // 공사 되돌리기
                    flag = false;
                }
            }
        }
    }
    return;
}
 
 
 
int main(int argc, char** argv)
{
    ios::sync_with_stdio(false); cin.tie(0); cout.tie(0);
    int test_case;
    int T;
     
    cin >> T;
     
    for (test_case = 1; test_case <= T; ++test_case)
    {
        // 입력 및 초기화
        int maxval = -1;
        mxlen = 0;
        vector<pair<int, int>> mxloc;
        cin >> n >> k;
        fill(map[0], map[0] + 81, 0);
        fill(visited[0], visited[0] + 81, false);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                cin >> map[i][j];
                if (maxval == map[i][j]) {
                    mxloc.push_back(make_pair(i, j));
                }
                else if (maxval < map[i][j]) {
                    maxval = map[i][j];
                    mxloc.clear();
                    mxloc.push_back(make_pair(i, j));
                }
            }
        }
        for (int i = 0; i < mxloc.size(); i++) {
            visited[mxloc.at(i).first][mxloc.at(i).second] = true;
            dfs(mxloc.at(i).first, mxloc.at(i).second, 1);
            visited[mxloc.at(i).first][mxloc.at(i).second] = false;
        }
         
        cout << "#" << test_case << " " << mxlen << '\n';
    }
    return 0;
}