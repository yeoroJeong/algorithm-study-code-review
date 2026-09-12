#include <vector>
using namespace std;

/*
타워디펜스게임 시뮬레이션
맵을 탈출하는 도망자, 도망자들 공격하여 탈출 방해하는 타워로 구성
타워는 자신의 공격순서시 사정거리 내 도망자 하나 공격
도망자 목표: 맵 탈출, 출발지 -> 도착지
출발지 -> 도착지 경로 유일

타워설명
각 타워는 각각의 재장전시간 가짐 => 한번 공격시 재장전시간 지나고 다시 공격가능
모든 타워는 공격 거리 3(맨해튼거리 abs(y-y') + abs(x - x') <= 3)
가장 마지막에 공격했던 대상이 살아있고, 이게 사정거리 안에 있으면 공격대상 유지(keypoint) => 이게 아닐경우 새로운 대상 찾음
대상 찾는 우선순위
1. 사정거리 내에 있는 대상
2. 그 중 남은 체력이 적은 대상
3. 그 중 맵에 먼저 나타난 대상
공격대상 선정 못한 타워는 다음 턴에 다시 공격대상 찾기


도망자설명
최대 300명, 행동주기 값(1 ~ 5)
도망자는 첫 도망자부터 매 행동주기마다 순차적으로 출발지에 나타남, 도착지까지 1칸씩 이동 및 도착지 도착 즉시 맵에서 사라짐
체력 값 가짐(모든 도망자는 같은 체력값 갖고 시작)
타워로 부터 공격당할때마다 체력 1씩 감소, 0되면 맵에서 사라짐

*매턴 모든 타워는 동시에 공격

아이디어
1. 경로를 1차원으로 펴기
경로가 유일하다는 게 레전드 힌트. 출발지에서 방문체크하며 한 칸씩 따라가면 roadY[i], roadX[i] 순서 배열이 나온다. 그러면 도망자 상태가 좌표 2개가 아니라 인덱스 하나로 줄어든다.
이동 = idx++
도착 = idx == len - 1
등장 = idx = 0
BFS/최단경로 같은 거 필요 없음.

2. 점유 배열 occ[경로인덱스] = 도망자번호
타워가 매 턴 도망자 300명을 전수 스캔하면 400타워 × 300명 × 수천 턴이라 터진다. 대신 addTower 시점에 그 타워의 사정거리 안에 들어오는 경로 인덱스만 미리 뽑아둔다 (inRange). 맨해튼 3이면 최대 25칸이니까, 매 턴 타워당 25번만 occ 조회하면 후보가 다 나온다.

3. 동시 공격은 damage 버퍼
체력을 바로 깎으면 같은 턴 뒤쪽 타워가 이미 죽은 애를 못 고르게 되어서 틀린다. damage[]에 모았다가 타워 루프 끝난 뒤 한꺼번에 차감. 체력보다 많이 맞는 케이스도 이걸로 자동 처리됨.

4. 타겟 유지 규칙
t.target에 직전 대상을 들고 있다가, 매 턴 "살아있고 + 지금 사정거리 안"이면 그대로 재사용. 중간에 잠깐 벗어났었는지는 아예 안 본다. 문제에서 굳이 주의라고 써준 부분이 이거라, 벗어났을 때 target을 -1로 밀어버리면 틀린다.

5. 쿨타임은 실제로 쐈을 때만
t.cool = T + t.interval을 대상 못 찾으면 건드리지 않는다. 준비된 타워는 사거리에 아무도 없어도 계속 준비 상태로 대기.

6. 등장 타이밍
T > 0 && T % mInterval == 0. 첫 도망자는 T=0이 아니라 T=mInterval에 나온다 (Fig 5-1에서 T=3). 여기서 한 주기 밀리면 전부 오답.

7. 턴 순서
타워 → 피해 → 사망 → 이동/등장. 이동을 먼저 하면 안 된다. 그리고 이동은 먼저 등장한 도망자부터 밀어야 앞뒤 자리가 안 겹친다 (얘가 경로상 항상 더 앞에 있으므로).
*/
const int MAXN = 20;
const int RANGE = 3;		// 모든 타워의 공격 사거리 (맨해튼)

