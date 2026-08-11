#include<iostream>
#include<vector>
using namespace std;

int N;
vector<int> num;
vector<int> oper(4);

int mx; // 최대값
int mn; // 최소값

void back(int cnt, int sum, int s) {
	// cnt는 연산횟수, sum은 현재 계산값, s는 몇번째 숫자랑 계산할지
	if (cnt == (N - 1)) { // 연산자 모두 사용할 경우
		if (sum < mn) {
			mn = sum;
		}
		if (sum > mx) {
			mx = sum;
		}
		return;
	}

	for (int i = 0; i < 4; i++) { 
		if (oper[i] > 0) {
			oper[i]--;
			int temp = sum;
			if (i == 0) {
				temp += num[s];
			}
			else if (i == 1) {
				temp -= num[s];
			}
			else if (i == 2) {
				temp *= num[s];
			}
			else if (i == 3) {
				temp /= num[s];
			}
			back(cnt + 1, temp, s + 1);
			oper[i]++;
		}
	}

}

int main() {
	int T;
	cin >> T;
	for (int t = 1; t <= T; t++) {
		cin >> N;
		num.assign(N, 0);
		for (int i = 0; i < 4; i++) {
			cin >> oper[i];
		}
		for (int i = 0; i < N; i++) {
			cin >> num[i];
		}
		// 입력 끝
		mx = -(1e9);
		mn = 1e9;
		// 최대값 최솟값 초기화
		back(0, num[0], 1); // 일단 sum에 num[0]값 넘겨

		cout << "#" << t << " " << (mx - mn) << "\n";
	}
	return 0;
}