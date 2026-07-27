#include<iostream>
#include<vector>

using namespace std;

int main(int argc, char** argv)
{
	int test_case;
	int T = 10;
	
	
	for(test_case = 1; test_case <= T; ++test_case)
	{
        int test_case;
        cin >> test_case;
        // 100 by 100
        vector<vector<int>> board (100, vector<int> (100, 0));
        long long maxNum = 0;
        // 입력 + 행 계산
        for(int row=0; row<100; ++row)
        {
            long long tmpRow = 0;
            for(int col=0; col<100; ++col)
            {
                cin >> board[row][col];
                tmpRow += board[row][col];
            }
            maxNum = max(maxNum, tmpRow);
        }

        // 열
        for(int col = 0; col<100; ++col)
        {
            long long tmp = 0;
            for(int row=0; row<100; ++row)
            {
                tmp+=board[row][col];
            }
            maxNum = max(maxNum, tmp);
        }
        

        // 좌 -> 우 대각 
        long long leftRightMax = 0;
        // 우 -> 좌 대각
        long long rightLeftMax = 0;
        for(int curIdx = 0; curIdx<100; ++curIdx)
        {
            leftRightMax += board[curIdx][curIdx];

            rightLeftMax += board[curIdx][99-curIdx];
        }
        maxNum = max(maxNum, leftRightMax);
        maxNum = max(maxNum, rightLeftMax);


        cout << "#" << test_case << " " << maxNum << "\n";
	}
	return 0;//정상종료시 반드시 0을 리턴해야합니다.
}