// 타워 한 기의 상태
struct Tower
{
	int y, x;
	int interval;			// 재장전 시간
	int cool;				// 다음에 공격 가능한 턴 (T >= cool 이면 준비 완료)
	int target;				// 직전에 공격했던 도망자 번호, 없으면 -1
	vector<int> inRange;	// 사거리 안에 들어오는 경로 인덱스 목록 (미리 계산)
};

// 도망자 한 명의 상태
struct Runner
{
	int idx;				// 현재 경로 인덱스 (아직 등장 전이면 -1)
	int hp;
	bool alive;				// 맵 위에 있는지 여부
};

int N;
vector<int> roadY, roadX;	// 출발지 -> 도착지 경로를 순서대로 편 배열
vector<Tower> towers;

int manhattan(int y1, int x1, int y2, int x2)
{
	int dy = y1 - y2;
	int dx = x1 - x2;
	if (dy < 0) dy = -dy;
	if (dx < 0) dx = -dx;
	return dy + dx;
}

void init(int n, int mMap[MAXN][MAXN])
{
	// 테스트케이스 시작: 이전 데이터 전부 초기화
	N = n;
	roadY.clear();
	roadX.clear();
	towers.clear();

	//출발지 위치 찾기
	int sy = 0, sx = 0;
	for (int y = 0; y < N; y++)
		for (int x = 0; x < N; x++)
			if (mMap[y][x] == 2) { sy = y; sx = x; }

	// 경로가 유일하므로 방문 체크하며 한 줄로 따라가면 순서가 그대로 나옴
	// 좌표 대신 이 배열의 인덱스로 도망자 위치를 관리한다.
	// (이동 = idx + 1, 도착 = idx == size - 1)
	vector<vector<bool>> visited(N, vector<bool>(N, false));
	int dy[4] = { -1, 1, 0, 0 };
	int dx[4] = { 0, 0, -1, 1 };

	int cy = sy, cx = sx;
	while (true)
	{
		visited[cy][cx] = true;
		roadY.push_back(cy);
		roadX.push_back(cx);

		if (mMap[cy][cx] == 3) break;	// 도착지 도달

		// 아직 안 밟은 길/도착지 칸은 유일하므로 찾는 즉시 확정
		int ny = -1, nx = -1;
		for (int d = 0; d < 4; d++)
		{
			int y = cy + dy[d];
			int x = cx + dx[d];
			if (y < 0 || x < 0 || y >= N || x >= N) continue;
			if (visited[y][x]) continue;
			if (mMap[y][x] != 1 && mMap[y][x] != 3) continue;
			ny = y;
			nx = x;
			break;
		}
		if (ny < 0) break;

		cy = ny;
		cx = nx;
	}
}

void addTower(int mRow, int mCol, int mInterval)
{
	Tower t;
	t.y = mRow;
	t.x = mCol;
	t.interval = mInterval;
	t.cool = 0;
	t.target = -1;

	// 사거리 안에 들어오는 경로 칸을 미리 뽑아둔다
	// 매 턴 도망자 300명을 전수 검사하는 대신, 여기 담긴 칸(최대 25개)만
	// occ 로 조회하면 후보가 전부 나온다.
	for (int i = 0; i < (int)roadY.size(); i++)
		if (manhattan(roadY[i], roadX[i], mRow, mCol) <= RANGE)
			t.inRange.push_back(i);

	towers.push_back(t);
}

