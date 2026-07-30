/*

*/
#include <iostream>
#include <vector>

using namespace std;

int main(int argc, char** argv)
{
	int test_case;
	int T;
	
	cin>>T;

	for(test_case = 1; test_case <= T; ++test_case)
	{

        int n,m;
        cin >> n;
        cin >> m;

        vector<vector<int>> board(n, vector<int> (n,0));
        for(int row = 0; row<n; ++row)
        {
            for(int col = 0; col<n; ++col)
            {
                cin >> board[row][col];
            }
        }


        int maxFly = 0;
        // 크기가 m x m인 것을 만들기
        for(int row = 0; row<n; ++row)
        {
            for(int col = 0; col<n; ++col)
            {
                
                int endRow = row+(m-1); // 열(세로)
                int endCol = col+(m-1); // 행(가로)
                if(endRow < n && endCol < n ) // 끝이 범위 안이라면
                {
                    int tmpFly=0;
                    for(int curRow = row; curRow<=endRow; ++curRow)
                    {
                        for(int curCol = col; curCol<=endCol; ++curCol)
                        {
                            tmpFly += (board[curRow][curCol]);
                        }
                    }
                    maxFly = max(maxFly, tmpFly);
                }
                else{
                    continue;
                }


            }
        }

        cout << "#" << test_case << " " << maxFly << "\n";
	}
	return 0;//정상종료시 반드시 0을 리턴해야합니다.
}