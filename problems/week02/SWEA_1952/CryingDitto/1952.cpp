// 1952. [모의 SW 역량테스트] 수영장
// https://swexpertacademy.com/main/code/problem/problemDetail.do?contestProbId=AV5PpFQaAQMDFAUq

#include <iostream>
#include <algorithm>

using namespace std;

int prices[4];// daily, monthly, 3 monthly, yearly prices
int plan[12]; // usage plan for each month
int calculate(int month)
{
    if (month >= 12)
        return 0;

    if (plan[month] == 0)
    {
        return calculate(month + 1);
    }
    // 1월 -> 2월 -> 3월 ... 12월, month가 13이상이 되어서야 리턴값 하나씩 돌아오기 시작
    int minVal = min({
        prices[0] * plan[month] + calculate(month + 1),
        prices[1] + calculate(month + 1),
        prices[2] + calculate(month + 3) // 3개월 동안 이용 가능하므로 세 달 스킵
        });
    return minVal;
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(0);
    int T = 0;
    cin >> T;
    for (int tc = 0; tc < T; tc++)
    {
        // input: prices
        for (int i = 0; i < 4; i++)
        {
            cin >> prices[i];
        }
        // input: plan
        for (int i = 0; i < 12; i++)
        {
            cin >> plan[i];
        }
        // compare the result of calculate(0) and yearly price
        int result = min(calculate(0), prices[3]);

        cout << "#" << tc + 1 << " " << result << "\n";
    }
    return 0;
}