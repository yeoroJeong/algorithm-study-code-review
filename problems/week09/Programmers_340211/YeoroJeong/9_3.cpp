#include <string>
#include <vector>
#include <set>
#include<algorithm>
using namespace std;
vector<vector<int>>ps;
vector<vector<int>>rs;
vector<vector<int>>cur_pos;
vector<vector<int>>dest_pos;
vector<int>dest_idx;
set<vector<int>>temp_s;
int ans;
int ed_cnt;
int m;
void move(int idx){
    if(cur_pos[idx][0]!=dest_pos[idx][0]){
        if(cur_pos[idx][0]<dest_pos[idx][0]){
            cur_pos[idx][0]+=1;
        }
        else{
            cur_pos[idx][0]-=1;
        }
    }
    else if(cur_pos[idx][1]!=dest_pos[idx][1]){
        if(cur_pos[idx][1]<dest_pos[idx][1]){
            cur_pos[idx][1]+=1;
        }
        else{
            cur_pos[idx][1]-=1;
        }
    }
}
int chk_crash(){
    temp_s.clear();
    int temp_ans=0;
    for(auto a: cur_pos){
        temp_s.insert(a);
    }
    for(auto a:temp_s){
        int cnt=0;
        for(auto b :cur_pos){
            if(a==b)
                cnt+=1;
        }
        if(cnt!=1)
            temp_ans+=1;
    }
    return temp_ans;
}
void solv(int time){
    if(time==0){
        ans+=chk_crash();   
    }
    if(cur_pos.empty())
        return;
    for(int i=0;i<cur_pos.size();i++){
        move(i);
    }
    ans+=chk_crash();
    for(int i=cur_pos.size()-1;i>=0;i--){
        if(cur_pos[i]==dest_pos[i]){
            if(dest_idx[i]==ed_cnt){
                cur_pos.erase(cur_pos.begin()+i);
                dest_pos.erase(dest_pos.begin()+i);
                dest_idx.erase(dest_idx.begin()+i);
                rs.erase(rs.begin()+i);
            }
            else{
                dest_idx[i]+=1;
                int next_idx=rs[i][dest_idx[i]];
                dest_pos[i]=ps[next_idx-1];
            }
            
        }
    }
    solv(time+1);
}

int solution(vector<vector<int>> points, vector<vector<int>> routes) {
    ans = 0;
    ed_cnt=routes[0].size()-1;
    ps=points;
    rs=routes;
    for(auto temp:routes){
        cur_pos.push_back(points[temp[0]-1]);
        dest_pos.push_back(points[temp[1]-1]);
    }
    for(int i=0;i<routes.size();i++){
        dest_idx.push_back(1);
    }
    solv(0);
    return ans;
}