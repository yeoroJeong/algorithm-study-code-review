#include <iostream>
#include <deque>
#include <algorithm>

using namespace std;

int N, K;

// 우, 좌, 상, 하
int dr[4] = { 0, 0, -1, 1 };
int dc[4] = { 1, -1, 0, 0 };

int mountainMap[9][9];
int visited[9][9];

int maxTrailLength;


/*
 * 현재 위치에서 만들 수 있는 등산로를 DFS로 탐색
 *
 * row, col         : 현재 위치
 * trailLength      : 현재까지의 등산로 길이
 * hasUsedCutting   : 지형 깎기 사용 여부
 *                    false = 아직 사용하지 않음
 *                    true  = 이미 사용함
 */
void dfs(int row, int col, int trailLength, bool hasUsedCutting)
{
    // 현재까지 만들어진 등산로의 최대 길이 갱신
    maxTrailLength = max(maxTrailLength, trailLength);

    for (int direction = 0; direction < 4; direction++)
    {
        int nextRow = row + dr[direction];
        int nextCol = col + dc[direction];

        // 지도 범위를 벗어난 경우
        if (nextRow < 0 || nextRow >= N ||
            nextCol < 0 || nextCol >= N)
        {
            continue;
        }

        // 이미 방문한 위치는 다시 방문하지 않음
        if (visited[nextRow][nextCol])
        {
            continue;
        }

        // 현재 위치보다 낮은 지형이라면 그대로 이동 가능
        if (mountainMap[nextRow][nextCol] < mountainMap[row][col])
        {
            visited[nextRow][nextCol] = 1;

            dfs(
                nextRow,
                nextCol,
                trailLength + 1,
                hasUsedCutting
            );

            // 다른 경로 탐색을 위해 방문 상태 복구
            visited[nextRow][nextCol] = 0;
        }

        // 아직 지형을 깎지 않았다면 한 번 깎아서 이동 가능한지 확인
        else if (!hasUsedCutting)
        {
            /*
             * 다음 지형을 현재 지형보다 정확히 1 낮게 만드는 것이
             * 가장 적게 깎으면서 이동할 수 있는 방법
             *
             * 필요한 절삭량:
             * nextHeight - currentHeight + 1
             */
            int cuttingAmount =
                mountainMap[nextRow][nextCol] - mountainMap[row][col] + 1;

            // 최대 K만큼 깎을 수 있는 경우
            if (cuttingAmount <= K)
            {
                mountainMap[nextRow][nextCol] -= cuttingAmount;
                visited[nextRow][nextCol] = 1;

                dfs(
                    nextRow,
                    nextCol,
                    trailLength + 1,
                    true
                );

                // 백트래킹: 방문 여부와 깎았던 지형 높이 복구
                visited[nextRow][nextCol] = 0;
                mountainMap[nextRow][nextCol] += cuttingAmount;
            }
        }
    }
}


int main()
{
    int T;
    cin >> T;

    for (int testCase = 1; testCase <= T; testCase++)
    {
        cin >> N >> K;

        maxTrailLength = 0;
        int highestMountain = 0;

        // 가장 높은 봉우리들의 위치 저장
        deque<pair<int, int>> highestPositions;

        for (int row = 0; row < N; row++)
        {
            for (int col = 0; col < N; col++)
            {
                cin >> mountainMap[row][col];

                highestMountain =
                    max(highestMountain, mountainMap[row][col]);
            }
        }

        // 등산로는 가장 높은 봉우리에서 시작
        for (int row = 0; row < N; row++)
        {
            for (int col = 0; col < N; col++)
            {
                if (mountainMap[row][col] == highestMountain)
                {
                    highestPositions.push_back({ row, col });
                }
            }
        }

        // 모든 최고 봉우리를 시작점으로 DFS 수행
        while (!highestPositions.empty())
        {
            int startRow = highestPositions.front().first;
            int startCol = highestPositions.front().second;

            highestPositions.pop_front();

            visited[startRow][startCol] = 1;

            dfs(startRow, startCol, 1, false);

            visited[startRow][startCol] = 0;
        }

        cout << "#" << testCase << " "
            << maxTrailLength << "\n";
    }

    return 0;
}