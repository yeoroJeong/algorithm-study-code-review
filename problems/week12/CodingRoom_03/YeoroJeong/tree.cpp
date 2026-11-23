#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <map>
using namespace std;
vector<int>result;
vector<int>cost;
bool first = true;
bool Comp(const vector<int>& a, const vector<int>& b) {
    for (int i = 0; i < a.size(); i++) {
        if (a[i] != b[i]) return a[i] < b[i];
    }
    return true;
}
int n;
// valid_idx는 남은 원소의 초기 배열 인덱스 목록이고, sep는 그중 stack1에 있는 원소의 개수다.
void solv(int target,int sep,vector<int>valid_idx,vector<int>temp ) {
    // 초기 배열을 모두 채우면 현재까지 찾은 배열 중 사전순 최솟값을 갱신한다.
    if (target>n) {
        if (first) {
            result = temp;
            first = false;
            return;
        }
        if (Comp(temp, result)) {
            result = temp;
        }
        return;
    }
    // 두 스택을 stack1의 바닥부터 stack2의 바닥까지 한 줄로 보면 원소 이동은 경계만 바꾼다.
    int move_cnt = cost[target - 1];
    // 경계에서 이동 비용만큼 떨어진 왼쪽과 오른쪽 칸이 추출 후보가 된다.
    int left = sep - 1 - move_cnt;
    int right = sep + move_cnt;
    if (left >= 0) {
        vector<int>next_temp;
        int next_idx = valid_idx[left];
        vector<int>next_valid;
        next_valid = valid_idx;
        next_valid.erase(find(next_valid.begin(), next_valid.end(), next_idx));

        next_temp = temp;
        next_temp[next_idx] = target;

        // 추출한 칸 왼쪽에 남은 원소의 개수가 새 경계가 된다.
        solv(target + 1, left,next_valid, next_temp);
    }
    if (right < valid_idx.size()) {
        vector<int>next_temp;
        int next_idx = valid_idx[right];
        vector<int>next_valid;
        next_valid = valid_idx;
        next_valid.erase(find(next_valid.begin(), next_valid.end(), next_idx));

        next_temp = temp;
        next_temp[next_idx] = target;

        solv(target + 1, right,next_valid, next_temp);
    }

    
}
vector<int> solution(vector<int> costs) {
    // 여기에 풀이를 작성하세요.
    first = true;
    cost = costs;
    n = costs.size();
    result.resize(n);
    // 처음에는 stack2가 비어 있으므로 꼭대기에서 costs[0]칸 아래에 1이 있다.
    int first_idx = n - costs[0];
    result[first_idx-1] = 1;
    vector<int>valid;
    for (int i = 0; i < n; i++) {
        valid.push_back(i);
    }
    valid.erase(find(valid.begin(), valid.end(), first_idx - 1));
    
    solv(2,first_idx-1,valid,result);
    
    return result;
}
