/*
이동 비용으로 스택 복원하기
정수 1~n까지가 각각 한 번씩 들어있는 스택 'stack1'
스택 'stack2'는 처음에 비어 있음
두 스택을 이용하여 정수 1~n까지 순서대로 추출
스택은 배열로 표현, 배열의 마지막 원소가 스택의 맨 위 원소

원소 추출 방법
목표 정수 target일때
1. target이 들어 있는 스택을 찾음
2. 해당 스택의 맨 위 원소가 target이 될 때까지, 맨 위 원소를 하나씩 다른 스택으로 옮김
3. target이 맨 위에 나타나면 추출하며 두 스택에서 완전히 제거
=> 이때 다른 스택으로 원소 하나를 옮길 때마다 비용 1 발생, 목표 정수 추출비용은 0

정수 1~n까지 각각 추출하는 과정에서 발생한 이동 비용 배열 'costs'가 주어짐
이 비용 배열을 만드는 stack1의 초기 상태 구하기
출력: stack1의 초기 상태

가능한 초기 상태가 여러 개라면 사전순으로 가장 앞서는 배열 반환

=> 역순으로 차곡차곡 dfs
*/

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <map>
using namespace std;

vector<int> best; // 지금까지 찾은 사전순 최소 초기 상태
bool found = false; // 하나라도 찾았는지
vector<int> C; // costs 복사본

// k: 지금 되돌릴 숫자 (n부터 1까지 내려옴)
// st[0], st[1]: 현재 두 스택
void dfs(int k, vector<vector<int>> st) {
    if (k == 0) { // 전부 되돌림
        if (st[1].empty()) { // 원래 stack2는 비어 있어야 함
            if (!found || st[0] < best) { // 사전순 비교
                best = st[0];
                found = true;
            }
        }
        return;
    }

    int c = C[k - 1]; // k를 꺼낼 때 옮긴 원소 개수(비용)

    for (int x = 0; x < 2; x++) { // x: k가 있던 스택
        int y = 1 - x; // y: 원소들이 옮겨졌던 스택
        if (st[y].size() < c) continue; // y에 c개 미만이면 불가능

        vector<vector<int>> ns = st; // 복사해서 새 상태 생성

        vector<int> p; // y의 맨 위에서 c개 꺼내기
        for (int j = 0; j < c; j++) {
            p.push_back(ns[y].back());
            ns[y].pop_back();
        }

        ns[x].push_back(k); // x에 k를 먼저 push
        for (int v : p) { // 꺼낸 순서 그대로 push
            ns[x].push_back(v);
        }

        dfs(k - 1, ns);
    }
}

vector<int> solution(vector<int> costs) {
    C = costs;
    best.clear();
    found = false;

    int n = costs.size();
    vector<vector<int>> st(2); // 빈 스택 2개
    dfs(n, st);

    return best;
}
