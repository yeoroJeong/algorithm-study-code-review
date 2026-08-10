#include <bits/stdc++.h>

using namespace std;

int N;
int dx[4] = {-1, 0, 0, 1};
int dy[4] = {0, 1, -1, 0};

int maxCore = 0;
int minWire = 999999;


// dx, dy는 이동할 방향
pair<bool, int> calWireLen(
    int row,
    int col,
    vector<vector<int>>& cell,
    int dxNum,
    int dyNum,
    int curWireCount
)
{
    row += dxNum;
    col += dyNum;

    // 배열의 범위를 벗어났다면 실패
    if(row < 0 || row >= N || col < 0 || col >= N)
    {
        return {false, -1};
    }

    // 멈춤 조건 - core가 있거나 전선이 있으면
    if(cell[row][col] == 1 || cell[row][col] == 2)
    {
        return {false, -1};
    }

    // 전선을 한 칸 설치했으므로 전선 길이 +1
    curWireCount++;

    // 성공 조건 - 전선이 가장 자리에 도착했다면
    if(row == 0 || row == N-1 || col == 0 || col == N-1)
    {
        return {true, curWireCount};
    }

    // 재귀로 계속 이어서 진행
    return calWireLen(
        row,
        col,
        cell,
        dxNum,
        dyNum,
        curWireCount
    );
}


// DFS
void dfs(
    int coreNum,
    vector<pair<int,int>>& corePos,
    vector<vector<int>>& cell,
    int connectedCore,
    int connectedWire
)
{
    // 모든 core를 검사했다면
    if(coreNum >= corePos.size())
    {
        // 연결한 core의 수가 더 많다면
        if(connectedCore > maxCore)
        {
            maxCore = connectedCore;
            minWire = connectedWire;
        }
        // 연결한 core의 수가 같다면
        // 전선의 길이가 더 짧은 경우를 선택
        else if(
            connectedCore == maxCore &&
            connectedWire < minWire
        )
        {
            minWire = connectedWire;
        }

        return;
    }


    int startRow = corePos[coreNum].first;
    int startCol = corePos[coreNum].second;


    // 현재 core에서 4개의 방향으로 전선 연결 검사
    for(int i=0; i<4; ++i)
    {
        pair<bool, int> tmp =
            calWireLen(
                startRow,
                startCol,
                cell,
                dx[i],
                dy[i],
                0
            );

        if(tmp.first)
        {
            // 전선 설치
            int row = startRow + dx[i];
            int col = startCol + dy[i];

            while(
                row >= 0 && row < N &&
                col >= 0 && col < N
            )
            {
                cell[row][col] = 2;

                row += dx[i];
                col += dy[i];
            }


            // 다음 core 검사
            dfs(
                coreNum + 1,
                corePos,
                cell,
                connectedCore + 1,
                connectedWire + tmp.second
            );


            // 전선 제거
            row = startRow + dx[i];
            col = startCol + dy[i];

            while(
                row >= 0 && row < N &&
                col >= 0 && col < N
            )
            {
                cell[row][col] = 0;

                row += dx[i];
                col += dy[i];
            }
        }
    }


    // 현재 core를 연결하지 않는 경우
    dfs(
        coreNum + 1,
        corePos,
        cell,
        connectedCore,
        connectedWire
    );
}


int main(int argc, char** argv)
{
    int test_case;
    int T;

    cin >> T;

    for(test_case = 1; test_case <= T; ++test_case)
    {
        cin >> N;

        vector<vector<int>> cell(
            N,
            vector<int>(N, 0)
        );

        // row, col로 저장
        vector<pair<int, int>> corePos;


        // 테스트 케이스마다 결과 초기화
        maxCore = 0;
        minWire = 999999;


        for(int row=0; row<N; ++row)
        {
            for(int col=0; col<N; ++col)
            {
                cin >> cell[row][col];

                // 가장자리가 아닌 위치에 core가 있으면
                if(
                    (row != 0 && row != N-1) &&
                    (col != 0 && col != N-1) &&
                    cell[row][col] == 1
                )
                {
                    // core 위치 배열에 추가
                    corePos.push_back({row,col});
                }
            }
        }


        // core가 하나도 없는 경우
        if(corePos.empty())
        {
            cout << "#" << test_case << " 0\n";
            continue;
        }


        // core 0번부터 DFS 시작
        dfs(
            0,
            corePos,
            cell,
            0,
            0
        );


        cout << "#" << test_case << " "
             << minWire << "\n";
    }

    return 0;
}