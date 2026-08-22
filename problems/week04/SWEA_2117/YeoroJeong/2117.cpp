#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <set>
#include <climits>
#include <array>
#include <cstring>
#include <queue>
#include <deque>

using namespace std;

int answer;
int N, M;

vector<vector<int>> board;

// service_range[i] : 서비스 범위 i에서 포함할 수 있는 최대 집의 개수
vector<int> service_range;


/*
 * 서비스 영역의 운영 비용 계산
 *
 * k가 0부터 시작하기 때문에
 * 실제 서비스 영역 K = k + 1에 해당
 *
 * 비용 = K^2 + (K - 1)^2
 */
int secu_cost(int k) {
	return (k + 1) * (k + 1) + k * k;
}


/*
 * 집의 좌표를 저장하기 위한 구조체
 */
struct Node
{
	int cur_x;
	int cur_y;
};


int main()
{
	int T;
	cin >> T;

	for (int test_case = 1; test_case <= T; test_case++)
	{
		cin >> N >> M;

		answer = 0;

		board.resize(N, vector<int>(N));

		// 서비스 범위별 최대 집 개수를 저장
		service_range.resize(2 * N, -1);

		// 집이 존재하는 좌표만 별도로 저장
		vector<Node> home;


		/*
		 * 도시 정보 입력
		 * 집이 있는 위치는 home에 저장
		 */
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {

				cin >> board[i][j];

				if (board[i][j] != 0) {
					home.push_back({ i, j });
				}
			}
		}


		/*
		 * 모든 좌표 (i, j)를
		 * 방범 서비스의 중심으로 설정하여 탐색
		 */
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {

				/*
				 * 현재 중심에서 서비스 범위별로
				 * 포함되는 집의 개수를 저장
				 */
				vector<int> temp_range(2 * N);


				/*
				 * 현재 중심과 모든 집 사이의
				 * 맨해튼 거리 계산
				 */
				for (int k = 0; k < home.size(); k++) {

					int curx = home[k].cur_x;
					int cury = home[k].cur_y;

					int distance = abs(i - curx) + abs(j - cury);


					/*
					 * 해당 집은 distance 이상의 서비스 범위에서
					 * 모두 포함될 수 있으므로 집 개수 증가
					 */
					for (int idx_range = distance;
						idx_range < 2 * N;
						idx_range++) {

						temp_range[idx_range]++;
					}
				}


				/*
				 * 현재 중심에서 구한 결과를 이용해
				 * 서비스 범위별 최대 집 개수 갱신
				 */
				for (int idx = 0; idx < temp_range.size(); idx++) {

					if (temp_range[idx] != 0) {
						service_range[idx] =
							max(service_range[idx], temp_range[idx]);
					}
				}


				temp_range.clear();
			}
		}


		/*
		 * 각 서비스 범위에서 얻는 수익과
		 * 운영 비용을 비교
		 */
		for (int i = 0; i < 2 * N; i++) {

			// 해당 범위에서 서비스할 수 있는 최대 집 개수
			int cnt_house = service_range[i];

			// 방범 서비스 운영 비용
			int cost = secu_cost(i);

			// 집들로부터 얻는 수익
			int fee = M * cnt_house;


			// 손해가 발생하지 않는 경우 정답 갱신
			if (fee >= cost) {
				answer = max(answer, cnt_house);
			}
		}


		cout << "#" << test_case << " " << answer << "\n";


		// 다음 테스트 케이스를 위해 초기화
		board.clear();
		service_range.clear();
		home.clear();
	}

	return 0;
}