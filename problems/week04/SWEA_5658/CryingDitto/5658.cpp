// 5658. [모의 SW 역량테스트] 보물상자 비밀번호
// https://swexpertacademy.com/main/code/problem/problemDetail.do?contestProbId=AWXRUN9KfZ8DFAUo&&

#include <iostream>
#include <string>
#include <set>
#include <iterator>
using namespace std;

int numCnt, slotSize, Kth;
string numStr;
set<string> numSet;

long long translateHexToDecimal(string& s)
{
	long long result = 0;
	for (int i = 0; i < s.length(); i++)
	{
		result *= 16; // ABC => A*16*16 + B*16 + C*1 = ((0*16+A)*16 + B)*16 + C)
		if (s[i] >= 'A' && s[i] <= 'F')
		{
			// s[i] - 'A' : diff between s[i] and A, which means the amount of value over ten
			// s[i] - 'A' + 10: restore s[i] to number
			result += s[i] - 'A' + 10;
		}
		else
		{
			result += s[i] - '0';
		}
	}
	return result;
}


int main()
{
	ios::sync_with_stdio(false);
	cin.tie(0);

	int T;
	cin >> T;

	for (int tc = 0; tc < T; tc++)
	{
		cin >> numCnt >> Kth;
		slotSize = numCnt / 4; // 4의 배수이므로 4로 나누면 한 변에 숫자 몇 개인지 알 수 있음
		numSet.clear();

		cin >> numStr;
		// push numbers in set

		for (int i = 0; i < slotSize; i++)
		{
			int readStart = (numCnt - i) % numCnt;// *slotSize;
			for (int j = 0; j < 4; j++)
			{
				// prevent out of range
				int start = (readStart + slotSize * j) % numCnt;

				string sub = numStr.substr(start, slotSize);
				if (sub.length() < slotSize) // start + slotSize >= numCnt
				{
					int part1 = numCnt - start;
					int part2 = slotSize - part1;
					sub = numStr.substr(start, part1);
					sub += numStr.substr(0, part2);
				}
				// push number to set
				auto check = numSet.insert(sub);
				// ======= DEBUG =======
				/*if (!check.second)
				{
					cout << "value " << sub << "duplicates";
				}*/
				// ======= DEBUG =======
			}
		}
		// Get result to print.
		auto iter = next(numSet.rbegin(), Kth - 1);
		string strVal = *iter;
		//cout << strVal << "\n";

		long long result = translateHexToDecimal(strVal);
		cout << "#" << tc + 1 << " " << result << "\n";
	}

	return 0;
}