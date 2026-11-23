#include <string>
#include <vector>
#include <queue>
#include <set>
using namespace std;
int dr[4]={0,1,0,-1};
int dc[4]={1,0,-1,0};
vector<int> group_cnt;
int solution(vector<vector<int>> land) {
    int answer = 0;
    int n=land.size();
    int m=land[0].size();
    group_cnt.resize(2,0);
    vector<vector<int>>visited;
    visited.resize(n,vector<int>(m,0));
    int label=1;
    for (int r=0;r<n;r++){
        for(int c=0;c<m;c++){
            if(visited[r][c]!=0 || land[r][c]==0) continue;
            queue<pair<int,int>> q;
            q.push({r,c});
            visited[r][c]=label;
            while(!q.empty()){
                int cur_r=q.front().first;
                int cur_c=q.front().second;
                
                q.pop();
                group_cnt[label]++;
                for(int i=0;i<4;i++){
                    int next_r=cur_r+dr[i];
                    int next_c=cur_c+dc[i];
                    if(next_r<0 || next_r>=n || next_c<0 || next_c>=m || land[next_r][next_c]==0 || visited[next_r][next_c]!=0) continue;
                    visited[next_r][next_c]=label;
                    q.push({next_r,next_c});
                }
            }
            label++;
            group_cnt.push_back(0);
        }
    }
    for(int i=0;i<m;i++){
        int temp_ans=0;
        set<int> temp_set;
        for(int j=0;j<n;j++){
            if(visited[j][i]==0) continue;
            temp_set.insert(visited[j][i]);
        }
        for(auto temp_label: temp_set){
            temp_ans+=group_cnt[temp_label];
        }
        answer=max(answer,temp_ans);
    }
    return answer;
}