/*
문제: 1244. [S/W 문제해결 응용] 2일차 - 최대 상금
링크: https://swexpertacademy.com/main/code/problem/problemDetail.do?contestProbId=AV15Khn6AN0CFAYD#none
작성자: 최수빈
알고리즘: 백트래킹, DFS
핵심 아이디어 : 문자열의 인덱스를 활용한 이중 for문으로 두 숫자를 선택해 자리를 바꾼다. 이때 `visited[교환 횟수][만들어진 숫자]`를 확인하여, 동일한 교환 횟수에 같은 숫자가 만들어진 적이 있다면 더 이상 탐색하지 않고 가지치기하는 것이 시간 초과를 막는 핵심.
시간 복잡도: O(상수) - 최대 6자리 숫자를 다루며, 방문 처리(가지치기) 덕분에 중복 경로가 차단되어 최악의 경우에도 연산량이 극적으로 줄어듦.
공간 복잡도: O(K * M) - K는 최대 교환 횟수 + 1(11), M은 최대 숫자 커버하는 1,000,000
다시 볼 포인트 & 최적화(AI 첨언) :
1. [배열 초기화 문법]: `visited[11][1000000] = { false, };`는 C++에서 배열 전체를 초기화해주지 않고, 배열 범위를 벗어난 잘못된 접근(Out of bounds) 에러를 발생시킬 수 있음. `#include <cstring>`을 추가하고 `memset(visited, false, sizeof(visited));`를 사용하는 것이 안전하고 올바른 초기화 방법.
2. [속도 최적화 (참조자 활용)]: `backtrack` 함수의 매개변수 `string s`를 `string& s`로 참조형(&)으로 넘기자. 현재 코드는 재귀마다 문자열을 새로 복사(Call by value)하고 있는데, 이미 탐색 후 원상복구하는 `swap` 로직을 잘 짜두었기 때문에 `&`만 붙여주면 메모리와 시간을 훨씬 아끼는 완벽한 백트래킹 코드가 됨.
*/
#include<iostream>
#include<string>
#include<algorithm>
#include<cstring>

using namespace std;
int mx = 0;
bool visited[11][1000000];

void backtrack(int depth, const int target, string s) {
	if (depth == target) {
		int currVal = stoi(s);
		if (currVal > mx) {
			mx = currVal;
		}
		return;
	}

	int len = s.length();
	for (int i = 0; i < len - 1; i++) {
		for (int j = i + 1; j < len; j++) {
			swap(s[i], s[j]);
			int check = stoi(s);
			if (!visited[depth + 1][check]) {
				visited[depth + 1][check] = true;
				backtrack(depth + 1, target, s);
			}
			// 탐색 후 복구
			swap(s[i], s[j]);
		}
	}
	return;
}

int main(int argc, char** argv)
{
	ios::sync_with_stdio(false); cin.tie(0); cout.tie(0);
	int test_case;
	int T;
	
	freopen("1244_input.txt", "r", stdin);
	cin >> T;
	
	for (test_case = 1; test_case <= T; ++test_case)
	{
		string s;
		int target;
		cin >> s >> target;
		mx = 0;

		memset(visited, false, sizeof(visited));
		// 숫자 최대: 6자리, 999999 2차원: 교환횟수 메모이제이션
		backtrack(0, target, s);
		cout << '#' << test_case << ' ' << mx << '\n';

	}
	return 0;
}