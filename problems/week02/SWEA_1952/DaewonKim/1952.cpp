#include<bits/stdc++.h>
using namespace std;

// AI 사용
/*
입력
- 첫줄 이용권 가격   (하루, 한달, 3달(연속), 1년)
- 2번째 줄          월별 이용계획 (단위: 하루, 12개의 입력)


*/
int main(int argc, char** argv)
{
	int test_case;
	int T;
	//freopen("input.txt", "r", stdin);
	cin>>T;

	for(test_case = 1; test_case <= T; ++test_case)
	{
    int answer = 0;
    int oneDay, oneMonth, threeMonth, oneYear = 0;
    cin >> oneDay >> oneMonth >> threeMonth >> oneYear;
      
    vector<int> plans(13, 0);
    for(int i=1; i<13; ++i)
    {
      cin >> plans[i]; 
    }

    vector<int> dp (13, 100000); // 무작위의 큰 값으로 지정
    dp[0] = 0;  // 그 전까지는 0

    for(int i=1; i<13; ++i)
    {
      /* 
      선택지 A dp[i-1] + plans[i]*oneDay
      - i-1월까지는 이미 최적으로 해결됐다고 가정(dp[i-1])하고, 
      i월은 하루짜리 이용권을 plans[i]번 사서 해결. 
      (plans[i]가 0이면 이 항은 그냥 dp[i-1]과 같아집니다 - 안 가는 달이니 추가 비용 없음)
      */
        dp[i] = dp[i-1] + plans[i] * oneDay;

        /*
        선택지 B dp[i-1] + oneMonth
        - i-1월까지는 해결된 상태에서, i월은 1달 이용권 하나로 통째로 커버.
        */
        dp[i] = min(dp[i], dp[i-1]+oneMonth);

        /*
        선택지 C (i>=3일 때만) dp[i-3] + threeMonth
        - i-3월까지는 해결된 상태에서, (i-2, i-1, i) 이렇게 연속된 3개월을 3달 이용권 하나로 한 번에 커버. 
        i-2월과 i-1월의 개별 계획을 따로 신경 쓸 필요가 없는 이유는, 3달 이용권이 그 구간 전체를 이미 다 포함하기 때문입니다.
        */
        if(i >=3)
        {
          dp[i] = min(dp[i], dp[i-3] + threeMonth); 
        }

       // 이 세 값 중 가장 작은 걸 dp[i]로 확정 짓고, 다음 달(i+1)로 넘어가는 구조 
    }

    answer = min(dp[12] , oneYear);
    cout << "#" << test_case << " " << answer << "\n";
	}
	return 0;//정상종료시 반드시 0을 리턴해야합니다.
}