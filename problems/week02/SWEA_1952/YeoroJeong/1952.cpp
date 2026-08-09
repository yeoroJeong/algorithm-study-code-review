#include <iostream>
#include <algorithm>
#include <climits>

using namespace std;

// 0: 1일권, 1: 1개월권, 2: 3개월권, 3: 1년권
int passCost[4];

// 월별 수영장 이용 일수
int monthlyUsage[13];

// 최소 이용 비용
int minTotalCost;


/*
 * 현재 월부터 이용권을 선택하며 최소 비용 탐색
 *
 * month       : 현재 확인 중인 월
 * currentCost : 지금까지 사용한 비용
 */
void dfs(int month, int currentCost)
{
    // 12월까지 모든 이용권 선택이 끝난 경우
    if (month > 12)
    {
        minTotalCost = min(minTotalCost, currentCost);
        return;
    }

    /*
     * 1일권 사용
     * 해당 월의 이용 일수만큼 1일권을 구매
     */
    dfs(
        month + 1,
        currentCost + passCost[0] * monthlyUsage[month]
    );

    /*
     * 1개월권 사용
     * 현재 한 달을 이용하고 다음 달로 이동
     */
    dfs(
        month + 1,
        currentCost + passCost[1]
    );

    /*
     * 3개월권 사용
     * 현재 월부터 3개월을 한 번에 처리
     */
    if (month + 2 <= 12)
    {
        dfs(
            month + 3,
            currentCost + passCost[2]
        );
    }
}


int main()
{
    int T;
    cin >> T;

    for (int testCase = 1; testCase <= T; testCase++)
    {
        // 이용권 가격 입력
        for (int i = 0; i < 4; i++)
        {
            cin >> passCost[i];
        }

        // 1월 ~ 12월 이용 횟수 입력
        for (int month = 1; month <= 12; month++)
        {
            cin >> monthlyUsage[month];
        }

        /*
         * 1년권을 구매하는 경우를 초기 최소값으로 설정
         * DFS에서는 1일권 / 1개월권 / 3개월권 조합을 탐색
         */
        minTotalCost = passCost[3];

        dfs(1, 0);

        cout << "#" << testCase << " "
            << minTotalCost << "\n";
    }

    return 0;
}