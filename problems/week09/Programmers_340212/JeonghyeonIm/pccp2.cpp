/*
n개의 퍼즐 제한 시간 내에 풀어야 하는 퍼즐 게임
각 퍼즐은 난이도와 소요 시간 정해져 있음
숙련도에 따라 퍼즐을 풀 때 틀리는 횟수가 바뀜

현재퍼즐
난이도 diff
소요 시간 time_cur
이전퍼즐
소요 시간 time_prev
당신의 숙련도 level

게임진행 방식
1. diff <= level이면 퍼즐 안틀리고 time_cur만큼 시간 사용하여 해결
2. diff > level이면 퍼즐을 총 diff - level 번 틀림
퍼즐을 틀릴 때마다 time_cur 만큼 시간을 사용, 추가로 time_prev만큼의 시간을 사용해 이전 퍼즐을 다시 풀고 와야함
이전 퍼즐 다시 풀 때는 이전 퍼즐의 난이도에 상관없이 틀리지 않음
diff - level 번 틀린 이후에 다시 퍼즐을 풀면 time_cur 만큼의 시간을 사용하여 퍼즐을 해결
=> 틀린횟수 *(time_cur + time_prev) + time_cur

제한 시간 limit
제한 시간 내에 퍼즐을 모두 해결하기 위한 숙련도의 최솟값을 정수로 return
만약에 시간이 limit보다 크다면 어려움 => 숙련도 오른쪽
만약에 시간이 limit보다 작으면 쉬움 => 숙련도 왼쪽
시간 == limit이면 적당 => return
*/

#include <string>
#include <vector>
#include <algorithm>

using namespace std;

long long cal(vector<int>& diffs, vector<int>& times, int level){ // 게임 시뮬레이션 후 소요시간 계산기
    long long sum = 0;
    int time_prev = 0;
    int time_cur = 0;
    for(int i = 0; i < diffs.size(); i++){
        time_cur = times[i];
        int diff_cur = diffs[i];
        
        if(diff_cur <= level){
            sum += time_cur;
        }else{
            sum += (long long)(diff_cur - level) * (time_cur + time_prev) + time_cur;
        }
        time_prev = time_cur;
    }
    return sum;
}

int solution(vector<int> diffs, vector<int> times, long long limit) {
    int answer = 0;
    
    int left = 1;
    int right = *max_element(diffs.begin(), diffs.end());
    
    while(left <= right){
        int mid = (left + right) / 2;
        
        if(cal(diffs, times, mid) > limit){ // 어려움: 숙련도 오른쪽
            left = mid + 1;
        } else{ // 쉬움: 숙련도 왼쪽
            answer = mid;
            right = mid - 1;
        }
    }
    
    return answer;
}

