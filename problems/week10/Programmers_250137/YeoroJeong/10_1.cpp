#include <string>
#include <vector>
#include <climits>
#include <algorithm>
#include <queue>
using namespace std;

int solution(vector<int> bandage, int health, vector<vector<int>> attacks) {
    int answer = health;
    int t=0;
    int count_time=0;
    queue<vector<int>> q;
    for (auto temp: attacks){
        q.push(temp);
    }
    while(!q.empty() && answer>0){
        int top_time=q.front()[0];
        int top_power=q.front()[1];
        if (t!=top_time){
            t++;
            count_time++;
            int plus_hp=bandage[1];
            if(count_time==bandage[0]){
                count_time=0;
                plus_hp+=bandage[2];
            }
            answer+=plus_hp;
            if(answer>health)
                answer=health;
            continue;
        }
        q.pop();
        answer-=top_power;
        count_time=0;
        t++;
    }
    if (answer<=0) answer=-1;
    return answer;
}