/*
세로 n 가로 m
당신이 시추관을 수직으로 단 하나만 뚫을 수 있을 때 가장 많은 석유을 뽑을 수 있는 시추관의 위치
시추관은 열 한나를 관통하는 형태, 열과 열 사이에 시추관 뚫을 수 없음

빈땅 0, 석유 1

가장 많은 석유량 return
*/

#include <string>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

int solution(vector<vector<int>> land) {
    int answer = 0;
    
    int n = land.size();
    int m = land[0].size();
    
    int dx[4] = {0, 0, 1, -1};
    int dy[4] = {1, -1, 0, 0};
    
    vector<int> oil(m, 0);
    vector<vector<bool>> visited(n, vector<bool>(m, false));
    
    for (int col = 0; col < m; col++){ // 열 순회
        for (int row = 0; row < n; row++){ // 행 순회
            if(land[row][col] == 1 && visited[row][col] == false) {
                vector<bool> colcheck(m, false);
                queue<pair<int,int>> q;
                q.push({row, col});
                visited[row][col] = true;
                
                int cnt = 0;
                while(!q.empty()) {
                    int cury = q.front().first;
                    int curx = q.front().second;
                    colcheck[curx] = true;
                    q.pop();
                    cnt++;
                    
                    for(int i = 0; i < 4; i++){
                        int ny = cury + dy[i];
                        int nx = curx + dx[i];
                        if(ny >= n || ny < 0 || nx >= m || nx < 0) {
                            continue;
                        }
                        if(land[ny][nx] == 0 || visited[ny][nx] == true){
                            continue;
                        }
                        q.push({ny, nx});
                        visited[ny][nx] = true;
                    }
                }
                for(int i = 0; i < m; i++){
                    if(colcheck[i]){
                        oil[i] += cnt;
                    }
                }
            }
            
        }
    }
    
    answer = *max_element(oil.begin(), oil.end());
    
    return answer;
}