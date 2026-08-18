#include<iostream>
#include <vector>
#include <algorithm>

using namespace std;

// 최소 회전 수 저장
// row, col, 방향, 보석 번호
int minRotate[11][11][4][12];

// 방향 순서 -> 0: 오른, 1: 아래, 2: 왼, 3: 위 
int dx[4] = {0,1,0,-1};
int dy[4] = {1,0,-1,0};

int minAnswer = 100000;

// 현재 어떤 방향인지, 찾아야하는 보석 번호, 최소 회전 수에 대한 체크가 필요
void dfs(int row, int col, int curDir, int curRotateCnt,
    vector<vector<int>>& board, int mustFoundJewlNum, int jewCnt)
{
    // 범위 밖을 벗어나면 종료
    if(row < 0 || row >= (int)board.size()
        || col < 0 || col >= (int)board.size())
    {
        return;
    }

    // 현재 위치가 찾아야 하는 보석이라면,
    if(mustFoundJewlNum <= jewCnt
        && board[row][col] == mustFoundJewlNum)
    {
        // 다음 보석 번호를 찾도록 변경
        mustFoundJewlNum += 1;
    }

    // 찾아야 하는 보석 갯수를 다 찾았다면
    if(mustFoundJewlNum > jewCnt)
    {
        minAnswer = min(minAnswer, curRotateCnt);
        return;
    }

    // 현재 회전 수가 이미 구한 정답보다 크거나 같다면 종료
    if(curRotateCnt >= minAnswer)
    {
        return;
    }

    // 현재 위치, 현재 방향, 다음 보석 번호에 대해서
    // 더 작은 회전 수로 이미 방문했다면 종료
    if(curRotateCnt >= minRotate[row][col][curDir][mustFoundJewlNum])
    {
        return;
    }

    // 현재 위치, 방향, 찾아야 하는 보석 번호까지의 최소 회전 수 갱신
    minRotate[row][col][curDir][mustFoundJewlNum] = curRotateCnt;

    curDir %= 4;

    // 현재 방향으로 직진
    int nextRow = row + dx[curDir];
    int nextCol = col + dy[curDir];


    dfs(nextRow, nextCol, curDir, curRotateCnt,
        board, mustFoundJewlNum, jewCnt);

    // 현재 방향에서 오른쪽으로 회전
    int nextDir = (curDir + 1) % 4;
    int nextRotateRow = row + dx[nextDir];
    int nextRotateCol = col + dy[nextDir];

    dfs(nextRotateRow, nextRotateCol, nextDir, curRotateCnt + 1,
        board, mustFoundJewlNum, jewCnt);
}

int main(int argc, char** argv)
{
    int test_case;
    int T;

    cin >> T;
    for(test_case = 1; test_case <= T; ++test_case)
    {
        int n;
        cin >> n;

        // 보석의 갯수
        int jewCnt = 0;

        vector<vector<int>> board(n, vector<int>(n, 0));
        for(int row = 0; row < n; ++row)
        {
            for(int col = 0; col < n; ++col)
            {
                cin >> board[row][col];

                if(board[row][col] != 0)
                {
                    jewCnt++;
                }
            }
        }

        // 큰 값으로 각 상태별 최소 회전 횟수를 초기화한 후 사용.
        for(int row = 0; row < 11; ++row)
        {
            for(int col = 0; col < 11; ++col)
            {
                for(int dir = 0; dir < 4; ++dir)
                {
                    for(int jew = 0; jew < 12; ++jew)
                    {
                        minRotate[row][col][dir][jew] = 100000;
                    }
                }
            }
        }

        minAnswer = 100000;
        // row, col, dir, curRotateCnt ...
        // 시작 위치: 0,0
        // 시작 방향: 0 -> 오른쪽
        // 처음 찾아야 하는 보석: 1번
        dfs(0, 0, 0, 0, board, 1, jewCnt);

        cout << "#" << test_case << " " << minAnswer << endl;
    }

    return 0;//정상종료시 반드시 0을 리턴해야합니다.
}