// 1954.달팽이 숫자
// https://swexpertacademy.com/main/code/problem/problemDetail.do?contestProbId=AV5PobmqAPoDFAUq
#include <iostream>
#include <vector>
using namespace std;

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(0);

    int T;
    cin >> T;
    for (int tc = 0; tc < T; tc++)
    {
        int size;
        cin >> size;
        vector<vector<int>> snail(size, vector<int>(size, 0));
        vector<vector<bool>> visited(size, vector<bool>(size, false));

        // 방향: 우 -> 하 -> 좌 -> 상
        int dIndex = 0;
        int dx[4] = { 0, 1, 0, -1 };
        int dy[4] = { 1, 0, -1, 0 };
        // 집어넣을 숫자
        int num = 1;
        // 배열 index
        int x = 0; int y = 0;
        int nx = 0; int ny = 0;

        // 근데 i, j를 안 쓰고 N도 작아서 굳이 이중for문 쓸 거 없이 그냥 size*size loop 돌려도 될 거 같네요.
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++) {
                snail[x][y] = num;
                visited[x][y] = true;

                nx = x + dx[dIndex];
                ny = y + dy[dIndex];

                // 가능한 index 범위 벗어난 경우
                if (nx < 0 || nx >= size || ny < 0 || ny >= size)
                {
                    // 방향 전환
                    dIndex = (dIndex + 1) % 4;
                }
                else if (visited[nx][ny]) {
                    // index 범위에 해당하지만 이미 visit한 경우 방향 꺾어야 함
                    dIndex = (dIndex + 1) % 4;
                }
                x = x + dx[dIndex];
                y = y + dy[dIndex];
                num++;
            }
        }

        // 정답 배열 출력
        cout << "#" << tc + 1 << "\n";
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++) {
                cout << snail[i][j] << " ";
            }
            cout << "\n";
        }
    }

    return 0;
}