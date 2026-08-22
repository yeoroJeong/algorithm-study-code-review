#include<iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <unordered_set>
#include <cmath>


using namespace std;

// 16진수를 10진수로 변환할 때 사용
map<char, int> alpha = {
    {'A',10}, {'B', 11}, {'C', 12} , {'D', 13}, {'E', 14}, {'F', 15}
};

int toDecimal(string s)
{
    int result = 0;
    int cur = 0;
    // s의 뒤에서부터 시작
    for(int i = s.size()-1; i>=0; --i)
    {
        // 알파벳이면, 
        if(isalpha(s[i]))
            result += (pow(16, cur) * alpha[s[i]]);
        else
            result += (pow(16, cur) * (s[i] - '0'));
        cur++; 
    }    
    return result;
};

// 내림차순 정렬
bool compare(string a, string b)
{
    // 만들어지는 크기는 모두 같으므로 자리마다 비교
    for(int i=0; i<a.size(); ++i)
    {
        if(a[i] == b[i])
        {
            continue;
        }
        else if( a[i] > b[i])
            return true;
        else
            return false;            
    }
};

int main(int argc, char** argv)
{
	int test_case;
	int T;

	cin>>T;
	/*
	   여러 개의 테스트 케이스가 주어지므로, 각각을 처리합니다.
	*/
	for(test_case = 1; test_case <= T; ++test_case)
	{
        int n, k;
        cin >> n >> k;

        // 모든 수를 만들기 위해 회전하는 횟수
        int rotateNum = n/4;
        
        // 중복을 제거하기 위해 unorderd_set 사용
        unordered_set<string> availNums;
        string origin = "";

        cin >> origin;
        string curString = origin;
        string nextString = origin;
        for(int rotate = 0; rotate < rotateNum; ++rotate)
        {
            // 회전시키기
            for(int idx = 0; idx <(int)origin.size(); ++idx)
            {
                int nextIdx = idx+1;
                if(nextIdx >= origin.size())
                    nextIdx = 0;    // 맨 끝 idx에 대한 처리

                nextString[nextIdx] = curString[idx];
            }

            // n/4개씩 슬라이싱 해서 가능한 수를 set에 추가
            for(int i=0; i<n; i+=(n/4))
            {
                string tmp= "";
                for(int j=i; j<(n/4)+i; ++j)
                {
                    tmp+= nextString[j];
                }
                availNums.insert(tmp);
            }

            // 다음 1회전 완료 후, 2회전을 위한 초기화
            curString = nextString;
        }

        vector<string> nums;
        for(string s: availNums)
        {
            nums.push_back(s);
        }

        // 크기 순 정렬
        sort(nums.begin(), nums.end(), compare);

        int answer = toDecimal(nums[k-1]);
        cout << "#" << test_case << " " << answer << "\n";
	}
	return 0;//정상종료시 반드시 0을 리턴해야합니다.
}