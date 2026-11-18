// 수하물 컨베이어 벨트
// https://www.codetree.ai/ko/frequent-problems/samsung-sw/problems/baggage-conveyor-belt/description

// N×N 크기의 격자
	// 가장 왼쪽 위 칸이 (1,1), 
	// 가장 오른쪽 아래 칸이 (N,N)
// 각 칸의 숫자
// 벽(0): 수하물이 지나갈 수 없는 칸입니다.
// 레일(1~4) : 수하물을 정해진 방향으로 옮기는 칸입니다. 1은 아래쪽, 2는 왼쪽, 3은 위쪽, 4는 오른쪽을 뜻합니다.
// 적재 장소(9) : 수하물이 벨트에서 내려 적재되는 칸입니다.적재 장소는 여러 개일 수 있습니다.

// 수하물은 B개
	// 1번부터 B번
	// i번 수하물의 무게는 w_i
	// i번 수하물은 t_i 턴에 (r_i, c_i) 칸으로 투입

// 시뮬레이션은 1번 턴부터 시작해 모든 수하물이 적재될 때까지 진행
// 턴의 구성
// 1단계 수하물 이동
	// 앞칸이 막힌 경우: 앞칸 수하물이 이번 턴에 이동하지 못하면, 이 수하물도 이동하지 못함
	// 같은 칸으로 몰린 경우: 
		// 둘 이상의 수하물이 같은 칸으로 이동하려 하면 그 가운데 가장 무거운 수하물만 이동 가능
		// 우선 순위 1) 가장 무거운 수하물 2) 무게 같으면 번호가 작은 수하물
// 2단계 적재 및 파손
	// 적재 장소 도착 -> 벨트 위에서 사라짐
	// 이번 턴의 번호를 그 수하물의 적재 턴으로 기록
	// 벨트 위에 남은 수하물 중 1단계에서 3턴 연속으로 이동하지 못한 수하물은 파손
	// 1번이라도 이동하면 횟수는 다시 0부터 셈
	// 파손된 수하물도 같은 규칙에 따라 이동, 적재, 무게 유지
// 3단계 수하물 투입
	// 투입 예정 턴이 지났거나, 이미 지났지만 투입되지 못한 수하물은 자신의 투입 칸에 올라가려고 함
	// 투입 못하는 경우 : 투입 칸에 이미 수하물 있는 경우
	// 둘 이상의 수하물이 같은 칸에 투입되려 하면, 투입 예정 턴이 가낭 이른 수하물만 투입
	// 투입 되지 못한 수하물은 다음 턴의 3단계에서 다시 투입 시도
	// 이번 턴에 투입된 수하물은 다음 턴부터 이동

// 출력
	// 1번부터 B번까지 각 수하물의 적재턴
	// 파손된 수하물은 적재 턴에 -1을 곱함

#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <map>
using namespace std;

#define MAX_N 8
#define MAX_B 30
#define MAX_T 199
#define OUT 9

int sizeN; // 격자 크기
int bagCntB; // 전체 수하물 개수

// 격자의 각 칸 숫자 = 0, 1, 2, 3, 4, 9
// 0은 벽, 9는 적재 장소
int grid[MAX_N + 1][MAX_N + 1];

struct Bag
{
	int idx;
	int time;
	int x, y;
	int w;
	
	int remainCnt = 0;
	bool doMove = false;
	int nx, ny;
};
Bag bagList[MAX_B+1];

bool isIn[MAX_B + 1]; // 투입 여부
bool isBroken[MAX_B + 1]; // 파손 여부
vector<int> bagInTime[MAX_T + 1]; // 투입 시점 같은 가방 번호끼리 vector에 저장
// 1하 2좌 3상 4우
int dx[5] = { 0, 1, 0, -1, 0 };
int dy[5] = { 0, 0, -1, 0, 1 };

int totalBagOutCnt = 0; // 적재된 가방 개수
bool isOut[MAX_B + 1]; // 적재 여부
int bagOutList[MAX_B + 1]; // 각 가방의 적재 시간 기록

int inBagCnt = 0;

void breakAtThird(int bIdx)
{
	bagList[bIdx].remainCnt++;
	if (bagList[bIdx].remainCnt >= 3)
	{
		isBroken[bIdx] = true;
	}
}

