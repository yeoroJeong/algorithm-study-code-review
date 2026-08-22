#include<iostream>
#include<string>
#include<unordered_set>
#include<algorithm>
#include<vector>

using namespace std;

/* 아이디어
unordered_set사용
1. N/4번 회전시 0회전과 같음
2. 회전하고 N/4개씩 끊어서 unordered_set에 넣음(단 넣을때 이미 있으면 안 넣음)
3. N/4 - 1번 회전 후 확인 후 10진수로 바꾸고 정렬하여 k번째 get하면 끝
*/
// rotate(s.begin(), s.end() - 1, s.end());

bool cmp(int a, int b) {
	return a > b;
}

int main() {
	int T;
	cin >> T;
	for (int t = 1; t <= T; t++) {
		int N, K;
		cin >> N >> K;
		string input;
		cin >> input;
		// 입력 끝
		unordered_set<string> num;
		for (int r = 0; r < N - 1; r++) {
			// 문자열 빼내기
			for (int s = 0; s < N; s += (N / 4)) {
				string temp = input.substr(s, N / 4);
				if (num.count(temp)) { // 이미 본 수면 패스
					continue;
				}
				else { // 처음 본 수면 넣어
					num.insert(temp);
				}
			}
			rotate(input.begin(), input.end() - 1, input.end());
			// 맨 뒤에껄 맨 앞으로 보냄 => 시계방향 회전
		}
		vector<int> nums;
		for (auto s : num) { // unordered_set에서 하나씩 빼서 10진수로 변환해서 nums vector에 넣어
			nums.push_back(stoi(s, nullptr, 16));
		}
		sort(nums.begin(), nums.end(), cmp); // 내림차순 정렬해서

		
		cout << "#" << t << " " << nums[K - 1] << "\n"; // K번째로 큰 수 출력
	}
	return 0;
}