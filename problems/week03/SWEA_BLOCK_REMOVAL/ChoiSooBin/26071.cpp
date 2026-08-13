/*
문제: 블록 깨기 (최대 점수 구하기)
작성자: 최수빈
알고리즘: DFS, 백트래킹 (완전 탐색)

[문제 요약]
일렬로 놓인 N개의 숫자 블록을 순서대로 하나씩 깨서 얻을 수 있는 최대 점수를 구하는 문제.
블록이 깨질 때마다 남은 양옆 블록이 빈자리 없이 달라붙는다. 
점수 계산: 양쪽 이웃이 살아있으면 (왼쪽 * 오른쪽), 한쪽만 있으면 (그쪽 숫자), 이웃이 없으면 (내 숫자).

[핵심 아이디어]
- "블록이 깨져 빈자리가 생기고 양옆이 붙는다"는 물리적 이동을 배열 복사로 구현하지 않는다!
- 대신 `broken` boolean 배열을 사용하여 깨진 블록을 논리적으로 무시하는 백트래킹(In-place)을 활용한다.
- 깰 블록을 기준으로 좌/우 방향으로 탐색(for문)하며 `broken == false`인 가장 가까운 블록을 '진짜 이웃'으로 판별하는 것이 문제의 핵심!

[풀이 과정]
1. 테스트 케이스마다 블록 배열(`block`)과 방문 처리 배열(`broken`)을 초기화한다. 
2. main 함수에서 `dfs(0, 0, 0)`(현재 노드 무의미, 깬 개수 0개, 점수 0점)을 단 한 번 호출하여 완탐을 시작한다.
3. DFS 내부 루프(1~N)에서 아직 깨지지 않은 블록을 선택하여 깬다(`broken[i] = true`).
4. 깨진 블록(i)을 기준으로 좌측(`j = i-1 ~ 1`)과 우측(`j = i+1 ~ n`)으로 각각 for문을 돌려 살아있는 진짜 이웃을 찾는다.
5. 이웃의 존재 여부에 따라 문제의 조건대로 점수를 계산한 뒤, 다음 재귀(`dfs(i, level + 1, score + tmp)`)로 넘긴다.
6. 재귀에서 돌아오면 백트래킹(`broken[i] = false`)하여 다른 순서로 깨는 경우를 마저 탐색한다.
7. `level == n` 도달 시, 누적된 점수(score)로 최댓값(mx)을 갱신하고 리턴한다.

[막혔던 부분과 오답 원인 및 해결 방법]
- 첫 번째 삽질 (리스트 슬라이싱의 늪과 시간 초과):
  [원인] 파이썬으로 접근했을 때 `blocks[:i] + blocks[i+1:]`처럼 빈자리를 채우기 위해 매번 새로운 배열을 복사해서 재귀로 넘김. N=10일 때 경우의 수(10!, 약 360만 번)만큼 배열 생성과 가비지 컬렉션이 발생하여 TLE(시간 초과) 발생.
  [해결] 원본 배열을 변형하지 않고 `broken` 배열 상태만 토글(true/false)하며 '건너뛰기(continue)' 처리하는 정통 백트래킹 방식으로 변경하여 시간 복잡도와 메모리를 줄임.

- 두 번째 삽질 (main 함수의 불필요한 패딩과 예외 처리):
  [원인] 양 끝을 1로 막아두고(block[0]=1, block[n+1]=1) main 함수에서 첫 번째 블록을 깨는 for문을 따로 돌리려 함. 이 때문에 N=1일 때의 하드코딩 예외처리가 필요해지고 코드가 지저분해짐.
  [해결] DFS 내부에 이미 전체 탐색 로직이 완벽하게 구현되어 있으므로, main 함수의 불필요한 for문과 패딩을 싹 다 지우고 0점 상태에서 `dfs(0,0,0)` 단 한 번만 진입하도록 깔끔하게 리팩토링함.

[복잡도]
- 시간 복잡도: O(N! * N). (최대 연산 약 3,600만 번. 1초 내 통과)
  ※ 단, N이 15 이상으로 커질 경우 비트마스킹 + DP (O(2^N * N^2)) 최적화가 필수적임.
- 공간 복잡도: O(N). 방문 배열과 깊이 10의 재귀 스택만 사용하므로 1MB 제한에 매우 안전함.
*/
#include<algorithm>
#include<iostream>
#include<climits>

using namespace std;

int n;
int mx;
bool broken[11];
int block[12];
void dfs(int node, int level, int score) {
	if (level == n) {
		if (mx < score) mx = score;
		return;
	}
	for (int i = 1; i <= n; i++) {
		int tmp = 1;
		if (broken[i]) continue;
		broken[i] = true;
		// 점수 계산 case 처리
		int right = 0, left = 0;
		for (int j = i + 1; j <= n; j++) {
			// 오른쪽에 남아있는게 있나 확인
			if (broken[j]) continue;
			right = block[j];
			break;
		}
		for (int j = i-1; j > 0; j--) {
			// 왼쪽에 남아있는게 있나 확인
			if (broken[j]) continue;
			left = block[j];
			break;
		}
		if (right && left) {
			tmp = right * left;
		}
		else if (right && !left) {
			tmp = right;
		}
		else if (!right && left) {
			tmp = left;
		}
		else {
			tmp = block[i];
		}
		
		dfs(i, level + 1, score + tmp);
		broken[i] = false;
	}

}


int main(int argc, char** argv)
{
	int test_case;
	int T;

	cin >> T;

	for (test_case = 1; test_case <= T; ++test_case)
	{
		mx = INT_MIN;
		cin >> n;

		fill(block, block + 12, false); // 0~11
		for (int i = 1; i <= n; i++) {
			cin >> block[i];
		}

		fill(broken, broken + 11, false);
		block[0] = 1; broken[0] = true;
		block[n + 1] = 1; broken[n + 1] = true;
		dfs(0,0,0);

		cout << '#' << test_case << ' ' << mx << '\n';
	}
	return 0;
}