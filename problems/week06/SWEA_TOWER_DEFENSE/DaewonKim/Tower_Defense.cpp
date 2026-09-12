const int MAXN = 20;
#include <iostream>
#include <vector>

using namespace std;

struct Thief
{
	int id;
	int x, y;
	int hp;
	bool isDead;
	bool isFinished;				// 들어갔는지
	vector<vector<bool>> visited; // 경로 이동에 따른 방문 처리
};

struct Tower
{
	int x, y;
	int thiefId; // 공격 대상
	bool canAttack;
	int reloadTime;				// 재장전 시간
	int curReloadTime = 0;		// 현재 재장전 시간

};

int moveCycle = 0;			// 도망자 행동주기
int attackDistance = 3;		// 타워 사거리

vector<Tower> Towers;
vector<Thief> Thiefs;

int startX, startY;
int endX, endY;

int mapSize;
vector<vector<int>> map;

void init(int N, int mMap[MAXN][MAXN])
{
	startX = -1;
	startY = -1;

	endX = -2;
	endY = -2;

	mapSize = N;

	map.assign(N, vector<int>(N));
	for (int x = 0; x < N; ++x)
	{
		for (int y = 0; y < N; ++y)
		{
			map[x][y] = mMap[x][y];
			// 시작 지점
			if (mMap[x][y] == 2)
			{
				startX = x;
				startY = y;
			}
			// 끝지점
			else if (mMap[x][y] == 3)
			{
				endX = x;
				endY = y;
			}
		}
	}
	Towers.clear();
}

void addTower(int mRow, int mCol, int mInterval)
{
	Tower T;
	T.x = mRow;
	T.y = mCol;
	T.reloadTime = mInterval; // 재장전 시간은 타워마다 다름.
	T.thiefId = -1;

	// 맨 처음은 공격 가능한 상태로 설정
	T.canAttack = true;
	T.curReloadTime = 0;
	Towers.push_back(T);
}

// 공격이 가능하면 -> 공격 대상 설정 
// 공격 대상이 없다면 , -1로 설정.
/*
1. 공격준비가 되지 않은 타워는 공격할 대상을 찾지 않는다.

2. 공격준비가 된 타워는 공격할 대상을 찾는다.
공격준비가 되었다는 말의 의미는,
마지막 공격 후 재장전 시간이 지났음을 의미한다.

2-a. 가장 마지막에 공격했던 대상이 살아 있고 그것이 사정거리내에 있다면,
공격대상을 유지한다.

(주의 : 마지막에 공격했던 대상이 공격 준비가 안되었을 때 잠깐 사정거리에서 벗어났더라도
현재 사정거리내에 존재한다면, 공격대상을 바꾸지 않고 그것을 공격대상으로 선정한다.)

우선순위 1. 사정거리 내에 있는 대상
우선순위 2. (우선순위 1 에 해당하는 도망자들이 둘 이상일 경우)
그 중 남은 체력이 가장 적은 대상.
우선순위 3. (우선순위 2 에 해당하는 도망자들이 둘 이상일 경우)
그 중 맵에 먼저 나타난 대상
*/
void attack()
{
	vector<int> targets(Towers.size(), -1);

	// 1. 모든 타워의 공격 대상 선정
	for (int i = 0; i < Towers.size(); ++i)
	{
		Tower& tower = Towers[i];

		// 아직 재장전 중
		if (!tower.canAttack)
		{
			tower.curReloadTime--;

			if (tower.curReloadTime > 0)
			{
				continue;
			}

			tower.canAttack = true;
		}

		int tX = tower.x;
		int tY = tower.y;

		// 기존 공격 대상이 존재한다면
		if (tower.thiefId != -1)
		{
			Thief& target = Thiefs[tower.thiefId];

			// 기존 대상이 살아 있고, 탈출하지 않았고,
			// 현재 사거리 안이라면 그대로 유지
			if (!target.isDead &&
				!target.isFinished &&
				abs(tX - target.x) + abs(tY - target.y) <= attackDistance)
			{
				targets[i] = tower.thiefId;
				continue;
			}

			// 기존 대상 유지 불가능
			tower.thiefId = -1;
		}

		// 새로운 공격 대상 탐색
		for (Thief& thief : Thiefs)
		{
			if (thief.isDead || thief.isFinished)
				continue;

			// 사정거리 밖
			if (abs(tX - thief.x) + abs(tY - thief.y) > attackDistance)
				continue;

			// 최초 후보
			if (tower.thiefId == -1)
			{
				tower.thiefId = thief.id;
			}
			else
			{
				Thief& target = Thiefs[tower.thiefId];

				// 체력이 더 적은 대상
				if (thief.hp < target.hp)
				{
					tower.thiefId = thief.id;
				}
				// 체력이 같으면 먼저 나타난 대상
				else if (thief.hp == target.hp &&
					thief.id < target.id)
				{
					tower.thiefId = thief.id;
				}
			}
		}

		// 이번 턴 공격 대상 저장
		if (tower.thiefId != -1)
		{
			targets[i] = tower.thiefId;
		}
	}

	// 2. 모든 타워의 공격량 계산
	vector<int> damage(Thiefs.size(), 0);

	for (int i = 0; i < Towers.size(); ++i)
	{
		if (targets[i] == -1)
			continue;

		damage[targets[i]]++;

		// 실제 공격한 타워만 재장전 시작
		Towers[i].canAttack = false;
		Towers[i].curReloadTime = Towers[i].reloadTime;
	}

	// 3. 모든 공격을 동시에 적용
	for (int i = 0; i < Thiefs.size(); ++i)
	{
		Thiefs[i].hp -= damage[i];
	}
}

