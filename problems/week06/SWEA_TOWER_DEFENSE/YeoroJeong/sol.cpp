#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <cstdio>
#include <vector>
#include <queue>
#include <algorithm>
#include <cstdlib>

using namespace std;

const int MAXN = 20;
const int MAXM = 300;
const int MAX_PATH = MAXN * MAXN + 1;

void init(int N, int mMap[MAXN][MAXN]);
void addTower(int mRow, int mCol, int mInterval);
void runSimulation(
    int M, int mInterval, int mHP,
    int mRetTs[], int mRetHP[]
);

// ==================== 로컬 테스트용 Main ====================

const int CMD_ADD = 200;
const int CMD_RUN = 300;

static int inputMap[MAXN][MAXN];
static int retTs[MAXM];
static int retHP[MAXM];

static bool run()
{
    int N;
    scanf("%d", &N);

    for (int r = 0; r < N; r++) {
        for (int c = 0; c < N; c++) {
            scanf("%d", &inputMap[r][c]);
        }
    }

    init(N, inputMap);

    int Q;
    scanf("%d", &Q);

    bool okay = true;

    for (int q = 0; q < Q; q++) {
        int cmd;
        scanf("%d", &cmd);

        if (cmd == CMD_ADD) {
            int row, col, interval;
            scanf("%d%d%d", &row, &col, &interval);
            addTower(row, col, interval);
        }
        else if (cmd == CMD_RUN) {
            int M, interval, hp;
            scanf("%d%d%d", &M, &interval, &hp);

            runSimulation(M, interval, hp, retTs, retHP);

            for (int i = 0; i < M; i++) {
                int expected;
                scanf("%d", &expected);

                if (retTs[i] != expected)
                    okay = false;
            }

            for (int i = 0; i < M; i++) {
                int expected;
                scanf("%d", &expected);

                if (retHP[i] != expected)
                    okay = false;
            }
        }
        else {
            okay = false;
        }
    }

    return okay;
}

int main()
{
    setbuf(stdout, NULL);

    // 직접 입력할 때는 아래 줄을 주석 처리
    freopen("sample_input.txt", "r", stdin);

    int TC, MARK;
    if (scanf("%d%d", &TC, &MARK) != 2)
        return 0;

    for (int tc = 1; tc <= TC; tc++) {
        int score = run() ? MARK : 0;
        printf("#%d %d\n", tc, score);
    }

    return 0;
}

// ==================== User Code ====================

struct Path
{
    int row;
    int col;
};

struct Turret
{
    int last_target;
    int interval_time;
    int unlocked;

    vector<int> can_lock;
    bool in_range[MAX_PATH];
};

struct Target
{
    int hp;
    int position;
    bool alive;
};

int dr[4] = { 1, 0, -1, 0 };
int dc[4] = { 0, 1, 0, -1 };

vector<Path> path_list;
vector<Turret> turret_list;

// 경로 인덱스: 출발지 1, 도착지 path_cnt
int path_cnt;

void init(int N, int mMap[MAXN][MAXN])
{
    path_list.clear();
    turret_list.clear();

    // BFS로 부모를 기록한 뒤 도착지부터 경로 복원
    int parent[MAXN][MAXN];

    for (int r = 0; r < N; r++) {
        for (int c = 0; c < N; c++) {
            parent[r][c] = -1;
        }
    }

    Path start = { -1, -1 };
    Path finish = { -1, -1 };

    for (int r = 0; r < N; r++) {
        for (int c = 0; c < N; c++) {
            if (mMap[r][c] == 2)
                start = { r, c };

            if (mMap[r][c] == 3)
                finish = { r, c };
        }
    }

    queue<Path> q;
    q.push(start);
    parent[start.row][start.col] = start.row * N + start.col;

    while (!q.empty()) {
        Path cur = q.front();
        q.pop();

        if (cur.row == finish.row && cur.col == finish.col)
            break;

        for (int dir = 0; dir < 4; dir++) {
            int nr = cur.row + dr[dir];
            int nc = cur.col + dc[dir];

            if (nr < 0 || nr >= N || nc < 0 || nc >= N)
                continue;

            if (mMap[nr][nc] == 0)
                continue;

            if (parent[nr][nc] != -1)
                continue;

            parent[nr][nc] = cur.row * N + cur.col;
            q.push({ nr, nc });
        }
    }

    vector<Path> reversed_path;
    Path cur = finish;

    while (true) {
        reversed_path.push_back(cur);

        if (cur.row == start.row && cur.col == start.col)
            break;

        int prev = parent[cur.row][cur.col];
        cur = { prev / N, prev % N };
    }

    path_list.push_back({ -1, -1 });

    for (int i = (int)reversed_path.size() - 1; i >= 0; i--) {
        path_list.push_back(reversed_path[i]);
    }

    path_cnt = (int)path_list.size() - 1;
}

