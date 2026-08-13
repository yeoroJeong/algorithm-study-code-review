// 4008. [모의 SW 역량테스트] 숫자 만들기
// https://swexpertacademy.com/main/code/problem/problemDetail.do?contestProbId=AWJRxtsKDKIDFAXc
#include <iostream>
#include <vector>
#include <climits>
using namespace std;

int numSize;
int opts[4]; // operators
vector<int> numbers;
int optTotalCnt = 0;

int minResult = INT_MAX;
int maxResult = INT_MIN;

int calculate(int num1, int num2, int opIdx)
{
	switch (opIdx)
	{
	case 0:
		return num1 + num2;
	case 1:
		return num1 - num2;
	case 2:
		return num1 * num2;
	case 3:
		if (num2 != 0)
			return num1 / num2;
	default:
		break;
	}

}
void search(int opCnt, int curIdx, int result)
{
	if (opCnt >= optTotalCnt)
	{
		// update global result
		if (result > maxResult)
		{
			maxResult = result;
		}
		if (result < minResult)
		{
			minResult = result;
		}
		return;
	}

	int nextNum = numbers[curIdx];

	for (int i = 0; i < 4; i++)
	{
		if (opts[i] > 0)
		{
			int nextResult = calculate(result, nextNum, i);
			opts[i]--;
			search(opCnt + 1, curIdx + 1, nextResult);
			opts[i]++;
		}
	}
}


int main() {
	ios::sync_with_stdio(false);
	cin.tie(NULL);

	int T;
	cin >> T;
	for (int tc = 0; tc < T; tc++)
	{
		cin >> numSize;
		numbers.resize(numSize);
		optTotalCnt = 0;
		minResult = INT_MAX;
		maxResult = INT_MIN;

		for (int i = 0; i < 4; i++)
		{
			cin >> opts[i];
			optTotalCnt += opts[i];
		}
		for (int i = 0; i < numSize; i++)
		{
			cin >> numbers[i];
		}
		// 첫 숫자와 숫자 접근용 index 잘 넣어줄 것
		// 초기 result에 첫 숫자 넣어주고 함수 안에서 첫 연산자와 같이 연산할 숫자 찾을 것
		search(0, 1, numbers[0]);
		// DEBUG
		//cout << maxResult << " " << minResult << "\n";
		cout << "#" << tc + 1 << " " << maxResult - minResult << "\n";
	}
	return 0;
}