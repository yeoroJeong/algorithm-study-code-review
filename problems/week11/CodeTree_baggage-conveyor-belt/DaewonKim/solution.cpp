#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>

using namespace std;


int n, b;

//  1은 아래쪽, 2는 왼쪽, 3은 위쪽, 4는 오른쪽  →  저장은 (입력값 - 1)
int dx[4] = { 1,0,-1,0 };
int dy[4] = { 0,-1,0,1 };

enum Object
{
    EXIT,
    BELT,
    NONE        // 벽
};

// 이동 관련 칸 정보
struct Section
{
    Object ob = NONE;

    bool isBox = false;                    // 박스가 있는지
    bool canCurrentBoxMove = false;        // 현재 칸에 있는 박스가 움직일 수 있는지.
    bool isBoxComing = false;            // 이번 턴에 현재 칸으로 들어오려는 박스가 있는지

    //   6: 0은 없는 걸로 통일
    int currentBoxId = 0;                // 이 칸에 들어있는 박스 id
    int comingBoxId = 0;                // 들어오려고 하는 박스의 id
    int dir = -1;                  
};


struct Box
{
    int putTurn = 0;                //    투입 예정 턴
    int startRow = 0;                //    투입 위치
    int startCol = 0;
    int weight = 0;                    //      1: 무게

    bool isPuted = false;            //    투입 되었는지
    bool isCrashed = false;            //    파손 되었는지
    bool isLoaded = false;            //    적재 되었는지

    bool curTurnMove = false;        //    현재 턴에 움직였는지 안움직였는지.
    int stopCnt = 0;                //    연속해서 못 움직인 턴.

    int curRow = 0;                    // 현재 위치
    int curCol = 0;
    int loadedTurn = 0;                // 적재된 턴
};

unordered_map<int, Box> boxes;
vector<pair<int, int>> exits;
int loadedCnt = 0;

bool inRange(int row, int col)
{
    return(row >= 0 && row < n && col >= 0 && col < n);
}

// 우선순위: 무게가 무거울수록, 같으면 id가 작을수록
bool isHigher(int a, int bId)
{
    if (boxes[a].weight != boxes[bId].weight)
        return boxes[a].weight > boxes[bId].weight;
    return a < bId;
}

// 박스 id의 이동 가능 여부를 칸과 박스 양쪽에 동시에 기록 (두 곳 저장이므로 한 함수에서만 바꾼다)
void setMovable(vector<vector<Section>>& board, int boxId, bool movable)
{
    board[boxes[boxId].curRow][boxes[boxId].curCol].canCurrentBoxMove = movable;
    boxes[boxId].curTurnMove = movable;
}

/*
    원래 코드에서 "다음 칸이 비었을 때"와 "다음 칸 박스가 움직일 때"의 처리가 완전히 같았으므로
    공통 부분을 함수로 분리.
    → 현재 칸 박스가 (nextRow, nextCol)로 들어가기 위한 경합 처리
*/
void competeForNext(vector<vector<Section>>& board, int curRow, int curCol, int nextRow, int nextCol)
{
    int curComingBoxId = board[curRow][curCol].currentBoxId;

    // 다음 칸에 들어가려고 하는 박스가 없다면 → 일단 내가 들어감
    if (!board[nextRow][nextCol].isBoxComing)
    {
        board[nextRow][nextCol].isBoxComing = true;
        board[nextRow][nextCol].comingBoxId = curComingBoxId;
        setMovable(board, curComingBoxId, true);
        return;
    }

    // 다음 칸에 들어가려고 하는 박스가 이미 있다면 → 우선순위 비교
    int prevComingBoxId = board[nextRow][nextCol].comingBoxId;

    if (isHigher(curComingBoxId, prevComingBoxId))
    {
        // 내가 이김: 들어오는 박스 갱신, 이전 승자는 취소
        board[nextRow][nextCol].comingBoxId = curComingBoxId;
        setMovable(board, prevComingBoxId, false);
        setMovable(board, curComingBoxId, true);
    }
    else
    {
        //   3: 내가 짐 → 명시적으로 못 움직임 처리 (원래 코드엔 이 분기가 없었음)
        setMovable(board, curComingBoxId, false);
    }
}

