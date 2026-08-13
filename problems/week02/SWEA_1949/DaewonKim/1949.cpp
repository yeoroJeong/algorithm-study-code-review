#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int len;
int k;
int dx[4] = {1, -1, 0, 0};
int dy[4] = {0, 0, 1, -1};

/*
    조건 1.

- 등산로는 산으로 올라갈 수 있도록 반드시 높은 지형에서 낮은 지형으로 가로 또는 세로 방향으로 연결이 되어야 한다.
- 즉, 높이가 같은 곳 혹은 높은 지형이나, 대각선 방향의 연결은 불가능하다.
   조건 2.긴 등산로를 만들기 위해 딱 한 곳을 정해서 최대 K 깊이만큼 지형을 깎는 공사를 할 수 있다.

*/

// curHeight -> 원본에 손상을 주지 않고 계산하기 위해
int dfs(int row, int col, int curHeight, vector<vector<int>> &mountain, vector<vector<bool>> &visited, bool isDigged)
{
  visited[row][col] = true;

  int maxPath = 1;

  // 다음 위치가 현재 위치보다 낮다면 바로 이동 가능
  // isDigged가 true라면 더 이상 공사는 불가능하지만 낮은 위치로는 이동 가능
  // isDigged가 false일 때,
  // (다음 위치 높이 - K) < (현재 위치 높이)라면 공사 후 이동 가능

  for (int dir = 0; dir < 4; ++dir)
  {
    int nextRow = row + dx[dir];
    int nextCol = col + dy[dir];

    // 조건 판별
    if (nextRow < 0 || nextRow >= len || nextCol < 0 || nextCol >= len)
      continue;

    // 방문했다면 안감.
    if (visited[nextRow][nextCol])
      continue;

    // 안 파도 갈 수 있는 경우
    if (mountain[nextRow][nextCol] < curHeight)
    {
      maxPath = max(
          maxPath,
          1 + dfs(
                  nextRow,
                  nextCol,
                  mountain[nextRow][nextCol],
                  mountain,
                  visited,
                  isDigged));
    }

    // 아직 안팠고, 파야 갈 수 있는 경우
    else if (!isDigged && mountain[nextRow][nextCol] - k < curHeight)
    {
      maxPath = max(
          maxPath,
          1 + dfs(
                  nextRow,
                  nextCol,
                  curHeight - 1,
                  mountain,
                  visited,
                  true));
    }
  }
/* 왜 curHeight - 1 인지 이해가 안될 수 있어 추가 작성
// 다음 칸을 현재 높이보다 딱 1 낮은 높이(curHeight - 1)까지 필요한 만큼만 깎는다.
// curHeight - 1은 '1만큼 깎는다'는 의미가 아니라 공사 후 다음 칸의 최종 높이를 의미한다.
// 이동 조건은 반드시 현재 높이 > 다음 높이여야 하므로, 가능한 높이 중 가장 높은 값인 curHeight - 1로 설정한다.
// 필요 이상으로 낮게 깎으면 이후 더 낮은 지형으로 이어질 수 있는 경로가 줄어들 수 있으므로 최소한으로만 깎는다.
*/
  visited[row][col] = false; // dfs 완료 이후 visited 초기화

  return maxPath;
}

// dfs, 백트레킹, 구현
int main(int argc, char **argv)
{
  int test_case;
  int T;

  // freopen("input.txt", "r", stdin);
  cin >> T;
  for (test_case = 1; test_case <= T; ++test_case)
  {
    len = 0;
    k = 0;
    cin >> len >> k;

    int maxPath = 1;
    vector<vector<int>> mountain(len, vector<int>(len, 0));

    // 최고 위치 저장용
    vector<pair<int, int>> topPos;
    int top = 0; // 최고 위치 기록

    for (int row = 0; row < len; ++row)
    {
      for (int col = 0; col < len; ++col)
      {
        cin >> mountain[row][col];

        // 더 높은 위치가 있다면, 갱신 + 배열 초기화 후 삽입
        if (top < mountain[row][col])
        {
          top = mountain[row][col];
          topPos.clear();
          topPos.push_back({row, col});
        }
        else if (top == mountain[row][col])
        {
          topPos.push_back({row, col});
        }
      }
    }

    vector<vector<bool>> visited(len, vector<bool>(len, false));

    for (int i = 0; i < topPos.size(); ++i)
    {
      int startRow = topPos[i].first;
      int startCol = topPos[i].second;

      // 아직 안깎았을 때,
      int tmp = dfs(
          startRow, 
          startCol, 
          mountain[startRow][startCol],   // 현 높이
          mountain, // 높이 참조를 위한 배열
          visited,  // dfs를 위한 visited 배열
          false);

      maxPath = max(maxPath, tmp);
    }

    cout << "#" << test_case << " " << maxPath << '\n';
  }

  return 0; // 정상종료시 반드시 0을 리턴해야합니다.
}