void addTower(int mRow, int mCol, int mInterval)
{
    Turret tower{};
    tower.last_target = -1;
    tower.interval_time = mInterval;
    tower.unlocked = 0;

    // 도착지에서는 즉시 탈출하므로 도착지는 제외
    for (int pos = 1; pos < path_cnt; pos++) {
        int dist =
            abs(mRow - path_list[pos].row) +
            abs(mCol - path_list[pos].col);

        if (dist <= 3) {
            tower.can_lock.push_back(pos);
            tower.in_range[pos] = true;
        }
    }

    turret_list.push_back(tower);
}

void runSimulation(
    int M, int mInterval, int mHP,
    int mRetTs[], int mRetHP[])
{
    vector<Turret> towers = turret_list;

    for (int i = 0; i < (int)towers.size(); i++) {
        towers[i].last_target = -1;
        towers[i].unlocked = 0;
    }

    Target targets[MAXM] = {};
    int damage[MAXM] = {};

    // 각 경로 칸에 있는 도망자 번호
    // -1이면 빈 칸
    int occupant[MAX_PATH];
    fill(occupant, occupant + MAX_PATH, -1);

    int target_cnt = 0;
    int finished = 0;

    // 0턴에는 도망자가 없으므로 1턴부터 처리
    int T = 1;

    while (finished < M) {
        fill(damage, damage + M, 0);

        // 1. 모든 타워가 공격 대상 선택
        for (int i = 0; i < (int)towers.size(); i++) {
            Turret& tower = towers[i];

            if (tower.unlocked > T)
                continue;

            int selected = -1;
            int last = tower.last_target;

            // 이전 타깃이 살아 있고 사거리 안이면 유지
            if (last >= 0 && targets[last].alive) {
                int pos = targets[last].position;

                if (tower.in_range[pos]) {
                    selected = last;
                }
            }

            // 이전 타깃을 유지할 수 없으면 새 타깃 탐색
            if (selected == -1) {
                for (int j = 0; j < (int)tower.can_lock.size(); j++) {
                    int pos = tower.can_lock[j];
                    int id = occupant[pos];

                    if (id == -1)
                        continue;

                    if (selected == -1) {
                        selected = id;
                    }
                    else if (targets[id].hp < targets[selected].hp) {
                        selected = id;
                    }
                    else if (targets[id].hp == targets[selected].hp
                        && id < selected) {
                        selected = id;
                    }
                }
            }

            // 대상이 없으면 이전 타깃도 해제
            tower.last_target = selected;

            if (selected != -1) {
                damage[selected]++;
                tower.unlocked = T + tower.interval_time;
            }
        }

        // 2. 모든 공격을 동시에 반영
        for (int id = 0; id < target_cnt; id++) {
            if (!targets[id].alive)
                continue;

            targets[id].hp -= damage[id];

            if (targets[id].hp <= 0) {
                targets[id].hp = 0;
                targets[id].alive = false;

                occupant[targets[id].position] = -1;

                mRetTs[id] = T;
                mRetHP[id] = 0;
                finished++;
            }
        }

        // 3. 행동주기이면 이동 후 새 도망자 등장
        if (T % mInterval == 0) {
            // 기존 배치를 지우고 이동한 위치로 다시 배치
            fill(occupant, occupant + MAX_PATH, -1);

            for (int id = 0; id < target_cnt; id++) {
                if (!targets[id].alive)
                    continue;

                targets[id].position++;

                if (targets[id].position == path_cnt) {
                    targets[id].alive = false;

                    mRetTs[id] = T;
                    mRetHP[id] = targets[id].hp;
                    finished++;
                }
                else {
                    occupant[targets[id].position] = id;
                }
            }

            if (target_cnt < M) {
                targets[target_cnt].hp = mHP;
                targets[target_cnt].position = 1;
                targets[target_cnt].alive = true;

                occupant[1] = target_cnt;
                target_cnt++;
            }
        }

        T++;
    }
}