// curRow,col -> 지금 검사하는 칸 / nextRow,col -> 그 다음 칸.
void checkMoveBox(vector<vector<Section>>& board, int curRow, int curCol, int nextRow, int nextCol)
{
    // 현재 칸에 박스 없으면 아무 일도 발생 X
    if (!board[curRow][curCol].isBox)
        return;

    // 다음 칸에 박스가 없다면 → 경합
    if (!board[nextRow][nextCol].isBox)
    {
        competeForNext(board, curRow, curCol, nextRow, nextCol);
    }
    // 다음 칸에 박스가 있다면
    else
    {
        // 다음 칸 박스가 움직일 수 없다면 → 현재 칸 박스도 못 움직임
        if (!board[nextRow][nextCol].canCurrentBoxMove)
            setMovable(board, board[curRow][curCol].currentBoxId, false);
        // 다음 칸 박스가 움직일 수 있다면 → 비게 될 칸이므로 경합
        else
            competeForNext(board, curRow, curCol, nextRow, nextCol);
    }
}

//   4: 매 턴 이동 판정 전에 임시 플래그 초기화 (원래 checkStatusAfterMove가 하려던 역할)
void resetSections(vector<vector<Section>>& board)
{
    for (int row = 0; row < n; ++row)
    {
        for (int col = 0; col < n; ++col)
        {
            board[row][col].isBoxComing = false;
            board[row][col].comingBoxId = 0;
            board[row][col].canCurrentBoxMove = false;
        }
    }
}

/*
    check가 완료 됐으면 실제 이동을 시킨다. (  5)
    각 칸의 다음 상태는 "자기 칸의 플래그"만으로 결정되므로, 격자 하나를 순서대로 수정해도 안전하다.
        - 들어오는 박스가 있다        → 그 박스가 새 주인 (승자는 항상 실제로 이동함)
        - 없고, 내 박스가 못 움직인다 → 그대로
        - 없고, 내 박스가 움직인다    → 빈 칸이 됨 (원래 코드에서 누락)
*/
void makeMove(vector<vector<Section>>& board)
{
    for (int row = 0; row < n; ++row)
    {
        for (int col = 0; col < n; ++col)
        {
            if (board[row][col].ob == NONE)
                continue;

            Section& s = board[row][col];

            if (s.isBoxComing)
            {
                s.isBox = true;
                s.currentBoxId = s.comingBoxId;
                boxes[s.currentBoxId].curRow = row;        
                boxes[s.currentBoxId].curCol = col;
            }
            else if (s.isBox && s.canCurrentBoxMove)
            {
                s.isBox = false;                        
                s.currentBoxId = 0;
            }
            // 들어오는 박스 없고 내 박스가 못 움직이면 그대로
        }
    }
}

/*
    적재 칸들에서부터 역방향 BFS를 실행하며 이동 판정 → makeMove
    BFS는 깊이 순으로 진행되므로, 어떤 칸을 판정할 때 그 앞 칸의 판정은 이미 끝나 있다.
*/
void moveBox(vector<vector<Section>>& board)
{
    resetSections(board);

    for (auto& pos : exits)
    {
        int exitRow = pos.first;
        int exitCol = pos.second;

        queue<pair<int, int>> q;
        vector<vector<bool>> visited(n, vector<bool>(n, false));
        q.push({ exitRow, exitCol });
        visited[exitRow][exitCol] = true;

        while (!q.empty())
        {
            int curRow = q.front().first;
            int curCol = q.front().second;
            q.pop();

            for (int dir = 0; dir < 4; ++dir)
            {
                int prevRow = curRow + dx[dir];
                int prevCol = curCol + dy[dir];

                //   2: BELT 검사를 dx[dir] 접근보다 먼저 (벽/적재 칸은 dir == -1)
                if (inRange(prevRow, prevCol)
                    && !visited[prevRow][prevCol]
                    && board[prevRow][prevCol].ob == BELT
                    && (curRow == prevRow + dx[board[prevRow][prevCol].dir])
                    && (curCol == prevCol + dy[board[prevRow][prevCol].dir]))
                {
                    // (prevRow, prevCol) -> (curRow, curCol) 가는 걸 검사.
                    checkMoveBox(board, prevRow, prevCol, curRow, curCol);        

                    q.push({ prevRow, prevCol });
                    visited[prevRow][prevCol] = true;
                }
            }
        }
    }

    makeMove(board);
}

