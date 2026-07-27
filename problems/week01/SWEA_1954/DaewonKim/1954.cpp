#include<iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main(int argc, char** argv)
{
	int test_case;
	int T;
	
	cin>>T;
	for(test_case = 1; test_case <= T; ++test_case)
	{
		int N;
        cin >> N;
        
 		vector<vector<int>> snail(N, vector<int>(N, 0));

        // 오른쪽, 아래, 왼쪽, 위
        int dr[4] = {0, 1, 0, -1};   
        int dc[4] = {1, 0, -1, 0};

        int row = 0;
        int col = 0;
        int dir = 0;

        for (int num = 1; num <= N * N; num++)
        {
            snail[row][col] = num;

            int nextRow = row + dr[dir];
            int nextCol = col + dc[dir];

            if (nextRow < 0 || nextRow >= N ||
                nextCol < 0 || nextCol >= N ||
                snail[nextRow][nextCol] != 0)
            {
                dir = (dir + 1) % 4;
                nextRow = row + dr[dir];
                nextCol = col + dc[dir];
            }

            row = nextRow;
            col = nextCol;
        }

        cout << "#" << test_case << "\n";
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
            {
                cout << snail[i][j] << " ";
            }
            cout << "\n";
        }
    }
	return 0;//정상종료시 반드시 0을 리턴해야합니다.
}