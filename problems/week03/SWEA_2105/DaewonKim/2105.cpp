/*
    1. 대각 이동 -> 다시 출발한 지점으로 와야함
    2. 임의의 1점에 출발 -> 대각 이동 -> 다시 출발한 지점으로 와야함
    3. 디저트를 먹지 못하는 경우 -> -1 출력
    4. 디저트의 종류는 모두 달라야함.


    문제 풀이 및 접근
    1. dfs, 백트레킹
    2. visited 배열을 만들어서 방문한 디저트 종류를 체크
    3. 디저트 종류의 경우, 1~100까지이고, 이것도 다시 초기화 해줘야함.
    4. 4개의 모서리 지점은 도달 불가

    ======
    
    -> 현재 코드: 종료 조건들을 통해 사각형이 보장되지 않음. (사각형이 아닌데, 성립되는 반례가 존재)
    => 개선점: 사각형이 되도록 보장을 해줘야함. 
    
    i) dir배열을 만들어서 관리
    대각 좌, 우 / 아래 대각 좌,우의 갯수가 1개 이상이어야 하며, 상쇄되는 대각선의 갯수가 서로 맞아야함.
    -> 현재 시간 초과발생.

    => 개선
    방향을 아예 1개로 고정해서 설정 -> 결과 값은 동일하다
    0 -> 1 -> 2 -> 3으로 방향 고정 + 도달 불가, 시작 불가 지점에 대한 예외 처리 
    
*/

#include<iostream>
#include<vector>
#include<algorithm>

using namespace std;

int maxDessertCount = -1;

/*
0: 아래 오른쪽 대각
1: 아래 왼쪽 대각
2: 위 왼쪽 대각
3: 위 오른쪽 대각

0,2의 갯수가 서로 맞아야 함.

1,3의 갯수가 서로 맞아야 함.
*/ 
int dx[4] = {1, 1, -1, -1};
int dy[4] = {1, -1, -1, 1};

void dfs(int row, int col, int curDirNum, vector<vector<int>>& desserts, vector<vector<bool>>& visited, vector<bool>& dessertTypes, 
    vector<pair<int,int>>& dir, int n, int startRow, int startCol, int count) {

    // 4개 모서리 지점은 도달 불가
    if( (row ==0 &&col == 0)
    || (row == 0 && col == n-1) 
    || (row == n-1 && col == 0)
    || (row == n-1 && col == n-1) ) {
        return;
    }

    if(row == startRow && col == startCol
        && count > 0) {
        for(int i=0; i<2; ++i)
        {
            // 방향이 서로 상쇄되지 않거나, 사각형이 만족되지 않는 경우,
            if(dir[i].second != dir[i+2].second || dir[i].second <= 0 || dir[i+2].second <= 0)
            {
                return;
            }
        }
        
        maxDessertCount = max(maxDessertCount, count);
        return;
    }

    if(visited[row][col]) {
        return;
    }

    if(dessertTypes[desserts[row][col]]) {
        return;
    }

    int dessertType = desserts[row][col];
    dessertTypes[dessertType] = true;
    visited[row][col] = true;

    for(int d = curDirNum; d <= curDirNum + 1 && d < 4; ++d)
    {
        int newRow = row + dx[d];
        int newCol = col + dy[d];

        if(newRow < 0 || newRow >= n ||
           newCol < 0 || newCol >= n)
            continue;

        dir[d].second++;

        dfs(newRow, newCol, d,
            desserts, visited, dessertTypes, dir,
            n, startRow, startCol, count + 1);

        dir[d].second--;
    }
    // 원상복구
    dessertTypes[dessertType] = false;
    visited[row][col] = false;
}

int main(int argc, char** argv)
{
	int test_case;
	int T;

	cin>>T;

	for(test_case = 1; test_case <= T; ++test_case)
	{
        int n;
        cin >> n;
        maxDessertCount = -1;
        
        vector<vector<int>> desserts(n, vector<int>(n, 0));
        vector<vector<bool>> visited(n, vector<bool>(n, false));
        vector<bool> dessertTypes(101, false); // 1~ 100 디저트 종류 체크용
        vector<pair<int,int>> dir;
        for(int i=0; i<4; ++i)
        {
            dir.push_back({i,0});
        }


        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
                cin >> desserts[i][j];
            }
        }

        for(int row = 0; row < n; row++){
            for(int col = 0; col < n; col++){
                // 시작 시, 사각형 만들기 불가 테두리 제외
                if(row == n-1
                    || col ==0 || col == n-1 ) {
                    continue;
                }
                // 지점, dir 방향- 시작 방향을 항상 고정해서 0-> 1-> 2-> 3
                dfs(row, col, 0, desserts, visited, dessertTypes, dir, n, row, col, 0);

            }
        }
        cout << "#" << test_case << " " << maxDessertCount << endl;

	}
	return 0;//정상종료시 반드시 0을 리턴해야합니다.
}