#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>

using namespace std;

int N;
int board[13][13];

// 우, 좌, 하, 상
int dr[4] = { 0, 0, 1, -1 };
int dc[4] = { 1, -1, 0, 0 };

// 가장자리에 있지 않은 코어들의 위치
vector<pair<int, int>> corePositions;

// 최대한 많이 연결한 코어의 개수
int maxConnectedCoreCount;

// 최대 코어 연결 시 사용한 최소 전선 길이
int minWireLength;


/*
 * 현재 코어에서 특정 방향으로 전선을 연결할 수 있는지 확인
 *
 * row, col : 현재 코어 위치
 * direction : 전선을 연결할 방향
 *
 * 가장자리까지 가는 도중 다른 코어나 전선을 만나면 연결 불가능
 */
bool canConnectToEdge(int row, int col, int direction)
{
    int nextRow = row;
    int nextCol = col;

    while (true)
    {
        nextRow += dr[direction];
        nextCol += dc[direction];

        // 배열 밖으로 나갔다면 가장자리까지 정상적으로 연결 가능
        if (nextRow < 0 || nextRow >= N ||
            nextCol < 0 || nextCol >= N)
        {
            return true;
        }

        // 다른 코어(1) 또는 이미 설치된 전선(2)을 만난 경우
        if (board[nextRow][nextCol] != 0)
        {
            return false;
        }
    }
}


/*
 * 현재 코어에서 특정 방향으로 전선을 설치하거나 제거
 *
 * value = 2 : 전선 설치
 * value = 0 : 전선 제거(백트래킹)
 *
 * 반환값 : 해당 방향에 설치/제거한 전선의 길이
 */
int updateWire(int row, int col, int direction, int value)
{
    int nextRow = row;
    int nextCol = col;
    int wireLength = 0;

    while (true)
    {
        nextRow += dr[direction];
        nextCol += dc[direction];

        // 가장자리를 넘어가면 종료
        if (nextRow < 0 || nextRow >= N ||
            nextCol < 0 || nextCol >= N)
        {
            break;
        }

        board[nextRow][nextCol] = value;
        wireLength++;
    }

    return wireLength;
}


/*
 * DFS + 백트래킹
 *
 * coreIndex          : 현재 확인할 코어 번호
 * connectedCoreCount : 지금까지 전원에 연결한 코어 개수
 * totalWireLength    : 지금까지 사용한 전체 전선 길이
 */
void dfs(int coreIndex, int connectedCoreCount, int totalWireLength)
{
    // 모든 코어를 확인한 경우
    if (coreIndex == corePositions.size())
    {
        // 연결된 코어가 기존 최대값보다 많은 경우
        if (connectedCoreCount > maxConnectedCoreCount)
        {
            maxConnectedCoreCount = connectedCoreCount;
            minWireLength = totalWireLength;
        }
        // 연결된 코어 개수가 같다면 전선 길이가 짧은 경우 선택
        else if (connectedCoreCount == maxConnectedCoreCount)
        {
            minWireLength = min(minWireLength, totalWireLength);
        }

        return;
    }

    auto [row, col] = corePositions[coreIndex];

    // 현재 코어를 상하좌우 4방향으로 연결해본다.
    for (int direction = 0; direction < 4; direction++)
    {
        if (!canConnectToEdge(row, col, direction))
        {
            continue;
        }

        // 전선 설치
        int wireLength = updateWire(row, col, direction, 2);

        // 현재 코어를 연결한 상태로 다음 코어 탐색
        dfs(
            coreIndex + 1,
            connectedCoreCount + 1,
            totalWireLength + wireLength
        );

        // 다른 경우를 탐색하기 위해 설치했던 전선 제거
        updateWire(row, col, direction, 0);
    }

    // 현재 코어를 연결하지 않고 다음 코어를 탐색하는 경우
    dfs(
        coreIndex + 1,
        connectedCoreCount,
        totalWireLength
    );
}


int main()
{
    int T;
    cin >> T;

    for (int testCase = 1; testCase <= T; testCase++)
    {
        cin >> N;

        maxConnectedCoreCount = 0;
        minWireLength = INT_MAX;

        corePositions.clear();

        for (int row = 0; row < N; row++)
        {
            for (int col = 0; col < N; col++)
            {
                cin >> board[row][col];

                if (board[row][col] == 1)
                {
                    // 가장자리 코어는 이미 전원에 연결되어 있으므로
                    // DFS 탐색 대상에서 제외
                    if (row == 0 || row == N - 1 ||
                        col == 0 || col == N - 1)
                    {
                        continue;
                    }

                    corePositions.push_back({ row, col });
                }
            }
        }

        dfs(0, 0, 0);

        cout << "#" << testCase << " "
            << minWireLength << "\n";
    }

    return 0;
}