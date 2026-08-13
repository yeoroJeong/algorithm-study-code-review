/*
문제: 블록 깨기 (최대 점수 구하기)
작성자: 최수빈
알고리즘: 비트마스킹(Bitmasking), 동적 계획법(DP - Memoization), DFS

[문제 요약]
일렬로 놓인 N개의 숫자 블록을 순서대로 하나씩 깨면서 얻을 수 있는 최대 점수를 구하는 문제다.
블록을 깨면 남아 있는 양옆 블록이 빈자리 없이 붙는다.

점수는 현재 블록의 살아 있는 이웃에 따라 결정된다.
- 양쪽 이웃이 모두 있으면: 왼쪽 * 오른쪽
- 한쪽 이웃만 있으면: 해당 이웃의 숫자
- 이웃이 없으면: 현재 블록의 숫자

[핵심 아이디어]
- 단순 DFS로 모든 제거 순서를 탐색하면 O(N!)의 시간 복잡도가 발생하므로 비트마스킹과 DP를 사용한다.
- A를 먼저 깨고 B를 깨는 경우와 B를 먼저 깨고 A를 깨는 경우처럼, 제거 순서는 달라도 현재 남아 있는 블록의 상태가 같다면 이후의 결과도 같다.
- 블록의 제거 여부를 비트로 표현하고, 해당 값을 `dp` 배열의 인덱스(state)로 사용해 동일한 상태의 중복 계산을 방지한다.
- 실제 배열에서 블록을 삭제하지 않고 비트 연산으로 제거 여부를 관리하여 상태 변경과 조회를 간단하게 처리한다.

[풀이 과정]
1. N개의 블록을 입력받고 `dp` 배열을 -1로 초기화한다.
2. 아무 블록도 깨지지 않은 상태인 `dfs(0)`부터 탐색한다.
3. `state == (1 << n) - 1`이면 모든 블록을 깬 상태이므로 0을 반환한다.
4. `dp[state] != -1`이면 이미 계산한 상태이므로 저장된 값을 반환한다.
5. 아직 깨지지 않은 블록을 하나 선택하고, 좌우로 탐색해 가장 가까운 살아 있는 이웃을 찾는다.
6. 이웃의 존재 여부에 따라 현재 블록을 깰 때 얻는 점수를 계산한다.
7. `next = state | (1 << i)`로 현재 블록을 제거한 다음 `score + dfs(next)`를 계산한다.
8. 가능한 모든 블록을 확인한 뒤 최댓값을 `dp[state]`에 저장하고 반환한다.

[오답 원인 및 해결]
- map + vector를 이용한 상태 관리
  [원인] 남아 있는 블록을 vector로 관리하고 이를 map의 key로 사용했다.
  map 탐색과 vector의 erase/insert 과정에서 추가적인 연산 비용이 발생해 시간 초과가 발생했다.
  [해결] 블록의 제거 상태를 하나의 정수 비트마스크로 표현하고, 배열 기반 DP로 변경했다.

- 이웃 탐색 조건 오류
  [원인] 오른쪽 이웃을 찾는 반복문의 조건을 `j < n`이 아닌 `j < 0`으로 작성해 탐색이 수행되지 않았다.
  또한 이웃이 없는 경우를 -1로 초기화했지만 이후 조건문에서는 0과 비교하고 있었다.
  [해결] 반복문 조건을 수정하고, 이웃이 없는 상태를 -1로 통일해 점수 계산 조건을 수정했다.

[복잡도]
- 시간 복잡도: O(2^N * N^2)
  가능한 상태는 2^N개이며, 각 상태에서 최대 N개의 블록을 선택하고 각 블록의 좌우 이웃을 탐색한다.
- 공간 복잡도: O(2^N)
  각 비트마스크 상태의 최댓값을 `dp` 배열에 저장한다.
*/
#include<algorithm>
#include<iostream>
#include<climits>


using namespace std;

int n;
int mx;
int block[10];
int dp[1024];

int dfs(int state) {
    if (state == ((1 << n) - 1)) return 0;
    if (dp[state] != -1) {
        return dp[state];    // 이미 계산한 적 있는 값? -> memoization 반환
    }
    int tmp = 0; 
    for (int i = 0; i < n; i++) {
        if (state & (1 << i))continue;    // 만약 i번째 bit가 1? 이미깨진 블록
        // 점수 계산하기
        int left = -1;
        int right = -1;
        for (int j = i - 1; j >= 0; j--) {
            if (!(state & (1 << j))) {
                left = block[j];
                break;
            }
        }
        for (int j = i + 1; j < n; j++) {
            if (!(state & (1 << j))) {
                right = block[j];
                break;
            }
        }

        int score = 0;
        if (left > -1 && right > -1) score = left * right;
        else if (left > -1 && right == -1) score = left;
        else if (left == -1 && right > -1) score = right;
        else score = block[i];

        int next = state | (1 << i);    // 비트마스킹 업데이트
        tmp = max(tmp, score + dfs(next));
    }
    return dp[state] = tmp;
}


int main(int argc, char** argv)
{
    int test_case;
    int T;

    freopen("26071_input.txt", "r", stdin);
    cin >> T;

    for (test_case = 1; test_case <= T; ++test_case)
    {
        mx = INT_MIN;
        cin >> n;

        fill(dp, dp + 1024, -1);
        fill(block, block + 10, false); // 0~9
        for (int i = 0; i < n; i++) {
            cin >> block[i];
        }
        int ans = dfs(0);

        cout << '#' << test_case << ' ' << ans << '\n';
    }
    return 0;
}