#include <iostream>
#include <algorithm>

using namespace std;

int N;

// 0이면 이미 제거된 블록
int blocks[11];

// 얻을 수 있는 최대 점수
int maxScore;


/*
 * 특정 블록을 제거했을 때 얻는 점수를 계산
 *
 * index : 제거할 블록의 위치
 *
 * 점수 규칙
 * 1. 양쪽에 블록이 존재하면        -> 왼쪽 값 * 오른쪽 값
 * 2. 한쪽에만 블록이 존재하면      -> 남아있는 블록 값
 * 3. 양쪽 모두 블록이 존재하지 않으면 -> 현재 블록 값
 */
int calculateRemoveScore(int index)
{
    int leftValue = 0;
    int rightValue = 0;

    // 현재 블록의 왼쪽에서 가장 가까운 남은 블록 탐색
    for (int leftIndex = index - 1; leftIndex >= 0; leftIndex--)
    {
        if (blocks[leftIndex] != 0)
        {
            leftValue = blocks[leftIndex];
            break;
        }
    }

    // 현재 블록의 오른쪽에서 가장 가까운 남은 블록 탐색
    for (int rightIndex = index + 1; rightIndex < N; rightIndex++)
    {
        if (blocks[rightIndex] != 0)
        {
            rightValue = blocks[rightIndex];
            break;
        }
    }

    // 양쪽 모두 블록이 없는 경우
    if (leftValue == 0 && rightValue == 0)
    {
        return blocks[index];
    }

    // 한쪽에만 블록이 남아있는 경우
    if (leftValue == 0 || rightValue == 0)
    {
        return max(leftValue, rightValue);
    }

    // 양쪽 모두 블록이 남아있는 경우
    return leftValue * rightValue;
}


/*
 * 블록을 제거하는 모든 순서를 탐색하는 DFS + 백트래킹
 *
 * removedCount : 지금까지 제거한 블록 개수
 * currentScore : 현재까지 얻은 점수
 */
void dfs(int removedCount, int currentScore)
{
    // 모든 블록을 제거한 경우
    if (removedCount == N)
    {
        maxScore = max(maxScore, currentScore);
        return;
    }

    // 현재 남아있는 모든 블록을 하나씩 제거해본다.
    for (int index = 0; index < N; index++)
    {
        // 이미 제거된 블록
        if (blocks[index] == 0)
        {
            continue;
        }

        // 현재 블록을 제거했을 때 얻는 점수
        int removeScore = calculateRemoveScore(index);

        // 백트래킹을 위해 원래 블록 값 저장
        int originalValue = blocks[index];

        // 현재 블록 제거
        blocks[index] = 0;

        // 다음 블록 제거
        dfs(
            removedCount + 1,
            currentScore + removeScore
        );

        // 다른 제거 순서를 탐색하기 위해 원상복구
        blocks[index] = originalValue;
    }
}


int main()
{
    int T;
    cin >> T;

    for (int testCase = 1; testCase <= T; testCase++)
    {
        cin >> N;

        maxScore = 0;

        for (int index = 0; index < N; index++)
        {
            cin >> blocks[index];
        }

        dfs(0, 0);

        cout << "#" << testCase << " "
             << maxScore << "\n";
    }

    return 0;
}