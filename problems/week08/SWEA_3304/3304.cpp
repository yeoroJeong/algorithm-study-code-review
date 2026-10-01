#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

int main()
{
	int T;
	cin >> T;

	for (int tc = 1; tc <= T; tc++) {
		string a, b;
		cin >> a >> b;

		int len1 = a.length(); // 첫 번째 문자열 길이
		int len2 = b.length(); // 두 번째 문자열 길이

		vector<vector<int>> dp(len1 + 1, vector<int>(len2 + 1, 0));
		// dp 점화식
		// dp[i][j] = 첫번째 문자열의 i - 1번째 문자열, 두번째 문자열의 j - 1번째 문자열까지 봤을때 최대길이

		for (int i = 1; i <= len1; i++) {
			for (int j = 1; j <= len2; j++) {
				if (a[i - 1] == b[j - 1]) {
					dp[i][j] = dp[i - 1][j - 1] + 1;
				}
				else {
					dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
				}
			}
		}


		cout << "#" << tc << " " << dp[len1][len2] << "\n";
	}

	return 0;
}