void runSimulation(int M, int mInterval, int mHP, int mRetTs[], int mRetHP[])
{
	int len = roadY.size();

	// 시뮬레이션 시작: 모든 타워는 공격 준비 완료 상태
	for (int i = 0; i < (int)towers.size(); i++)
	{
		towers[i].cool = 0;
		towers[i].target = -1;
	}

	// 도망자 초기화 (아직 아무도 맵에 없음)
	vector<Runner> runner(M);
	for (int i = 0; i < M; i++)
	{
		runner[i].idx = -1;
		runner[i].hp = mHP;
		runner[i].alive = false;
	}

	vector<int> occ(len, -1);	// 경로 인덱스 -> 그 칸에 있는 도망자 번호, 비었으면 -1
	vector<int> damage(M, 0);	// 이번 턴에 누적된 피해량

	int spawned = 0;			// 지금까지 등장시킨 도망자 수
	int finished = 0;			// 죽거나 탈출해서 처리 끝난 수

	for (int T = 0; finished < M; T++)
	{
		for (int i = 0; i < M; i++) damage[i] = 0;

		// 1.공격 준비된 타워가 대상 선정
		for (int i = 0; i < (int)towers.size(); i++)
		{
			Tower& t = towers[i];
			if (t.cool > T) continue;		// 재장전 중이면 아무것도 안 함

			int pick = -1;
			int prev = t.target;

			// 1-1.직전 대상이 살아있고 지금 사거리 안이면 그대로 유지
			//     중간에 잠깐 사거리를 벗어났었는지는 보지 않는다 <- 함정
			if (prev != -1 && runner[prev].alive
				&& manhattan(roadY[runner[prev].idx], roadX[runner[prev].idx], t.y, t.x) <= RANGE)
			{
				pick = prev;
			}
			// 1-2. 아니면 새로 선정: 체력 적은 순 -> 먼저 등장한 순
			else
			{
				for (int k = 0; k < (int)t.inRange.size(); k++)
				{
					int who = occ[t.inRange[k]];
					if (who == -1) continue;

					if (pick == -1) { pick = who; continue; }
					if (runner[who].hp < runner[pick].hp) pick = who;
					else if (runner[who].hp == runner[pick].hp && who < pick) pick = who;
				}
			}

			t.target = pick;
			if (pick == -1) continue;		// 대상 없음 -> 직전 대상 없는 상태로 턴 종료
											// (쿨타임은 갱신하지 않으므로 계속 준비 상태 유지)

			damage[pick]++;
			t.cool = T + t.interval;		// 실제로 쐈을 때만 재장전 시작
		}

		// 2.모든 공격을 동시에 반영
		// 체력을 타워 루프 안에서 바로 깎으면 뒤쪽 타워가 대상 선정을 못하게 되므로
		// damage 에 모아뒀다가 여기서 한꺼번에 처리한다.
		// (남은 체력보다 많이 맞는 경우도 여기서 자연스럽게 커버됨)
		for (int i = 0; i < M; i++)
		{
			if (!runner[i].alive || damage[i] == 0) continue;

			runner[i].hp -= damage[i];
			if (runner[i].hp > 0) continue;

			// 체력 0 이하 -> 사망, 즉시 맵에서 제거
			occ[runner[i].idx] = -1;
			runner[i].alive = false;
			mRetTs[i] = T;
			mRetHP[i] = 0;
			finished++;
		}

		// 3. 행동주기가 도래한 턴이면 이동 + 신규 등장
		// 첫 도망자는 T = 0 이 아니라 T = mInterval 에 나타난다
		if (T > 0 && T % mInterval == 0)
		{
			// 먼저 등장한 도망자가 항상 경로상 앞에 있으므로
			// 0번부터 순서대로 밀면 앞칸이 비어 있어 충돌이 없다
			for (int i = 0; i < spawned; i++)
			{
				if (!runner[i].alive) continue;

				occ[runner[i].idx] = -1;
				runner[i].idx++;

				if (runner[i].idx == len - 1)	// 도착지 = 도착 즉시 탈출
				{
					runner[i].alive = false;
					mRetTs[i] = T;
					mRetHP[i] = runner[i].hp;
					finished++;
				}
				else
				{
					occ[runner[i].idx] = i;
				}
			}

			// 남은 도망자가 있으면 출발지에 한 명 등장
			if (spawned < M)
			{
				runner[spawned].idx = 0;
				runner[spawned].alive = true;
				occ[0] = spawned;
				spawned++;
			}
		}
	}
}