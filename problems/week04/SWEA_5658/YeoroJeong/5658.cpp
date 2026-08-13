#include <iostream>
#include <deque>
#include <vector>
#include <string>
#include <algorithm>
#include <set>

using namespace std;

int N, K;
int answer;

// 보물상자의 현재 숫자 배치
deque<char> locker;

// 만들어진 숫자를 중복 제거 + 내림차순으로 저장
set<int, greater<int>> nums;


/*
 * 현재 locker에 저장된 숫자를 N/4개씩 나누어
 * 16진수 문자열 → 10진수 정수로 변환한 뒤 nums에 저장
 */
void casting_locker() {
	string temp = "";

	for (int i = 0; i < locker.size(); i++) {

		// 한 변에 해당하는 N/4개의 숫자를 모두 모았다면 변환
		if (i % (N / 4) == 0 && i != 0) {
			int temp_val = stoi(temp, nullptr, 16);

			temp = "";
			nums.insert(temp_val);
		}

		// 현재 16진수 문자 추가
		temp += locker[i];
	}

	// 반복문이 끝난 뒤 남아있는 마지막 한 변 처리
	int temp_val = stoi(temp, nullptr, 16);
	temp = "";
	nums.insert(temp_val);
}


/*
 * 보물상자를 회전시키면서
 * 각 회전 상태에서 만들 수 있는 모든 숫자를 저장
 */
void solv() {

	// 회전하기 전 초기 상태 처리
	casting_locker();

	/*
	 * 한 변의 길이가 N/4이므로
	 * N/4번 회전하면 다시 기존 조합이 반복된다.
	 *
	 * 초기 상태는 이미 처리했기 때문에
	 * 추가로 N/4 - 1번만 회전
	 */
	for (int i = 1; i < N / 4; i++) {

		// 가장 뒤의 숫자를 앞으로 이동시켜 한 칸 회전
		int temp = locker.back();
		locker.pop_back();
		locker.push_front(temp);

		// 회전된 상태에서 만들어지는 숫자 저장
		casting_locker();
	}
}


int main()
{
	int T;
	cin >> T;

	for (int test_case = 1; test_case <= T; test_case++)
	{
		cin >> N >> K;

		answer = 0;

		// 보물상자의 숫자를 순서대로 저장
		for (int i = 0; i < N; i++) {
			char num;
			cin >> num;

			locker.push_back(num);
		}

		// 가능한 모든 회전 상태 탐색
		solv();

		/*
		 * nums는 greater<int> 기준으로 정렬되어 있으므로
		 * vector로 변환하면 큰 숫자부터 저장된다.
		 */
		vector<int> pwd(nums.begin(), nums.end());

		// K번째로 큰 숫자 선택
		answer = pwd[K - 1];

		cout << "#" << test_case << " " << answer << "\n";

		// 다음 테스트 케이스를 위해 초기화
		locker.clear();
		pwd.clear();
		nums.clear();
	}

	return 0;
}