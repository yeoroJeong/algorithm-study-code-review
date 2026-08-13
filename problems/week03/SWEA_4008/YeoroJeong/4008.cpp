
#include <iostream>
#include <climits>
#include <algorithm>

using namespace std;

int N;

// 입력 숫자 배열
int numbers[13];

// 연산자 개수
int plusCount;
int minusCount;
int multiplyCount;
int divideCount;

// 만들 수 있는 최댓값 / 최솟값
int maxResult = INT_MIN;
int minResult = INT_MAX;


/*
 * 연산자를 하나씩 선택하며 가능한 모든 계산 결과 탐색
 *
 * numberIndex   : 다음에 사용할 숫자의 인덱스
 * currentResult : 현재까지 계산한 결과
 */
void dfs(int numberIndex, int currentResult)
{
    // 모든 숫자를 사용한 경우
    if (numberIndex == N)
    {
        maxResult = max(maxResult, currentResult);
        minResult = min(minResult, currentResult);

        return;
    }

    // 덧셈 연산자를 사용할 수 있는 경우
    if (plusCount > 0)
    {
        plusCount--;

        dfs(
            numberIndex + 1,
            currentResult + numbers[numberIndex]
        );

        // 다른 연산자 조합 탐색을 위해 복구
        plusCount++;
    }

    // 뺄셈 연산자를 사용할 수 있는 경우
    if (minusCount > 0)
    {
        minusCount--;

        dfs(
            numberIndex + 1,
            currentResult - numbers[numberIndex]
        );

        minusCount++;
    }

    // 곱셈 연산자를 사용할 수 있는 경우
    if (multiplyCount > 0)
    {
        multiplyCount--;

        dfs(
            numberIndex + 1,
            currentResult * numbers[numberIndex]
        );

        multiplyCount++;
    }

    // 나눗셈 연산자를 사용할 수 있는 경우
    if (divideCount > 0)
    {
        divideCount--;

        dfs(
            numberIndex + 1,
            currentResult / numbers[numberIndex]
        );

        divideCount++;
    }
}


int main()
{
    int T;
    cin >> T;

    for (int testCase = 1; testCase <= T; testCase++)
    {
        cin >> N;

        // 테스트 케이스마다 결과 초기화
        maxResult = INT_MIN;
        minResult = INT_MAX;

        // +, -, *, / 연산자 개수 입력
        cin >> plusCount
            >> minusCount
            >> multiplyCount
            >> divideCount;

        for (int index = 0; index < N; index++)
        {
            cin >> numbers[index];
        }

        /*
         * 첫 번째 숫자를 초기값으로 사용하고,
         * 두 번째 숫자(index 1)부터 연산자를 적용
         */
        dfs(1, numbers[0]);

        cout << "#" << testCase << " "
             << maxResult - minResult << "\n";
    }

    return 0;
}
