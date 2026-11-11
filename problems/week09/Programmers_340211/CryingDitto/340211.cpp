
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

// 현재 퍼즐의 난이도를 diff, 당신의 숙련도를 level
	// 1 ≤ diffs의 길이 = times의 길이 = n ≤ 300, 000
// diff가 될 수 있는 숫자의 범위
	// 1 ≤ diffs[i] ≤ 100,000
	// level의 범위로 봐도 될 것

// 현재 퍼즐의 소요 시간을 time_cur 
// 이전 퍼즐의 소요 시간을 time_prev
// 전체 제한 시간 limit
// 제한 시간 내에 퍼즐을 모두 해결하기 위한 숙련도의 최솟값 구하기

// case1 : diff ≤ level
	// 퍼즐을 틀리지 않고 time_cur만큼의 시간을 사용하여 해결합니다.
	// time_cur
// case2 : diff > level이면, 퍼즐을 총 diff - level번 틀립니다.
	// 퍼즐을 틀릴 때마다, time_cur + time_prev만큼의 시간을 사용
	// diff - level번 틀린 이후에 다시 퍼즐을 풀면 time_cur만큼의 시간을 사용하여 퍼즐을 해결합니다.
	// (diff-level) * (time_cur + time_prev) + time_cur


int solution(vector<int> diffs, vector<int> times, long long limit) {
	int answer = 0;

	// level 범위 이분 탐색
	// 초기 left 1, 초기 right diff들의 최댓값
	int left = 1;
	int right = *max_element(diffs.begin(), diffs.end());
	long long minTime = 0;
	int prevMid = 0;
	while (left < right)
	{
		int mid = left + (right - left) / 2;
		long long temp = 0;

		for (int i = 0; i < diffs.size(); i++)
		{
			int diff = diffs[i];
			int timeCur = times[i];
			if (mid >= diff)
			{
				temp += (long long)timeCur;
			}
			else
			{
				int timePrev = 0;
				if (i >= 1) timePrev = times[i - 1];
				temp += ((long long)(diff - mid) * (long long)(timeCur + timePrev)) + timeCur;
			}
		}

		if (temp > limit)
		{
			// level이 더 커져야 limit 안에 퍼즐 풀이 가능
			left = mid + 1;
		}
		else
		{
			// 일단 limit 안에 퍼즐 풀이 가능
			// 더 작은 level로도 퍼즐 풀이 가능한지 찾기 위해 왼쪽 탐색할 것
			right = mid;
		}
	}
	// left가 right보다 커졌다면 right가 조건을 만족하는 최소의 level임
	answer = right;
	return answer;
}