void loadOrCrash(vector<vector<Section>>& board, int turn)
{
    for (int id = 1; id <= b; ++id)
    {
        Box& box = boxes[id];
        if (!box.isPuted || box.isLoaded)
            continue;

        // 적재 장소에 도착 → 적재 후 격자에서 제거
        if (board[box.curRow][box.curCol].ob == EXIT)
        {
            box.isLoaded = true;
            box.loadedTurn = turn;
            board[box.curRow][box.curCol].isBox = false;
            board[box.curRow][box.curCol].currentBoxId = 0;
            ++loadedCnt;
            continue;
        }

        // 벨트 위에 남은 박스: 연속 정지 횟수 갱신 및 파손 판정
        if (box.curTurnMove)
            box.stopCnt = 0;
        else if (++box.stopCnt >= 3)
            box.isCrashed = true;
    }
}

void putBox(vector<vector<Section>>& board, int turn)
{
    // 칸별로 투입 예정 턴이 가장 이른 박스 하나 선택
    vector<vector<int>> pick(n, vector<int>(n, 0));
    for (int id = 1; id <= b; ++id)
    {
        Box& box = boxes[id];
        if (box.isPuted || box.putTurn > turn)
            continue;

        int& p = pick[box.startRow][box.startCol];
        if (p == 0 || box.putTurn < boxes[p].putTurn)
            p = id;
    }

    for (int row = 0; row < n; ++row)
    {
        for (int col = 0; col < n; ++col)
        {
            int id = pick[row][col];
            if (id == 0 || board[row][col].isBox)
                continue;

            board[row][col].isBox = true;
            board[row][col].currentBoxId = id;

            boxes[id].isPuted = true;
            boxes[id].stopCnt = 0;
            boxes[id].curTurnMove = false;        // 이번 턴엔 이동 판정 대상 아님
            boxes[id].curRow = row;
            boxes[id].curCol = col;
        }
    }
}

int main() {
    /*
    총 3단계
    - moveBox -> loadOrCrash -> putBox
    */
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n;
    vector<vector<Section>> board(n, vector<Section>(n));

    for (int row = 0; row < n; ++row)
    {
        for (int col = 0; col < n; ++col)
        {
            int tmp = 0;
            cin >> tmp;
            if (tmp == 9)
            {
                exits.push_back({ row,col });
                board[row][col].ob = EXIT;
            }
            else if (tmp != 0)
            {
                board[row][col].ob = BELT;
                board[row][col].dir = tmp - 1;       
            }
            else
                board[row][col].ob = NONE;
        }
    }

    cin >> b;
    for (int id = 1; id <= b; ++id)
    {
        Box& box = boxes[id];
        cin >> box.putTurn >> box.startRow >> box.startCol >> box.weight;
        --box.startRow;
        --box.startCol;
    }

    for (int turn = 1; loadedCnt < b; ++turn)
    {
        moveBox(board);
        loadOrCrash(board, turn);
        putBox(board, turn);
    }

    for (int id = 1; id <= b; ++id)
    {
        int ans = boxes[id].isCrashed ? -boxes[id].loadedTurn : boxes[id].loadedTurn;
        cout << ans << (id == b ? '\n' : ' ');
    }

    return 0;
}