
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

int N, H, W;

// 현재 벽돌 맵
vector<vector<int>> board;

// N번 구슬을 발사한 뒤 남은 최소 벽돌 개수
int minRemainingBricks;

// 상, 하, 좌, 우
int dr[4] = { 0, 0, -1, 1 };
int dc[4] = { -1, 1, 0, 0 };


/*
 * 디버깅용 맵 출력 함수
 */
// void printBoard(const vector<vector<int>>& currentBoard)
// {
//     for (int row = 0; row < currentBoard.size(); row++)
//     {
//         for (int col = 0; col < currentBoard[row].size(); col++)
//         {
//             cout << currentBoard[row][col] << " ";
//         }
//         cout << "\n";
//     }
//
//     cout << "--------------------\n";
// }


/*
 * 특정 벽돌을 기준으로 연쇄 폭발 처리
 *
 * row, col     : 현재 폭발할 벽돌 위치
 * currentBoard : 현재 벽돌 상태
 *
 * 벽돌 값이 K라면 자기 자신을 포함하여
 * 상하좌우로 K - 1칸까지 영향을 줌
 */
void breakBrick(
    int row,
    int col,
    vector<vector<int>>& currentBoard)
{
    // 이미 빈 공간이라면 아무 작업도 하지 않음
    if (currentBoard[row][col] == 0)
    {
        return;
    }

    // 현재 벽돌의 폭발 범위 저장
    int explosionRange = currentBoard[row][col];

    // 현재 벽돌 제거
    currentBoard[row][col] = 0;

    // 값이 1이면 자기 자신만 제거하면 됨
    if (explosionRange == 1)
    {
        return;
    }

    // 4방향으로 연쇄 폭발
    for (int direction = 0; direction < 4; direction++)
    {
        int nextRow = row;
        int nextCol = col;

        for (int distance = 1;
             distance < explosionRange;
             distance++)
        {
            nextRow += dr[direction];
            nextCol += dc[direction];

            // 맵 범위를 벗어나면 해당 방향 탐색 종료
            if (nextRow < 0 || nextRow >= H ||
                nextCol < 0 || nextCol >= W)
            {
                break;
            }

            // 해당 위치의 벽돌도 연쇄적으로 제거
            breakBrick(
                nextRow,
                nextCol,
                currentBoard
            );
        }
    }
}


/*
 * 특정 열에 구슬을 발사한 뒤
 * 폭발 + 중력 처리가 완료된 새로운 맵 반환
 *
 * column       : 구슬을 떨어뜨릴 열
 * currentBoard : 현재 벽돌 상태
 */
vector<vector<int>> shootBall(
    int column,
    const vector<vector<int>>& currentBoard)
{
    // 현재 DFS 상태를 복사하여 다음 상태 생성
    vector<vector<int>> nextBoard = currentBoard;

    int hitRow = -1;

    // 위쪽에서부터 가장 먼저 만나는 벽돌 탐색
    for (int row = 0; row < H; row++)
    {
        if (nextBoard[row][column] != 0)
        {
            hitRow = row;
            break;
        }
    }

    // 해당 열에 벽돌이 존재한다면 연쇄 폭발
    if (hitRow != -1)
    {
        breakBrick(
            hitRow,
            column,
            nextBoard
        );
    }

    /*
     * 중력 처리
     *
     * 각 열마다 아래쪽에서 가장 가까운 빈 공간을 찾아
     * 위에 있는 벽돌을 아래로 이동
     */
    for (int col = 0; col < W; col++)
    {
        int lowestEmptyRow = -1;

        for (int row = H - 1; row >= 0; row--)
        {
            // 현재 위치가 빈 공간인 경우
            if (nextBoard[row][col] == 0)
            {
                if (row > lowestEmptyRow)
                {
                    lowestEmptyRow = row;
                }
            }

            // 현재 위치에 벽돌이 있는 경우
            else
            {
                // 아래쪽에 빈 공간이 존재하면 이동
                if (lowestEmptyRow != -1)
                {
                    swap(
                        nextBoard[row][col],
                        nextBoard[lowestEmptyRow][col]
                    );

                    /*
                     * 방금 벽돌이 있던 위치 위쪽에서
                     * 다음 빈 공간을 탐색
                     */
                    for (int emptyRow = lowestEmptyRow - 1;
                         emptyRow >= row;
                         emptyRow--)
                    {
                        if (nextBoard[emptyRow][col] == 0)
                        {
                            lowestEmptyRow = emptyRow;
                            break;
                        }
                    }
                }
            }
        }
    }

    return nextBoard;
}


/*
 * 모든 구슬 발사 위치 조합을 DFS로 탐색
 *
 * shotCount    : 지금까지 발사한 구슬 개수
 * currentBoard : 현재 벽돌 상태
 */
void dfs(
    int shotCount,
    const vector<vector<int>>& currentBoard)
{
    // N개의 구슬을 모두 발사한 경우
    if (shotCount == N)
    {
        int remainingBrickCount = 0;

        // 남아있는 벽돌 개수 계산
        for (int row = 0; row < H; row++)
        {
            for (int col = 0; col < W; col++)
            {
                if (currentBoard[row][col] != 0)
                {
                    remainingBrickCount++;
                }
            }
        }

        minRemainingBricks =
            min(minRemainingBricks, remainingBrickCount);

        // printBoard(currentBoard);

        return;
    }

    /*
     * 현재 구슬을 모든 열에 한 번씩 떨어뜨려보며
     * 가능한 모든 경우 탐색
     */
    for (int column = 0; column < W; column++)
    {
        vector<vector<int>> nextBoard =
            shootBall(column, currentBoard);

        dfs(
            shotCount + 1,
            nextBoard
        );
    }
}


int main()
{
    int T;
    cin >> T;

    for (int testCase = 1;
         testCase <= T;
         testCase++)
    {
        cin >> N >> W >> H;

        minRemainingBricks = INT_MAX;

        // H행 W열 크기로 초기화
        board.assign(
            H,
            vector<int>(W, 0)
        );

        for (int row = 0; row < H; row++)
        {
            for (int col = 0; col < W; col++)
            {
                cin >> board[row][col];
            }
        }

        dfs(0, board);

        cout << "#" << testCase << " "
             << minRemainingBricks << "\n";
    }

    return 0;
}
