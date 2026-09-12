#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <cstdio>

#define CMD_INIT 100
#define CMD_ADD  200
#define CMD_MOVE 300
#define MAX_STOPOVER 5

extern void init(int N);
extern void addBuilding(int mID, int mRow, int mCol,
    int mHeight, int mWidth,
    int mRelRow, int mRelCol);
extern int move(int mStartID, int mEndID, int M, int mStopover[]);

static bool run()
{
    int Q;
    std::scanf("%d", &Q);

    bool okay = false;

    for (int q = 0; q < Q; ++q)
    {
        int cmd;
        std::scanf("%d", &cmd);

        if (cmd == CMD_INIT)
        {
            int N;
            std::scanf("%d", &N);
            init(N);
            okay = true;
        }
        else if (cmd == CMD_ADD)
        {
            int mID, mRow, mCol, mHeight, mWidth, mRelRow, mRelCol;
            std::scanf("%d %d %d %d %d %d %d",
                &mID, &mRow, &mCol,
                &mHeight, &mWidth,
                &mRelRow, &mRelCol);

            addBuilding(mID, mRow, mCol,
                mHeight, mWidth,
                mRelRow, mRelCol);
        }
        else if (cmd == CMD_MOVE)
        {
            int mStartID, mEndID, M;
            int mStopover[MAX_STOPOVER];
            int expected;

            std::scanf("%d %d %d", &mStartID, &mEndID, &M);

            for (int i = 0; i < M; ++i)
                std::scanf("%d", &mStopover[i]);

            std::scanf("%d", &expected);

            int ret = move(mStartID, mEndID, M, mStopover);
            if (ret != expected)
                okay = false;
        }
    }

    return okay;
}

int main()
{
    setbuf(stdout, NULL);

    // 로컬 테스트 시 필요하면 아래 주석을 해제한다.
    freopen("input.txt", "r", stdin);
    // freopen("output.txt", "w", stdout);

    int T, MARK;
    std::scanf("%d %d", &T, &MARK);

    for (int tc = 1; tc <= T; ++tc)
    {
        int score = run() ? MARK : 0;
        std::printf("#%d %d\n", tc, score);
    }

    return 0;
}

#define corner (1<<4)-1       // 코너는 진입 후 네 방향 중 원하는 방향으로 전환할 수 있다.
#define max_visit (1<<6)-1    // M <= 5이므로 실제 사용하는 방문 마스크는 0 ~ 31이다.

#include<set>
#include<queue>
#include<unordered_map>
#include<cstdlib>
struct Path
{
    int direction;
    int row;
    int col;
    int cnt;
    int is_visited;
};

using namespace std;

// board[r][c]의 하위 4비트는 해당 칸에서 지원하는 도로 방향이다.
int board[50][50];
// -1이면 방향 전환이 가능한 건물 코너, 양수이면 해당 위치의 건물 ID이다.
int position_to[50][50];
int dc[4] = { 1,0,-1,0 };
int dr[4] = { 0,1,0,-1 };
int visited[50][50][4][max_visit];
unordered_map<int, pair<int, int>> buildings;
int n;
void init(int N)
{
    n = N;
    memset(visited, 0, sizeof(visited));
    memset(position_to, 0, sizeof(position_to));
    memset(board, 0, sizeof(board));
    buildings.clear();

}

