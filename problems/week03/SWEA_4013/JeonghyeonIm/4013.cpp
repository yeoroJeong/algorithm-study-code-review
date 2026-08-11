#include<iostream>
#include<deque>
#include<cmath>
using namespace std;

int main() {
	int T;
	cin >> T;
	for (int t = 1; t <= T; t++) {
		int K;
		cin >> K;
		deque<int> mag[5];
		int input;
		for (int m = 1; m <= 4; m++) {
			for (int i = 0; i < 8; i++) {
				cin >> input;
				mag[m].push_back(input);
			}
		}
		// 자석 정보 입력 끝
		while (K--) {
			int magnum, turn;
			cin >> magnum >> turn;
			int rt[5] = { 0, }; // rotate 줄임말
			// 한번에 회전해야함. => 회전 정보 기억해서 한번에
			// int 선언해서 0이면 회전x, 1이면 시계, -1이면 반시계
			// idx 기준
			// 1의 2랑 2의 6 맞닿아 있음
			//	2의 2랑 3의 6 맞닿아 있음
			//	3의 2랑 4의 6 맞닿아 있음
			// 즉 오른쪽 전파시
			rt[magnum] = turn;
			for (int i = magnum; i < 4; i++) {
				if (mag[i][2] != mag[i + 1][6]) {
					rt[i + 1] = rt[i] * -1;
				}
				else {
					break;
				} // 한 번 끊기면 그 뒤로는 전파 안 됨
			}
			// 즉 왼쪽으로 전파시
			for (int i = magnum; i > 1; i--) {
				if (mag[i][6] != mag[i - 1][2]) {
					rt[i - 1] = rt[i] * -1;
				}
				else {
					break;
				} // 한 번 끊기면 그 뒤로는 전파 안 됨
			}
			
			// 각 자석 회전 정보 입력 완료
			for (int i = 1; i <= 4; i++) {
				if (rt[i] == 1) { // 시계 방향
					int temp = mag[i].back();
					mag[i].pop_back();
					mag[i].push_front(temp);
				}
				else if (rt[i] == -1) { // 반시계 방향
					int temp = mag[i].front();
					mag[i].pop_front();
					mag[i].push_back(temp);
				}
			}
			// 회전 끝
		}
		int sum = 0;
		for (int i = 1; i <= 4; i++) { // 점수계산 1번자석부터 4번자석까지
			// 각각 2의 0승, 1승, 2승, 3승인것 확인 pow함수 사용
			if (mag[i][0] == 1) {
				sum += pow(2, i - 1);
			}
		}
		cout << "#" << t << " " << sum << "\n";
	}
	return 0;
}