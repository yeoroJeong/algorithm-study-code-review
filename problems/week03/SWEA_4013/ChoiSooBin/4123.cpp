/*
문제: 4123. [모의 SW 역량테스트] 숫자 만들기
링크: https://swexpertacademy.com/main/code/problem/problemDetail.do?contestProbId=AWJRxtsKDKIDFAXc
작성자: 최수빈(sbyy77dev)
알고리즘: DFS, 백트래킹

[문제 요약]
N개의 숫자가 적혀 있는 게임 판이 주어지고, 그 사이사이에 +, -, x, / 연산자 카드를 끼워 넣어 수식을 만드는 문제다.
연산자 우선순위는 무시하고 무조건 왼쪽부터 차례대로 계산하며, 주어진 연산자 카드를 모두 사용해 만들 수 있는 결과 중 최댓값과 최솟값을 찾아 그 차이를 구해야 한다.

[핵심 아이디어]
입력으로 주어지는 숫자의 순서는 변경할 수 없으므로, 숫자들 사이에 연산자를 배치하는 모든 경우의 수를 재귀(DFS)로 완탐(완전 탐색)해야 한다.
백트래킹을 할 때 현재 상태의 '계산 결과(누적값)'를 변수에 덮어쓰지 않고, 재귀 함수의 인자로 직접 넘겨주어 상태 보존을 확실하게 하는 것이 핵심이다.

[풀이 과정]
1. 테스트 케이스마다 숫자의 개수 N과 4가지 연산자의 개수, 그리고 N개의 숫자를 입력받는다.
2. 탐색 전, 최댓값(mx)과 최솟값(mn)은 확실한 범위인 INT_MIN, INT_MAX로 매번 초기화해 준다.
3. 첫 번째 숫자를 초기 누적값으로 쥐고 DFS 탐색(level=1)을 시작한다.
4. 4가지 연산자를 순회하며 남은 연산자 카드가 있다면(op[i] > 0), 카드를 1장 차감하고 다음 레벨의 재귀를 호출한다.
5. 이때 누적 변수를 오염시키지 않기 위해, DFS 매개변수 자리에 직접 연산 결과(예: ans + mth[level])를 적어 다음 깊이로 던져준다.
6. 재귀에서 빠져나오면 차감했던 연산자 카드를 다시 원상 복구(+)하는 백트래킹을 수행한다.
7. 사용한 연산자 개수가 총 N-1개가 되어 수식이 완성되면(level == n), 현재 완성된 결과값으로 최댓값과 최솟값을 갱신하고 종료(return)한다.
8. 최종적으로 갱신된 mx와 mn의 차이를 출력한다.

[복잡도]
- 시간 복잡도: 숫자 N은 최대 12개, 연산자 총합은 11개다. 백트래킹으로 O(4^(N-1)) 형태의 분기를 타지만, N이 작고 남은 연산자 개수만큼만 가지를 뻗으므로 제한 시간 3초 안에 충분히 통과한다.
- 공간 복잡도: 재귀 깊이가 최대 N이고, 배열 크기도 크지 않아(숫자 13, 연산자 4) 메모리 사용량은 O(N)으로 매우 넉넉하다.

[막혔던 부분과 오답 원인 및 해결 방법]
- 첫 번째 삽질 (전역 변수 초기화 누락 및 0 초기화 함정):
  테스트 케이스를 반복할 때 정답을 담을 mx, mn 변수를 제대로 초기화하지 않았다. 나중에 0으로 초기화하게끔 수정했지만, 만약 연산 결과가 모두 음수거나 모두 양수라면 0에서 값이 갱신되지 않는 치명적 논리 오류가 발생했다.
  -> 반드시 나올 수 있는 범위 밖의 값(INT_MAX, INT_MIN)으로 확실히 초기화하여 해결했다.

- 두 번째 삽질 (종료 조건 위치 실수):
  초기 코드에서 기저 조건(Base Case)의 return 문을 연산자 반복문(for) 안쪽에 넣는 바람에, 정답 갱신 후 남은 다른 연산자 분기를 탐색해보지도 못하고 함수가 싹 종료되어 버렸다.
  [수정 전] for문 안에 종료 조건이 있어서 다른 연산자 탐색 불가
  [수정 후] if(level == n) 블록을 반복문 바깥 최상단으로 빼서 정상적으로 모든 가지를 탐색하도록 해결.

- 세 번째 삽질 (변수 상태 오염):
  누적 변수 ans 자체를 덮어쓰고 재귀를 넘겨서 상태가 오염되었다. 더하기(+) 분기를 타고 돌아온 뒤 빼기(-) 분기를 탈 때, 이전 결과가 남아버리는 문제가 발생했다.
  [수정 전] 변수를 덮어쓰거나 불필요한 매개변수 남용
  if (i == 0) {
	  res = ans + mth[level];
  }
  op[i] -= 1;
  dfs(level + 1, ans, res);
  op[i] += 1;

  [수정 후] 함수 호출 괄호 안에서 직접 값을 계산해 넘겨줌!
  if (i == 0) {
	  op[i] -= 1;
	  dfs(level + 1, ans + mth[level]);
	  op[i] += 1;
  }
  -> 백트래킹할 때 현재 상태의 원본 값은 절대 건드리지 않고 인자로 바로 넘겨주는 방식으로 깔끔하게 해결했다.
*/

#include<iostream>
#include<climits>
#include<cmath>
#include<algorithm>
using namespace std;
int op[4];
int mth[13];
int n;
int mn, mx;
int cnt = 0;

void dfs(int level, int ans) {	// dfs 들어갈 때마다 level + 1
	if (level == n) {	// 종료조건
		if (mx < ans) mx = ans;
		if (mn > ans) mn = ans;
		cnt++;
		return;
	}
	for (int i = 0; i < 4; i++) {
		if (op[i]) {
			op[i] -= 1;
			if (i == 0) {
				dfs(level + 1, ans + mth[level]);
			}
			else if (i == 1) {
				dfs(level + 1, ans - mth[level]);
			}
			else if (i == 2) {
				dfs(level + 1, ans * mth[level]);
			}
			else {	// i == 3
				dfs(level + 1, ans / mth[level]);
			}
			op[i] += 1;
		}
	}
	
	
}

int main(int argc, char* argv)
{
	ios::sync_with_stdio(false); cin.tie(0); cout.tie(0);
	int test_case;
	int T;
	int res = 0;
	freopen("4123_input.txt", "r", stdin);
	cin >> T;

	for (test_case = 1; test_case <= T; ++test_case)
	{
		fill(op, op + 4, 0);
		fill(mth, mth + 13, 0);
		mn = INT_MAX; mx = INT_MIN;
		cin >> n;
		for (int i = 0; i < 4; i++) {
			cin >> op[i];
		}
		for (int i = 0; i < n; i++) {
			cin >> mth[i];
		}
		dfs(1, mth[0]);
		res = mx - mn;
		cout << '#' << test_case << ' ' << res << '\n';
	}
	return 0;
}