void addBuilding(int mID, int mRow, int mCol,
    int mHeight, int mWidth,
    int mRelRow, int mRelCol)
{
    // 건물 ID로 출입구의 실제 좌표를 바로 찾을 수 있게 저장한다.
    buildings[mID] = { mRow + mRelRow,mCol + mRelCol };
    position_to[mRow + mRelRow][mCol + mRelCol] = mID;

    // 위/오른쪽/아래/왼쪽 외곽 도로의 진행 방향을 OR로 누적한다.
    for (int col = mCol - 1; col <= mCol + mWidth; col++) {
        board[mRow - 1][col] |= 1;
    }
    for (int col = mCol - 1; col <= mCol + mWidth; col++) {
        board[mRow + mHeight][col] |= (1 << 2);
    }
    for (int row = mRow - 1; row <= mRow + mHeight; row++) {
        board[row][mCol + mWidth] |= (1 << 1);
    }
    for (int row = mRow - 1; row <= mRow + mHeight; row++) {
        board[row][mCol - 1] |= (1 << 3);
    }
    // 코너에서는 진입 방향과 관계없이 방향을 바꿀 수 있다.
    board[mRow - 1][mCol - 1] = corner;
    board[mRow - 1][mCol + mWidth] = corner;
    board[mRow + mHeight][mCol - 1] = corner;
    board[mRow + mHeight][mCol + mWidth] = corner;

    position_to[mRow - 1][mCol - 1] = -1;
    position_to[mRow - 1][mCol + mWidth] = -1;
    position_to[mRow + mHeight][mCol - 1] = -1;
    position_to[mRow + mHeight][mCol + mWidth] = -1;
}
int move(int mStartID, int mEndID, int M, int mStopover[])
{
    // 상태: (현재 도로 좌표, 진행 방향, 아직 방문하지 않은 경유지 비트마스크)
    // 최대 상태 수는 N*N*4*2^M이며 후속 상태는 삽입 시 방문 처리한다.
    memset(visited, 0, sizeof(visited));
    int temp_visited = (1 << M) - 1;
    int st_row = buildings[mStartID].first;
    int st_col = buildings[mStartID].second;

    int end_row = buildings[mEndID].first;
    int end_col = buildings[mEndID].second;
    queue<Path> q;

    // 출입구 주변 도로 중 가능한 곳으로 비용 1을 지불하고 출발한다.
    // 출입구에서 외곽 도로로 우회전하므로 방향은 (i + 1) % 4이다.
    for (int i = 0; i < 4; i++) {
        int temp_row = st_row + dr[i];
        int temp_col = st_col + dc[i];
        if (board[temp_row][temp_col] != 0) {
            int temp_dir = (i + 1) % 4;
            q.push({ temp_dir, temp_row, temp_col, 1, temp_visited });
        }
    }
    while (!q.empty()) {
        Path cur = q.front();
        int cur_row = cur.row;
        int cur_col = cur.col;
        int cur_cnt = cur.cnt;
        int cur_visited = cur.is_visited;
        int cur_direction = cur.direction;

        q.pop();

        // 진행 방향의 오른쪽 칸이 건물 출입구라면 현재 도로 상태에서 방문 가능하다.
        int chk = (cur_direction + 1) % 4;

        int chk_row = cur_row + dr[chk];
        int chk_col = cur_col + dc[chk];

        bool chk_flag = true;
        if (chk_row < 0 || chk_row >= n || chk_col < 0 || chk_col >= n) chk_flag = false;

        if (chk_flag) {
            if (chk_row == end_row && chk_col == end_col) {
                if (cur_visited == 0) {
                    // 모든 경유지를 방문한 뒤 도착 건물로 들어가는 비용 1을 더한다.
                    return cur_cnt + 1;
                }
            }
            for (int i = 0; i < M; i++) {
                int stop_id = mStopover[i];
                int stop_row = buildings[stop_id].first;
                int stop_col = buildings[stop_id].second;
                if (stop_row == chk_row && stop_col == chk_col) {
                    if (cur_visited & (1 << i)) {
                        // 경유 건물 진입 후 같은 도로 상태로 복귀하는 고정 비용은 2이다.
                        cur_cnt += 2;
                        int temp_bit = (1 << i);
                        cur_visited ^= temp_bit;
                    }
                }
            }
        }
        if (position_to[cur_row][cur_col] == -1) {
            // 건물 코너에서는 방향을 자유롭게 바꾸되 다음 칸의 방향 지원 여부를 확인한다.
            for (int next_dir = 0; next_dir < 4; next_dir++) {
                int next_row = cur_row + dr[next_dir];
                int next_col = cur_col + dc[next_dir];
                if (next_row < 0 || next_row >= n || next_col < 0 || next_col >= n) continue;
                if (visited[next_row][next_col][next_dir][cur_visited] == 1) continue;
                if (board[next_row][next_col] & (1 << next_dir)) {
                    // 삽입 시 방문 처리하여 동일 상태가 큐에 중복 삽입되는 것을 막는다.
                    visited[next_row][next_col][next_dir][cur_visited] = 1;
                    q.push({ next_dir,next_row,next_col,cur_cnt + 1,cur_visited });
                }
            }
        }
        else {
            // 일반 도로에서는 현재 진행 방향을 유지해야 한다.
            int next_row = cur_row + dr[cur_direction];
            int next_col = cur_col + dc[cur_direction];
            if (visited[next_row][next_col][cur_direction][cur_visited] == 1) continue;
            if (board[next_row][next_col] & (1 << cur_direction)) {

                visited[next_row][next_col][cur_direction][cur_visited] = 1;
                q.push({ cur_direction,next_row,next_col,cur_cnt + 1,cur_visited });
            }
        }

    }
    return -1;
}
