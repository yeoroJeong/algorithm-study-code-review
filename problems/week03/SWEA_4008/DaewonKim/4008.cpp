/*
    조건
    1. 숫자 배열은 달라지지 않는다
    2. 연산자는 사용하면 1 감소

    출력
    - 최대값과 최소값의 차이
    
    접근법
    - dfs, backtracking
    

*/

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int maxNum;
int minNum;

void dfs(int curNumIdx, vector<int> nums, vector<int> oper, int result)
{
    // 종료조건
    if(curNumIdx >= nums.size())
    {
        maxNum = max(maxNum, result);
        minNum = min(minNum, result);
        return;
    }
    
    for(int i=0; i<4; ++i)
    {
        if(oper[i] > 0)
        {
            int nextResult = result;
            switch(i)
            {
                case 0: // +
                    nextResult += nums[curNumIdx];
                    break;
                case 1: // -
                    nextResult -= nums[curNumIdx];
                    break;
                case 2: // *
                    nextResult *= nums[curNumIdx];
                    break;
                case 3: // /
                    nextResult /= nums[curNumIdx];
                    break;
            }
            oper[i]--;
            dfs(curNumIdx + 1, nums, oper, nextResult);
            oper[i]++;
        }
    }
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

        maxNum = -1000000;
        minNum = 1000000;

        vector<int> nums (n,0);
        vector<int> oper (4,0); // 순서 + - * /


        // 연산자 
        for(int i = 0; i < 4; ++i)
        {
            cin >> oper[i];
        }

        // 숫자
        for(int i=0; i<n; ++i)
        {
            cin >> nums[i];
        }
        
        dfs(1, nums, oper, nums[0]);
        cout << "#" << test_case << " " << maxNum - minNum << "\n";



	}
	return 0;//정상종료시 반드시 0을 리턴해야합니다.
}