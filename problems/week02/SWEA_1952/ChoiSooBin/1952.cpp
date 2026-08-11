/*
문제: 수영장
링크: https://swexpertacademy.com/main/code/problem/problemDetail.do?contestProbId=AV5PpFQaAQMDFAUq
작성자: 최수빈(sbyy77dev)
알고리즘: DP (동적 계획법)

[문제 요약]
1년 동안 각 달의 수영장 이용 계획(일수)이 주어지고, 1일/1달/3달/1년 이용권의 가격이 주어진다.
이용권을 적절히 조합하여 1년 동안 수영장을 이용하는 데 드는 최소 비용을 구하는 문제다.

[핵심 아이디어]
전형적인 1차원 DP(동적 계획법) 문제.
i번째 달까지의 최소 비용을 구할 때, '이전 달(i-1)까지의 최소 비용 + 이번 달 이용권'과 '3달 전(i-3)까지의 최소 비용 + 3달 이용권'을 비교하며 바텀업(Bottom-up) 방식으로 해를 쌓아나가는 것이 핵심이다.

[풀이 과정]
1. 1일 이용권으로만 다닐 때의 가격과 1달 이용권 가격을 비교하여, 각 달마다 더 싼 금액을 dp 배열에 기본 비용으로 저장해 둔다.
2. 1월~3월(초기 기저 상태)은 1~2단계에서 구한 누적 비용과 3달 이용권 1장 가격을 비교하여 최솟값으로 세팅한다.
3. 4월부터 12월까지 반복문을 돌며 점화식을 적용한다.
   - case1: 이전 달(i-1)까지의 최소 비용 + 이번 달 기본 비용
   - case2: 3달 전(i-3)까지의 최소 비용 + 3달 이용권 가격
   - 둘 중 더 작은 값을 현재 달(month3[i])의 최소 비용으로 갱신한다.
4. 마지막으로 12월까지 계산된 최소 비용(month3[12])과 처음부터 1년 이용권을 샀을 때의 가격을 비교해 최종 최솟값을 출력한다.

[복잡도]
- 시간 복잡도: 달의 개수가 12개로 고정되어 있으므로 반복문을 돌아도 연산 횟수가 매우 적다. O(1)에 수렴하여 제한 시간(3초) 안에 무조건 통과한다.
- 공간 복잡도: 1차원 배열(크기 13~15) 몇 개만 사용하므로 O(1) 수준으로 메모리는 아주 널널하다.

[막혔던 부분과 오답 원인 및 해결 방법]
- 첫 번째 삽질 (그리디와 DP의 혼동, 잘못된 상태 전이):
  초기 에러 코드를 보면 for문 안에서 if (dp[i] + period < dp[i + 3]) 조건으로 3달치 비용을 덮어쓰고, i += 2 로 강제로 인덱스를 점프하는 방식을 사용했다.
  [원인] DP는 '과거의 최적해'를 바탕으로 '현재의 최적해'를 점화식을 통해 구해야 하는데, 현재 위치에서 미래의 배열 값을 조건부로 수정하고 루프를 건너뛰려다 보니 앞선 선택이 꼬이면서 최적 부분 구조(Optimal Substructure)가 파괴되었다. (시뮬레이션/그리디식 접근)
  [해결] 배열을 용도별로(1달 단가용 dp, 누적합용 month3) 분리하고, 4월부터는 미래를 내다보는 게 아니라 i-1, i-3 처럼 '과거의 결과'를 끌어와 현재(i)를 갱신하는 완벽한 상향식(Bottom-up) DP로 로직을 수정하여 정답을 맞췄다.

- 두 번째 삽질 (배열 인덱스와 초기화 꼬임):
  에러 코드에서는 dp 배열을 15까지 넉넉하게 잡고 dp[13], dp[14]에 12월 값을 억지로 밀어 넣으면서 경계값 처리가 매우 지저분해졌다. 
  [해결] 최종 통과 코드에서는 1~3월 구간을 별도의 반복문으로 먼저 초기화(Base case 설정)해 주고, 4월부터 12월까지만 i-3을 참조하도록 분리하여 인덱스 참조 에러(Out of Bounds) 위험을 깔끔하게 없앴다.
*/

#include<iostream>
#include<climits>
#include<cmath>
#include<algorithm>
using namespace std;
 
int max_val = INT_MAX;
 
int main(int argc, char* argv)
{
    int test_case;
    int T;
    cin >> T;
 
    for (test_case = 1; test_case <= T; ++test_case)
    {
        int day, month, period, year;
        int use[13];
        int dp[15] = { 0, };
        //int use[12] = { 0, };    // 어차피 한 달에 최대 31일 int로 충분
        fill(use, use + 13, 0);
        cin >> day >> month >> period >> year;
        int day_cnt = 0;
        int ans = 0;
        int month3[13];
        fill(month3, month3 + 13, 0);
        for (int i = 1; i <= 12; i++) {
            cin >> use[i];
            day_cnt += use[i];
        }
        // 1일 이용권
        for (int i = 1; i <= 12; i++) {
            dp[i] = use[i] * day;
        }
        // ans = 1달 이용권
        for (int i = 1; i <= 12; i++) {
            if (dp[i] > month) dp[i] = month;
        }
        int b = 0;
        for (int j = 1; j <= 3; j++) {
            month3[j] = min(period, dp[j] + b);
            b = dp[j] + b;
        }
 
        for (int i = 4; i <= 12; i++) {
        // 현재 최소 그냥 추가
            int case1 = month3[i - 1] + dp[i];
            // 현재까지 3달 묶기
            int case2 = month3[i - 3] + min(period, (dp[i] + dp[i - 1] + dp[i - 2]));
            month3[i] = min(case1, case2);
        }
        ans = min(month3[12], year);
 
         
        cout << '#' << test_case << ' ' << ans << '\n';
    }
    return 0;
}