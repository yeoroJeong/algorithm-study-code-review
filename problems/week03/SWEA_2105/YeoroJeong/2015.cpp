
#include <iostream>
#include <algorithm>

using namespace std;

int N;

// 카페별 디저트 종류
int cafe[21][21];

// 대각선 이동 방향
// 우하 → 좌하 → 좌상 → 우상
int dr[4] = { 1, 1, -1, -1 };
int dc[4] = { 1, -1, -1, 1 };

// 해당 종류의 디저트를 이미 먹었는지 확인
int visitedDessert[101];

int maxDessertCount;


/*
 * 대각선 방향으로 이동하며
 * 서로 다른 디저트를 먹는 사각형 경로 탐색
 *
 * row, col        : 현재 위치
 * startRow/Col    : 출발한 카페 위치
 * direction       : 현재 이동 방향
 * dessertCount    : 지금까지 먹은 디저트 개수
 */
void dfs(
    int row,
    int col,
    int startRow,
    int startCol,
    int direction,
    int dessertCount)
{
    /*
     * 현재 방향 그대로 이동하거나
     * 다음 방향으로 한 번 꺾을 수 있음
     */
    for (int nextDirection = direction;
         nextDirection <= direction + 1 && nextDirection < 4;
         nextDirection++)
    {
        int nextRow = row + dr[nextDirection];
        int nextCol = col + dc[nextDirection];

        // 지도 범위를 벗어난 경우
        if (nextRow < 0 || nextRow >= N ||
            nextCol < 0 || nextCol >= N)
        {
            continue;
        }

        /*
         * 출발점으로 돌아왔다면 사각형 경로 완성
         * 최소 4개의 카페를 방문해야 정상적인 사각형
         */
        if (nextRow == startRow &&
            nextCol == startCol &&
            dessertCount >= 4)
        {
            maxDessertCount =
                max(maxDessertCount, dessertCount);

            continue;
        }

        int nextDessertType = cafe[nextRow][nextCol];

        // 이미 먹은 종류의 디저트라면 이동 불가능
        if (visitedDessert[nextDessertType])
        {
            continue;
        }

        // 현재 디저트 선택
        visitedDessert[nextDessertType] = 1;

        // 다음 카페 탐색
        dfs(
            nextRow,
            nextCol,
            startRow,
            startCol,
            nextDirection,
            dessertCount + 1
        );

        // 다른 경로 탐색을 위해 방문 여부 복구
        visitedDessert[nextDessertType] = 0;
    }
}


int main()
{
    int T;
    cin >> T;

    for (int testCase = 1; testCase <= T; testCase++)
    {
        cin >> N;

        maxDessertCount = -1;

        for (int row = 0; row < N; row++)
        {
            for (int col = 0; col < N; col++)
            {
                cin >> cafe[row][col];
            }
        }

        /*
         * 사각형을 만들 수 있는 위치만 시작점으로 사용
         *
         * 너무 아래쪽이나 좌우 끝에서는
         * 정상적인 대각선 사각형을 만들 수 없음
         */
        for (int startRow = 0; startRow < N - 2; startRow++)
        {
            for (int startCol = 1; startCol < N - 1; startCol++)
            {
                int startDessertType = cafe[startRow][startCol];

                // 시작 카페의 디저트는 이미 먹은 것으로 처리
                visitedDessert[startDessertType] = 1;

                dfs(
                    startRow,
                    startCol,
                    startRow,
                    startCol,
                    0,
                    1
                );

                // 다음 시작점 탐색을 위해 복구
                visitedDessert[startDessertType] = 0;
            }
        }

        cout << "#" << testCase << " "
             << maxDessertCount << "\n";
    }

    return 0;
}
