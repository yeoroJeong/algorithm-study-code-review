#include <string>
#include <vector>
#include <climits>
using namespace std;
int dx[4]={0,1,0,-1};
int dy[4]={1,0,-1,0};
int answer;
int n,m;
vector<vector<int>>visited_r;
vector<vector<int>>visited_b;
vector<pair<int,int>>cart_pos(5);
vector<vector<int>>board;
bool chk(int x,int y,int label){
    if(x<0||x>=n||y<0||y>=m) return false;
    if(board[x][y]==5) return false;
    if(label==0){
        if(visited_r[x][y]!=0) return false;
    }
    else{
        if(visited_b[x][y]!=0) return false;
    }
    return true;
}
void solv(int rx,int ry,int bx,int by,int cnt){
    if(cnt>answer) return;
    bool r_move=true;
    bool b_move=true;
    if(rx==cart_pos[3].first&& ry==cart_pos[3].second) r_move=false;
    if(bx==cart_pos[4].first && by==cart_pos[4].second) b_move=false;
    if(!r_move && !b_move){
        answer=min(answer,cnt);
        return;
    }
    if(r_move&&b_move){
        for(int i=0;i<4;i++){
            for(int j=0;j<4;j++){
                int next_rx=rx+dx[i];
                int next_ry=ry+dy[i];
                int next_bx=bx+dx[j];
                int next_by=by+dy[j];
                if(!chk(next_rx,next_ry,0)||!chk(next_bx,next_by,1)) continue;
                if(next_rx==next_bx && next_ry==next_by) continue;
                if(next_rx==bx&&next_ry==by&&next_bx==rx&&next_by==ry) continue;
                visited_r[next_rx][next_ry]=1;
                visited_b[next_bx][next_by]=1;
                solv(next_rx,next_ry,next_bx,next_by,cnt+1);
                visited_r[next_rx][next_ry]=0;
                visited_b[next_bx][next_by]=0;
            }
        }
    }
    else if(r_move){
        for(int i=0;i<4;i++){
            int next_rx=rx+dx[i];
            int next_ry=ry+dy[i];
            int next_bx=bx;
            int next_by=by;
            if(!chk(next_rx,next_ry,0)) continue;
            if(next_rx==next_bx && next_ry==next_by) continue;
            if(next_rx==bx&&next_ry==by&&next_bx==rx&&next_by==ry) continue;
            visited_r[next_rx][next_ry]=1;
            solv(next_rx,next_ry,next_bx,next_by,cnt+1);
            visited_r[next_rx][next_ry]=0;
        }
    }
    else if(b_move){
        for(int i=0;i<4;i++){
            int next_rx=rx;
            int next_ry=ry;
            int next_bx=bx+dx[i];
            int next_by=by+dy[i];
            if(!chk(next_bx,next_by,1)) continue;
            if(next_rx==next_bx && next_ry==next_by) continue;
            if(next_rx==bx&&next_ry==by&&next_bx==rx&&next_by==ry) continue;
            
            visited_b[next_bx][next_by]=1;
            solv(next_rx,next_ry,next_bx,next_by,cnt+1);
            
            visited_b[next_bx][next_by]=0;
        }
    }
    return;
}
int solution(vector<vector<int>> maze) {
    answer = INT_MAX;
    board=maze;
    n=maze.size();
    m=maze[0].size();
    visited_r.resize(n,vector<int>(m,0));
    visited_b.resize(n,vector<int>(m,0));
    for(int i=0;i<n;i++){
        for(int j=0;j<m;j++){
            if(maze[i][j]!=0 && maze[i][j]<5){
                cart_pos[maze[i][j]]={i,j};
            }
        }
    }
    visited_r[cart_pos[1].first][cart_pos[1].second]=1;
    visited_b[cart_pos[2].first][cart_pos[2].second]=1;
    solv(cart_pos[1].first,cart_pos[1].second,cart_pos[2].first,cart_pos[2].second,0);
    answer=(answer==INT_MAX)?0:answer;
    return answer;
}