int main()
{
	cin >> sizeN;
	for (int i = 1; i <= sizeN; i++)
	{
		for (int j = 1; j <= sizeN; j++)
		{
			cin >> grid[i][j];
		}
	}

	cin >> bagCntB;
	fill(isIn, isIn + MAX_B, false);
	fill(isOut, isOut + MAX_B, false);
	fill(isBroken, isBroken + MAX_B, false);

	for (int i = 1; i <= bagCntB; i++)
	{
		int t, r, c, w;
		cin >> t >> r >> c >> w;
		// 수하물 번호가 곧 index
		bagList[i] = { i, t, r,c,w };
		bagInTime[t].push_back(i);
	}

	int time = 1;
	while (totalBagOutCnt < bagCntB)
	{
		// 1단계 수하물 이동
		// 다음 위치에 올 수 있는 가방 모아보기
		vector<int> bagMoveList[MAX_N+1][MAX_N+1];
		set<pair<int, int>> positionSet;

		map<pair<int, int>, int> posToBagNum;

		for (int i = 1; i <= bagCntB; i++)
		{
			if (!isIn[i]) continue; // 투입 안 된 가방 무시
			if (isOut[i]) continue; // 적재된 가방 무시

			// 투입된 가방이면 이동 필요하니까 시뮬레이션
			const Bag& curBag = bagList[i];
			int dir = grid[curBag.x][curBag.y];
			int nx = curBag.x + dx[dir];
			int ny = curBag.y + dy[dir];

			posToBagNum[{curBag.x, curBag.y}] = curBag.idx;

			bagMoveList[nx][ny].push_back(curBag.idx);
			bagList[i].nx = nx;
			bagList[i].ny = ny;
			positionSet.insert({ nx, ny });
		}

		// 올 수 있는 가방 중에 제일 무겁거나, 가방 번호 작은 것 옮기기
		for (auto pos : positionSet)
		{
			const vector<int>& bagVector = bagMoveList[pos.first][pos.second];
			int maxW = 0;
			int minNum = MAX_B + 1;
			int selectedBagNum = 0;
			for (int idx = 0; idx < bagVector.size(); idx++)
			{
				int bagNumber = bagVector[idx];

				if (bagList[bagNumber].w > maxW)
				{
					// 무거운 것이 투입 우선권 가짐
					maxW = bagList[bagNumber].w;
					selectedBagNum = bagNumber;
				}
				else if (bagList[bagNumber].w == maxW)
				{
					// 번호 작은 것이 투입 우선권 가짐
					if (selectedBagNum > bagNumber)
					{
						selectedBagNum = bagNumber;
					}
				}
			}

			// 선택된 가방은 움직일 예정으로 표시
			bagList[selectedBagNum].doMove = true;
		}

		// 반복 검색 - 원래 for문 하나만 작성했었는데 반복 탐색을 해서 doMove가 바뀌지 않을 때까지 해야 한다고 AI가 알려줘서 고쳤음
		bool changed = true;
		while (changed)
		{
			changed = false;
			for (int bIdx = 1; bIdx <= bagCntB; bIdx++)
			{
				if (!isIn[bIdx] || isOut[bIdx]) continue;

				Bag& bag = bagList[bIdx];
				if (!bag.doMove) continue;

				auto it = posToBagNum.find({ bag.nx, bag.ny });
				if (it == posToBagNum.end()) continue;

				// 움직일 예정인 가방의 다음 위치에 다른 가방이 이미 위치해있을 때
				int fowardBagNum = it->second;
				if (!bagList[fowardBagNum].doMove)
				{
					// 앞에 있는 가방이 안 움직이므로 이 가방도 못 움직임
					bag.doMove = false;
					changed = true;
				}
			}
		}

		// 가방 이동
		for (int bIdx = 1; bIdx <= bagCntB; bIdx++)
		{
			if (!isIn[bIdx] || isOut[bIdx]) continue; // 투입 안 된 가방 무시, 적재된 가방 무시

			Bag& curBag = bagList[bIdx];
			if (!curBag.doMove)
			{
				breakAtThird(bIdx);
				continue;
			}

				// 다음 가려는 위치에는 가방 없음
			curBag.x = curBag.nx;
			curBag.y = curBag.ny;
			curBag.nx = 0;
			curBag.ny = 0;
			curBag.remainCnt = 0;
			
		}


		// 이동 다 하고 나서 적재 위치인지 체크
		posToBagNum.clear();
		for (int bIdx = 1; bIdx <= bagCntB; bIdx++)
		{
			bagList[bIdx].doMove = false;
			if (!isIn[bIdx] || isOut[bIdx]) continue;
			Bag& bag = bagList[bIdx];
			if (grid[bag.x][bag.y] == OUT)
			{
				isOut[bIdx] = true;
				// 적재된 시간 기록
				bagOutList[bIdx] = isBroken[bIdx] ? -time : time;
				totalBagOutCnt++;
				continue;
			}
			posToBagNum.insert({ {bag.x, bag.y}, bIdx });
		}
		
		// 3단계 가방 투입
		// 투입 다 했으면 검사 안 해도 되므로!
		if (inBagCnt < bagCntB)
		{
			for (int t = 1; t <= time; t++)
			{
				if (inBagCnt >= bagCntB) break;
				for (int bIdx : bagInTime[t])
				{
					if (isIn[bIdx]) continue;
					// 해당 가방 투입 위치에 가방 없는지 확인
					int x = bagList[bIdx].x;
					int y = bagList[bIdx].y;
					if (posToBagNum.find({x, y}) == posToBagNum.end())
					{
						isIn[bIdx] = true;
						inBagCnt++;

						posToBagNum[{x, y}] = bIdx;
					}

				}
			}
		}

		time++;
	}

	for (int i = 1; i <= bagCntB; i++)
	{
		cout << bagOutList[i] << " ";
	}
	cout << "\n";

}