/*
	runSimulation 진행순서
	1. 공격
	1-2. 공격 이후 죽음 판정

	2. 도망자의 행동 주기가 되었다면?
		2-1. 이동
		2-2. 도적 생성이 덜 되었으면 생성

*/
void runSimulation(int M, int mInterval, int mHP, int mRetTs[], int mRetHP[])
{
	moveCycle = mInterval;
	Thiefs.clear();

	int curTime = 0;

	// 타워 상태 초기화
	for (Tower& tower : Towers)
	{
		tower.canAttack = true;
		tower.curReloadTime = 0;
		tower.thiefId = -1;
	}

	int generateThiefCnt = 0;

	int inCnt = 0;
	int deadCnt = 0;

	while (inCnt + deadCnt < M)
	{
		// 1. 모든 타워 공격
		attack();

		// 2. 공격 이후 사망 판정
		for (Thief& thief : Thiefs)
		{
			if (!thief.isDead &&
				!thief.isFinished &&
				thief.hp <= 0)
			{
				thief.isDead = true;

				mRetTs[thief.id] = curTime;
				mRetHP[thief.id] = 0;

				deadCnt++;
			}
		}

		// 3. 행동 주기가 도래했다면
		if (curTime % mInterval == 0 && curTime != 0)
		{
			int dx[4] = { 0, 1, 0, -1 };
			int dy[4] = { 1, 0, -1, 0 };

			// 기존 도망자 이동
			for (Thief& thief : Thiefs)
			{
				if (thief.isDead || thief.isFinished)
					continue;

				for (int i = 0; i < 4; ++i)
				{
					int nextX = thief.x + dx[i];
					int nextY = thief.y + dy[i];

					if (nextX < 0 || nextX >= mapSize ||
						nextY < 0 || nextY >= mapSize)
						continue;

					if (map[nextX][nextY] != 1 &&
						map[nextX][nextY] != 3)
						continue;

					if (thief.visited[nextX][nextY])
						continue;

					thief.x = nextX;
					thief.y = nextY;

					thief.visited[nextX][nextY] = true;

					// 도착지 도달
					if (nextX == endX && nextY == endY)
					{
						thief.isFinished = true;

						mRetTs[thief.id] = curTime;
						mRetHP[thief.id] = thief.hp;

						inCnt++;
					}

					break;
				}
			}

			// 새로운 도망자 생성
			if (generateThiefCnt < M)
			{
				Thief T;

				T.id = generateThiefCnt;
				T.x = startX;
				T.y = startY;
				T.hp = mHP;
				T.isDead = false;
				T.isFinished = false;

				T.visited.assign(
					mapSize,
					vector<bool>(mapSize, false)
				);

				T.visited[startX][startY] = true;

				Thiefs.push_back(T);

				generateThiefCnt++;
			}
		}

		curTime++;
	}
}
