#include <iostream>
#include<vector>
using namespace std;
int main()
{
	for (int t = 1; t <= 10; t++) {
		int T;
		cin >> T;
		
		vector<vector<int>> arr(100, vector<int>(100, 0));
		for (int i = 0; i < 100; i++) {
			for (int j = 0; j < 100; j++) {
				cin >> arr[i][j];
			}
		}

		int mx = 0;
		for (int i = 0; i < 100; i++) {
			int sum1 = 0; // 각 행의 합
			int sum2 = 0; // 각 열의 합
			for (int j = 0; j < 100; j++) {
				sum1 += arr[i][j];
				sum2 += arr[j][i];
			}
			if (mx < sum1) {
				mx = sum1;
			}
			if (mx < sum2) {
				mx = sum2;
			}
		}

		int sum1 = 0; // '/'방향 대각선
		int sum2 = 0; // '\'방향 대각선

		for (int i = 0; i < 100; i++) {
			sum1 += arr[i][i];
			sum2 += arr[99 - i][i];
		}
		if (mx < sum1) {
			mx = sum1;
		}
		if (mx < sum2) {
			mx = sum2;
		}

		cout << "#" << T << " " << mx << "\n";
	}
	return 0;
}