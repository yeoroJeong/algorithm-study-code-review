#include <string>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;
/*
    가로 : m
    세로 : n

    수직으로 단 1개의 시추관 -> 가장 많은 석유를 뽑을 수 있는 위치

    크기 배열 
    board[n][m]
    
    시추관을 꽂을 때, 붙어있는 석유 + 지나가는 석유 부분은 전부 시추 가능
    -> bfs
    + 방향 고려
    
    bfs

    1인 곳에서부터 bfs를 들어가야함.
*/
int solution(vector<vector<int>> land) {
    int answer = 0;

    int n = (int)land.size();      // 행
    int m = (int)land[0].size();   // 열

    vector<vector<bool>> visited(n, vector<bool>(m, false));
    vector<int> petroleums(m, 0);  // 각 열에서 얻을 수 있는 석유량

    int dx[4] = {1, -1, 0, 0};
    int dy[4] = {0, 0, 1, -1};

    queue<pair<int, int>> q;

    for(int col = 0; col < m; ++col)
    {
        for(int row = 0; row < n; ++row)
        {
            // 아직 방문하지 않은 석유 덩어리에서만 BFS 시작
            if(land[row][col] == 1 && !visited[row][col])
            {
                int oilSize = 0;                 // 현재 석유 덩어리 크기
                vector<bool> usedCol(m, false);  // 현재 덩어리가 걸친 열 체크

                q.push({row, col});
                visited[row][col] = true;

                while(!q.empty())
                {
                    auto [x, y] = q.front();
                    q.pop();

                    oilSize++;
                    usedCol[y] = true;

                    for(int k = 0; k < 4; ++k)
                    {
                        int nextX = x + dx[k];
                        int nextY = y + dy[k];

                        // 범위 검사를 먼저 해야 함
                        if(nextX >= 0 && nextX < n &&
                           nextY >= 0 && nextY < m &&
                           land[nextX][nextY] == 1 &&
                           !visited[nextX][nextY])
                        {
                            visited[nextX][nextY] = true;
                            q.push({nextX, nextY});
                        }
                    }
                }

                // 현재 석유 덩어리가 걸친 모든 열에 덩어리 크기 추가
                for(int c = 0; c < m; ++c)
                {
                    if(usedCol[c])
                    {
                        petroleums[c] += oilSize;
                    }
                }
            }
        }
    }

    answer = *max_element(petroleums.begin(), petroleums.end());

    return answer;
}