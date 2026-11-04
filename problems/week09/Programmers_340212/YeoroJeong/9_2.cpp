#include <string>
#include <vector>

using namespace std;
int N;
long long max_time;
vector<int>temp_dif;
vector<int>temp_time;
bool solv(int level,int idx,int prev_time,long long cost){
    if(cost>max_time)
        return false;
    if(idx==N)
        return true;
    int cur_time=temp_time[idx];
    int cur_diff=temp_dif[idx];
    int temp_cost=
        (cur_diff<=level ? cur_time:(cur_diff-level)*(cur_time+prev_time)+cur_time);
    return solv(level,idx+1,cur_time,cost+temp_cost);
}
int solution(vector<int> diffs, vector<int> times, long long limit) {
    int answer = 0;
    N=diffs.size();
    max_time=limit;
    temp_dif=diffs;
    temp_time=times;
    answer=1;
    while(true){
        if(solv(answer,0,0,0))
            break;
        answer++;
    }
    return answer;
}