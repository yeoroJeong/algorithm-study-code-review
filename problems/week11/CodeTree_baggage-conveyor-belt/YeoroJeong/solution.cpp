// 문제: CodeTree 수하물 컨베이어 벨트
// 링크: https://www.codetree.ai/ko/frequent-problems/samsung-sw/problems/baggage-conveyor-belt/description
// 작성자: 정현수
// 알고리즘: 구현, 시뮬레이션, 우선순위 큐, 재귀
// 핵심 아이디어: 빈 칸부터 우선순위에 따라 연쇄 이동하고, 버전으로 오래된 후보를 제거합니다.
// 복잡도: 전체 시각 수를 T, 수하물 수를 B라 하면 매 시각 N^2칸을 순회하고
//         수하물 후보를 힙에 등록합니다. 지연 삭제된 후보가 큐에 남을 수 있습니다.
// 다시 볼 포인트: 연쇄 이동 순서, 후보 버전 관리, 실패 횟수 유지 및 초기화

#include <iostream>
#include<queue>
#include<unordered_set>
using namespace std;

// 수하물의 투입 시각, 무게, 현재 위치, 입력 순서와 이동 후보 등록 버전입니다.
struct Product
{
	int t = -1;
	int weight;
	int cur_r;
	int cur_c;
	int idx;
	int version;
};
// 같은 칸의 투입 대기열은 투입 시각이 빠른 수하물부터 꺼냅니다.
struct Comparein {
	const bool operator()(const Product& a, const Product& b) {
		return a.t > b.t;
	}
};
// 이동 경쟁에서는 무거운 수하물, 입력 번호가 작은 수하물 순으로 우선합니다.
struct Comparemove {
	const bool operator()(const Product& a, const Product& b) {
		if (a.weight != b.weight)
			return a.weight < b.weight;
		return a.idx > b.idx;
	}
};
// 두 수하물의 시각·위치·번호·무게가 같은지 비교하는 보조 함수입니다.
bool comp_p(Product& a, Product& b) {
	if (a.t != b.t) return false;
	if (a.cur_c != b.cur_c) return false;
	if (a.cur_r != b.cur_r) return false;
	if (a.idx != b.idx) return false;
	if (a.weight != b.weight)return false;
	return true;
}
int board[9][9];
// 벨트 방향: 1=아래, 2=왼쪽, 3=위, 4=오른쪽입니다.
int dr[5] = { 0,1,0,-1,0 };
int dc[5] = { 0,0,-1,0,1 };
// 칸별 투입 대기열과 해당 칸으로 이동하려는 후보의 우선순위 큐입니다.
priority_queue<Product, vector<Product>, Comparein> in_prod[9][9];
priority_queue<Product, vector<Product>, Comparemove> comp_prod[9][9];
// t == -1이면 빈 칸입니다.
Product cur_prod[9][9];
// 이동 후보 등록 시 추가하고, 이동에 성공하면 제거하여 실패한 수하물을 남깁니다.
unordered_set<int>fail_list;
vector<int>result(30);
vector<int>fail_cnt(30);
int total;
// 오래된 이동 후보를 큐에서 지연 삭제하기 위한 수하물별 최신 버전입니다.
vector<int>version(30);
// 빈 칸으로 우선순위가 가장 높은 수하물을 옮긴 뒤, 비워진 이전 칸을 재귀 처리합니다.
void move_chain(int x, int y, int cur_t) {
	

	
	Product temp;
	
	// 최신 버전과 다른 후보는 이전 시각에 등록된 정보이므로 제거합니다.
	while (!comp_prod[x][y].empty()) {
		temp = comp_prod[x][y].top();
		int cur_t = temp.version;
		int cur_idx = temp.idx;
		if (version[cur_idx] == cur_t) break;
		comp_prod[x][y].pop();
	}
	// 유효한 후보가 없거나 목적지가 이미 차 있으면 이동할 수 없습니다.
	if (comp_prod[x][y].empty()||cur_prod[x][y].t!=-1) return;
	int before_r = temp.cur_r;
	int before_c = temp.cur_c;
	int cur_idx = temp.idx;
	temp.cur_r = x;
	temp.cur_c = y;

	comp_prod[x][y].pop();
	fail_list.erase(cur_idx);
	// 3회 미만의 연속 실패는 성공 시 초기화하고, 3회 이상 기록은 유지합니다.
	fail_cnt[cur_idx] = (fail_cnt[cur_idx] >= 3) ? fail_cnt[cur_idx] : 0;

	cur_prod[x][y] = temp;
	// 출구에 도착하면 결과 시각을 기록하고 남은 수하물 수를 줄입니다.
	if (board[x][y] == 9) {
		cur_prod[x][y].t = -1;
		result[cur_idx] = (fail_cnt[cur_idx] < 3) ? cur_t : (cur_t * -1);
		total--;
	}

	// 출발 칸을 비워 이 칸으로 들어오려던 수하물의 연쇄 이동을 처리합니다.
	cur_prod[before_r][before_c].t = -1;


	move_chain(before_r, before_c, cur_t);
	return;
}

int main() {
	// Please write your code here.
	int N;
	cin >> N;
	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= N; j++) {
			cin >> board[i][j];
		}
	}
	int B;
	cin >> B;
	total = B;

	// T는 현재 시뮬레이션 시각입니다. 수하물 번호는 입력 순서로 부여합니다.
	int T = 1;
	for (int i = 0; i < B; i++) {
		int t, r, c, w;
		cin >> t >> r >> c >> w;
		in_prod[r][c].push({ t,w,r,c,i,T });
	}
	while (true) {
		if (total == 0) break;
		// 1. 현재 벨트 위 수하물을 다음 칸의 이동 경쟁 큐에 등록합니다.
		for (int i = 1; i <= N; i++) {
			for (int j = 1; j <= N; j++) {
				if (cur_prod[i][j].t != -1) {
					int nxt_r = i + dr[board[i][j]];
					int nxt_c = j + dc[board[i][j]];
					cur_prod[i][j].version = T;

					comp_prod[nxt_r][nxt_c].push(cur_prod[i][j]);
					fail_list.insert(cur_prod[i][j].idx);
					version[cur_prod[i][j].idx] = T;
				}
			}
		}
		for (int i = 1; i <= N; i++) {
			for (int j = 1; j <= N; j++) {
				// 2. 빈 칸에서 시작하여 이동 가능한 수하물을 연쇄적으로 옮깁니다.
				if (cur_prod[i][j].t == -1) {
					move_chain(i, j, T);
				}
			}
		}

		// 3. 이번 이동 단계에서 남은 수하물의 실패 횟수를 증가시킵니다.
		for (auto a : fail_list) {
			fail_cnt[a]++;
		}


		for (int i = 1; i <= N; i++) {
			for (int j = 1; j <= N; j++) {
				// 4. 이동 처리 후 빈 칸에 투입 시각이 된 수하물을 하나 올립니다.
				if (cur_prod[i][j].t == -1 && !in_prod[i][j].empty() && T >= in_prod[i][j].top().t) {
					Product temp = in_prod[i][j].top();
					in_prod[i][j].pop();
					cur_prod[i][j] = temp;
				}
			}
		}
		T++;
	}
	// 입력 순서대로 출구 도착 결과를 출력합니다.
	for (int i = 0; i < B; i++) {
		cout << result[i] << " ";
	}
	return 0;
}
